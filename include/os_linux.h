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

#ifndef __OS_LINUX_H__
#define __OS_LINUX_H__

/* Include Linux Kernel Headers */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/if_ether.h>
#include <linux/if_arp.h>
#include <linux/inetdevice.h>
#include <linux/slab.h>
#include <linux/semaphore.h>
#include <linux/spinlock.h>
#include <linux/skbuff.h>
#include <linux/io.h>
#include <linux/delay.h>
#include <linux/irq.h>
#include <linux/version.h>
#include <linux/ctype.h>
#include <linux/unistd.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/vmalloc.h>
#include <linux/inet.h>
#include <linux/sched.h>
#include <linux/workqueue.h>
#include <linux/kthread.h>
#include <linux/bitmap.h>
#include <linux/types.h>
#include <asm/io.h>
#include <asm/byteorder.h>
#include <asm/unaligned.h>

/* Compatibility Macros for Different Kernel Versions */
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 15, 0)
#define NETDEV_TX_OK 0
#define NETDEV_TX_BUSY 1
#endif

/* Debugging Macros */
#ifdef DEBUG
#define DBGPRINT(fmt, args...) printk(KERN_DEBUG "DEBUG: " fmt, ##args)
#else
#define DBGPRINT(fmt, args...)
#endif

/* General Constants and Macros */
#define TRUE 1
#define FALSE 0

#define MAX_INI_BUFFER_SIZE 4096

/* Memory Allocation Wrappers */
#define os_alloc_mem(pAd, ptr, size) \
    ((*ptr) = kmalloc(size, GFP_ATOMIC))
#define os_free_mem(pAd, ptr) \
    kfree(ptr)

/* File System Wrappers */
typedef struct file *RTMP_OS_FD;
typedef struct inode RTMP_OS_FS_INFO;
#define IS_FILE_OPEN_ERR(fd) (IS_ERR(fd))
#define RtmpOSFileOpen(filename, flags, mode) filp_open(filename, flags, mode)
#define RtmpOSFileClose(fd) filp_close(fd, NULL)
#define RtmpOSFileRead(fd, buffer, len) kernel_read(fd, buffer, len, &fd->f_pos)
#define RtmpOSFSInfoChange(pOSFSInfo, bOpen) \
    ((void)(bOpen)) /* Placeholder, only used if needed */

/* Spinlock Wrappers */
#define OS_SPINLOCK_TYPE spinlock_t
#define OS_INIT_SPINLOCK(lock) spin_lock_init(lock)
#define OS_LOCK_SPINLOCK(lock) spin_lock(lock)
#define OS_UNLOCK_SPINLOCK(lock) spin_unlock(lock)

/* Semaphore Wrappers */
#define OS_SEM_TYPE struct semaphore
#define OS_INIT_SEMAPHORE(sem) sema_init(sem, 1)
#define OS_WAIT_SEMAPHORE(sem) down(sem)
#define OS_RELEASE_SEMAPHORE(sem) up(sem)

/* Time Wrappers */
#define OS_MDELAY(ms) mdelay(ms)
#define OS_UDELAY(us) udelay(us)

/* Networking Wrappers */
#define OS_SET_PACKET_OWNER(pPkt, pNetDev) skb_orphan(pPkt)
#define OS_PACKET_DATA_PTR(pPkt) skb_data(pPkt)
#define OS_PACKET_LEN(pPkt) skb_len(pPkt)
#define OS_NETDEV_PRIV(dev) netdev_priv(dev)

/* Wireless Event Wrappers */
#define RT_WLAN_EVENT_CUSTOM IWEVCUSTOM
#define RtmpOSWrielessEventSend(dev, event, flag, mac, buf, len) \
    wireless_send_event(dev, event, NULL, buf)

/* IRQ Lock Wrappers */
typedef unsigned long OS_IRQ_FLAGS;
#define OS_IRQ_LOCK(flags) local_irq_save(flags)
#define OS_IRQ_UNLOCK(flags) local_irq_restore(flags)

/* Platform-specific Function Declarations */
void RTMP_DRV_OPS_FUNCTION(void);

#endif /* __OS_LINUX_H__ */
