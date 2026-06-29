#ifndef PLANT_OVERVIEW_WINDOW_HPP_
#define PLANT_OVERVIEW_WINDOW_HPP_

#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "BaseWindow.hpp"
#include "GrowattData.hpp"

class PlantOverviewWindow : public BaseWindow
{
        Q_OBJECT
public:
        PlantOverviewWindow(QWidget * parent = nullptr);

public slots:
        void updateRealtimeData(const GrowattRealtimeData &data);
        void showError(const QString &msg);

private:
        QLabel * mPlantNameLabel;
        QLabel * mStatusLabel;
        QLabel * mCurrentPowerValueLabel;
        QLabel * mCurrentPowerUnitLabel;
        QLabel * mTodayEnergyValueLabel;
        QLabel * mTodayEnergyLabel;
        QLabel * mBatteryValueLabel;
        QLabel * mBatteryLabel;
        QLabel * mGridValueLabel;
        QLabel * mLoadLabel;
        QLabel * mTotalEnergyValueLabel;
        QLabel * mCo2ValueLabel;
        QLabel * mRevenueLabel;
};

#endif // PLANT_OVERVIEW_WINDOW_HPP_