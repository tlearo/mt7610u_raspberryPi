# Set default mode to STA if not provided
ifeq ($(WIFI_MODE),)
RT28xx_MODE = STA
else
RT28xx_MODE = $(WIFI_MODE)
endif

# Set default chipset if not provided
ifeq ($(CHIPSET),)
CHIPSET = mt7610u
endif

MODULE = $(CHIPSET)

# Define paths
RT28xx_DIR := $(shell pwd)
LINUX_SRC := /lib/modules/$(shell uname -r)/build
LINUX_SRC_MODULE := /lib/modules/$(shell uname -r)/kernel/drivers/net/wireless/
CROSS_COMPILE ?=

# Export variables for use in sub-Makefiles
export RT28xx_DIR RT28xx_MODE LINUX_SRC CROSS_COMPILE MODULE

# Targets
.PHONY: all clean install uninstall

# Default build target
all:
	@echo "Building $(MODULE) driver for kernel $(shell uname -r)"
	$(MAKE) -C $(LINUX_SRC) M=$(RT28xx_DIR)/os/linux modules

# Clean target
clean:
	@echo "Cleaning build artifacts..."
	$(MAKE) -C $(LINUX_SRC) M=$(RT28xx_DIR)/os/linux clean
	@rm -rf os/linux/*.mod os/linux/*.o os/linux/.*.cmd os/linux/*.ko os/linux/*.mod.c
	@echo "Clean complete."

# Install target
install:
	@echo "Installing $(MODULE).ko to $(LINUX_SRC_MODULE)"
	@sudo cp -v os/linux/$(MODULE).ko $(LINUX_SRC_MODULE)
	@sudo depmod -a
	@echo "Installation complete. Please reboot or reload the module."

# Uninstall target
uninstall:
	@echo "Removing $(MODULE).ko from $(LINUX_SRC_MODULE)"
	@sudo rm -v $(LINUX_SRC_MODULE)/$(MODULE).ko
	@sudo depmod -a
	@echo "Uninstallation complete."

# Additional helper target for debugging
rebuild: clean all

