ifeq ($(WIFI_MODE),)
RT28xx_MODE = STA
else
RT28xx_MODE = $(WIFI_MODE)
endif

ifeq ($(CHIPSET),)
CHIPSET = mt7610u
endif

MODULE = $(CHIPSET)

RT28xx_DIR = $(shell pwd)
LINUX_SRC = /lib/modules/$(shell uname -r)/build
LINUX_SRC_MODULE = /lib/modules/$(shell uname -r)/kernel/drivers/net/wireless/
CROSS_COMPILE =

export RT28xx_DIR RT28xx_MODE LINUX_SRC CROSS_COMPILE MODULE

# Targets
.PHONY: all clean install uninstall

all:
	$(MAKE) -C $(LINUX_SRC) M=$(RT28xx_DIR)/os/linux modules

clean:
	$(MAKE) -C $(LINUX_SRC) M=$(RT28xx_DIR)/os/linux clean
	rm -rf os/linux/*.mod os/linux/*.o os/linux/.*.cmd os/linux/*.ko os/linux/*.mod.c

install:
	cp -v os/linux/$(MODULE).ko /lib/modules/$(shell uname -r)/kernel/drivers/net/wireless/
	depmod -a

uninstall:
	rm -v /lib/modules/$(shell uname -r)/kernel/drivers/net/wireless/$(MODULE).ko
	depmod -a
