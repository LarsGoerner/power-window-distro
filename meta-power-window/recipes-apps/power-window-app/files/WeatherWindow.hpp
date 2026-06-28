#ifndef WEATHERWINDOW_HPP_
#define WEATHERWINDOW_HPP_

#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSlider>
#include <QLineEdit>
#include <QPushButton>
#include <QDebug>
#include <QFile>
#include <QPainter>
#include <QChartView>
#include <QtCharts>
#include <QTextStream>
#include <QTabBar>
#include <QMap>
#include <QPointF>
#include <QVector>
#include <QPixmap>
#include "BaseWindow.hpp"
#include "WeatherFetcher.hpp"

class WeatherWindow : public BaseWindow
{
        Q_OBJECT
public:
        WeatherWindow(QWidget * parent = nullptr);

private slots:
        void updateChart(int idx);
        void onCurrentWeather(const CurrentWeather & data);
        void onForecastReady(const QVector<WeatherForecastPoint> & points);
        void onWeatherError(const QString & msg);

private:
        enum WeatherDataType { WEATHER_TODAY = 0, WEATHER_5_DAY };

        QLabel * mCityLabel;
        QLabel * mTempLabel;
        QLabel * mDescLabel;
        QLabel * mHumidityLabel;
        QLabel * mWindLabel;
        QLabel * mIconLabel;

        QTabBar * mTabBar;
        QChart * mChart;
        QValueAxis * mTempAxis;
        QValueAxis * mRainAxis;
        QDateTimeAxis * mTimeAxis;
        QChartView * mChartView;
        QMap<int, QVector<QPair<QDateTime, float>>> mRainAmountDatasets;
        QMap<int, QVector<QPair<QDateTime, float>>> mHighTempDatasets;
        QMap<int, QVector<QPair<QDateTime, float>>> mLowTempDatasets;
        
        WeatherFetcher * mWeatherFetcher;

        void populateTodayData(const QVector<WeatherForecastPoint> & points);
        void populate5DayData(const QVector<WeatherForecastPoint> & points);
        void addSeriesToChart(const QVector<QPair<QDateTime, float>> & data,
                              QAbstractAxis * axis, const QString & name,
                              Qt::GlobalColor color);
};

#endif // WEATHERWINDOW_HPP_