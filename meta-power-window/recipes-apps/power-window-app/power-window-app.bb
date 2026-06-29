SUMMARY = "Power Window App"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"
PV = "1.0"
PR = "r0"

DEPENDS = "qtbase qtcharts openssl"

SRC_URI = " \
    file://main.cpp \
    file://BaseWindow.hpp \
    file://PvStatsData.hpp \
    file://GrowattData.hpp \
    file://KeyboardWidget.hpp file://KeyboardWidget.cpp \
    file://SwipeManager.hpp file://SwipeManager.cpp \
    file://ScreenSaverManager.hpp file://ScreenSaverManager.cpp \
    file://WeatherFetcher.hpp file://WeatherFetcher.cpp \
    file://WeatherWindow.hpp file://WeatherWindow.cpp \
    file://SettingsWindow.hpp file://SettingsWindow.cpp \
    file://GrowattFetcher.hpp file://GrowattFetcher.cpp \
    file://CurrentStatsWindow.hpp file://CurrentStatsWindow.cpp \
    file://OverallStatsWindow.hpp file://OverallStatsWindow.cpp \
    file://PlantOverviewWindow.hpp file://PlantOverviewWindow.cpp \
    file://power-window-app.pro \
    file://power-window-app.service \
"
SRC_URI += "file://style.qss"
SRC_URI += "file://weatherIcons/"

S = "${WORKDIR}"

inherit qmake5 systemd

do_install(){
    install -d ${D}${bindir}
    install -m 0755 power-window-app ${D}${bindir}/
    install -m 0644 ${S}/PvMockData.json ${D}${bindir}/ 

    install -d ${D}${systemd_system_unitdir}
    install -m 0644 ${S}/power-window-app.service ${D}${systemd_system_unitdir}/

    install -d ${D}${datadir}/power-window-app/weatherIcons
    install -m 0644 ${S}/weatherIcons/*.png ${D}${datadir}/power-window-app/weatherIcons/
    install -m 0644 ${S}/style.qss ${D}${datadir}/power-window-app/
}

SYSTEMD_SERVICE:${PN} = "power-window-app.service"
SYSTEMD_AUTO_ENABLE = "enable"
