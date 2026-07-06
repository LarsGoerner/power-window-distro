# Rockchip vendor BSP kernel 6.1.x for RK3506G2.
#
# No mainline kernel support exists for RK3506. Pin against Rockchip's
# vendor tree (rockchip-linux/kernel) on the develop-6.1 branch. The
# SRCREV below is validated on the Core3506 sibling board (identical
# SoC) — see docs/REFERENCE.md.

SUMMARY = "Rockchip vendor BSP kernel 6.1 for RK3506G2"
DESCRIPTION = "Vendor BSP Linux kernel 6.1.x for the RK3506 SoC family \
(triple Cortex-A7 + Cortex-M0 AMP). Ships the Lyra Ultra W DTS and \
config fragments for ext4, systemd, and AIC8800DC WiFi/BT. The DSI \
display + touch + USB-HID stack is built only when the machine sets \
MACHINE_FEATURES += \"screen\" (see SRC_URI:append below)."
HOMEPAGE = "https://github.com/rockchip-linux/kernel"

LICENSE = "GPL-2.0-only"
LIC_FILES_CHKSUM = "file://COPYING;md5=6bc538ed5bd9a7fc9398086aedcd7e46"

DEPENDS += "coreutils-native openssl-native lz4-native"

SRCREV = "b4ef083dc0c3608e744deabb43dc6b781aadbe6e"
SRC_URI = "\
    git://github.com/rockchip-linux/kernel.git;protocol=https;branch=develop-6.1 \
    file://rk3506-power-window-pinctrl.dtsi \
    file://rk3506-power-window.dtsi \
    file://rk3506-power-window.dts \
    file://display-dsi.cfg \
    file://ext4-builtin.cfg \
    file://optee.cfg \
    file://systemd.cfg \
    file://wifi-bt.cfg \
    file://esp32-c6.cfg \
    file://dht11.cfg \
    file://0001-add-waveshare-dsi-backlight-driver.patch \
    file://0002-goodix-touch-poll-mode.patch \
    file://0003-allow-non-zero-decimals.patch \
"

PROVIDES += "virtual/kernel"

# In-tree fragments merged on top of rk3506_defconfig.
# NOTE: rk3506-display.config is deliberately NOT included — display
# stack is deferred until a test panel is available.
KERNEL_CONFIG_FRAGMENTS = " \
    rk3506-ethernet.config \
    rk3506-usb-host.config \
    rk3506-wifibt.config \
    file://esp32-c6.cfg \
"

COMPATIBLE_MACHINE = "power-window"

LINUX_VERSION = "6.1"
LINUX_VERSION_EXTENSION = "-rockchip-rk3506"

inherit kernel

# S must be set AFTER `inherit kernel` — class also sets S, and our
# override tells do_symlink_kernsrc to use the git checkout.
S = "${WORKDIR}/git"

KBUILD_DEFCONFIG = "rk3506_defconfig"

do_configure:prepend() {
    # Stage out-of-tree .dtsi includes first (they are #include'd by the
    # .dts files below but are not themselves Makefile build targets).
    for dtsi in ${WORKDIR}/*.dtsi; do
        [ -f "$dtsi" ] || continue
        cp "$dtsi" ${S}/arch/arm/boot/dts/
    done

    # Stage out-of-tree DTS into the kernel's dts dir and register each
    # in the Makefile so it gets compiled alongside in-tree DTBs.
    for dts in ${WORKDIR}/*.dts; do
        [ -f "$dts" ] || continue
        cp "$dts" ${S}/arch/arm/boot/dts/
        dtb=$(basename "$dts" .dts).dtb
        if ! grep -q "$dtb" ${S}/arch/arm/boot/dts/Makefile; then
            sed -i "/rk3506b-evb1-v10.dtb/a\\\\t${dtb} \\\\" \
                ${S}/arch/arm/boot/dts/Makefile
        fi
    done
}

# kernel.bbclass does not apply KBUILD_DEFCONFIG automatically — that's
# kernel-yocto's job. Apply it here, merge SRC_URI .cfg fragments and
# in-tree .config fragments, then olddefconfig for dependency resolution.
do_configure() {
    oe_runmake -C ${S} O=${B} ${KBUILD_DEFCONFIG}

    for cfg in ${WORKDIR}/*.cfg; do
        [ -f "$cfg" ] || continue
        ${S}/scripts/kconfig/merge_config.sh -m -O ${B} ${B}/.config "$cfg"
    done

    for frag in ${KERNEL_CONFIG_FRAGMENTS}; do
        if [ -f "${S}/arch/arm/configs/$frag" ]; then
            ${S}/scripts/kconfig/merge_config.sh -m -O ${B} ${B}/.config \
                ${S}/arch/arm/configs/$frag
        fi
    done

    oe_runmake -C ${S} O=${B} olddefconfig
}