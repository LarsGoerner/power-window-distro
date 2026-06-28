SUMMARY = "ESP32-C6 WiFi initialization and control"
DESCRIPTION = "System services and configuration for ESP32-C6 Wifi modole \
connected via SPI (ESP-Hosted-NG)"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

RDEPENDS:${PN} = "esp-hosted-module wpa-supplicant"

SRC_URI = " \
    file://esp32-c6-init.service \
    file://esp32-c6-wpa.service \
    file://esp32-c6-dhcp.service \
    file://wpa_supplicant.conf \
"

inherit systemd

S = "${WORKDIR}"

do_install(){
    install -d ${D}${systemd_system_unitdir}
    install -m 0644 ${S}/esp32-c6-init.service ${D}${systemd_system_unitdir}/
    install -m 0644 ${S}/esp32-c6-wpa.service ${D}${systemd_system_unitdir}/
    install -m 0644 ${S}/esp32-c6-dhcp.service ${D}${systemd_system_unitdir}/

    install -d ${D}${sysconfdir}/wpa_supplicant
    install -m 0600 ${S}/wpa_supplicant.conf ${D}${sysconfdir}/wpa_supplicant/
}

SYSTEMD_SERVICE:${PN} = "esp32-c6-init.service esp32-c6-wpa.service esp32-c6-dhcp.service"
SYSTEMD_AUTO_ENABLE = "enable"