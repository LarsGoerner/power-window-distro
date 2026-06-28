#ifndef PVSTATSDATA_HPP_
#define PVSTATSDATA_HPP_

#include <QDateTime>

struct PvStatsData
{
        QDateTime datetime;
        float pvOutput;
        float loadConsumption;
        
        // GRID IO
        float exportToGrid;
        float importFromGrid;

        // BATTERY
        float charging;
        float discharging;
};

#endif // PVSTATSDATA_HPP_