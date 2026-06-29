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
        if (mData == nullptr || mData->isEmpty()) {
                qWarning("Statistical data not available");
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
        float systemPerc = mSystemProduction > 0 ? mSystemSelfConsumption / mSystemProduction : 0;
        float loadPerc = mLoadConsumption > 0 ? mLoadSelfConsumed / mLoadConsumption : 0;
        mSystemPb->setValue(static_cast<int>(systemPerc * 100));
        mLoadPb->setValue(static_cast<int>(loadPerc * 100));
}