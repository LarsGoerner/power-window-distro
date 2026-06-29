
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
#include "BaseWindow.hpp"
#include "WeatherWindow.hpp"

WeatherWindow::WeatherWindow(QWidget * parent) : BaseWindow(parent)
{
        QVBoxLayout * mainLayout = new QVBoxLayout(this);
        mainLayout->setContentsMargins(10, 10, 10, 10);
        mainLayout->setSpacing(8);

        // CURRENT WEATHER CARD
        QWidget * card = new QWidget(this);
        card->setObjectName("weatherCard");
        QHBoxLayout * cardLayout = new QHBoxLayout(card);
        cardLayout->setContentsMargins(0, 0, 0, 0);

        QVBoxLayout * leftCol = new QVBoxLayout(card);
        mIconLabel = new QLabel(card);
        mIconLabel->setFixedSize(64, 64);
        leftCol->addWidget(mIconLabel, 0, Qt::AlignTop);
        mCityLabel = new QLabel("--", card);
        mCityLabel->setObjectName("cityLabel");
        leftCol->addWidget(mCityLabel);
        mDescLabel = new QLabel("", card);
        mDescLabel->setObjectName("descLabel");
        leftCol->addWidget(mDescLabel);
        cardLayout->addLayout(leftCol, 1);

        QVBoxLayout * rightCol = new QVBoxLayout(card);
        mTempLabel = new QLabel("--", card);
        mTempLabel->setAlignment(Qt::AlignRight);
        rightCol->addWidget(mTempLabel);
        mHumidityLabel = new QLabel("", card);
        mHumidityLabel->setAlignment(Qt::AlignRight);
        rightCol->addWidget(mHumidityLabel);
        mWindLabel = new QLabel("", card);
        mWindLabel->setAlignment(Qt::AlignRight);
        rightCol->addWidget(mWindLabel);
        cardLayout->addLayout(rightCol, 1);

        mainLayout->addWidget(card);

        // WEATHER CHART
        //      MENU BAR
        mTabBar = new QTabBar(this);
        mTabBar->addTab("Heute");
        mTabBar->addTab("5 Tage Trend");
        mainLayout->addWidget(mTabBar);

        //      CHART
        mChart = new QChart();
        mChart->setMargins(QMargins(0, 0, 0, 0));
        mChart->setTitle("");
        mChart->legend()->setVisible(true);
        mChart->legend()->setAlignment(Qt::AlignRight);

        mRainAxis = new QValueAxis(this);
        mRainAxis->setTitleText("Regen [mm]");
        mChart->addAxis(mRainAxis, Qt::AlignRight);

        mTempAxis = new QValueAxis(this);
        mTempAxis->setTitleText("Temperatur [°C]");
        mChart->addAxis(mTempAxis, Qt::AlignLeft);

        mTimeAxis = new QDateTimeAxis(this);
        mTimeAxis->setFormat("HH:mm");
        mTimeAxis->setTitleText("Uhrzeit");
        mChart->addAxis(mTimeAxis, Qt::AlignBottom);

        mChartView = new QChartView(mChart, this);
        mChartView->setRenderHint(QPainter::Antialiasing);
        mChartView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        mainLayout->addWidget(mChartView, 1);

        connect(mTabBar, &QTabBar::currentChanged, this, &WeatherWindow::updateChart);
        
        // FETCHER
        mWeatherFetcher = new WeatherFetcher(this);
        connect(mWeatherFetcher, &WeatherFetcher::currentWeatherReady, this, &WeatherWindow::onCurrentWeather);
        connect(mWeatherFetcher, &WeatherFetcher::forecastReady, this, &WeatherWindow::onForecastReady);
        connect(mWeatherFetcher, &WeatherFetcher::errorOccurred, this, &WeatherWindow::onWeatherError);
        mWeatherFetcher->start();
}

void WeatherWindow::updateChart(int idx)
{
        mChart->removeAllSeries();

        if (idx == WEATHER_TODAY) {
                mTimeAxis->setFormat("HH:mm");
                mTimeAxis->setTitleText("Uhrzeit");
        } else if (idx == WEATHER_5_DAY) {
                mTimeAxis->setFormat("dd.MM.yy");
                mTimeAxis->setTitleText("Datum");
        } else {
                qWarning() << "Unknown WeatherDataType";
                return;
        }

        QLineSeries * rainSeries = new QLineSeries();
        rainSeries->setName("Regen");
        const auto &rainDataPoints = mRainAmountDatasets[idx];
        if (rainDataPoints.isEmpty()) {
                qWarning() << "No rain data";
                return;
        }
        QDateTime minTime = rainDataPoints.first().first;
        QDateTime maxTime = rainDataPoints.first().first;
        double maxRain = 0.0;
        for (const auto &point : rainDataPoints) {
                QDateTime dt = point.first;
                float val = point.second;
                if (val > maxRain) { maxRain = val; }

                rainSeries->append(dt.toMSecsSinceEpoch(), val);

                if (dt < minTime) { minTime = dt; }
                if (dt > maxTime) { maxTime = dt; }
        }
        mChart->addSeries(rainSeries);
        mRainAxis->setRange(0, maxRain + 1.0);
        rainSeries->attachAxis(mTimeAxis);
        rainSeries->attachAxis(mRainAxis);

        QLineSeries * highTempSeries = new QLineSeries();
        highTempSeries->setName("Max T");
        const auto &highTempDataPoints = mHighTempDatasets[idx];
        if (highTempDataPoints.isEmpty()) {
                qWarning() << "No high temp data";
                return;
        }
        double maxTemp = -999.0;
        for (const auto &point : highTempDataPoints) {
                QDateTime dt = point.first;
                float val = point.second;

                if (val > maxTemp) { maxTemp = val; }
                highTempSeries->append(dt.toMSecsSinceEpoch(), val);

                if (dt < minTime) { minTime = dt; }
                if (dt > maxTime) { maxTime = dt; }
        }
        mChart->addSeries(highTempSeries);
        highTempSeries->attachAxis(mTimeAxis);
        highTempSeries->attachAxis(mTempAxis);

        QLineSeries * lowTempSeries = new QLineSeries();
        lowTempSeries->setName("Min T");
        const auto &lowTempDataPoints = mLowTempDatasets[idx];
        if (lowTempDataPoints.isEmpty()) {
                qWarning() << "No low temp data";
                return;
        }
        double minTemp = 999.0;
        for (const auto &point : lowTempDataPoints) {
                QDateTime dt = point.first;
                float val = point.second;

                if (val < minTemp) { minTemp = val; }
                lowTempSeries->append(dt.toMSecsSinceEpoch(), val);

                if (dt < minTime) { minTime = dt; }
                if (dt > maxTime) { maxTime = dt; }
        }
        mChart->addSeries(lowTempSeries);
        mTempAxis->setRange(minTemp - 2.0, maxTemp + 2.0);
        lowTempSeries->attachAxis(mTimeAxis);
        lowTempSeries->attachAxis(mTempAxis);

        if (idx == WEATHER_TODAY) {
                QDate d = minTime.date();
                mTimeAxis->setRange(QDateTime(d, QTime(0, 0)), QDateTime(d, QTime(23, 59)));
        } else {
                mTimeAxis->setRange(minTime, maxTime);
        }
}

void WeatherWindow::onCurrentWeather(const CurrentWeather & data)
{
        mCityLabel->setText(data.cityName);
        mTempLabel->setText(QString("%1°C").arg(qRound(data.temp)));
        mDescLabel->setText(data.description);
        mHumidityLabel->setText(QString("Feuchte: %1%").arg(data.humidity));
        mWindLabel->setText(QString("Wind: %1 m/s").arg(data.windSpeed, 0, 'f', 1));

        // load weather icon
        QString iconPath = QString("/usr/share/power-window-app/weatherIcons/%1.png").arg(data.icon);
        QPixmap pix(iconPath);
        if (!pix.isNull()) { mIconLabel->setPixmap(pix.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation)); }
}

void WeatherWindow::onForecastReady(const QVector<WeatherForecastPoint> & points)
{
        mRainAmountDatasets.clear();
        mHighTempDatasets.clear();
        mLowTempDatasets.clear();

        populateTodayData(points);
        populate5DayData(points);

        updateChart(mTabBar->currentIndex());
}

void WeatherWindow::onWeatherError(const QString & msg)
{
        qWarning() << "Weather: " << msg;
        mDescLabel->setText("Fehler: " + msg);
}

void WeatherWindow::populateTodayData(const QVector<WeatherForecastPoint> & points)
{
        QVector<WeatherForecastPoint> today;
        QDate currentDate = QDate::currentDate();
        for (const auto & point : points) {
                if (point.dt.date() == currentDate) { today.append(point); }
        }

        QVector<QPair<QDateTime, float>> rain;
        QVector<QPair<QDateTime, float>> highTemp;
        QVector<QPair<QDateTime, float>> lowTemp;
        for (const auto & point : today) {
                rain.append({ point.dt, static_cast<float>(point.rain) });
                highTemp.append({ point.dt, static_cast<float>(point.tempMax) });
                lowTemp.append({ point.dt, static_cast<float>(point.tempMin) });
        }

        mRainAmountDatasets[WEATHER_TODAY] = rain;
        mHighTempDatasets[WEATHER_TODAY] = highTemp;
        mLowTempDatasets[WEATHER_TODAY] = lowTemp;
}

void WeatherWindow::populate5DayData(const QVector<WeatherForecastPoint> & points)
{
        QMap<QDate, QVector<WeatherForecastPoint>> byDay;
        for (const auto & point : points) { byDay[point.dt.date()].append(point); }

        QVector<QPair<QDateTime, float>> rain;
        QVector<QPair<QDateTime, float>> highTemp;
        QVector<QPair<QDateTime, float>> lowTemp;
        for (auto it = byDay.begin(); it != byDay.end(); ++it) {
                double maxT = -999;
                double minT = 999;
                double totalRain = 0;
                for (const auto & point : it.value()) {
                        if (point.tempMax > maxT) { maxT = point.tempMax; }
                        if (point.tempMin < minT) { minT = point.tempMin; }
                        totalRain += point.rain;
                }
                QDateTime noon(it.key(), QTime(12, 0));
                rain.append({ noon, static_cast<float>(totalRain) });
                highTemp.append({ noon, static_cast<float>(maxT) });
                lowTemp.append({ noon, static_cast<float>(minT) });
        }
        mRainAmountDatasets[WEATHER_5_DAY] = rain;
        mHighTempDatasets[WEATHER_5_DAY] = highTemp;
        mLowTempDatasets[WEATHER_5_DAY] = lowTemp;
}

void WeatherWindow::addSeriesToChart(const QVector<QPair<QDateTime, float>> & data,
                      QAbstractAxis * axis, const QString & name, Qt::GlobalColor color)
{
        if (data.isEmpty()) { return; }

        QLineSeries * series = new QLineSeries();
        series->setName(name);
        series->setColor(color);
        for (const auto & point : data) { series->append(point.first.toMSecsSinceEpoch(), point.second); }
        mChart->addSeries(series);
        series->attachAxis(mTimeAxis);
        series->attachAxis(axis);
}
