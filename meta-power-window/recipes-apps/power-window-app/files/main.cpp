#include <QApplication>
#include <QObject>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScreen>
#include <QDebug>
#include "SwipeManager.hpp"
#include "ScreenSaverManager.hpp"
#include "BaseWindow.hpp"
#include "WeatherWindow.hpp"
#include "SettingsWindow.hpp"
#include "CurrentStatsWindow.hpp"
#include "OverallStatsWindow.hpp"
#include "PvStatsData.hpp"
#include "PlantOverviewWindow.hpp"
#include "GrowattFetcher.hpp"
#include "GrowattData.hpp"

int main(int argc, char ** argv)
{
        QApplication app(argc, argv);
        QVector<PvStatsData> growattEnergyData;

        SwipeManager sm;
        new ScreenSaverManager(30, &sm); // self-registers -> no need to store

        WeatherWindow * wtrWnd = new WeatherWindow();
        PlantOverviewWindow * plantOvvWnd = new PlantOverviewWindow();
        CurrentStatsWindow * curStatWnd = new CurrentStatsWindow(&growattEnergyData);
        OverallStatsWindow * ovrStatWnd = new OverallStatsWindow(&growattEnergyData);
        SettingsWindow * setWnd = new SettingsWindow();

        // GROWATT FETCHER
        GrowattFetcher * gwFetcher = new GrowattFetcher();
        QObject::connect(gwFetcher, &GrowattFetcher::loginResult, setWnd, [setWnd, gwFetcher](bool ok, const QString &msg) {
                if (ok) {
                        setWnd->setGrowattStatus("Verbunden", "green");
                        gwFetcher->fetchPlantList();
                } else { setWnd->setGrowattStatus(msg, "red"); }
        });
        QObject::connect(gwFetcher, &GrowattFetcher::plantListReady, setWnd, [setWnd, gwFetcher](const QJsonArray &plants) {
                setWnd->populateGrowattPlants(plants);
                if (!plants.isEmpty()) {
                        QString firstId = plants[0].toObject()["plantId"].toString();
                        gwFetcher->fetchRealtimeData(firstId);
                        gwFetcher->fetchEnergyData(firstId, QDate::currentDate());
                        gwFetcher->startPolling(30);
                }
        });
        QObject::connect(gwFetcher, &GrowattFetcher::realtimeDataReady, plantOvvWnd, &PlantOverviewWindow::updateRealtimeData);
        QObject::connect(gwFetcher, &GrowattFetcher::energyDataReady, [&growattEnergyData, curStatWnd, ovrStatWnd](const QVector<PvStatsData> &data) {
                growattEnergyData = data;
                curStatWnd->updateData(&growattEnergyData);
                ovrStatWnd->updateData(&growattEnergyData);
        });
        QObject::connect(gwFetcher, &GrowattFetcher::errorOccurred, plantOvvWnd, &PlantOverviewWindow::showError);
        QObject::connect(setWnd, &SettingsWindow::growattConnectRequested, gwFetcher, [gwFetcher, setWnd](const QString &serverUrl) {
                gwFetcher->login(setWnd->growattUsername(), setWnd->growattPassword(), serverUrl);
        });
        QObject::connect(setWnd, &SettingsWindow::growattPlantSelected, gwFetcher, [gwFetcher](const QString &plantId) {
                gwFetcher->fetchRealtimeData(plantId);
                gwFetcher->fetchEnergyData(plantId, QDate::currentDate());
        });

        sm.addWindow(wtrWnd);
        sm.addWindow(plantOvvWnd);
        sm.addWindow(curStatWnd);
        sm.addWindow(ovrStatWnd);
        sm.addWindow(setWnd);

        sm.show();
        QScreen * screen = QGuiApplication::primaryScreen();
        if (screen) { sm.setFixedSize(screen->size()); }
        else { qWarning() << "Failed to get primary screen reference"; }

        if (gwFetcher->isLoggedIn() && !gwFetcher->username().isEmpty()) {
                gwFetcher->login(gwFetcher->username(), gwFetcher->password(), gwFetcher->serverUrl());
        }

        return app.exec();
}
