SUMMARY = "Power Window App"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

DEPENDS = "qtbase qtdeclarative qtcharts openssl"

SRC_URI += "\
    file://src/main.cpp \
    file://qml/main.qml \
    file://qml/AmbientPage.qml \
    file://qml/WeatherPage.qml \
    file://qml/SettingsPage.qml \
    file://qml/NewsTickerPage.qml \
    file://qml/DisplayTab.qml \
    file://qml/WifiTab.qml \
    file://qml/GrowattTab.qml \
    file://qml/UpdateTab.qml \
    file://qml/LinksTab.qml \
    file://src/Theme.hpp file://src/Theme.cpp \
    file://src/Backlight.hpp file://src/Backlight.cpp \
    file://src/Updater.hpp file://src/Updater.cpp \
    file://src/WifiManager.hpp file://src/WifiManager.cpp \
    file://src/DhtSensor.hpp file://src/DhtSensor.cpp \
    file://src/WeatherFetcher.hpp file://src/WeatherFetcher.cpp \
    file://src/ScreenSaver.hpp file://src/ScreenSaver.cpp \
    file://src/NewsTicker.hpp file://src/NewsTicker.cpp \
    file://resources.qrc \
    file://power-window-app.pro \
    file://power-window-app.service \
    file://weatherIcons/ \
"

S = "${WORKDIR}"

inherit qmake5 systemd

RDEPENDS:${PN} += " \
    qtdeclarative-qmlplugins \
    qtquickcontrols2 \
    qtcharts \
    qtcharts-qmlplugins \
"

do_install() {
    install -d ${D}${bindir}
    install -m 0755 ${B}/power-window-app ${D}${bindir}/

    install -d ${D}${systemd_system_unitdir}
    install -m 0644 ${S}/power-window-app.service ${D}${systemd_system_unitdir}/

    install -d ${D}${datadir}/power-window-app/weatherIcons
    install -m 0644 ${S}/weatherIcons/*.png ${D}${datadir}/power-window-app/weatherIcons/
}

SYSTEMD_SERVICE:${PN} = "power-window-app.service"
SYSTEMD_AUTO_ENABLE = "enable"