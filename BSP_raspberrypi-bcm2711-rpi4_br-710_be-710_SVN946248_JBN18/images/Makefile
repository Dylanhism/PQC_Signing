ROOT_DIR := $(notdir $(CURDIR))
ifndef QCONFIG
QCONFIG=qconfig.mk
endif
include $(QCONFIG)

HOST_MKIFS := mkifs
HOST_OBJCOPY := ntoaarch64-objcopy

SUFFIXES := .build .ifs .bin

.PHONY: all clean rpi4

all: rpi4

rpi4: ifs-rpi4

ifs-%: %.build
	$(HOST_MKIFS) -vv -r../install $(MKIFSFLAGS) $^ $@.bin

clean:
	$(RM_HOST) ifs-*.bin *.sym ssh_host_*_key*
