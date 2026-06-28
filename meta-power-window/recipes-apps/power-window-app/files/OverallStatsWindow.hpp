#ifndef DAYSTATSWINDOW_HPP_
#define DAYSTATSWINDOW_HPP_

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
#include "BaseWindow.hpp"
#include "PvStatsData.hpp"

class OverallStatsWindow : public BaseWindow
{
        Q_OBJECT
public:
        OverallStatsWindow(QVector<PvStatsData> * data, QWidget * parent = nullptr);
        void updateData(QVector<PvStatsData> * data);

private:
        enum TimeRange { TR_DAY = 0, TR_WEEK, TR_MONTH, TR_YEAR };

        QTabBar * mTabBar;
        QChart * mChart;
        QValueAxis * mEnergyAxis;
        QDateTimeAxis * mTimeAxis;
        QChartView * mChartView;
        QVector<PvStatsData> * mData;

        void updateChart(int idx);
};

#endif // DAYSTATSWINDOW_HPP_