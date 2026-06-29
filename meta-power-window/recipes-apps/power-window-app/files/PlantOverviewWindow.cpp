#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>
#include "BaseWindow.hpp"
#include "GrowattData.hpp"
#include "PlantOverviewWindow.hpp"

PlantOverviewWindow::PlantOverviewWindow(QWidget * parent) : BaseWindow(parent)
{
        QVBoxLayout * mainLayout = new QVBoxLayout(this);
        mainLayout->setContentsMargins(20, 20, 20, 20);
        mainLayout->setSpacing(15);

        // Plant name + status
        QHBoxLayout * headerRow = new QHBoxLayout();
        mPlantNameLabel = new QLabel("--", this);
        headerRow->addWidget(mPlantNameLabel);
        headerRow->addStretch();
        mStatusLabel = new QLabel("", this);
        headerRow->addWidget(mStatusLabel);
        mainLayout->addLayout(headerRow);

        // current power card
        QWidget * powerCard = new QWidget(this);
        powerCard->setObjectName("powerCard");
        QVBoxLayout * powerLayout = new QVBoxLayout(powerCard);
        powerLayout->setAlignment(Qt::AlignCenter);
        QHBoxLayout * powerRow = new QHBoxLayout();
        powerRow->setAlignment(Qt::AlignCenter);
        mCurrentPowerValueLabel = new QLabel("0", powerCard);
        powerRow->addWidget(mCurrentPowerValueLabel);
        mCurrentPowerUnitLabel = new QLabel(" W", powerCard);
        powerRow->addWidget(mCurrentPowerUnitLabel);
        powerLayout->addLayout(powerRow);
        QLabel * subtitle = new QLabel("Aktuelle Leistung", powerCard);
        subtitle->setAlignment(Qt::AlignCenter);
        powerLayout->addWidget(subtitle);
        mainLayout->addWidget(powerCard);

        // Two metric cards side by side
        QHBoxLayout * metricsRow = new QHBoxLayout();
        metricsRow->setSpacing(15);

        // Today energy card
        QWidget * todayCard = new QWidget(this);
        todayCard->setObjectName("todayCard");
        QVBoxLayout * todayLayout = new QVBoxLayout();
        todayLayout->setAlignment(Qt::AlignCenter);
        mTodayEnergyValueLabel = new QLabel("0 Wh", todayCard);
        mTodayEnergyValueLabel->setAlignment(Qt::AlignCenter);
        todayLayout->addWidget(mTodayEnergyValueLabel);
        mTodayEnergyLabel = new QLabel("Heute", todayCard);
        mTodayEnergyLabel->setAlignment(Qt::AlignCenter);
        todayLayout->addWidget(mTodayEnergyLabel);
        metricsRow->addLayout(todayLayout);

        // Battery card
        QWidget * batteryCard = new QWidget(this);
        batteryCard->setObjectName("batteryCard");
        QVBoxLayout * batteryLayout = new QVBoxLayout();
        batteryLayout->setAlignment(Qt::AlignCenter);
        mBatteryValueLabel = new QLabel("--", batteryCard);
        mBatteryValueLabel->setAlignment(Qt::AlignCenter);
        batteryLayout->addWidget(mBatteryValueLabel);
        mBatteryLabel = new QLabel("Akku", batteryCard);
        mBatteryLabel->setAlignment(Qt::AlignCenter);
        batteryLayout->addWidget(mBatteryLabel);
        metricsRow->addLayout(batteryLayout);

        mainLayout->addLayout(metricsRow);

        // Information rows
        QHBoxLayout * gridRow = new QHBoxLayout();
        gridRow->addWidget(new QLabel("Netz:", this));
        mGridValueLabel = new QLabel("--", this);
        gridRow->addWidget(mGridValueLabel);
        gridRow->addStretch();
        mainLayout->addLayout(gridRow);

        QHBoxLayout * loadRow = new QHBoxLayout();
        loadRow->addWidget(new QLabel("Last:", this));
        mLoadLabel = new QLabel("--", this);
        loadRow->addWidget(mLoadLabel);
        loadRow->addStretch();
        mainLayout->addLayout(loadRow);

        QHBoxLayout * totalRow = new QHBoxLayout();
        totalRow->addWidget(new QLabel("Gesamt:", this));
        mTotalEnergyValueLabel = new QLabel("--", this);
        totalRow->addWidget(mTotalEnergyValueLabel);
        totalRow->addStretch();
        mainLayout->addLayout(totalRow);

        QHBoxLayout * co2Row = new QHBoxLayout();
        co2Row->addWidget(new QLabel("CO2-Ersparnis:", this));
        mCo2ValueLabel = new QLabel("--", this);
        co2Row->addWidget(mCo2ValueLabel);
        co2Row->addStretch();
        mainLayout->addLayout(co2Row);

        mainLayout->addStretch();
}

void PlantOverviewWindow::updateRealtimeData(const GrowattRealtimeData &data)
{
        mPlantNameLabel->setText(data.plantName);

        double power = data.currentPower;
        if (power >= 1000) {
                mCurrentPowerValueLabel->setText(QString::number(power / 1000, 'f', 2));
                mCurrentPowerUnitLabel->setText(" kW");
        }
        else {
                mCurrentPowerValueLabel->setText(QString::number(power, 'f', 0));
                mCurrentPowerUnitLabel->setText(" W");
        }

        double energy = data.todayEnergy;
        if (energy >= 1000) { mTodayEnergyValueLabel->setText(QString::number(energy / 1000, 'f', 1) + " kWh"); }
        else { mTodayEnergyValueLabel->setText(QString::number(energy, 'f', 0) + " Wh"); }

        if (data.batterySoc >= 0) { mBatteryValueLabel->setText(QString::number(data.batterySoc, 'f', 0) + " %"); }
        else { mBatteryValueLabel->setText("--"); }

        // Status indicator
        if (data.plantStatus == "online") { mStatusLabel->setText("Online"); }
        else { mStatusLabel->setText("Offline"); }

        // Grid
        double gridDiff = data.gridExportPower - data.gridImportPower;
        if (gridDiff > 0) { mGridValueLabel->setText(QString::number(gridDiff, 'f', 1) + " W (Export)"); }
        else if (gridDiff < 0) { mGridValueLabel->setText(QString::number(-gridDiff, 'f', 1) + " W (Import)"); }
        else { mGridValueLabel->setText("0 W"); }

        mLoadLabel->setText(QString::number(data.loadPower, 'f', 1) + " W");
        mTotalEnergyValueLabel->setText(QString::number(data.totalEnergy, 'f', 1) + " kWh");
        mCo2ValueLabel->setText(QString::number(data.co2Saved, 'f', 1) + " kg");
}

void PlantOverviewWindow::showError(const QString &msg)
{
        mPlantNameLabel->setText("Fehler");
        mStatusLabel->setText(msg);
}
