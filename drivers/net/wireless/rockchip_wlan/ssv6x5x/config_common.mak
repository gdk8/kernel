CONFIG_SSV6X5X=m
#ccflags-y += -DCONFIG_SSV6200_CORE
CONFIG_MAC80211_LEDS=y
CONFIG_MAC80211_DEBUGFS=y
CONFIG_MAC80211_MESH=y
CONFIG_PM=y
CONFIG_MAC80211_RC_MINSTREL=y
CONFIG_MAC80211_RC_MINSTREL_HT=y

//ccflags-y += -D_ICOMM_MAC80211_

ccflags-y += -D__CHECK_ENDIAN__ -DDEBUG
###########################################################################
# Compiler options                                                        #
###########################################################################
ccflags-y += -Werror

# Enable -g to help debug. Deassembly from .o to .S would help to track to 
# the problomatic line from call stack dump.
#ccflags-y += -g

#########################################################
# option enable shal
#ccflags-y += -DSSV_SUPPORT_SSV6006
ccflags-y += -DSSV_SUPPORT_SSV6020

############################################################
# If you change the settings, please change the file synchronization
# smac\firmware\include\config.h & compiler firmware
############################################################
#ccflags-y += -DSDIO_USE_SLOW_CLOCK

############################################################
# Options should be able to set as parameters.             #
############################################################
#ccflags-y += -DCONFIG_SSV6XXX_HW_DEBUG

#######################################################
ccflags-y += -DCONFIG_SSV6200_HAS_RX_WORKQUEUE
ccflags-y += -DUSE_THREAD_RX

# Generic decision table applicable to both AP and STA modes.
ccflags-y += -DUSE_GENERIC_DECI_TBL

########################################################
## The following definition move to indivdual platform
## should not enable again here. 

#ccflags-y += -DCONFIG_SSV6XXX_DEBUGFS
#### end of move to individual platform

#ccflags-y += -DENABLE_WAKE_IO_ISR_WHEN_HCI_ENQUEUE

#enable smart icomm
ccflags-y += -DCONFIG_SSV_CTL
ccflags-y += -DCONFIG_SSV_SMARTLINK

#enable USB LPM function
#ccflags-y += -DSSV_SUPPORT_USB_LPM

# Don't use stack for sdio_memcpy_xxx()
ccflags-y += -DCONFIG_MMC_DISALLOW_STACK

# Process HWIF SDIO Rx interrupt directly in function which is called by kernel, instead of work queue.
ccflags-y += -DHWIF_SDIO_RX_IRQ

ccflags-y += -DCONFIG_FW_ALIGNMENT_CHECK

# Filter out beacon and probe request while STA connected (NOT for concurrence mode)
ccflags-y += -DCONFIG_STA_BCN_FILTER
#ccflags-y += -DCONFIG_SSV_USE_SDIO_DAT1_AS_INT

# Skip Tx while there is SDIO interrupt status has Rx
ccflags-y += -DCONFIG_SDIO_FAVOR_RX

# support hw scan
ccflags-y += -DCONFIG_HW_SCAN

# support USB multi tx urb
#ccflags-y += -DCONFIG_USB_TX_MULTI_URB

# data rx path don't include mcu component
#ccflags-y += -DCONFIG_RX2HOST_DIRECTLY

# support ACS
#ccflags-y += -DCONFIG_ENABLE_ACS_FUNC

# support MIFI
# ccflags-y += -DCONFIG_MIFI

# support BLE 
#ccflags-y += -DCONFIG_BLE

# BlE HCI BUS setting
# 1: over SDIO SSV_BLE_HCI_OVER_SDIO
# 0: over UART SSV_BLE_HCI_OVER_UART
ccflags-y += -DCONFIG_BLE_HCI_BUS=1

#ccflags-y += -DCONFIG_ENABLE_HOST_THERMAL

