#ifndef GROWATT_DATA_HPP_
#define GROWATT_DATA_HPP_

#include <QString>

struct GrowattRealtimeData
{
        double currentPower;
        double todayEnergy;
        double totalEnergy;
        double batterySoc;
        double batteryChargePower;
        double batteryDischargePower;
        double gridImportPower;
        double gridExportPower;
        double loadPower;
        QString plantName;
        QString plantStatus;
        double co2Saved;
        double revenue;
};

#endif // GROWATT_DATA_HPP_