#include <QApplication>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScreen>
#include <QDebug>
#include <QFile>
#include "SwipeManager.hpp"
#include "ScreenSaverManager.hpp"
#include "BaseWindow.hpp"
#include "WeatherWindow.hpp"
#include "SettingsWindow.hpp"
#include "CurrentStatsWindow.hpp"
#include "OverallStatsWindow.hpp"
#include "PvStatsData.hpp"

static QVector<PvStatsData> loadPvMockData(void)
{
        QFile file(QStringLiteral("/bin/PvMockData.json"));
        QString jsonStr;
        QVector<PvStatsData> pvMockData;

        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
                qWarning("Failed to open PV mockup data");
                return pvMockData;
        }
        jsonStr = file.readAll();
        file.close();

        QJsonDocument d = QJsonDocument::fromJson(jsonStr.toUtf8());
        QJsonArray arr = d.array();
        for (const QJsonValue &val : arr) {
                PvStatsData dat;
                QJsonObject o = val.toObject();

                QString dateStr = o["date"].toString(); // format yyyy-MM-dd
                QString timeStr = o["time"].toString(); // format hh:mm
                QDate date = QDate::fromString(dateStr, "yyyy-MM-dd");
                QTime time = QTime::fromString(timeStr, "hh:mm");
                QDateTime dt(date, time);
                dat.datetime = dt;

                dat.pvOutput = static_cast<float>(o["pvOutput"].toDouble());
                dat.loadConsumption = static_cast<float>(o["loadConsumption"].toDouble());
                dat.exportToGrid = static_cast<float>(o["exportToGrid"].toDouble());
                dat.importFromGrid = static_cast<float>(o["importFromGrid"].toDouble());
                dat.charging = static_cast<float>(o["charging"].toDouble());
                dat.discharging = static_cast<float>(o["discharging"].toDouble());

                pvMockData.append(dat);
        }

        return pvMockData;
}

int main(int argc, char ** argv)
{
        QApplication app(argc, argv);
        QVector<PvStatsData> pvMockData = loadPvMockData();

        SwipeManager sm;
        ScreenSaverManager * scrSvrMgr = new ScreenSaverManager(30, &sm);

        WeatherWindow * wtrWnd = new WeatherWindow();
        CurrentStatsWindow * curStatWnd = new CurrentStatsWindow(&pvMockData);
        OverallStatsWindow * ovrStatWnd = new OverallStatsWindow(&pvMockData);
        SettingsWindow * setWnd = new SettingsWindow();

        sm.addWindow(wtrWnd);
        sm.addWindow(curStatWnd);
        sm.addWindow(ovrStatWnd);
        sm.addWindow(setWnd);

        sm.show();
        QScreen * screen = QGuiApplication::primaryScreen();
        if (screen) { sm.setFixedSize(screen->size()); }
        else { qWarning() << "Failed to get primary screen reference"; }

        return app.exec();
}
