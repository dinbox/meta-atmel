SECTION = "kernel"
DESCRIPTION = "Linux kernel for Microchip ARM SoCs (aka AT91)"
SUMMARY = "Linux kernel for Microchip ARM SoCs (aka AT91)"
LICENSE = "GPLv2"

inherit kernel

RDEPENDS_${KERNEL_PACKAGE_NAME}-base = ""

S = "${WORKDIR}/git"

python __anonymous () {
    if d.getVar('UBOOT_FIT_IMAGE', True) == 'xyes':
        d.appendVar('DEPENDS', ' u-boot-mkimage-native dtc-native')
}

do_configure_prepend() {
	if [ ! -f "${WORKDIR}/defconfig" ] && [ -n "${KBUILD_DEFCONFIG}" ]; then
		if [ -f "${S}/arch/${ARCH}/configs/${KBUILD_DEFCONFIG}" ]; then
			cp -f ${S}/arch/${ARCH}/configs/${KBUILD_DEFCONFIG} ${WORKDIR}/defconfig
                else
                        bbfatal "A KBUILD_DEFCONFIG '${KBUILD_DEFCONFIG}' was specified, but not present in the source tree"
                fi
        fi
}

do_configure_append() {
	frags=""
	for fragment in ${WORKDIR}/*.cfg
	do
		if [ -f ${fragment} ]; then
			cp -v ${fragment} ${B}
			frags=$frags" `basename ${fragment}`"
		fi
	done

	if [ ! -z "${frags}" ]; then
		echo "Fragments are: ${frags}"
		PATH=${S}/scripts/kconfig:${PATH}
		CFLAGS="${CFLAGS} ${TOOLCHAIN_OPTIONS}" HOSTCC="${BUILD_CC} ${BUILD_CFLAGS} ${BUILD_LDFLAGS}" HOSTCPP="${BUILD_CPP}" CC="${KERNEL_CC}" ARCH=${ARCH} merge_config.sh -n .config ${frags}  2>&1
		if [ $? -ne 0 ]; then
			bbfatal_log "Could not configure kernel fragments: ${frags}"
		fi
	fi
}

do_deploy_append() {
	if [ "${UBOOT_FIT_IMAGE}" = "xyes" ]; then
		DTB_PATH="${B}/arch/${ARCH}/boot/dts/"
		if [ ! -e "${DTB_PATH}" ]; then
			DTB_PATH="${B}/arch/${ARCH}/boot/"
		fi

		if [ -e ${S}/arch/${ARCH}/boot/dts/${MACHINE}.its ]; then
			cp ${S}/arch/${ARCH}/boot/dts/${MACHINE}*.its ${DTB_PATH}
			cd ${DTB_PATH}
			mkimage -f ${MACHINE}.its ${MACHINE}.itb
			install -m 0644 ${MACHINE}.itb ${DEPLOYDIR}/${MACHINE}.itb
			cd -
		fi
	fi
}

KERNEL_MODULE_AUTOLOAD += "atmel_usba_udc g_serial"
