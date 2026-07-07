#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "Theme.hpp"
#include "Backlight.hpp"
#include "Updater.hpp"
#include "WifiManager.hpp"

int main(int argc, char ** argv)
{
        QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

        QGuiApplication app(argc, argv);
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

        // load qml file into binary resource system
        const QUrl url(QStringLiteral("qrc:/main.qml"));
        QObject::connect(&appEngine, &QQmlApplicationEngine::objectCreated, &app,
                         [url](QObject * obj, const QUrl &objUrl) {
                if (!obj && url == objUrl) { QCoreApplication::exit(-1); }
        }, Qt::QueuedConnection);

        appEngine.load(url);
        return app.exec();
}