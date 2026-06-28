#ifndef WEATHER_FETCHER_HPP_
#define WEATHER_FETCHER_HPP_

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QVector>
#include <QTimer>
#include <QNetworkAccessManager>

struct CurrentWeather
{
        double temp;
        double feelsLikeTemp;
        int humidity;
        int weatherCode;
        double windSpeed;
        QString description;
        QString icon;
        QString cityName;
};

struct WeatherForecastPoint
{
        QDateTime dt;
        double temp;
        double tempMin;
        double tempMax;
        double rain;
};

class WeatherFetcher : public QObject
{
        Q_OBJECT
public:
        explicit WeatherFetcher(QObject * parent = nullptr);
        void start();

signals:
        void locationReady(double lat, double lon, const QString & city);
        void currentWeatherReady(const CurrentWeather & data);
        void forecastReady(const QVector<WeatherForecastPoint> & points);
        void errorOccurred(const QString & message);

private:
        QNetworkAccessManager * mNet;
        QString mApiKey;
        QTimer * mPollTimer;
        QDateTime mNextCurrentWeatherRefresh;
        QDateTime mNextWeatherForecastRefresh;
        double mLat;
        double mLon;
        QString mCity;
        bool mLatLonKnown = false;

        void poll();
        void fetchLocation();
        void fetchCurrentWeather();
        void parseCurrentWeather(const QJsonObject & obj);
        void fetchWeatherForecast();
        void parseWeatherForecast(const QJsonObject & obj);
};

#endif // WEATHER_FETCHER_HPP_