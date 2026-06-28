SUMMARY = "Qt5 swipe window test app"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

DEPENDS = "qtbase"

SRC_URI = " \
    file://main.cpp \
    file://SwipeManager.hpp \
    file://BaseWindow.hpp \
    file://swipe-window-test.pro \
    file://swipe-window-test.service \
"

S = "${WORKDIR}"

inherit qmake5 systemd

do_install(){
    install -d ${D}${bindir}
    install -m 0755 swipe-window-test ${D}${bindir}/
    install -d ${D}${systemd_system_unitdir}
    install -m 0644 ${S}/swipe-window-test.service ${D}${systemd_system_unitdir}/
}

SYSTEMD_SERVICE:${PN} = "swipe-window-test.service"
SYSTEMD_AUTO_ENABLE = "enable"
