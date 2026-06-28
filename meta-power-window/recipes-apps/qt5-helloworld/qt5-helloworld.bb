SUMMARY = "Qt5 Test application"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

DEPENDS = "qtbase"

SRC_URI = " \
    file://main.cpp \
    file://qt5-helloworld.pro \
    file://helloworld.service \
"

S = "${WORKDIR}"

inherit qmake5 systemd

do_install(){
    install -d ${D}${bindir}
    install -m 0755 qt5-helloworld ${D}${bindir}/
    install -d ${D}${systemd_system_unitdir}
    install -m 0644 ${S}/helloworld.service ${D}${systemd_system_unitdir}/
}

SYSTEMD_SERVICE:${PN} = "helloworld.service"
SYSTEMD_AUTO_ENABLE = "enable"
