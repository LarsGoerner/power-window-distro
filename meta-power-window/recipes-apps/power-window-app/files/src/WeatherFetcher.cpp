#include <QObject>
#include <QString>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QProcess>
#include "WeatherFetcher.hpp"

#define POLL_DELAY_MS                   60000
#define POLL_RETRY_DELAY_SECS           300
#define CUR_WETHER_POLL_DELAY_SECS      3600
#define IP_API_URL              "http://ip-api.com/json/"
#define OWM_CUR_WEATHER_API_TMP_URL "https://api.openweathermap.org/data/2.5/weather" \
                                    "?lat=%1&lon=%2&appid=%3&units=metric"
#define OWM_FORECAST_API_TMP_URL "https://api.openweathermap.org/data/2.5/forecast" \
                                    "?lat=%1&lon=%2&appid=%3&units=metric"
#define TZ_DIR          "/usr/share/zoneinfo/"
#define TZ_LOCAL        "/etc/localtime"

WeatherFetcher::WeatherFetcher(QObject * parent) : QObject(parent)
{
        mNet = new QNetworkAccessManager(this);
        mApiKey = qEnvironmentVariable("OWM_API_KEY");
        mPollTimer = new QTimer(this);
        connect(mPollTimer, &QTimer::timeout, this, &WeatherFetcher::poll);
}

void WeatherFetcher::start()
{
        mNextRefresh = QDateTime();
        mPollTimer->start(POLL_DELAY_MS);
        poll();
}

double WeatherFetcher::temperature() const { return mTemperature; }
double WeatherFetcher::feelTemperature() const { return mFeelTemperature; }
double WeatherFetcher::humidity() const { return mHumidity; }
int WeatherFetcher::pressure() const { return mPressure; }
double WeatherFetcher::windSpeed() const { return mWindSpeed; }
double WeatherFetcher::windGust() const { return mWindGust; }
int WeatherFetcher::windDirection() const { return mWindDirection; }
QString WeatherFetcher::description() const { return mDescription; }
QString WeatherFetcher::iconCode() const { return mIconCode; }
QString WeatherFetcher::cityName() const { return mCity; }
QVariantList WeatherFetcher::forecast() const { return mForecast; }
QVariantList WeatherFetcher::todayForecast() const { return mTodayForecast; }
bool WeatherFetcher::ready() const { return mReady; }

void WeatherFetcher::refresh()
{
        mNextRefresh = QDateTime();
        poll();
}

void WeatherFetcher::poll()
{
        QDateTime now = QDateTime::currentDateTimeUtc();
        if (mNextRefresh.isValid() && now < mNextRefresh) { return; }
        if (mCity.isEmpty()) { fetchLocation(); }
        else {
                fetchCurrentWeather();
                fetchWeatherForecast();
        }
}

void WeatherFetcher::fetchLocation()
{
        QNetworkReply * reply = mNet->get(QNetworkRequest(QUrl(IP_API_URL)));
        connect(reply, &QNetworkReply::finished, this, [this, reply]() {
                reply->deleteLater();
                if (reply->error() != QNetworkReply::NoError) {
                        mNextRefresh = QDateTime::currentDateTimeUtc().addSecs(POLL_RETRY_DELAY_SECS);
                        return;
                }
                QJsonObject obj = QJsonDocument::fromJson(reply->readAll()).object();
                if (obj["status"].toString() == "success") {
                        mLat = obj["lat"].toDouble();
                        mLon = obj["lon"].toDouble();
                        mCity = obj["city"].toString();
                        QString tz = obj["timezone"].toString();
                        if (!tz.isEmpty()) { QProcess::execute("ln", { "-sf", TZ_DIR + tz, TZ_LOCAL }); }
                        fetchCurrentWeather();
                        fetchWeatherForecast();
                } else {
                        mNextRefresh = QDateTime::currentDateTimeUtc().addSecs(POLL_RETRY_DELAY_SECS);
                }
        });
}

void WeatherFetcher::fetchCurrentWeather()
{
        QString url = QString(OWM_CUR_WEATHER_API_TMP_URL).arg(mLat).arg(mLon).arg(mApiKey);
        QNetworkReply * reply = mNet->get(QNetworkRequest(QUrl(url)));
        connect(reply, &QNetworkReply::finished, this, [this, reply]() {
                reply->deleteLater();
                if (reply->error() != QNetworkReply::NoError) {
                        mNextRefresh = QDateTime::currentDateTimeUtc().addSecs(POLL_RETRY_DELAY_SECS);
                        return;
                }
                parseCurrentWeather(reply->readAll());
        });
}

void WeatherFetcher::fetchWeatherForecast()
{
        QString url = QString(OWM_FORECAST_API_TMP_URL).arg(mLat).arg(mLon).arg(mApiKey);
        QNetworkReply * reply = mNet->get(QNetworkRequest(QUrl(url)));
        connect(reply, &QNetworkReply::finished, this, [this, reply]() {
                reply->deleteLater();
                if (reply->error() != QNetworkReply::NoError) { return; }
                parseWeatherForecast(reply->readAll());
        });
}

void WeatherFetcher::parseCurrentWeather(const QByteArray &data)
{
        QJsonObject obj = QJsonDocument::fromJson(data).object();
        if (obj.isEmpty()) { return; }
        
        mTemperature = obj["main"].toObject()["temp"].toDouble();
        mFeelTemperature = obj["main"].toObject()["feels_like"].toDouble();
        mHumidity = obj["main"].toObject()["humidity"].toInt();
        mPressure = obj["main"].toObject()["pressure"].toInt();
        mWindSpeed = obj["wind"].toObject()["speed"].toDouble();
        mWindGust = obj["wind"].toObject()["gust"].toDouble();
        mWindDirection = obj["wind"].toObject()["deg"].toInt();
        mDescription = obj["weather"].toArray()[0].toObject()["description"].toString();
        mIconCode = obj["weather"].toArray()[0].toObject()["icon"].toString();

        qint64 dt = obj["dt"].toInt();
        mNextRefresh = QDateTime::fromSecsSinceEpoch(dt).addSecs(CUR_WETHER_POLL_DELAY_SECS);
        mReady = true;
        emit currentWeatherChanged();
}

void WeatherFetcher::parseWeatherForecast(const QByteArray &data)
{
        QJsonObject obj = QJsonDocument::fromJson(data).object();
        QJsonArray list = obj["list"].toArray();
        if (list.isEmpty()) { return; }

        // today's 3-hour intervals
        QString today = QDate::currentDate().toString("yyyy-MM-dd");
        mTodayForecast.clear();

        // aggregate 3-hour intervals into daily buckets
        QMap<QString, QJsonArray> days;
        for (const QJsonValue &val : list) {
                QJsonObject item = val.toObject();
                QString dtTxt = item["dt_txt"].toString();
                QString date = dtTxt.left(10);
                days[date].append(item);
                if (dtTxt.startsWith(today)) {
                        QVariantMap e;
                        e["time"] = dtTxt.mid(11, 5);
                        e["temp"] = item["main"].toObject()["temp"].toDouble();
                        e["tempMin"] = item["main"].toObject()["temp_min"].toDouble();
                        e["tempMax"] = item["main"].toObject()["temp_max"].toDouble();
                        e["rain"] = item["rain"].toObject()["3h"].toDouble(0.0);
                        mTodayForecast.append(e);
                }
        }

        // aggregate daily min/max + icon
        QVariantList result;
        QStringList keys = days.keys();
        std::sort(keys.begin(), keys.end());
        for (const QString &date : keys) {
                const QJsonArray &items = days[date];
                double minTemp = 999.0;
                double maxTemp = -999.0;
                double totalRain = 0.0;
                QMap<QString, int> iconCount;
                for (const QJsonValue &val : items) {
                        QJsonObject item = val.toObject();
                        double t = item["main"].toObject()["temp"].toDouble();
                        if (t < minTemp) { minTemp = t; }
                        if (t > maxTemp) { maxTemp = t; }
                        QString icon = item["weather"].toArray()[0].toObject()["icon"].toString();
                        iconCount[icon]++;
                        totalRain += item["rain"].toObject()["3h"].toDouble(0.0);
                }

                // most common icon
                QString icon;
                int maxCount = 0;
                for (auto it = iconCount.constBegin(); it != iconCount.constEnd(); it++) {
                        if (it.value() > maxCount) { maxCount = it.value(); icon = it.key(); }
                }

                QVariantMap day;
                day["day"] = QDate::fromString(date, "yyyy-MM-dd").toString("ddd");
                day["tempMin"] = minTemp;
                day["tempMax"] = maxTemp;
                day["icon"] = icon;
                day["rain"] = totalRain;
                result.append(day);
        }
        mForecast = result;
        emit forecastChanged();
}