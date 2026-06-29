#ifndef GROWATT_FETCHER_HPP_
#define GROWATT_FETCHER_HPP_

#include <QObject>
#include <QString>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonArray>
#include <QJsonObject>
#include <QSettings>
#include <QCryptographicHash>
#include "GrowattData.hpp"
#include "PvStatsData.hpp"

class GrowattFetcher : public QObject
{
        Q_OBJECT
public:
        explicit GrowattFetcher(QObject * parent = nullptr);

        void loadSettings();
        void saveSettings();
        void clearSettings();

        void login(const QString &username, const QString &pwd, const QString &serverUrl);
        void fetchPlantList();
        void fetchRealtimeData(const QString &plantId);
        void fetchEnergyData(const QString &plantId, const QDate &date);

        void startPolling(int intervalSecs = 30);
        void stopPolling();

        QString username() const { return mUsername; }
        QString password() const { return mPassword; }
        QString serverUrl() const { return mServerUrl; }
        QString currentPlantId() const { return mCurrentPlantId; }
        bool isLoggedIn() const { return mLoggedIn; }

signals:
        void loginResult(bool success, const QString &msg);
        void plantListReady(const QJsonArray &plants);
        void realtimeDataReady(const GrowattRealtimeData &data);
        void energyDataReady(const QVector<PvStatsData> &data);
        void errorOccurred(const QString &error);

private:
        QNetworkAccessManager * mNet;
        QTimer * mPollTimer;
        QString mUsername;
        QString mPassword;
        QString mServerUrl;
        QString mCurrentPlantId;
        QString mUserId;
        bool mLoggedIn;

        QByteArray deriveKey();
        QString hashPassword(const QString &password) const { return QString(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Md5).toHex()); }
        QString encryptPassword(const QString &password);
        QString decryptPassword(const QString &encPwd);

        void performLogin(const QString &user, const QString &pass);
        void handlePlantListReply(QNetworkReply * reply);
        void handleRealtimeReply(QNetworkReply * reply);
        void handleEnergyReply(QNetworkReply * reply);
};

#endif // GROWATT_FETCHER_HPP_