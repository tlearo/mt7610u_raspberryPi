/*
 *************************************************************************
 * Ralink Tech Inc.
 * 5F., No.36, Taiyuan St., Jhubei City,
 * Hsinchu County 302,
 * Taiwan, R.O.C.
 *
 * (c) Copyright 2002-2010, Ralink Technology, Inc.
 *
 * This program is free software; you can redistribute it and/or modify  *
 * it under the terms of the GNU General Public License as published by  *
 * the Free Software Foundation; either version 2 of the License, or     *
 * (at your option) any later version.                                   *
 *                                                                       *
 * This program is distributed in the hope that it will be useful,       *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 * GNU General Public License for more details.                          *
 *                                                                       *
 * You should have received a copy of the GNU General Public License     *
 * along with this program; if not, write to the                         *
 * Free Software Foundation, Inc.,                                       *
 * 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 *                                                                       *
 *************************************************************************
 */

#ifndef __RT_CONFIG_H__
#define __RT_CONFIG_H__

/* Include Standard and Platform-Specific Headers */
#include "os_linux.h"    /* Linux-specific macros and utilities */
#include "rtmp_def.h"    /* RTMP core definitions */
#include "rtmp_comm.h"   /* Common RTMP structures and enums */
#include "rtmp_osabl.h"  /* OS Abstraction Layer (if applicable) */
#include "rtmp_timer.h"  /* Timer management definitions */
#include "rtmp_chip.h"   /* Chip-specific definitions */
#include "rtmp.h"        /* Main driver functions and macros */
#include "eeprom.h"      /* EEPROM-related definitions */
#include "wlan_config.h" /* Wireless LAN configuration */

/* Feature and Driver-Specific Flags */
// Uncomment these macros to enable/disable features
#define CONFIG_STA_SUPPORT /* Enable Station (STA) mode */
// #define CONFIG_AP_SUPPORT  /* Uncomment to enable Access Point (AP) mode */
#define RT_CFG80211_SUPPORT /* Enable CFG80211 support */
#define SYSTEM_LOG_SUPPORT  /* Enable system logging */
#define IDS_SUPPORT         /* Enable intrusion detection support */

/* Kernel Compatibility Macros */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 4, 0)
#define RTMP_USE_NEW_KERNEL_API /* Use new API for Linux 5.4+ */
#endif

/* Debugging Macros */
#ifdef DEBUG
#define DBGPRINT(fmt, args...) printk(KERN_DEBUG "RT_DEBUG: " fmt, ##args)
#else
#define DBGPRINT(fmt, args...)
#endif

/* Maximum Buffer Sizes */
#define MAX_INI_BUFFER_SIZE 4096 /* Max buffer size for INI files */
#define MAX_MBSSID_NUM 8         /* Max number of BSSIDs supported */
#define MAX_WDS_NUM 4            /* Max number of WDS interfaces */

/* Default Paths for Configuration Files */
#ifdef CONFIG_STA_SUPPORT
#define STA_PROFILE_PATH "/etc/Wireless/RT2860STA/RT2860STA.dat"
#endif /* CONFIG_STA_SUPPORT */

#ifdef CONFIG_AP_SUPPORT
#define AP_PROFILE_PATH "/etc/Wireless/RT2860AP/RT2860AP.dat"
#endif /* CONFIG_AP_SUPPORT */

/* Forward Declarations of Global Functions */
void RTMPDrvOpen(void *pAdSrc);
void RTMPDrvClose(void *pAdSrc);
void RTMPInit(void *pAdSrc);
void RTMPDeinit(void *pAdSrc);

#endif /* __RT_CONFIG_H__ */
