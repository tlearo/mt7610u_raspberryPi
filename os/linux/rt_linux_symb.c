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

#define RTMP_MODULE_OS
#define RTMP_MODULE_OS_UTIL

#include "rtmp_comm.h"
#include "rtmp_osabl.h"
#include "rt_os_util.h"

#ifdef OS_ABL_SUPPORT

/* Debug symbols */
EXPORT_SYMBOL(RTDebugLevel);
EXPORT_SYMBOL(RTDebugFunc);

/* ===== Utility Functions ===== */
EXPORT_SYMBOL(RtmpUtilInit);
EXPORT_SYMBOL(RTMPFreeNdisPacket);
EXPORT_SYMBOL(AdapterBlockAllocateMemory);

/* ===== Timer Management ===== */
EXPORT_SYMBOL(RTMP_SetPeriodicTimer);
EXPORT_SYMBOL(RTMP_OS_Add_Timer);
EXPORT_SYMBOL(RTMP_OS_Mod_Timer);
EXPORT_SYMBOL(RTMP_OS_Del_Timer);
EXPORT_SYMBOL(RTMP_OS_Init_Timer);
EXPORT_SYMBOL(RTMP_OS_Release_Timer);

/* ===== Memory Management ===== */
EXPORT_SYMBOL(os_alloc_mem);
EXPORT_SYMBOL(os_alloc_mem_suspend);
EXPORT_SYMBOL(os_free_mem);

/* ===== Packet Management ===== */
EXPORT_SYMBOL(ClonePacket);
EXPORT_SYMBOL(duplicate_pkt_with_TKIP_MIC);
EXPORT_SYMBOL(RTMPAllocateNdisPacket);
EXPORT_SYMBOL(RtmpOsPktCopy);
EXPORT_SYMBOL(RtmpOsPktClone);
EXPORT_SYMBOL(RtmpOsPktDataPtrAssign);
EXPORT_SYMBOL(RtmpOsPktLenAssign);
EXPORT_SYMBOL(RtmpOsPktProtocolAssign);

/* ===== File System Operations ===== */
EXPORT_SYMBOL(RtmpOSFileOpen);
EXPORT_SYMBOL(RtmpOSFileWrite);
EXPORT_SYMBOL(RtmpOSFileRead);
EXPORT_SYMBOL(RtmpOSFileClose);
EXPORT_SYMBOL(RtmpOSFileSeek);

/* ===== Task Management ===== */
EXPORT_SYMBOL(RtmpOSTaskNotifyToExit);
EXPORT_SYMBOL(RtmpOSTaskInit);
EXPORT_SYMBOL(RtmpOSTaskAttach);
EXPORT_SYMBOL(RtmpOSTaskKill);
EXPORT_SYMBOL(RtmpOsCheckTaskLegality);

/* ===== Spinlocks and Semaphores ===== */
EXPORT_SYMBOL(RtmpOsAllocateLock);
EXPORT_SYMBOL(RtmpOsFreeSpinLock);
EXPORT_SYMBOL(RtmpOsSpinLockBh);
EXPORT_SYMBOL(RtmpOsSpinUnLockBh);
EXPORT_SYMBOL(RtmpOsSemaInit);
EXPORT_SYMBOL(RtmpOsSemaDestroy);
EXPORT_SYMBOL(RtmpOsSemaWaitInterruptible);
EXPORT_SYMBOL(RtmpOsSemaWakeUp);

/* ===== CFG80211 Support ===== */
#ifdef RT_CFG80211_SUPPORT
extern UCHAR Cfg80211_Chan[];
EXPORT_SYMBOL(CFG80211OS_UnRegister);
EXPORT_SYMBOL(CFG80211_SupBandInit);
EXPORT_SYMBOL(Cfg80211_Chan);
EXPORT_SYMBOL(CFG80211OS_RegHint);
EXPORT_SYMBOL(CFG80211OS_ScanEnd);
EXPORT_SYMBOL(CFG80211OS_ConnectResultInform);
#endif /* RT_CFG80211_SUPPORT */

/* ===== USB Support ===== */
#ifdef RTMP_MAC_USB
EXPORT_SYMBOL(RtmpOsUsbUrbDataGet);
EXPORT_SYMBOL(RtmpOsUsbContextGet);
EXPORT_SYMBOL(RtmpOsUsbInitHTTxDesc);
#endif /* RTMP_MAC_USB */

/* ===== Flash Support ===== */
#if defined(RTMP_RBUS_SUPPORT) || defined(RTMP_FLASH_SUPPORT)
EXPORT_SYMBOL(RtmpFlashRead);
EXPORT_SYMBOL(RtmpFlashWrite);
#endif /* RTMP_RBUS_SUPPORT || RTMP_FLASH_SUPPORT */

/* ===== Vendor-Specific Features ===== */
#ifdef VENDOR_FEATURE4_SUPPORT
EXPORT_SYMBOL(OS_NumOfMemAlloc);
EXPORT_SYMBOL(OS_NumOfMemFree);
#endif /* VENDOR_FEATURE4_SUPPORT */

#ifdef VENDOR_FEATURE2_SUPPORT
EXPORT_SYMBOL(OS_NumOfPktAlloc);
EXPORT_SYMBOL(OS_NumOfPktFree);
#endif /* VENDOR_FEATURE2_SUPPORT */

/* ===== Miscellaneous ===== */
EXPORT_SYMBOL(RTMP_GetCurrentSystemTime);
EXPORT_SYMBOL(RTMP_GetCurrentSystemTick);
EXPORT_SYMBOL(RtmpOsWirelessExtVerGet);

#endif /* OS_ABL_SUPPORT */
