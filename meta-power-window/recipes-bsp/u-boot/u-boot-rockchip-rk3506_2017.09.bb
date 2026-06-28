# Rockchip vendor U-Boot 2017.09 for RK3506G2.
#
# Produces idbloader.img (DDR init + U-Boot SPL) and u-boot.img
# (U-Boot + OP-TEE FIT). Both are consumed by the wic image assembly
# via the Rockchip boot chain.
#
# Pinning copied from the validated Core3506 reference BSP at
# ~/projects/luckfox-yocto/ (2026-04-10 bringup). The two repos are
# intentionally kept separate — see docs/REFERENCE.md.

SUMMARY = "Rockchip vendor U-Boot 2017.09 for RK3506G2"
DESCRIPTION = "Rockchip's vendor U-Boot 2017.09 fork for the RK3506 SoC family. \
Produces idbloader.img (DDR init + U-Boot SPL) and u-boot.img (FIT with \
OP-TEE + U-Boot) for the Rockchip boot chain. No mainline U-Boot support \
for RK3506 exists at time of writing."
HOMEPAGE = "https://github.com/rockchip-linux/u-boot"

LICENSE = "GPL-2.0-only"
LIC_FILES_CHKSUM = "file://Licenses/gpl-2.0.txt;md5=b234ee4d69f5fce4486a80fdaf4a4263"

DEPENDS += "bc-native dtc-native python3-native"

SRC_URI = "\
    git://github.com/rockchip-linux/u-boot.git;protocol=https;branch=next-dev;name=default \
    git://github.com/rockchip-linux/rkbin.git;protocol=https;branch=master;name=rkbin;destsuffix=rkbin \
    file://rk3506-distroboot.config \
    file://fit-signature.config \
    file://0001-rk3506-prioritize-distro-over-boot_fit.patch \
"

SRCREV = "aeec6f2bfd5ce0cfcdfe0ffc7f84d9d143683856"
SRCREV_rkbin = "89e1a7edf48293f6b531c5eca568a775d1694808"
SRCREV_FORMAT = "default_rkbin"

PROVIDES += "virtual/bootloader"

S = "${WORKDIR}/git"
# Vendor U-Boot 2017.09 doesn't handle out-of-tree build cleanly.
B = "${S}"

# Layer ships only the Lyra Ultra machine; refuse to bind to anything else
# even if another layer defines a clashing machine name.
COMPATIBLE_MACHINE = "power-window"

# Machine config supplies UBOOT_MACHINE (defconfig target) and, for
# RK3506B boards, RK3506_UBOOT_CONFIG_FRAGMENT = "rk3506b.config".
UBOOT_MACHINE ?= "rk3506_defconfig"
RK3506_UBOOT_CONFIG_FRAGMENT ?= ""

# rkbin blob paths selected by SoC variant (rk3506 vs rk3506b).
RKBIN_SOC_VARIANT ?= "rk3506"
RKBIN_DDR = "${@'bin/rk35/rk3506b_ddr_750MHz_v1.06.bin' if d.getVar('RKBIN_SOC_VARIANT') == 'rk3506b' else 'bin/rk35/rk3506_ddr_750MHz_v1.06.bin'}"
RKBIN_TEE = "bin/rk35/rk3506_tee_v2.40.bin"
RKBIN_MINIALL_INI = "${@'RKBOOT/RK3506BMINIALL.ini' if d.getVar('RKBIN_SOC_VARIANT') == 'rk3506b' else 'RKBOOT/RK3506MINIALL.ini'}"

# OP-TEE secure-world provider toggle.
#   "rkbin"           - vendor closed blob rk3506_tee_v2.10.bin (default,
#                       and the only provider shipped in this public repo —
#                       builds out of the box for everyone).
#   "optee-os-rk3506" - source-built OP-TEE port. NOT included in this
#                       public release: the optee-os-rk3506 / -tadevkit
#                       recipes fetched from a developer-local optee_os
#                       tree that external builders don't have, so they
#                       were removed. The toggle + load-address wiring is
#                       kept so the port can be dropped back in (re-add the
#                       recipes) once it is upstreamed. Selecting it without
#                       those recipes present yields "nothing provides
#                       optee-os-rk3506".
# The FIT optee load/entry differs by provider: the vendor blob wants
# 0x1000 (RK3506TOS.ini); the source port ran from the M5-relocated TZDRAM
# 0x18000000 (optee_os plat-rockchip/conf.mk CFG_TZDRAM_START).
OPTEE_PROVIDER ??= "rkbin"

# R1 HW secure-RAM isolation toggle (default "0"). When "1": OP-TEE TEE_RAM
# is relocated low and FW_DDR slot-0 HW-isolates [0,16M) from the NS CPU
# (the validated config, JOURNAL cont.22-24). Requires OPTEE_PROVIDER =
# "optee-os-rk3506" (our source OP-TEE, built with CFG_RK3506_TEE_HW_ISOLATE
# =y by its recipe). The 0004 patch relocates the NS boot structures above
# the protected region + migrates the preloader atags out of it; the FIT
# optee load drops to 0x1000 (= CFG_TZDRAM_START). See docs/R1_UBOOT_PORT.md
# + LICENSING §6.1 in the optee-rk3506 repo.
#RK3506_TEE_HW_ISOLATE ??= "0"
#SRC_URI += "${@bb.utils.contains('RK3506_TEE_HW_ISOLATE', '1', 'file://0004-rk3506-R1-relocate-atags-for-hw-isolated-tee.patch', '', d)}"

# FIT optee load: 0x1000 for the vendor blob (RK3506TOS.ini) AND for the
# HW-isolate low-TEE config; 0x18000000 for our M5-relocated (no-isolate)
# source build.
OPTEE_TEE_LOAD = "${@'0x1000' if (d.getVar('RK3506_TEE_HW_ISOLATE') == '1' or d.getVar('OPTEE_PROVIDER') != 'optee-os-rk3506') else '0x18000000'}"
DEPENDS += "${@'optee-os-rk3506' if d.getVar('OPTEE_PROVIDER') == 'optee-os-rk3506' else ''}"

# Secure-boot / FIT-signature toggle (default "0"; SECURITY.md §6 steps 1-4
# / §7 step 8). When "1": the SPL is built with CONFIG_SPL_FIT_SIGNATURE=y
# and the FIT is RSA-2048-signed with the pubkey embedded in the SPL
# control DTB, so the SPL verifies the FIT config signature and refuses an
# unsigned/wrong-key FIT. Enforced UN-FUSED — no OTP/eFuse burn (R11). On-
# target proven 2026-05-23 (wrong-key FIT rejected; JOURNAL). eFuse burns
# (BootROM-mandated authenticity) stay production-only.
#
# RK3506_TEE_FIT_KEY_DIR: dir holding the signing key as dev.key (RSA-2048
# private PEM) + dev.crt (matching the FIT signature node's
# key-name-hint="dev"). Leave empty for a dev build and the recipe
# generates an EPHEMERAL throwaway key under ${B}/fit-keys (reused across
# rebuilds in the same build tree). Production MUST set this to an
# offline/HSM key dir.
RK3506_TEE_FIT_SIGNATURE ??= "0"
RK3506_TEE_FIT_KEY_DIR ??= ""
# CONFIG_FIT_SIGNATURE makes the host mkimage link libcrypto; keygen also
# needs openssl. Pull openssl-native only when signing is enabled.
DEPENDS += "${@'openssl-native' if d.getVar('RK3506_TEE_FIT_SIGNATURE') == '1' else ''}"

inherit deploy

# HOSTCC: vendor U-Boot 2017.09 defaults to HOSTCC=cc (doesn't exist
# in Yocto's env). KCFLAGS: -Werror trips on GCC 13.x
# -Wmay-be-uninitialized in scarthgap.
EXTRA_OEMAKE = " \
    HOSTCC='gcc' \
    HOSTCXX='g++' \
    CROSS_COMPILE=${TARGET_PREFIX} \
    PYTHON=python3 \
    KCFLAGS='-Wno-error' \
"

do_configure() {
    oe_runmake ${UBOOT_MACHINE}

    if [ -n "${RK3506_UBOOT_CONFIG_FRAGMENT}" ]; then
        if [ -f "${S}/configs/${RK3506_UBOOT_CONFIG_FRAGMENT}" ]; then
            ${S}/scripts/kconfig/merge_config.sh -m -O ${B} ${B}/.config \
                ${S}/configs/${RK3506_UBOOT_CONFIG_FRAGMENT}
        else
            bbwarn "Config fragment ${RK3506_UBOOT_CONFIG_FRAGMENT} not found, skipping"
        fi
    fi

    # Distro-boot fallback (enables CMD_PART, CMD_FS_GENERIC, extlinux loader).
    if [ -f "${WORKDIR}/rk3506-distroboot.config" ]; then
        ${S}/scripts/kconfig/merge_config.sh -m -O ${B} ${B}/.config \
            ${WORKDIR}/rk3506-distroboot.config
    fi

    # FIT-signature: CONFIG_FIT_SIGNATURE + CONFIG_SPL_FIT_SIGNATURE.
    if [ "${RK3506_TEE_FIT_SIGNATURE}" = "1" ] && [ -f "${WORKDIR}/fit-signature.config" ]; then
        ${S}/scripts/kconfig/merge_config.sh -m -O ${B} ${B}/.config \
            ${WORKDIR}/fit-signature.config
        # merge_config.sh -m only merges; enabling FIT_SIGNATURE reveals NEW
        # symbols (e.g. FIT_ROLLBACK_PROTECT) with no default, which would
        # make do_compile's silentoldconfig prompt and abort. Resolve all
        # new symbols to their defaults non-interactively.
        oe_runmake olddefconfig
    fi
}

#do_compile:prepend() {
#    # SD-boot fix: arch_cpu_init() calls board_set_iomux() which
#    # overwrites the BootROM's pin mux with eMMC-specific values,
#    # breaking SPL reads from SD. Remove both the SET and the matching
#    # UNSET in spl_board_storages_fixup; the BootROM already configured
#    # the correct mux for whatever media it selected.
#    sed -i 's|board_set_iomux(IF_TYPE_MMC|// board_set_iomux(IF_TYPE_MMC|' \
#        ${S}/arch/arm/mach-rockchip/rk3506/rk3506.c
#    sed -i '/spl_board_storages_fixup/,/^}/ {
#        s|if (loader->boot_device.*|/* SD boot fix: skip iomux unset */|
#        s|.*board_unset_iomux.*|/* board_unset_iomux removed */|
#    }' ${S}/arch/arm/mach-rockchip/rk3506/rk3506.c
#}

do_compile:prepend() {
    # SD-boot fix: arch_cpu_init() calls board_set_iomux() which
    # overwrites the BootROM's pin mux with eMMC-specific values,
    # breaking SPL reads from SD. Remove both the SET and the matching
    # UNSET in spl_board_storages_fixup; the BootROM already configured
    # the correct mux for whatever media it selected.
    sed -i 's|board_set_iomux(IF_TYPE_MMC|// board_set_iomux(IF_TYPE_MMC|' \
        ${S}/arch/arm/mach-rockchip/rk3506/rk3506.c
    sed -i '/^void spl_board_storages_fixup/,/^}$/c\
    void spl_board_storages_fixup(struct spl_image_loader *loader)\
    { \
        if (!loader) { return; } \
    }' ${S}/arch/arm/mach-rockchip/rk3506/rk3506.c
}

do_compile() {
    # make_fit_optee.sh expects tee.bin in the source tree. OPTEE_PROVIDER
    # selects which secure world: the vendor closed blob (default,
    # reverts cleanly) or our upstream port staged by optee-os-rk3506
    # (already 28-byte-header-stripped, FIT-ready).
    if [ "${OPTEE_PROVIDER}" = "optee-os-rk3506" ]; then
        cp ${RECIPE_SYSROOT}${datadir}/optee-rk3506/tee.bin ${S}/tee.bin
    else
        cp ${WORKDIR}/rkbin/${RKBIN_TEE} ${S}/tee.bin
    fi

    oe_runmake

    # The Makefile's `u-boot.img` target uses `mkimage -f auto` which
    # omits the OP-TEE node. `u-boot.itb` uses make_fit_optee.sh which
    # includes it, but the default TEE offset (0x08400000) is wrong for
    # RK3506 — the load/entry must be OPTEE_TEE_LOAD (0x1000 for the
    # vendor blob per RK3506TOS.ini; 0x18000000 for our M5-relocated
    # TZDRAM). Regenerate manually.
    ${S}/arch/arm/mach-rockchip/make_fit_optee.sh -t ${OPTEE_TEE_LOAD} > ${B}/u-boot.its

    if [ "${RK3506_TEE_FIT_SIGNATURE}" = "1" ]; then
        # Sign the FIT and embed the RSA pubkey in the SPL control DTB so the
        # SPL enforces the signature (un-fused). make_fit_optee.sh already
        # emits the signature node (key-name-hint="dev"); mkimage signs it.
        KEYDIR="${RK3506_TEE_FIT_KEY_DIR}"
        if [ -z "${KEYDIR}" ]; then
            KEYDIR="${B}/fit-keys"
            if [ ! -f "${KEYDIR}/dev.key" ]; then
                mkdir -p "${KEYDIR}"
                openssl genpkey -algorithm RSA -out "${KEYDIR}/dev.key" -pkeyopt rsa_keygen_bits:2048
                openssl req -batch -new -x509 -key "${KEYDIR}/dev.key" -out "${KEYDIR}/dev.crt" \
                    -days 3650 -subj "/CN=rk3506-optee-dev-fit-signing-key"
                bbwarn "RK3506_TEE_FIT_SIGNATURE: generated EPHEMERAL dev signing key at ${KEYDIR}. Set RK3506_TEE_FIT_KEY_DIR to an offline/HSM key for production."
            fi
        fi
        # -p 0x1200 (rockchip OFFS_DATA) is REQUIRED for external-data
        # signing; -K injects the pubkey into the SPL DTB; -r marks the
        # config signature required.
        ${B}/tools/mkimage -f ${B}/u-boot.its -k "${KEYDIR}" -K ${B}/spl/u-boot-spl.dtb \
            -E -p 0x1200 -r ${B}/u-boot.itb
        # Repack u-boot-spl.bin so the SPL carries the now-signed control DTB
        # (mirrors the build's nodtb + [bss-pad] + dtb cat; the pad.bin is
        # absent iff CONFIG_SPL_SEPARATE_BSS). boot_merger below picks this up.
        if [ -f "${B}/spl/u-boot-spl-pad.bin" ]; then
            cat ${B}/spl/u-boot-spl-nodtb.bin ${B}/spl/u-boot-spl-pad.bin ${B}/spl/u-boot-spl.dtb > ${B}/spl/u-boot-spl.bin
        else
            cat ${B}/spl/u-boot-spl-nodtb.bin ${B}/spl/u-boot-spl.dtb > ${B}/spl/u-boot-spl.bin
        fi
    else
        ${B}/tools/mkimage -f ${B}/u-boot.its -E ${B}/u-boot.itb
    fi
    cp ${B}/u-boot.itb ${B}/u-boot.img

    # idbloader.img: vendor SPL blob does NOT support OP-TEE FIT loading.
    # The compiled U-Boot SPL (CONFIG_SPL_OPTEE=y) has the right FIT
    # parser. Use boot_merger with the compiled SPL.
    cp ${WORKDIR}/rkbin/${RKBIN_MINIALL_INI} ${WORKDIR}/rkbin/miniall-spl.ini
    sed -i "s|FlashBoot=.*|FlashBoot=${B}/spl/u-boot-spl.bin|" ${WORKDIR}/rkbin/miniall-spl.ini
    sed -i "s|^PATH=.*|PATH=${B}/idbloader-spl.bin|" ${WORKDIR}/rkbin/miniall-spl.ini
    sed -i "s|^IDB_PATH=.*|IDB_PATH=${B}/idbloader.img|" ${WORKDIR}/rkbin/miniall-spl.ini
    cd ${WORKDIR}/rkbin && ./tools/boot_merger miniall-spl.ini && cd ${B}
}

do_deploy() {
    install -d ${DEPLOYDIR}
    [ -f "${B}/idbloader.img" ]     && install -m 0644 ${B}/idbloader.img     ${DEPLOYDIR}/idbloader.img
    [ -f "${B}/u-boot.img" ]        && install -m 0644 ${B}/u-boot.img        ${DEPLOYDIR}/u-boot.img
    [ -f "${B}/idbloader-spl.bin" ] && install -m 0644 ${B}/idbloader-spl.bin ${DEPLOYDIR}/idbloader-spl.bin
    [ -f "${B}/u-boot-dtb.bin" ]    && install -m 0644 ${B}/u-boot-dtb.bin    ${DEPLOYDIR}/u-boot-dtb.bin
    [ -f "${B}/spl/u-boot-spl.bin" ] && install -m 0644 ${B}/spl/u-boot-spl.bin ${DEPLOYDIR}/u-boot-spl.bin
}

addtask deploy after do_compile

# Vendor 2017.09 predates modern U-Boot QA conventions.
INSANE_SKIP:${PN} = "ldflags"