#include <QApplication>
#include <QQmlApplicationEngine>
#include "Theme.hpp"
#include "Backlight.hpp"
#include "Updater.hpp"
#include "WifiManager.hpp"
#include "DhtSensor.hpp"
#include "WeatherFetcher.hpp"

int main(int argc, char ** argv)
{
        QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

        QApplication app(argc, argv);
        app.setApplicationName("power-window-app");
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

        // load qml file into binary resource system
        const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
        QObject::connect(&appEngine, &QQmlApplicationEngine::objectCreated, &app,
                         [url](QObject * obj, const QUrl &objUrl) {
                if (!obj && url == objUrl) { QCoreApplication::exit(-1); }
        }, Qt::QueuedConnection);

        appEngine.load(url);
        return app.exec();
}