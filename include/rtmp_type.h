#ifndef __RTMP_TYPE_H__
#define __RTMP_TYPE_H__

#include <stdint.h>      // Standard integer types (uint8_t, uint16_t, etc.)
#include <stddef.h>      // Standard definitions (NULL, size_t, etc.)
#include <stdbool.h>     // Boolean type (true, false)
#include <linux/types.h> // Kernel-specific types (if building for Linux)

// Type Definitions
typedef uint8_t UCHAR;    // Unsigned 8-bit
typedef uint16_t USHORT;  // Unsigned 16-bit
typedef uint32_t UINT;    // Unsigned 32-bit
typedef int32_t INT;      // Signed 32-bit
typedef uint64_t ULONG;   // Unsigned 64-bit
typedef int64_t LONGLONG; // Signed 64-bit

typedef unsigned char BOOLEAN; // Boolean type (if <stdbool.h> is not used)

// Boolean Definitions
#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

// Null Pointer Definition
#ifndef NULL
#define NULL ((void *)0)
#endif

// Alignment Macros
#ifndef ALIGN
#define ALIGN(x, a) (((x) + ((a) - 1)) & ~((a) - 1))
#endif

#ifndef __ALIGN_MASK
#define __ALIGN_MASK(x, mask) (((x) + (mask)) & ~(mask))
#endif

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#endif

// Helper Macros
#define NDIS_STATUS_SUCCESS 0
#define NDIS_STATUS_FAILURE -1
#define NDIS_STATUS_RESOURCES -2

#define MAX_NUM_OF_DEVICES 32
#define MAX_BUFFER_SIZE 4096

// Bit Manipulation Macros
#define SET_BIT(x, n) ((x) |= (1U << (n)))
#define CLEAR_BIT(x, n) ((x) &= ~(1U << (n)))
#define TEST_BIT(x, n) (((x) & (1U << (n))) != 0)

// Debugging Macros
#ifdef DEBUG
#define DBGPRINT(fmt, args...) printk(KERN_DEBUG "[DBG] %s: " fmt, __FUNCTION__, ##args)
#else
#define DBGPRINT(fmt, args...) // No-op in release builds
#endif

// Memory Management Abstractions
#define MEM_ALLOC_FLAG GFP_ATOMIC
#define os_alloc_mem(p, s) kmalloc((s), MEM_ALLOC_FLAG)
#define os_free_mem(p) kfree((p))

// Network Packet Handling
#define OS_PKT_HEAD_BUF_EXT(pkt, len) skb_reserve((pkt), (len))
#define OS_PKT_TAIL_BUF_EXT(pkt, len) skb_put((pkt), (len))
#define OS_PKT_DATA(pkt) ((pkt)->data)
#define OS_PKT_LEN(pkt) ((pkt)->len)

// Time Conversion Macros
#define SEC_TO_JIFFIES(s) ((s) * HZ)
#define MSEC_TO_JIFFIES(ms) (((ms) * HZ) / 1000)
#define USEC_TO_JIFFIES(us) (((us) * HZ) / 1000000)

// Endian Conversion (assuming little-endian platform)
#define cpu2le16(x) (x)
#define le2cpu16(x) (x)
#define cpu2le32(x) (x)
#define le2cpu32(x) (x)

// Error Codes
#define RTMP_IO_EINVAL -EINVAL
#define RTMP_IO_ENOMEM -ENOMEM
#define RTMP_IO_EIO -EIO

// Structures and Unions
typedef struct _MAC_ADDR
{
    UCHAR Addr[6]; // MAC address (6 bytes)
} MAC_ADDR, *PMAC_ADDR;

typedef struct _RTMP_ADAPTER
{
    MAC_ADDR MacAddress;    // Adapter's MAC address
    UINT MaxTxPower;        // Maximum transmission power
    BOOLEAN bHardwareRadio; // Hardware radio status
    // Add additional adapter-specific fields here
} RTMP_ADAPTER, *PRTMP_ADAPTER;

// Debug Levels
#define RT_DEBUG_OFF 0
#define RT_DEBUG_ERROR 1
#define RT_DEBUG_WARN 2
#define RT_DEBUG_INFO 3
#define RT_DEBUG_TRACE 4

// Define the current debug level
extern int RTDebugLevel;

// Debug Print Macro
#define DBGPRINT_LEVEL(level, fmt, args...)                \
    do                                                     \
    {                                                      \
        if (RTDebugLevel >= (level))                       \
            printk(KERN_DEBUG "[%d] " fmt, level, ##args); \
    } while (0)

#endif /* __RTMP_TYPE_H__ */
