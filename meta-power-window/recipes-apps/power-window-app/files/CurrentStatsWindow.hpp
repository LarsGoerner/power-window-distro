#ifndef CURRENTSTATSWINDOW_HPP_
#define CURRENTSTATSWINDOW_HPP_

#include <QWidget>
#include <QProgressBar>
#include <QVector>
#include "BaseWindow.hpp"
#include "PvStatsData.hpp"

class CurrentStatsWindow : public BaseWindow
{
        Q_OBJECT
public:
        CurrentStatsWindow(QVector<PvStatsData> * data, QWidget * parent = nullptr);
        void updateData(QVector<PvStatsData> * data);

private:
        QVector<PvStatsData> * mData;
        QProgressBar * mSystemPb;
        QProgressBar * mLoadPb;
        
        float mSystemProduction; /** self consumed + exported */
        float mSystemSelfConsumption;
        float mExportedToGrid;

        float mLoadConsumption; /** self consumed + imported */
        float mLoadSelfConsumed;
        float mImportedFromGrid;

        void calcCurrentStats();
        void updateBars();
};

#endif // CURRENTSTATSWINDOW_HPP_