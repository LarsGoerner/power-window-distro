#include <QApplication>
#include <QQmlApplicationEngine>
#include "Theme.hpp"
#include "Backlight.hpp"
#include "Updater.hpp"
#include "WifiManager.hpp"
#include "DhtSensor.hpp"
#include "WeatherFetcher.hpp"
#include "ScreenSaver.hpp"
#include "NewsTicker.hpp"

int main(int argc, char ** argv)
{
        QApplication app(argc, argv);
        app.setApplicationName("power-window-app");
        app.setApplicationVersion("1.0");
        app.setOrganizationName("lago");
        QQmlApplicationEngine appEngine;
        Theme theme;
        qmlRegisterSingletonInstance("PowerWindow", 1, 0, "Theme", &theme);
        Backlight backlight;
        qmlRegisterSingletonInstance("PowerWindow", 1, 0, "Backlight", &backlight);
        Updater updater;
        qmlRegisterSingletonInstance("PowerWindow", 1, 0, "Updater", &updater);
        WifiManager wifiManager;
        qmlRegisterSingletonInstance("PowerWindow", 1, 0, "WifiManager", &wifiManager);
        DhtSensor dhtSensor;
        qmlRegisterSingletonInstance("PowerWindow", 1, 0, "DhtSensor", &dhtSensor);
        WeatherFetcher weatherFetcher;
        qmlRegisterSingletonInstance("PowerWindow", 1, 0, "WeatherFetcher", &weatherFetcher);
        weatherFetcher.start();
        ScreenSaver screenSaver(&backlight);
        qmlRegisterSingletonInstance("PowerWindow", 1, 0, "ScreenSaver", &screenSaver);
        NewsTicker newsTicker;
        qmlRegisterSingletonInstance("PowerWindow", 1, 0, "NewsTicker", &newsTicker);
        newsTicker.start();

        // load qml file into binary resource system
        const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
        QObject::connect(&appEngine, &QQmlApplicationEngine::objectCreated, &app,
                         [url](QObject * obj, const QUrl &objUrl) {
                if (!obj && url == objUrl) { QCoreApplication::exit(-1); }
        }, Qt::QueuedConnection);

        appEngine.load(url);
        return app.exec();
}