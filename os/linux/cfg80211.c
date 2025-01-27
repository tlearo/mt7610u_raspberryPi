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

#include "../../include/rtmp_comm.h"
#include "../../include/rt_os_util.h"
#include "../../include/rt_os_net.h"
#include "../../include/rt_config.h"

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 28))
#ifdef RT_CFG80211_SUPPORT

#define CFG80211_NUM_OF_CHAN_5GHZ (sizeof(Cfg80211_Chan) - CFG80211_NUM_OF_CHAN_2GHZ)

/* Array of supported rates */
static const struct ieee80211_rate Cfg80211_SupRate[] = {
    {.flags = IEEE80211_RATE_SHORT_PREAMBLE, .bitrate = 10, .hw_value = 0},
    {.flags = IEEE80211_RATE_SHORT_PREAMBLE, .bitrate = 20, .hw_value = 1},
    {.flags = IEEE80211_RATE_SHORT_PREAMBLE, .bitrate = 55, .hw_value = 2},
    {.flags = IEEE80211_RATE_SHORT_PREAMBLE, .bitrate = 110, .hw_value = 3},
    {.flags = 0, .bitrate = 60, .hw_value = 4},
    {.flags = 0, .bitrate = 90, .hw_value = 5},
    {.flags = 0, .bitrate = 120, .hw_value = 6},
    {.flags = 0, .bitrate = 180, .hw_value = 7},
    {.flags = 0, .bitrate = 240, .hw_value = 8},
    {.flags = 0, .bitrate = 360, .hw_value = 9},
    {.flags = 0, .bitrate = 480, .hw_value = 10},
    {.flags = 0, .bitrate = 540, .hw_value = 11}};

/* Supported channels */
static const UCHAR Cfg80211_Chan[] = {
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
    36, 38, 40, 44, 46, 48, 52, 54, 56, 60, 62, 64,
    100, 104, 108, 112, 116, 118, 120, 124, 126, 128, 132, 134, 136,
    140, 149, 151, 153, 157, 159, 161, 165, 167, 169, 171, 173,
    184, 188, 192, 196, 208, 212, 216};

static const UINT32 CipherSuites[] = {
    WLAN_CIPHER_SUITE_WEP40,
    WLAN_CIPHER_SUITE_WEP104,
    WLAN_CIPHER_SUITE_TKIP,
    WLAN_CIPHER_SUITE_CCMP};

/* Logging and error helper macros */
#define CFG80211_LOG(level, fmt, args...) \
    printk(level "cfg80211: " fmt, ##args)

#define CFG80211_ERR(fmt, args...) \
    CFG80211_LOG(KERN_ERR, fmt, ##args)

/* ===================== Function Definitions ========================= */

/*
 * Set the channel for the device.
 */
static int CFG80211_OpsChannelSet(struct wiphy *pWiphy, struct net_device *pDev,
                                  struct ieee80211_channel *pChan, enum nl80211_channel_type ChannelType)
{
    VOID *pAd;
    CMD_RTPRIV_IOCTL_80211_CHAN ChanInfo;
    UINT32 ChanId;

    CFG80211_ERR("Setting channel\n");

    MAC80211_PAD_GET(pAd, pWiphy);
    ChanId = ieee80211_frequency_to_channel(pChan->center_freq);

    memset(&ChanInfo, 0, sizeof(ChanInfo));
    ChanInfo.ChanId = ChanId;

    /* Set the channel type */
    switch (ChannelType)
    {
    case NL80211_CHAN_NO_HT:
        ChanInfo.ChanType = RT_CMD_80211_CHANTYPE_NOHT;
        break;
    case NL80211_CHAN_HT20:
        ChanInfo.ChanType = RT_CMD_80211_CHANTYPE_HT20;
        break;
    case NL80211_CHAN_HT40MINUS:
        ChanInfo.ChanType = RT_CMD_80211_CHANTYPE_HT40MINUS;
        break;
    case NL80211_CHAN_HT40PLUS:
        ChanInfo.ChanType = RT_CMD_80211_CHANTYPE_HT40PLUS;
        break;
    default:
        CFG80211_ERR("Unsupported channel type\n");
        return -EINVAL;
    }

    /* Apply the channel configuration */
    RTMP_DRIVER_80211_CHAN_SET(pAd, &ChanInfo);
    return 0;
}

/*
 * Change the type/configuration of the virtual interface.
 */
static int CFG80211_OpsVirtualInfChg(struct wiphy *pWiphy, struct net_device *pDev,
                                     enum nl80211_iftype Type, struct vif_params *params)
{
    VOID *pAd;
    UINT32 Filter = 0;

    MAC80211_PAD_GET(pAd, pWiphy);
    CFG80211_ERR("Changing virtual interface to type %d\n", Type);

    /* Verify and update interface type */
    switch (Type)
    {
    case NL80211_IFTYPE_STATION:
    case NL80211_IFTYPE_ADHOC:
    case NL80211_IFTYPE_MONITOR:
        pDev->ieee80211_ptr->iftype = Type;
        break;
    default:
        CFG80211_ERR("Unsupported interface type\n");
        return -EINVAL;
    }

    if (params && params->flags)
    {
        /* Set monitor mode flags */
        if (params->flags & NL80211_MNTR_FLAG_FCSFAIL)
            Filter |= RT_CMD_80211_FILTER_FCSFAIL;
        if (params->flags & NL80211_MNTR_FLAG_PLCPFAIL)
            Filter |= RT_CMD_80211_FILTER_PLCPFAIL;
    }

    RTMP_DRIVER_80211_VIF_SET(pAd, Filter, Type);
    return 0;
}

/* ===================== Driver Registration ========================= */

struct cfg80211_ops CFG80211_Ops = {
    .set_channel = CFG80211_OpsChannelSet,
    .change_virtual_intf = CFG80211_OpsVirtualInfChg,
    /* Add other function implementations here */
};

#endif /* RT_CFG80211_SUPPORT */
#endif /* LINUX_VERSION_CODE */