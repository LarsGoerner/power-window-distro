# Shared machine settings for all Rockchip RK3506 SoC family.
#
# Individual board .conf files include this and set board-specific
# overrides (DTB name, RAM size, machine features).

# RK3506 is triple Cortex-A7, 32-bit ARMv7-A with NEON + VFPv4-D32.
# Yocto's tune-cortexa7 covers the per-core tuning; the "triple core"
# aspect is kernel-visible only.
require conf/machine/include/arm/armv7a/tune-cortexa7.inc

DEFAULTTUNE = "armv7athf-neon"

# Vendor BSP kernel — no mainline support for RK3506 exists.
OLDEST_KERNEL = "6.1"

PREFERRED_PROVIDER_virtual/kernel = "linux-rockchip-rk3506"
PREFERRED_PROVIDER_virtual/bootloader = "u-boot-rockchip-rk3506"
PREFERRED_VERSION_linux-rockchip-rk3506 = "6.1%"

# In-tree defconfig shipped by the vendor kernel tree.
KBUILD_DEFCONFIG = "rk3506_defconfig"

SERIAL_CONSOLES = "115200;ttyS0"
KERNEL_CONSOLE = "ttyS0,115200"

# rkbin blob / U-Boot config fragment selection for the RK3506 SoC family.
# Boards override these in their machine conf:
#   RK3506G2: RKBIN_SOC_VARIANT = "rk3506"  (default)
#   RK3506B : RKBIN_SOC_VARIANT = "rk3506b" + RK3506_UBOOT_CONFIG_FRAGMENT = "rk3506b.config"
RKBIN_SOC_VARIANT ?= "rk3506"
RK3506_UBOOT_CONFIG_FRAGMENT ?= ""

IMAGE_FSTYPES = "wic ext4"

IMAGE_BOOT_FILES = "zImage ${KERNEL_DEVICETREE}"

IMAGE_CLASSES += "rockchip_rk3506_image"

MACHINE_FEATURES = "usbhost"

# Same wic fstab-injection issue as the RV1106 layer — wic otherwise
# emits a phantom /dev/sda1 /boot line that blocks local-fs.target for
# 90 s on boot and drops to emergency mode.
WIC_CREATE_EXTRA_ARGS = "--no-fstab-update"