FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SPLASH_IMAGES = "file://power-window-logo.png;outsuffix=default"

# Landscape the boot splash for the 10.1-DSI-TOUCH-A: psplash writes raw to
# /dev/fb0 and ignores the DRM panel-orientation hint, so pass --angle via a
# systemd drop-in over psplash-start.service.
SRC_URI += " \
    file://psplash-angle.conf \
    ${SPLASH_IMAGES} \
"

do_install:append() {
    install -d ${D}${systemd_system_unitdir}/psplash-start.service.d
    install -m0644 ${WORKDIR}/psplash-angle.conf \
        ${D}${systemd_system_unitdir}/psplash-start.service.d/angle.conf
}

FILES:${PN} += "${systemd_system_unitdir}/psplash-start.service.d/angle.conf"