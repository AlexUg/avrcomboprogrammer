#
#             LUFA Library
#     Copyright (C) Dean Camera, 2019.
#
#  dean [at] fourwalledcubicle [dot] com
#           www.lufa-lib.org
#
# --------------------------------------
#         LUFA Project Makefile.
# --------------------------------------

# Run "make help" for target help.

LUFA_PATH    = /home/alexandr/git/electric/lufa/LUFA

MCU          = atmega32u4
ARCH         = AVR8
BOARD        = AVRISP_SERIAL
F_CPU        = 16000000
F_USB        = $(F_CPU)
OPTIMIZATION = s
TARGET       = AVRISP-MKII_Serial
SRC          = main.c AVRISP-MKII.c USBtoSerial.c Descriptors.c Lib/V2Protocol.c Lib/V2ProtocolParams.c Lib/ISP/ISPProtocol.c Lib/ISP/ISPTarget.c Lib/XPROG/XPROGProtocol.c \
               Lib/XPROG/XPROGTarget.c Lib/XPROG/XMEGANVM.c Lib/XPROG/TINYNVM.c $(LUFA_SRC_USB) $(LUFA_SRC_USBCLASS)
CC_FLAGS     = -DSERIAL_ENABLE -DUSE_LUFA_CONFIG_HEADER -IConfig/
LD_FLAGS     = 

AVRDUDE_PORT = /dev/ttyACM0
AVRDUDE_BAUD = 57600
AVRDUDE_PROGRAMMER = avr109

# Default target
all:

# Include LUFA-specific DMBS extension modules
DMBS_LUFA_PATH ?= $(LUFA_PATH)/Build/LUFA
include $(DMBS_LUFA_PATH)/lufa-sources.mk
include $(DMBS_LUFA_PATH)/lufa-gcc.mk

# Include common DMBS build system modules
DMBS_PATH      ?= $(LUFA_PATH)/Build/DMBS/DMBS
include $(DMBS_PATH)/core.mk
include $(DMBS_PATH)/cppcheck.mk
include $(DMBS_PATH)/doxygen.mk
include $(DMBS_PATH)/dfu.mk
include $(DMBS_PATH)/gcc.mk
include $(DMBS_PATH)/hid.mk
include $(DMBS_PATH)/avrdude.mk
include $(DMBS_PATH)/atprogram.mk


