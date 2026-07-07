#ifndef WEATHER_FETCHER_HPP_
#define WEATHER_FETCHER_HPP_

#include <QObject>
#include <QString>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QDateTime>

class WeatherFetcher : public QObject
{
        Q_OBJECT
        Q_PROPERTY(double temperature READ temperature NOTIFY currentWeatherChanged)
        Q_PROPERTY(double humidity READ humidity NOTIFY currentWeatherChanged)
        Q_PROPERTY(QString description READ description NOTIFY currentWeatherChanged)
        Q_PROPERTY(QString iconCode READ iconCode NOTIFY currentWeatherChanged)
        Q_PROPERTY(QString cityName READ cityName NOTIFY currentWeatherChanged)
        Q_PROPERTY(bool ready READ ready NOTIFY currentWeatherChanged)
        Q_PROPERTY(QVariantList forecast READ forecast NOTIFY forecastChanged)
        Q_PROPERTY(QVariantList todayForecast READ todayForecast NOTIFY forecastChanged)

public:
        explicit WeatherFetcher(QObject * parent = nullptr);
        void start();
        double temperature() const;
        double humidity() const;
        QString description() const;
        QString iconCode() const;
        QString cityName() const;
        QVariantList forecast() const;
        QVariantList todayForecast() const;
        bool ready() const;
        Q_INVOKABLE void refresh();

signals:
        void currentWeatherChanged();
        void forecastChanged();

private:
        QNetworkAccessManager * mNet;
        QTimer * mPollTimer;
        QString mApiKey;
        QDateTime mNextRefresh;
        double mLat = 0;
        double mLon = 0;
        QString mCity;
        double mTemperature = 0.0;
        double mHumidity = 0.0;
        QString mDescription;
        QString mIconCode;
        bool mReady = false;
        QVariantList mForecast;
        QVariantList mTodayForecast;

        void poll();
        void fetchLocation();
        void fetchCurrentWeather();
        void fetchWeatherForecast();
        void parseCurrentWeather(const QByteArray &data);
        void parseWeatherForecast(const QByteArray &data);
};

#endif // WEATHER_FETCHER_HPP_