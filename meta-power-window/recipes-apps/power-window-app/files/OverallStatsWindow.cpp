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
#include "OverallStatsWindow.hpp"

OverallStatsWindow::OverallStatsWindow(QVector<PvStatsData> * data, QWidget * parent) : BaseWindow(parent), mData(data)
{
        QVBoxLayout * layout = new QVBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);

        // MENU BAR
        mTabBar = new QTabBar(this);
        mTabBar->addTab("Tag");
        mTabBar->addTab("Woche");
        mTabBar->addTab("Monat");
        mTabBar->addTab("Jahr");
        layout->addWidget(mTabBar);

        // CHART
        mChart = new QChart();
        mChart->setMargins(QMargins(0, 0, 0, 0));
        mChart->setTitle("Energie");
        mChart->legend()->setVisible(true);
        mChart->legend()->setAlignment(Qt::AlignBottom);

        mEnergyAxis = new QValueAxis(this);
        mEnergyAxis->setMax(100.0);
        mEnergyAxis->setMin(0.0);
        mChart->addAxis(mEnergyAxis, Qt::AlignLeft);

        mTimeAxis = new QDateTimeAxis(this);
        mTimeAxis->setFormat("HH:mm");
        mChart->addAxis(mTimeAxis, Qt::AlignBottom);

        mChartView = new QChartView(mChart, this);
        mChartView->setRenderHint(QPainter::Antialiasing);
        mChartView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        layout->addWidget(mChartView, 1);

        connect(mTabBar, &QTabBar::currentChanged, this, &OverallStatsWindow::updateChart);
        updateChart(TR_DAY);
}

void OverallStatsWindow::updateData(QVector<PvStatsData> * data)
{
        mData = data;
}

void OverallStatsWindow::updateChart(int idx)
{
        if (mData == nullptr || mData->isEmpty()) {
                qWarning("Stats data not available");
                return;
        }
        QVector<PvStatsData> filteredData;

        if (idx == TR_DAY) {
                QDate today = QDate::currentDate();
                for (const PvStatsData &dat : *mData) {
                        if (dat.datetime.date().daysTo(today) == 0) {
                                filteredData.append(dat);
                        }
                }
        } else {
                qWarning("Unknown time range");
                return;
        }

        mChart->removeAllSeries();

        QLineSeries * pvOutputSeries = new QLineSeries();
        pvOutputSeries->setName("PV output");
        QLineSeries * loadConsumptionSeries = new QLineSeries();
        loadConsumptionSeries->setName("Load Consumption");
        QLineSeries * exportedToGridSeries = new QLineSeries();
        exportedToGridSeries->setName("Exported to Grid");
        QLineSeries * importedFromGridSeries = new QLineSeries();
        importedFromGridSeries->setName("Imported from Grid");
        QLineSeries * chargingSeries = new QLineSeries();
        chargingSeries->setName("Charging");
        QLineSeries * dischargingSeries = new QLineSeries();
        dischargingSeries->setName("Discharging");

        qDebug() << "Number of filtered data: " << filteredData.size();

        for (const PvStatsData &dat : filteredData) {
                pvOutputSeries->append(dat.datetime.toMSecsSinceEpoch(), dat.pvOutput);
                loadConsumptionSeries->append(dat.datetime.toMSecsSinceEpoch(), dat.loadConsumption);
                exportedToGridSeries->append(dat.datetime.toMSecsSinceEpoch(), dat.exportToGrid);
                importedFromGridSeries->append(dat.datetime.toMSecsSinceEpoch(), dat.importFromGrid);
                chargingSeries->append(dat.datetime.toMSecsSinceEpoch(), dat.charging);
                dischargingSeries->append(dat.datetime.toMSecsSinceEpoch(), dat.discharging);
        }

        mChart->addSeries(pvOutputSeries);
        pvOutputSeries->attachAxis(mTimeAxis);
        pvOutputSeries->attachAxis(mEnergyAxis);

        mChart->addSeries(loadConsumptionSeries);
        loadConsumptionSeries->attachAxis(mTimeAxis);
        loadConsumptionSeries->attachAxis(mEnergyAxis);

        mChart->addSeries(exportedToGridSeries);
        exportedToGridSeries->attachAxis(mTimeAxis);
        exportedToGridSeries->attachAxis(mEnergyAxis);

        mChart->addSeries(importedFromGridSeries);
        importedFromGridSeries->attachAxis(mTimeAxis);
        importedFromGridSeries->attachAxis(mEnergyAxis);

        mChart->addSeries(chargingSeries);
        chargingSeries->attachAxis(mTimeAxis);
        chargingSeries->attachAxis(mEnergyAxis);

        mChart->addSeries(dischargingSeries);
        dischargingSeries->attachAxis(mTimeAxis);
        dischargingSeries->attachAxis(mEnergyAxis);

        mTimeAxis->setRange(QDateTime(QDate(2025, 10, 24)), QDateTime(QDate(2025, 10, 26)));
        mEnergyAxis->setRange(0.0, 100.0);
}