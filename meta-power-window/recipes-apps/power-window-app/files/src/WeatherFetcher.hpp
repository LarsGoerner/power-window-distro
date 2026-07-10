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
        Q_PROPERTY(double feelTemperature READ feelTemperature NOTIFY currentWeatherChanged)
        Q_PROPERTY(double humidity READ humidity NOTIFY currentWeatherChanged)
        Q_PROPERTY(int pressure READ pressure NOTIFY currentWeatherChanged)
        Q_PROPERTY(double windSpeed READ windSpeed NOTIFY currentWeatherChanged)
        Q_PROPERTY(double windGust READ windGust NOTIFY currentWeatherChanged)
        Q_PROPERTY(int windDirection READ windDirection NOTIFY currentWeatherChanged)
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
        double feelTemperature() const;
        double humidity() const;
        int pressure() const;
        double windSpeed() const;
        double windGust() const;
        int windDirection() const;
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
        double mFeelTemperature = 0.0;
        double mHumidity = 0.0;
        int mPressure = 0;
        double mWindSpeed = 0.0;
        double mWindGust = 0.0;
        int mWindDirection = 0;
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