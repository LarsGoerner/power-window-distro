
FILESEXTRAPATHS:prepend := "${THISDIR}/files:"
SRC_URI += "file://0001-linuxfb-add-rotation-support.patch"

PACKAGECONFIG:remove = "eglfs gles2 opengl"
PACKAGECONFIG:append = " linuxfb openssl"