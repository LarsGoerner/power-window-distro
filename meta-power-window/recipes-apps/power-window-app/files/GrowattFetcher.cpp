#include <cstdint>
#include <QObject>
#include <QString>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkCookieJar>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QSettings>
#include <QFile>
#include <QDebug>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include "GrowattData.hpp"
#include "PvStatsData.hpp"
#include "GrowattFetcher.hpp"

static const char * GROWATT_UA = "Dalvik/2.1.0 (Linux; U; Android 12; en_US; SHINEPHONE_APP_706)";
static const char * GROWATT_ACCEPT = "application/json, */*";
static const char * GROWATT_LOGIN_API = "/newTwoLoginAPI.do";
static const char * GROWATT_PLANT_LIST_API = "/PlantListAPI.do";
static const char * GROWATT_PLANT_DETAIL_API = "/PlantDetailAPI.do";

GrowattFetcher::GrowattFetcher(QObject * parent) : QObject(parent)
{
        mNet = new QNetworkAccessManager(this);
        mNet->setCookieJar(new QNetworkCookieJar(mNet));
        mPollTimer = new QTimer(this);
        connect(mPollTimer, &QTimer::timeout, this, [this]() {
                if (!mCurrentPlantId.isEmpty() && mLoggedIn) { fetchRealtimeData(mCurrentPlantId); }
        });
        loadSettings();
}

void GrowattFetcher::loadSettings()
{
        QSettings settings("power-window", "growatt");
        mUsername = settings.value("username").toString();
        QString encPwd = settings.value("password").toString();
        mPassword = encPwd.isEmpty() ? QString() : decryptPassword(encPwd);
        mServerUrl = settings.value("server_url", "https://openapi.growatt.com").toString();
        mCurrentPlantId = settings.value("plant_id").toString();
}

void GrowattFetcher::saveSettings()
{
        QSettings settings("power-window", "growatt");
        settings.setValue("username", mUsername);
        settings.setValue("password", encryptPassword(mPassword));
        settings.setValue("server_url", mServerUrl);
        settings.setValue("plant_id", mCurrentPlantId);
        settings.sync();
}

void GrowattFetcher::clearSettings()
{
        QSettings settings("power-window", "growatt");
        settings.clear();
        settings.sync();
        mUsername.clear();
        mPassword.clear();
        mServerUrl = "https://openapi.growatt.com";
        mCurrentPlantId.clear();
        mLoggedIn = false;
}

void GrowattFetcher::login(const QString &username, const QString &pwd, const QString &serverUrl)
{
        mUsername = username;
        mPassword = pwd;
        mServerUrl = serverUrl;
        performLogin(username, pwd);
}

void GrowattFetcher::fetchPlantList()
{
        QUrl url(mServerUrl + GROWATT_PLANT_LIST_API);
        QUrlQuery query;
        query.addQueryItem("userId", mUserId);
        url.setQuery(query);
        QNetworkRequest req(url);
        req.setRawHeader("User-Agent", QByteArray(GROWATT_UA));
        req.setRawHeader("Accept", QByteArray(GROWATT_ACCEPT));
        QNetworkReply * reply = mNet->get(req);
        connect(reply, &QNetworkReply::finished, this, [this, reply]() {
                handlePlantListReply(reply);
        });
}

void GrowattFetcher::fetchRealtimeData(const QString &plantId)
{
        QUrl url(mServerUrl + GROWATT_PLANT_DETAIL_API);
        QUrlQuery query;
        query.addQueryItem("plantId", plantId);
        query.addQueryItem("type", "1");
        query.addQueryItem("date", QDate::currentDate().toString("yyyy-MM-dd"));
        url.setQuery(query);
        QNetworkRequest req(url);
        req.setRawHeader("User-Agent", QByteArray(GROWATT_UA));
        req.setRawHeader("Accept", QByteArray(GROWATT_ACCEPT));
        QNetworkReply * reply = mNet->get(req);
        connect(reply, &QNetworkReply::finished, this, [this, reply]() {
                handleRealtimeReply(reply);
        });
}

void GrowattFetcher::fetchEnergyData(const QString &plantId, const QDate &date)
{
        QUrl url(mServerUrl + GROWATT_PLANT_DETAIL_API);
        QUrlQuery query;
        query.addQueryItem("plantId", plantId);
        query.addQueryItem("type", "3");
        query.addQueryItem("date", date.toString("yyyy-MM-dd"));
        url.setQuery(query);
        QNetworkRequest req(url);
        req.setRawHeader("User-Agent", QByteArray(GROWATT_UA));
        req.setRawHeader("Accept", QByteArray(GROWATT_ACCEPT));
        QNetworkReply * reply = mNet->get(req);
        connect(reply, &QNetworkReply::finished, this, [this, reply]() {
                handleEnergyReply(reply);
        });
}

void GrowattFetcher::startPolling(int intervalSecs)
{
        mPollTimer->start(intervalSecs * 1000);
}

void GrowattFetcher::stopPolling()
{
        mPollTimer->stop();
}

QByteArray GrowattFetcher::deriveKey()
{
        QByteArray material;
        QFile macFile("/sys/class/net/wlan0/address");
        if (macFile.open(QIODevice::ReadOnly)) {
                material += macFile.readAll().trimmed();
                macFile.close();
        }
        QFile cpuFile("/proc/cpuinfo");
        if (cpuFile.open(QIODevice::ReadOnly)) {
                while (!cpuFile.atEnd()) {
                        QString line = QString::fromUtf8(cpuFile.readLine());
                        if (line.startsWith("Serial")) {
                                material += line.section(':', 1).trimmed().toLatin1();
                                break;
                        }
                }
                cpuFile.close();
        }
        if (material.isEmpty()) { material = QByteArray("power-window-growatt-fallback"); }
        return QCryptographicHash::hash(material, QCryptographicHash::Sha256);
}

QString GrowattFetcher::encryptPassword(const QString &password)
{
        if (password.isEmpty()) { return {}; }
        QByteArray key = deriveKey();
        const int ivLen = 12;
        const int tagLen = 16;
        QByteArray iv(ivLen, 0);
        RAND_bytes(reinterpret_cast<uint8_t *>(iv.data()), ivLen);

        EVP_CIPHER_CTX * ctx = EVP_CIPHER_CTX_new();
        EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), nullptr, nullptr, nullptr);
        EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, ivLen, nullptr);
        EVP_EncryptInit_ex(ctx, nullptr, nullptr,
                           reinterpret_cast<const uint8_t *>(key.constData()),
                           reinterpret_cast<const uint8_t *>(iv.constData()));
        
        QByteArray plainText = password.toUtf8();
        QByteArray cipherText(plainText.size() + 16, 0);
        int outLen = 0;
        int totalLen = 0;
        EVP_EncryptUpdate(ctx, reinterpret_cast<uint8_t *>(cipherText.data()), &outLen,
                          reinterpret_cast<const uint8_t *>(plainText.constData()), plainText.size());
        totalLen = outLen;
        EVP_EncryptFinal_ex(ctx, reinterpret_cast<uint8_t *>(cipherText.data()) + totalLen, &outLen);
        totalLen += outLen;
        cipherText.resize(totalLen);

        QByteArray tag(tagLen, 0);
        EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, tagLen, reinterpret_cast<uint8_t *>(tag.data()));
        EVP_CIPHER_CTX_free(ctx);

        return QString::fromLatin1((iv + cipherText + tag).toBase64());
}

QString GrowattFetcher::decryptPassword(const QString &encPwd)
{
        if (encPwd.isEmpty()) { return {}; }
        QByteArray key = deriveKey();
        const int ivLen = 12;
        const int tagLen = 16;
        QByteArray data = QByteArray::fromBase64(encPwd.toLatin1());
        if (data.size() < ivLen + tagLen) { return {}; }

        QByteArray iv = data.left(ivLen);
        QByteArray tag = data.right(tagLen);
        QByteArray ct = data.mid(ivLen, data.size() - ivLen - tagLen);

        EVP_CIPHER_CTX * ctx = EVP_CIPHER_CTX_new();
        EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), nullptr, nullptr, nullptr);
        EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, ivLen, nullptr);
        EVP_DecryptInit_ex(ctx, nullptr, nullptr,
                           reinterpret_cast<const uint8_t *>(key.constData()),
                           reinterpret_cast<const uint8_t *>(iv.constData()));

        QByteArray plainText(ct.size(), 0);
        int outLen = 0;
        int totalLen = 0;
        EVP_DecryptUpdate(ctx, reinterpret_cast<uint8_t *>(plainText.data()), &outLen,
                          reinterpret_cast<const uint8_t *>(ct.constData()), ct.size());
        totalLen = outLen;
        EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, tagLen,
                            reinterpret_cast<uint8_t *>(tag.data()));
        int ret = EVP_DecryptFinal_ex(ctx, reinterpret_cast<uint8_t *>(plainText.data()) + totalLen, &outLen);
        EVP_CIPHER_CTX_free(ctx);
        if (ret != 1) {
                qWarning("Failed to decrypt password");
                return {};
        }
        totalLen += outLen;
        plainText.resize(totalLen);
        return QString::fromUtf8(plainText);
}

void GrowattFetcher::performLogin(const QString &user, const QString &pass)
{
        QUrl url(mServerUrl + GROWATT_LOGIN_API);
        QNetworkRequest req(url);
        req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
        req.setRawHeader("User-Agent", QByteArray(GROWATT_UA));
        req.setRawHeader("Accept", QByteArray(GROWATT_ACCEPT));
        QUrlQuery body;
        body.addQueryItem("userName", user);
        body.addQueryItem("password", hashPassword(pass));

        QNetworkReply * reply = mNet->post(req, body.toString(QUrl::FullyEncoded).toUtf8());
        connect(reply, &QNetworkReply::finished, this, [this, reply]() {
                reply->deleteLater();
                if (reply->error() != QNetworkReply::NoError) {
                        mLoggedIn = false;
                        emit loginResult(false, "Login fehlgeschlagen: " + reply->errorString());
                        return;
                }
                QByteArray data = reply->readAll();
                QJsonDocument doc = QJsonDocument::fromJson(data);
                if (doc.isObject()) {
                        QJsonObject obj = doc.object();
                        int result = obj["result"].toInt();
                        if (result == 1 || obj["success"].toBool()) {
                                mLoggedIn = true;
                                mUserId = obj["back"].toObject()["user"].toObject()["id"].toString();
                                saveSettings();
                                emit loginResult(true, "Angemeldet");
                                return;
                        }
                }
                qDebug() << "Login response: " << data;
                emit loginResult(false, "Login fehlgeschlagen: Falscher Antwortcode");
        });
}

void GrowattFetcher::handlePlantListReply(QNetworkReply * reply)
{
        reply->deleteLater();
        if (reply->error() != QNetworkReply::NoError) {
                emit errorOccurred("Konnte Kraftwerkliste nicht abrufen");
                return;
        }
        QByteArray responseData = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(responseData);
        if (doc.isObject()) {
                QJsonObject obj = doc.object();
                if (obj["result"].toInt() == 1 && obj.contains("back")) {
                        QJsonArray plants = obj["back"].toArray();
                        emit plantListReady(plants);
                        return;
                }
        }
        qWarning() << "Failed to parse plant list: " << responseData;
        emit errorOccurred("Konnte Kraftwerkliste nicht lesen");
}

void GrowattFetcher::handleRealtimeReply(QNetworkReply * reply)
{
        reply->deleteLater();
        if (reply->error() != QNetworkReply::NoError) { return; }
        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        if (!doc.isObject()) { return; }
        QJsonObject obj = doc.object();
        if (obj["result"].toInt() != 1) { return; }

        QJsonObject plantData = obj["back"].toObject()["plantData"].toObject();
        GrowattRealtimeData data;
        data.currentPower = plantData["currentPower"].toDouble();
        data.todayEnergy = plantData["todayEnergy"].toDouble();
        data.totalEnergy = plantData["totalEnergy"].toDouble();
        data.batterySoc = plantData["batterySoc"].toDouble();
        data.batteryChargePower = plantData["batteryChargePower"].toDouble();
        data.batteryDischargePower = plantData["batteryDischargePower"].toDouble();
        data.gridImportPower = plantData["gridImportPower"].toDouble();
        data.gridExportPower = plantData["gridExportPower"].toDouble();
        data.loadPower = plantData["loadPower"].toDouble();
        data.plantName = plantData["plantName"].toString();
        data.plantStatus = plantData["status"].toInt() == 1 ? "online" : "offline";
        data.co2Saved = plantData["co2Saved"].toDouble();
        data.revenue = plantData["revenue"].toDouble();
        emit realtimeDataReady(data);
}

void GrowattFetcher::handleEnergyReply(QNetworkReply * reply)
{
        reply->deleteLater();
        if (reply->error() != QNetworkReply::NoError) { return; }
        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        if (!doc.isObject() || doc.object()["result"].toInt() != 1) { return; }
        QJsonArray ja = doc.object()["back"].toObject()["chartData"].toArray();
        QVector<PvStatsData> stats;
        for (const auto &val : ja) {
                QJsonObject obj = val.toObject();
                PvStatsData data;
                data.datetime = QDateTime::fromString(obj["dateTime"].toString(), Qt::ISODate);
                data.pvOutput = obj["pvOutput"].toDouble();
                data.loadConsumption = obj["loadConsumption"].toDouble();
                data.exportToGrid = obj["exportToGrid"].toDouble();
                data.importFromGrid = obj["importFromGrid"].toDouble();
                data.charging = obj["charging"].toDouble();
                data.discharging = obj["discharging"].toDouble();
                stats.append(data);
        }
        emit energyDataReady(stats);
}
