SUMMARY = "Power Window RK3506 network packages"
DESCRIPTION = "Network bundle for RK3506 boards"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

inherit packagegroup

RDEPENDS:${PN} = "\
    net-tools \
    esp32-c6-wifi \
"

RRECOMMENDS:${PN} = " \
    ${@bb.utils.contains('MACHINE_FEATURES', 'wifi', 'wpa-supplicant iw wireless-regdb', '', d)} \
"