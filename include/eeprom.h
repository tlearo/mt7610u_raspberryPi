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
 *************************************************************************/

#ifndef __EEPROM_H__
#define __EEPROM_H__

#include <stdint.h>

/* Type definitions for compatibility */
typedef unsigned short USHORT;
typedef unsigned char UCHAR;
typedef int NTSTATUS;

/* For ioctl check usage */
#define EEPROM_IS_PROGRAMMED 0x80

#ifdef RTMP_MAC_USB
#define EEPROM_SIZE 0x400
#endif /* RTMP_MAC_USB */

#ifdef RT_BIG_ENDIAN
typedef union _EEPROM_WORD_STRUC
{
    struct
    {
        UCHAR Byte1; /* High Byte */
        UCHAR Byte0; /* Low Byte */
    } field;
    USHORT word;
} EEPROM_WORD_STRUC;
#else
typedef union _EEPROM_WORD_STRUC
{
    struct
    {
        UCHAR Byte0; /* Low Byte */
        UCHAR Byte1; /* High Byte */
    } field;
    USHORT word;
} EEPROM_WORD_STRUC;
#endif /* RT_BIG_ENDIAN */

/* EEPROM Data Layout */
#define BOARD_TYPE_MINI_CARD 0 /* Mini card */
#define BOARD_TYPE_USB_PEN 1   /* USB pen */

/* EEPROM antenna select format */
#ifdef RT_BIG_ENDIAN
typedef union _EEPROM_NIC_CONFIG2_STRUC
{
    struct
    {
        USHORT DACTestBit : 1;
        USHORT CoexBit : 1;
        USHORT bInternalTxALC : 1;
        USHORT AntOpt : 1;
        USHORT AntDiversity : 1;
        USHORT Rsv1 : 1;
        USHORT BW40MAvailForA : 1;
        USHORT BW40MAvailForG : 1;
        USHORT EnableWPSPBC : 1;
        USHORT BW40MSidebandForA : 1;
        USHORT BW40MSidebandForG : 1;
        USHORT CardbusAcceleration : 1;
        USHORT ExternalLNAForA : 1;
        USHORT ExternalLNAForG : 1;
        USHORT DynamicTxAgcControl : 1;
        USHORT HardwareRadioControl : 1;
    } field;
    USHORT word;
} EEPROM_NIC_CONFIG2_STRUC, *PEEPROM_NIC_CONFIG2_STRUC;
#else
typedef union _EEPROM_NIC_CONFIG2_STRUC
{
    struct
    {
        USHORT HardwareRadioControl : 1;
        USHORT DynamicTxAgcControl : 1;
        USHORT ExternalLNAForG : 1;
        USHORT ExternalLNAForA : 1;
        USHORT CardbusAcceleration : 1;
        USHORT BW40MSidebandForG : 1;
        USHORT BW40MSidebandForA : 1;
        USHORT EnableWPSPBC : 1;
        USHORT BW40MAvailForG : 1;
        USHORT BW40MAvailForA : 1;
        USHORT Rsv1 : 1;
        USHORT AntDiversity : 1;
        USHORT AntOpt : 1;
        USHORT bInternalTxALC : 1;
        USHORT CoexBit : 1;
        USHORT DACTestBit : 1;
    } field;
    USHORT word;
} EEPROM_NIC_CONFIG2_STRUC, *PEEPROM_NIC_CONFIG2_STRUC;
#endif /* RT_BIG_ENDIAN */

/* EEPROM TX Power Structure */
#ifdef RT_BIG_ENDIAN
typedef union _EEPROM_TX_PWR_STRUC
{
    struct
    {
        signed char Byte1; /* High Byte */
        signed char Byte0; /* Low Byte */
    } field;
    USHORT word;
} EEPROM_TX_PWR_STRUC, *PEEPROM_TX_PWR_STRUC;
#else
typedef union _EEPROM_TX_PWR_STRUC
{
    struct
    {
        signed char Byte0; /* Low Byte */
        signed char Byte1; /* High Byte */
    } field;
    USHORT word;
} EEPROM_TX_PWR_STRUC, *PEEPROM_TX_PWR_STRUC;
#endif /* RT_BIG_ENDIAN */

/* Other Structures and Prototypes */

struct _RTMP_ADAPTER;

#ifdef RTMP_USB_SUPPORT
NTSTATUS RTUSBReadEEPROM16(struct _RTMP_ADAPTER *pAd, USHORT offset, USHORT *pData);
NTSTATUS RTUSBWriteEEPROM16(struct _RTMP_ADAPTER *pAd, USHORT offset, USHORT value);
#endif /* RTMP_USB_SUPPORT */

#endif /* __EEPROM_H__ */
