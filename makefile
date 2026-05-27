MODULES = hal
BOARD ?= edu-ciaa-nxp
VERBOSE=n
MUJU ?= ./muju

CFLAGS += -Ihal/inc

include $(MUJU)/module/base/makefile
