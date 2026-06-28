#include <QObject>
#include <QString>
#include <QDateTime>
#include <QVector>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include "WeatherFetcher.hpp"

WeatherFetcher::WeatherFetcher(QObject * parent) : QObject(parent)
{
        mNet = new QNetworkAccessManager(this);
        mApiKey = qEnvironmentVariable("OWM_API_KEY");
        mPollTimer = new QTimer(this);
        connect(mPollTimer, &QTimer::timeout, this, &WeatherFetcher::poll);
}

void WeatherFetcher::start()
{
        mNextWeatherForecastRefresh = QDateTime(); // set as invalid time
        mNextCurrentWeatherRefresh = QDateTime();
        mPollTimer->start(60000); // check every 60 secs
        poll();
}

void WeatherFetcher::poll()
{
        QDateTime now = QDateTime::currentDateTimeUtc();

        bool needCurrent = !mNextCurrentWeatherRefresh.isValid() || now > mNextCurrentWeatherRefresh;
        bool needForecast = !mNextWeatherForecastRefresh.isValid() || now > mNextWeatherForecastRefresh;

        if (!mLatLonKnown && (needCurrent || needForecast)) {
                fetchLocation();
                return;
        }

        if (needCurrent) { fetchCurrentWeather(); }
        if (needForecast) { fetchWeatherForecast(); }
}

void WeatherFetcher::fetchLocation()
{
        QNetworkReply * reply = mNet->get(QNetworkRequest(QUrl("http://ip-api.com/json/")));
        connect(reply, &QNetworkReply::finished, this, [this, reply]() {
                reply->deleteLater();
                if (reply->error() != QNetworkReply::NoError) {
                        emit errorOccurred("Standortabfrage fehlgeschlagen: " + reply->errorString());
                        mNextCurrentWeatherRefresh = QDateTime::currentDateTimeUtc().addSecs(300); // retry in 5 min
                        return;
                }
                QJsonObject obj = QJsonDocument::fromJson(reply->readAll()).object();
                if (obj["status"].toString() == "success") {
                        mLat = obj["lat"].toDouble();
                        mLon = obj["lon"].toDouble();
                        mCity = obj["city"].toString();
                        mLatLonKnown = true;
                        emit locationReady(mLat, mLon, mCity);
                        poll();
                } else {
                        emit errorOccurred("Standortabfrage fehlgeschlagen");
                        mNextCurrentWeatherRefresh = QDateTime::currentDateTimeUtc().addSecs(300); // retry in 5 min
                }
        });
}

void WeatherFetcher::fetchCurrentWeather()
{
        QString url = QString("https://api.openweathermap.org/data/2.5/weather"
                              "?lat=%1&lon=%2&appid=%3&units=metric"
                             ).arg(mLat).arg(mLon).arg(mApiKey);

        QNetworkReply * reply = mNet->get(QNetworkRequest(QUrl(url)));
        connect(reply, &QNetworkReply::finished, this, [this, reply]() {
                reply->deleteLater();
                if (reply->error() != QNetworkReply::NoError) {
                        emit errorOccurred("Konnte das aktuelle Wetter nicht abrufen: " + reply->errorString());
                        mNextCurrentWeatherRefresh = QDateTime::currentDateTimeUtc().addSecs(300); // retry in 5 min
                        return;
                }
                QJsonObject obj = QJsonDocument::fromJson(reply->readAll()).object();
                parseCurrentWeather(obj);
        });
}

void WeatherFetcher::parseCurrentWeather(const QJsonObject & obj)
{
        CurrentWeather curWeather;
        curWeather.temp = obj["main"]["temp"].toDouble();
        curWeather.feelsLikeTemp = obj["main"]["feels_like"].toDouble();
        curWeather.humidity = obj["main"]["humidity"].toInt();
        curWeather.windSpeed = obj["main"]["speed"].toDouble();
        curWeather.weatherCode = obj["weather"][0]["id"].toInt();
        curWeather.description = obj["weather"][0]["description"].toString();
        curWeather.icon = obj["weather"][0]["icon"].toString();
        curWeather.cityName = mCity;

        qint64 owmDt = obj["dt"].toInt();
        mNextCurrentWeatherRefresh = QDateTime::fromSecsSinceEpoch(owmDt).addSecs(3600); // refresh after 1 hour

        emit currentWeatherReady(curWeather);
}

void WeatherFetcher::fetchWeatherForecast()
{
        QString url = QString("https://api.openweathermap.org/data/2.5/forecast"
                              "?lat=%1&lon=%2&appid=%3&units=metric"
                             ).arg(mLat).arg(mLon).arg(mApiKey);

        QNetworkReply * reply = mNet->get(QNetworkRequest(QUrl(url)));
        connect(reply, &QNetworkReply::finished, this, [this, reply]() {
                reply->deleteLater();
                if (reply->error() != QNetworkReply::NoError) {
                        emit errorOccurred("Konnte die Wettervorhersage nicht abrufen: " + reply->errorString());
                        mNextWeatherForecastRefresh = QDateTime::currentDateTimeUtc().addSecs(300); // retry in 5 min
                        return;
                }
                QJsonObject obj = QJsonDocument::fromJson(reply->readAll()).object();
                parseWeatherForecast(obj);
        });
}

void WeatherFetcher::parseWeatherForecast(const QJsonObject & obj)
{
        QVector<WeatherForecastPoint> points;
        QJsonArray list = obj["list"].toArray();

        for (const QJsonValue & val : list) {
                QJsonObject item = val.toObject();
                WeatherForecastPoint fcp;
                fcp.dt = QDateTime::fromSecsSinceEpoch(item["dt"].toInt());
                fcp.temp = item["main"].toObject()["temp"].toDouble();
                fcp.tempMin = item["main"].toObject()["temp_min"].toDouble();
                fcp.tempMax = item["main"].toObject()["temp_max"].toDouble();
                fcp.rain = item["rain"].toObject()["3h"].toDouble(0.0);
                points.append(fcp);
        }

        if (points.isEmpty()) {
                emit errorOccurred("Keine Wettervorhersage verfügbar");
                mNextWeatherForecastRefresh = QDateTime::currentDateTimeUtc().addSecs(600); // retry in 10 min
                return;
        }
        mNextWeatherForecastRefresh = points.first().dt;
        emit forecastReady(points);
}
