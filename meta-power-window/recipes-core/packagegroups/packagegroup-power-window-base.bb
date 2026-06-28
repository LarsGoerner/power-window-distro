SUMMARY = "Power Window RK3506 base system packages"
DESCRIPTION = "Base userspace bundle for RK3506 boards: openssh, iproute2, \
ethtool, util-linux, e2fsprogs. Pulls in wpa-supplicant + iw + wireless-regdb \
when MACHINE_FEATURES includes 'wifi'."
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

inherit packagegroup

RDEPENDS:${PN} = "\
    e2fsprogs \
    ethtool \
    iproute2 \
    openssh-keygen \
    openssh-sftp-server \
    openssh-ssh \
    openssh-sshd \
    util-linux \
"

RRECOMMENDS:${PN} = " \
    ${@bb.utils.contains('MACHINE_FEATURES', 'wifi', 'wpa-supplicant iw wireless-regdb', '', d)} \
"