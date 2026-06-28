# Rockchip proprietary boot blobs for RK3506
#
# Stages DDR init, SPL, OP-TEE and MINIALL.ini from the rkbin repo into
# DEPLOYDIR so the U-Boot recipe and host-side rkdeveloptool flasher
# can consume them. The Lyra Ultra W uses the RK3506B DDR blob variant
# (rk3506b_ddr_*.bin); other blobs are shared with RK3506G2.
#
# Pinning copied from ~/projects/luckfox-yocto/ (2026-04-10 bringup).

SUMMARY = "Rockchip binary blobs for RK3506B boot chain"
DESCRIPTION = "Proprietary DDR init, SPL, OP-TEE and USB downloader \
binaries from Rockchip's rkbin repository. Required by the RK3506 boot \
chain — the BootROM cannot initialize DDR without the rk3506b_ddr blob."
HOMEPAGE = "https://github.com/rockchip-linux/rkbin"

LICENSE = "CLOSED"
LICENSE_FLAGS = "commercial"

SRC_URI = "git://github.com/rockchip-linux/rkbin.git;protocol=https;branch=master"
SRCREV = "89e1a7edf48293f6b531c5eca568a775d1694808"

S = "${WORKDIR}/git"

COMPATIBLE_MACHINE = "luckfox-lyra-ultra"

inherit deploy

# RK3506 blob selection.
RKBIN_SOC_VARIANT ?= "rk3506"
RKBIN_DDR = "bin/rk35/rk3506_ddr_750MHz_v1.06.bin"
RKBIN_SPL = "bin/rk35/rk3506_spl_v1.12.bin"
RKBIN_USBPLUG = "bin/rk35/rk3506_usbplug_v1.03.bin"
RKBIN_TEE = "bin/rk35/rk3506_tee_v2.40.bin"
RKBIN_MINIALL_INI = "RKBOOT/RK3506MINIALL.ini"

do_configure[noexec] = "1"
do_compile[noexec] = "1"

do_install() {
    # Fail loudly if the pinned commit no longer ships the expected blobs.
    if [ ! -f "${S}/${RKBIN_MINIALL_INI}" ]; then
        bbfatal "MINIALL.ini not found at ${RKBIN_MINIALL_INI} — pinned rkbin SRCREV may be stale"
    fi
    for blob in "${RKBIN_DDR}" "${RKBIN_SPL}" "${RKBIN_USBPLUG}" "${RKBIN_TEE}"; do
        if [ ! -f "${S}/${blob}" ]; then
            bbfatal "Required blob missing: ${blob} at rkbin SRCREV ${SRCREV}"
        fi
    done

    install -d ${D}${datadir}/rkbin
    install -m 0644 ${S}/${RKBIN_DDR}          ${D}${datadir}/rkbin/
    install -m 0644 ${S}/${RKBIN_SPL}          ${D}${datadir}/rkbin/
    install -m 0644 ${S}/${RKBIN_USBPLUG}      ${D}${datadir}/rkbin/
    install -m 0644 ${S}/${RKBIN_TEE}          ${D}${datadir}/rkbin/
    install -m 0644 ${S}/${RKBIN_MINIALL_INI}  ${D}${datadir}/rkbin/
}

do_deploy() {
    install -d ${DEPLOYDIR}
    install -m 0644 ${S}/${RKBIN_DDR}          ${DEPLOYDIR}/
    install -m 0644 ${S}/${RKBIN_SPL}          ${DEPLOYDIR}/
    install -m 0644 ${S}/${RKBIN_USBPLUG}      ${DEPLOYDIR}/
    install -m 0644 ${S}/${RKBIN_TEE}          ${DEPLOYDIR}/
    install -m 0644 ${S}/${RKBIN_MINIALL_INI}  ${DEPLOYDIR}/
}

addtask deploy after do_install

FILES:${PN} = "${datadir}/rkbin"

# Pre-built blobs, not built from source.
INSANE_SKIP:${PN} = "already-stripped"