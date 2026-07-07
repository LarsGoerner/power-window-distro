SUMMARY = "Power Window App"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

DEPENDS = "qtbase qtdeclarative qtcharts openssl"

SRC_URI += "\
    file://main.cpp \
    file://main.qml \
    file://Theme.hpp file://Theme.cpp \
    file://Backlight.hpp file://Backlight.cpp \
    file://Updater.hpp file://Updater.cpp \
    file://WifiManager.hpp file://WifiManager.cpp \
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