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

#include "rt_config.h"

#if defined(CONFIG_RA_HW_NAT) || defined(CONFIG_RA_HW_NAT_MODULE)
#include "../../../../../../net/nat/hw_nat/ra_nat.h"
#include "../../../../../../net/nat/hw_nat/frame_engine.h"
#endif

#ifdef SYSTEM_LOG_SUPPORT
// Wireless system event messages
const char *pWirelessSysEventText[IW_SYS_EVENT_TYPE_NUM] = {
    "had associated successfully",
    "had disassociated",
    "had deauthenticated",
    "had been aged-out and disassociated",
    "occurred CounterMeasures attack",
    "occurred replay counter different in Key Handshaking",
    "occurred RSNIE different in Key Handshaking",
    "occurred MIC different in Key Handshaking",
    "occurred ICV error in RX",
    "occurred MIC error in RX",
    "Group Key Handshaking timeout",
    "Pairwise Key Handshaking timeout",
    "RSN IE sanity check failure",
    "set key done in WPA/WPAPSK",
    "set key done in WPA2/WPA2PSK",
    "connects with our wireless client",
    "disconnects with our wireless client",
    "scan completed",
    "scan terminate!! Busy!! Enqueue fail!!",
    "channel switch to ",
    "wireless mode is not supported",
    "blacklisted in MAC filter list",
    "Authentication rejected because of challenge failure",
    "Scanning",
    "Start a new IBSS",
    "Join the IBSS",
    "Shared WEP fail"};

#ifdef IDS_SUPPORT
// Wireless IDS spoof attack event messages
const char *pWirelessSpoofEventText[IW_SPOOF_EVENT_TYPE_NUM] = {
    "detected conflict SSID",
    "detected spoofed association response",
    "detected spoofed reassociation responses",
    "detected spoofed probe response",
    "detected spoofed beacon",
    "detected spoofed disassociation",
    "detected spoofed authentication",
    "detected spoofed deauthentication",
    "detected spoofed unknown management frame",
    "detected replay attack"};

// Wireless IDS flooding attack event messages
const char *pWirelessFloodEventText[IW_FLOOD_EVENT_TYPE_NUM] = {
    "detected authentication flooding",
    "detected association request flooding",
    "detected reassociation request flooding",
    "detected probe request flooding",
    "detected disassociation flooding",
    "detected deauthentication flooding",
    "detected 802.1x eap-request flooding"};
#endif // IDS_SUPPORT
#endif // SYSTEM_LOG_SUPPORT

NDIS_STATUS RTMPReadParametersHook(PRTMP_ADAPTER pAd)
{
    PSTRING src = NULL;
    RTMP_OS_FD srcf;
    RTMP_OS_FS_INFO osFSInfo;
    INT retval = NDIS_STATUS_FAILURE;
    PSTRING buffer;

    os_alloc_mem(pAd, (UCHAR **)&buffer, MAX_INI_BUFFER_SIZE);
    if (!buffer)
    {
        DBGPRINT(RT_DEBUG_ERROR, ("Failed to allocate memory for buffer\n"));
        return NDIS_STATUS_FAILURE;
    }
    memset(buffer, 0x00, MAX_INI_BUFFER_SIZE);

#ifdef CONFIG_STA_SUPPORT
    if (pAd->OpMode == OPMODE_STA)
    {
        src = STA_PROFILE_PATH;
    }
#endif // CONFIG_STA_SUPPORT

#ifdef MULTIPLE_CARD_SUPPORT
    src = (PSTRING)pAd->MC_FileName;
#endif // MULTIPLE_CARD_SUPPORT

    // Set default parameters
    RTMPSetDefaultProfileParameters(pAd);
    retval = NDIS_STATUS_SUCCESS;

    if (src && *src)
    {
        RtmpOSFSInfoChange(&osFSInfo, TRUE);

        srcf = RtmpOSFileOpen(src, O_RDONLY, 0);
        if (IS_FILE_OPEN_ERR(srcf))
        {
            DBGPRINT(RT_DEBUG_ERROR, ("Open file \"%s\" failed\n", src));
        }
        else
        {
            retval = RtmpOSFileRead(srcf, buffer, MAX_INI_BUFFER_SIZE);
            if (retval > 0)
            {
                RTMPSetProfileParameters(pAd, buffer);
                retval = NDIS_STATUS_SUCCESS;
            }
            else
            {
                DBGPRINT(RT_DEBUG_ERROR, ("Read file \"%s\" failed (errCode=%d)\n", src, retval));
            }

            if (RtmpOSFileClose(srcf) != 0)
            {
                DBGPRINT(RT_DEBUG_ERROR, ("Close file \"%s\" failed\n", src));
                retval = NDIS_STATUS_FAILURE;
            }
        }

        RtmpOSFSInfoChange(&osFSInfo, FALSE);
    }

#ifdef HOSTAPD_SUPPORT
    for (int i = 0; i < pAd->ApCfg.BssidNum; i++)
    {
        pAd->ApCfg.MBSSID[i].Hostapd = FALSE;
        DBGPRINT(RT_DEBUG_TRACE, ("Reset ra%d hostapd support = FALSE\n", i));
    }
#endif // HOSTAPD_SUPPORT

#ifdef SINGLE_SKU_V2
    RTMPSetSingleSKUParameters(pAd);
#endif // SINGLE_SKU_V2

    os_free_mem(NULL, buffer);
    return retval;
}

#ifdef SYSTEM_LOG_SUPPORT
void RtmpDrvSendWirelessEvent(
    VOID *pAdSrc,
    USHORT Event_flag,
    PUCHAR pAddr,
    UCHAR BssIdx,
    CHAR Rssi)
{
    PRTMP_ADAPTER pAd = (PRTMP_ADAPTER)pAdSrc;
    PSTRING pBuf = NULL, pBufPtr = NULL;
    USHORT event, type, BufLen;
    UCHAR event_table_len = 0;

    if (!pAd->CommonCfg.bWirelessEvent)
        return;

    type = Event_flag & 0xFF00;
    event = Event_flag & 0x00FF;

    switch (type)
    {
    case IW_SYS_EVENT_FLAG_START:
        event_table_len = IW_SYS_EVENT_TYPE_NUM;
        break;
#ifdef IDS_SUPPORT
    case IW_SPOOF_EVENT_FLAG_START:
        event_table_len = IW_SPOOF_EVENT_TYPE_NUM;
        break;
    case IW_FLOOD_EVENT_FLAG_START:
        event_table_len = IW_FLOOD_EVENT_TYPE_NUM;
        break;
#endif // IDS_SUPPORT
    }

    if (event_table_len == 0 || event >= event_table_len)
    {
        DBGPRINT(RT_DEBUG_ERROR, ("%s: Invalid event type or event\n", __FUNCTION__));
        return;
    }

    os_alloc_mem(NULL, (UCHAR **)&pBuf, IW_CUSTOM_MAX_LEN);
    if (!pBuf)
    {
        DBGPRINT(RT_DEBUG_ERROR, ("%s: Memory allocation failed for wireless event\n", __FUNCTION__));
        return;
    }

    memset(pBuf, 0, IW_CUSTOM_MAX_LEN);
    pBufPtr = pBuf;

    if (pAddr)
        pBufPtr += sprintf(pBufPtr, "STA(%02x:%02x:%02x:%02x:%02x:%02x) ", PRINT_MAC(pAddr));
    else if (BssIdx < MAX_MBSSID_NUM(pAd))
        pBufPtr += sprintf(pBufPtr, "BSS(ra%d) ", BssIdx);
    else
        pBufPtr += sprintf(pBufPtr, "");

    if (type == IW_SYS_EVENT_FLAG_START)
    {
        pBufPtr += sprintf(pBufPtr, "%s", pWirelessSysEventText[event]);
        if (Event_flag == IW_CHANNEL_CHANGE_EVENT_FLAG)
            pBufPtr += sprintf(pBufPtr, "%3d", Rssi);
    }
#ifdef IDS_SUPPORT
    else if (type == IW_SPOOF_EVENT_FLAG_START)
        pBufPtr += sprintf(pBufPtr, "%s (RSSI=%d)", pWirelessSpoofEventText[event], Rssi);
    else if (type == IW_FLOOD_EVENT_FLAG_START)
        pBufPtr += sprintf(pBufPtr, "%s", pWirelessFloodEventText[event]);
#endif // IDS_SUPPORT
    else
        pBufPtr += sprintf(pBufPtr, "Unknown event");

    BufLen = pBufPtr - pBuf;
    RtmpOSWrielessEventSend(pAd->net_dev, RT_WLAN_EVENT_CUSTOM, Event_flag, NULL, (PUCHAR)pBuf, BufLen);
    os_free_mem(NULL, pBuf);
}
#endif // SYSTEM_LOG_SUPPORT
