#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QProgressBar>
#include <QDebug>
#include "CurrentStatsWindow.hpp"

CurrentStatsWindow::CurrentStatsWindow(QVector<PvStatsData> * data, QWidget * parent) : BaseWindow(parent), mData(data)
{
        QVBoxLayout * layout = new QVBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);

        mSystemPb = new QProgressBar(this);
        layout->addWidget(mSystemPb);

        mLoadPb = new QProgressBar(this);
        layout->addWidget(mLoadPb);

        updateBars();
}

void CurrentStatsWindow::updateData(QVector<PvStatsData> * data)
{
        mData = data;
        updateBars();
}

void CurrentStatsWindow::calcCurrentStats()
{
        if (mData == nullptr)
        {
                qWarning() << "Statistical data reference is NULL";
                return;
        }

        mSystemProduction = 0.0f;
        mSystemSelfConsumption = 0.0f;
        mExportedToGrid = 0.0f;
        mLoadConsumption = 0.0f;
        mLoadSelfConsumed = 0.0f;
        mImportedFromGrid = 0.0f;

        for (const PvStatsData &dat : *mData) {
                mExportedToGrid += dat.exportToGrid;
                mImportedFromGrid += dat.importFromGrid;
                mSystemProduction = dat.pvOutput;
                mLoadConsumption = dat.loadConsumption;
        }
        mSystemSelfConsumption += mSystemProduction - mExportedToGrid;
        mLoadSelfConsumed += mLoadConsumption - mImportedFromGrid;
}

void CurrentStatsWindow::updateBars()
{
        calcCurrentStats();
        float systemPerc = mSystemSelfConsumption / mSystemProduction;
        float loadPerc = mLoadSelfConsumed / mLoadConsumption;
        mSystemPb->setValue(systemPerc);
        mSystemPb->setValue(loadPerc);
}