# Atmel SAMA5 defaults
require conf/machine/include/soc-family.inc
require conf/machine/include/tune-cortexa5.inc
require conf/machine/include/bootloaders.inc

SOC_FAMILY = "sama5"

PREFERRED_PROVIDER_virtual/kernel_sama5 ?= "linux-mchp"
PREFERRED_PROVIDER_virtual/bootloader_sama5 ?= "u-boot-at91"
PREFERRED_PROVIDER_u-boot_sama5 ?= "u-boot-at91"

PREFERRED_PROVIDER_jpeg ?= "jpeg"
PREFERRED_PROVIDER_jpeg-native ?= "jpeg-native"

SERIAL_CONSOLES ?= "115200;ttyS0"
WIC_CREATE_EXTRA_ARGS ?= "--no-fstab-update"
