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

#ifndef __RT_DEF_H__
#define __RT_DEF_H__

/* Standard Includes */
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/netdevice.h>

/* General Constants */
#define MAX_LEN_OF_SSID 32     /* Maximum length of SSID */
#define MAX_LEN_OF_BSSID 6     /* MAC address length */
#define MAX_NUM_OF_CHANNELS 14 /* Max number of supported channels */
#define MAX_SCAN_RESULTS 64    /* Maximum scan results */
#define MAX_TX_POWER 30        /* Max transmit power in dBm */
#define MIN_TX_POWER 0         /* Min transmit power in dBm */

/* Driver-Specific Constants */
#define RTMP_ADAPTER_FLAG_INITIALIZED 0x0001
#define RTMP_ADAPTER_FLAG_RUNNING 0x0002
#define RTMP_ADAPTER_FLAG_HALT_IN_PROGRESS 0x0004

#define NUM_OF_TX_RING 4 /* Number of TX rings */
#define NUM_OF_RX_RING 1 /* Number of RX rings */

/* Queue Definitions */
#define MAX_TX_QUEUE_SIZE 512 /* Maximum size of TX queue */
#define MAX_RX_QUEUE_SIZE 512 /* Maximum size of RX queue */

/* Packet Definitions */
#define PACKET_TYPE_DATA 0x01
#define PACKET_TYPE_MGMT 0x02
#define PACKET_TYPE_CTRL 0x04

/* Common Return Codes */
#define NDIS_STATUS_SUCCESS 0
#define NDIS_STATUS_FAILURE -1
#define NDIS_STATUS_RESOURCES -2
#define NDIS_STATUS_INVALID_DATA -3

/* Debug Levels */
#define RT_DEBUG_OFF 0
#define RT_DEBUG_ERROR 1
#define RT_DEBUG_WARN 2
#define RT_DEBUG_TRACE 3
#define RT_DEBUG_INFO 4

#ifdef DEBUG
#define DBGPRINT(level, fmt, args...)                    \
    do                                                   \
    {                                                    \
        if (level <= RT_DEBUG_TRACE)                     \
            printk(KERN_DEBUG "RT_DEBUG: " fmt, ##args); \
    } while (0)
#else
#define DBGPRINT(level, fmt, args...)
#endif

/* Data Rate Definitions */
#define RATE_1MBPS 0x02
#define RATE_2MBPS 0x04
#define RATE_5_5MBPS 0x0B
#define RATE_11MBPS 0x16
#define RATE_6MBPS 0x0C
#define RATE_9MBPS 0x12
#define RATE_12MBPS 0x18
#define RATE_18MBPS 0x24
#define RATE_24MBPS 0x30
#define RATE_36MBPS 0x48
#define RATE_48MBPS 0x60
#define RATE_54MBPS 0x6C

/* Enumeration for Operation Modes */
typedef enum _OP_MODE
{
    OP_MODE_STA = 0, /* Station mode */
    OP_MODE_AP,      /* Access Point mode */
    OP_MODE_MONITOR, /* Monitor mode */
    OP_MODE_UNKNOWN  /* Unknown mode */
} OP_MODE;

/* Macros for Memory Management */
#define MEM_ALLOC_FLAG GFP_KERNEL
#define os_alloc_mem(ad, mem, size) ((*mem) = kzalloc(size, MEM_ALLOC_FLAG))
#define os_free_mem(ad, mem) kfree(mem)

/* Adapter Flags Management */
#define RTMP_SET_FLAG(pAd, flag) ((pAd)->Flags |= (flag))
#define RTMP_CLEAR_FLAG(pAd, flag) ((pAd)->Flags &= ~(flag))
#define RTMP_TEST_FLAG(pAd, flag) (((pAd)->Flags & (flag)) != 0)

/* Data Structures */
typedef struct _RTMP_ADAPTER
{
    struct net_device *net_dev;    /* Network device pointer */
    unsigned long Flags;           /* Adapter flags */
    u8 mac_addr[MAX_LEN_OF_BSSID]; /* MAC address */
    u8 ssid[MAX_LEN_OF_SSID];      /* Current SSID */
    int ssid_len;                  /* Length of SSID */
    int tx_power;                  /* Transmit power in dBm */
    int op_mode;                   /* Current operation mode (OP_MODE) */
    spinlock_t irq_lock;           /* IRQ spinlock */
    spinlock_t tx_queue_lock;      /* TX queue lock */
    spinlock_t rx_queue_lock;      /* RX queue lock */
} RTMP_ADAPTER, *PRTMP_ADAPTER;

/* Prototypes for Utility Functions */
void RTMPSetDefaultProfileParameters(PRTMP_ADAPTER pAd);
void RTMPSetProfileParameters(PRTMP_ADAPTER pAd, const char *buffer);
void RTMPSetSingleSKUParameters(PRTMP_ADAPTER pAd);
int RTMPStartAdapter(PRTMP_ADAPTER pAd);
void RTMPStopAdapter(PRTMP_ADAPTER pAd);
void RTMPResetAdapter(PRTMP_ADAPTER pAd);

#endif /* __RT_DEF_H__ */
