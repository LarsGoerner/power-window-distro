# Minimal Luckfox RK3506 image for eMMC boot.
# Self-contained. Flash via rkdeveloptool wl 0 from maskrom mode.

SUMMARY = "Power Window image"
DESCRIPTION = "Minimal bootable image for the Power Window. \
Provides serial console, networking, SSH, ALSA, the AIC8800DC WiFi/BT \
driver, and the luckfox-status webserver. No display, touch, or NPU."
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

inherit core-image

COMPATIBLE_MACHINE = "(power-window)"

IMAGE_INSTALL = " \
    packagegroup-core-boot \
    packagegroup-power-window-base \
    packagegroup-power-window-display \
    packagegroup-power-window-network \
    kernel-modules \
    optee-client \
    i2c-tools \
    screen \
    psplash \
    nano \
"

# QT5
IMAGE_INSTALL += "qtbase qtdeclarative qtdeclarative-qmlplugins qtquickcontrols2"
#IMAGE_INSTALL += "qtwayland"
IMAGE_INSTALL += "qtvirtualkeyboard qtvirtualkeyboard-qmlplugins"
IMAGE_INSTALL += "ca-certificates openssl"
IMAGE_INSTALL += "ttf-dejavu-sans"
#IMAGE_INSTALL += "qt5-helloworld"
#IMAGE_INSTALL += "swipe-window-test"
IMAGE_INSTALL += "power-window-app"

# USER SETTINGS
ROOT_PASSWORD ?= "powerwindow"
ROOT_PASSWORD_HASH ??= ""
ROOTFS_POSTPROCESS_COMMAND += "set_root_password;"
set_root_password() {
        if [ -n "${ROOT_PASSWORD_HASH}" ]; then
                hashed="${ROOT_PASSWORD_HASH}"
        else
                hashed=$(python3 -c "import crypt; print(crypt.crypt('${ROOT_PASSWORD}', crypt.mksalt(crypt.METHOD_SHA512)))")
                if [ "${ROOT_PASSWORD}" = "powerwindow" ]; then
                        bbwarn "ROOT_PASSWORD is set to the default value. Better use a custom password for production builds."
                fi
        fi
        python3 -c "
import sys
pw = sys.argv[1]
rootfs = sys.argv[2]
with open(rootfs + '/etc/shadow') as f:
	data = f.read()
with open(rootfs + '/etc/shadow', 'w') as f:
	for line in data.splitlines(True):
		if line.startswith('root:'):
			parts = line.split(':')
			parts[1] = pw
			f.write(':'.join(parts))
		else:
			f.write(line)
" "${hashed}" ${IMAGE_ROOTFS}
}

ROOTFS_POSTPROCESS_COMMAND += "copy_fonts_to_lib;"
copy_fonts_to_lib() {
    mkdir -p ${IMAGE_ROOTFS}${nonarch_libdir}/fonts
    find ${IMAGE_ROOTFS}/usr/share/fonts -name "*.ttf" -exec cp {} ${IMAGE_ROOTFS}${nonarch_libdir}/fonts/ \;
}

# OPKG PRE-CONFIG
ROOTFS_POSTPROCESS_COMMAND += "configure_opkg_feed;"
configure_opkg_feed() {
    mkdir -p ${IMAGE_ROOTFS}/etc/opkg
    cat > ${IMAGE_ROOTFS}/etc/opkg/opkg-feed.conf <<EOF
src/gz power-window-all http://192.168.2.218:80/all
src/gz power-window-armv7at2hf-neon http://192.168.2.218:80/armv7at2hf-neon
EOF
}

# BIGGER /TMP FOR OPKG & SYSTEMD
ROOTFS_POSTPROCESS_COMMAND += "configure_tmpfs_size;"
configure_tmpfs_size() {
    # /run: add explicit size to fstab entry (run.mount unit doesn't exist)
    sed -i 's|^tmpfs\s*/run\s.*|tmpfs /run tmpfs mode=0755,nodev,nosuid,strictatime,size=128M 0 0|' ${IMAGE_ROOTFS}/etc/fstab
    # /tmp: systemd drop-in (tmp.mount unit exists)
    mkdir -p ${IMAGE_ROOTFS}${systemd_system_unitdir}/tmp.mount.d
    cat > ${IMAGE_ROOTFS}${systemd_system_unitdir}/tmp.mount.d/size.conf <<EOF
[Mount]
Options=mode=1777,nosuid,nodev,size=256M
EOF
}


WKS_FILE = "power-window-sd.wks"

IMAGE_ROOTFS_SIZE ?= "131072"
IMAGE_OVERHEAD_FACTOR ?= "1.2"

EXTRA_IMAGE_FEATURES += "package-management"
