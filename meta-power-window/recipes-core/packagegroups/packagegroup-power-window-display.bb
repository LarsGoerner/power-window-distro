# DSI display userspace — pulled in by power-window-image only when
# MACHINE_FEATURES contains "screen" (so it's a no-op on the headless
# build). Provides the screen-bringup tools and the boot splash.
#
# libdrm-tests bundles every binary from poky's libdrm recipe — modetest,
# proptest, vbltest, drmdevice — sufficient for first-light:
#   modetest -M rockchip -s <conn>:800x1280 -F SMPTE   (colour-bars test)
#
# psplash draws a graphical boot splash to /dev/fb0 (provided by the
# kernel's DRM fbdev emulation — see display-dsi.cfg). The psplash bbappend
# rotates it 270° to match the landscape-mounted portrait panel.
#
# kmscube is intentionally omitted: it requires DISTRO_FEATURES "opengl",
# which this distro doesn't enable (the RK3506 has no 3D GPU; pulling Mesa
# swrast just for a triangle demo costs ~10s of MB). To add it anyway, put
# 'opengl' in DISTRO_FEATURES and append ' kmscube' to RDEPENDS below.
#
# The LVGL demo (luckfox-lvgl-demo) stays deferred — re-enable its
# COMPATIBLE_MACHINE and add it to RDEPENDS here once validated on HW.

SUMMARY = "Power Window DSI display userspace (screen feature)"
DESCRIPTION = "Screen-bringup tools (libdrm-tests) + boot splash (psplash) \
for the 10.1-DSI-TOUCH-A panel. Pulled in only when MACHINE_FEATURES \
contains 'screen'."
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

inherit packagegroup

RDEPENDS:${PN} = " \
    libdrm-tests \
"

#     psplash \
#
