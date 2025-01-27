# Chipset Configuration
CHIPSET = mt7610u
MODULE = $(CHIPSET)

# Kernel and Driver Mode
RT28xx_MODE = STA
HAS_CFG80211_SUPPORT = y

# Features and Functionality
HAS_WPA_SUPPLICANT = y
HAS_NATIVE_WPA_SUPPLICANT_SUPPORT = y
HAS_DOT11_N_SUPPORT = y
HAS_DOT11_VHT_SUPPORT = y
HAS_NEW_RATE_ADAPT_SUPPORT = y
HAS_STATS_COUNT = y

# USB Configuration
HAS_USB_SUPPORT_SELECTIVE_SUSPEND = n
HAS_USB_FIRMWARE_MULTIBYTE_WRITE = n

# Logging and Debugging
WFLAGS += -g -Wall -Wstrict-prototypes -Wno-trigraphs
WFLAGS += -DSYSTEM_LOG_SUPPORT -DENHANCED_STAT_DISPLAY
WFLAGS += -DAGGREGATION_SUPPORT -DPIGGYBACK_SUPPORT -DWMM_SUPPORT
WFLAGS += -DRT28xx_MODE=$(RT28xx_MODE) -DCHIPSET=$(MODULE)
WFLAGS += -DRT_CFG80211_SUPPORT -DEXT_BUILD_CHANNEL_LIST

# Include Paths
WFLAGS += -I$(RT28xx_DIR)/include

# Kernel-Specific Settings
EXTRA_CFLAGS := $(WFLAGS)

# Compiler and Linker Settings
CC := $(CROSS_COMPILE)gcc
LD := $(CROSS_COMPILE)ld

# Platform-Specific Settings
ifeq ($(PLATFORM),PC)
    LINUX_SRC = /lib/modules/$(shell uname -r)/build
    EXTRA_CFLAGS += -DLINUX
endif
