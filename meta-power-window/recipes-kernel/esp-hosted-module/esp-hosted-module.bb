SUMMARY = "ESP-Hosted-NG SPI WiFi/BT driver for ESP32-C6"
DESCRIPTION = "Out-of-tree kernel module providing WiFi (cfg80211) and \
Bluetooth over SPI for ESP32-C6 using ESP-Hosted-NG"
HOMEPAGE = "https://github.com/espressif/esp-hosted"
LICENSE = "GPL-2.0-only"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/GPL-2.0-only;md5=801f80980d171dd6425610833a22dbe6"

SRCREV = "release/ng-1.0.6"
SRC_URI = "\
    git://github.com/espressif/esp-hosted.git;protocol=https;branch=master;destsuffix=esp-hosted;nobranch=1 \
    file://0001-change-gpio-pins-for-rk3506.patch \
"

inherit module

S = "${WORKDIR}/esp-hosted/esp_hosted_ng/host"
MODULES_INSTALL_TARGETS = "esp32_spi.ko"

EXTRA_OEMAKE = "target=spi"

do_compile() {
    oe_runmake KERNEL=${STAGING_KERNEL_DIR} ARCH=${ARCH} CROSS_COMPILE=${TARGET_PREFIX} EXTRA_CFLAGS="${CFLAGS}"
}


module_do_install() {
    install -d ${D}${nonarch_base_libdir}/modules/${KERNEL_VERSION}/extra
    install -m 0644 ${S}/esp32_spi.ko ${D}${nonarch_base_libdir}/modules/${KERNEL_VERSION}/extra/
}
