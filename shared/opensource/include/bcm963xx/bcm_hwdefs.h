/*
    Copyright 2000-2010 Broadcom Corporation

    Unless you and Broadcom execute a separate written software license
    agreement governing use of this software, this software is licensed
    to you under the terms of the GNU General Public License version 2
    (the �GPL?, available at http://www.broadcom.com/licenses/GPLv2.php,
    with the following added to such license:

        As a special exception, the copyright holders of this software give
        you permission to link this software with independent modules, and to
        copy and distribute the resulting executable under terms of your
        choice, provided that you also meet, for each linked independent
        module, the terms and conditions of the license of that module. 
        An independent module is a module which is not derived from this
        software.  The special exception does not apply to any modifications
        of the software.

    Notwithstanding the above, under no circumstances may you combine this
    software in any way with any other Broadcom software provided under a
    license other than the GPL, without Broadcom's express prior written
    consent.
*/                       

/***********************************************************************/
/*                                                                     */
/*   MODULE:  bcm_hwdefs.h                                             */
/*   PURPOSE: Define all base device addresses and HW specific macros. */
/*                                                                     */
/***********************************************************************/
#ifndef _BCM_HWDEFS_H
#define _BCM_HWDEFS_H

#ifdef __cplusplus
extern "C" {
#endif

#define	DYING_GASP_API

/*****************************************************************************/
/*                    Physical Memory Map                                    */
/*****************************************************************************/

#define PHYS_DRAM_BASE          0x00000000      /* Dynamic RAM Base */
#if defined(CONFIG_BRCM_IKOS)
#define PHYS_FLASH_BASE         0x18000000      /* Flash Memory     */
#else
#define PHYS_FLASH_BASE         0x1FC00000      /* Flash Memory     */
#endif

/*****************************************************************************/
/* Note that the addresses above are physical addresses and that programs    */
/* have to use converted addresses defined below:                            */
/*****************************************************************************/
#define DRAM_BASE           (0x80000000 | PHYS_DRAM_BASE)   /* cached DRAM */
#define DRAM_BASE_NOCACHE   (0xA0000000 | PHYS_DRAM_BASE)   /* uncached DRAM */

/* Binary images are always built for a standard MIPS boot address */
#define IMAGE_BASE          (0xA0000000 | PHYS_FLASH_BASE)

/* Some chips don't support alternative boot vector */
#if defined(CONFIG_BRCM_IKOS)
#define FLASH_BASE          (0xA0000000 | PHYS_FLASH_BASE)  /* uncached Flash  */
#define BOOT_OFFSET         0
#else
#if defined(_BCM96328_) || defined(CONFIG_BCM96328) || defined(_BCM96362_) || defined(CONFIG_BCM96362) || defined(_BCM963268_) || defined(CONFIG_BCM963268) 
#define FLASH_BASE          0xB8000000
#else
#define FLASH_BASE          (0xA0000000 | (MPI->cs[0].base & 0xFFFFFF00))
#endif
#define BOOT_OFFSET         (FLASH_BASE - IMAGE_BASE)
#endif

/*****************************************************************************/
/*  Select the PLL value to get the desired CPU clock frequency.             */
/*****************************************************************************/
#define FPERIPH            50000000

/*****************************************************************************/
/* Board memory type offset                                                  */
/*****************************************************************************/
#define ONEK                            1024
#if 1//__MSTC__,JhenYang
#define FLASH_LENGTH_BOOT_ROM           (128*ONEK)
#define FLASH_LENGTH_BOOT_ROM_2         (64*ONEK)
#else
#define FLASH_LENGTH_BOOT_ROM           (64*ONEK)
#endif
#if 1 //__VERIZON__, Autumn
#define FLASH_IMAGE_DOWNLOAD_SIZE       (16*ONEK*ONEK)
#endif
/*****************************************************************************/
/*       NVRAM Offset and definition                                         */
/*****************************************************************************/
#define NVRAM_SECTOR                    0
#define NVRAM_DATA_OFFSET               0x0580
#define NVRAM_DATA_ID                   0x0f1e2d3c  // This is only for backwards compatability

#define NVRAM_LENGTH                    ONEK    // 1k nvram 
#define NVRAM_VERSION_NUMBER            6
#define NVRAM_FULL_LEN_VERSION_NUMBER   5 /* version in which the checksum was
                                             placed at the end of the NVRAM
                                             structure */

#define NVRAM_BOOTLINE_LEN              256
#define NVRAM_BOARD_ID_STRING_LEN       16
#define NVRAM_MAC_ADDRESS_LEN           6

#define NVRAM_GPON_SERIAL_NUMBER_LEN    13
#define NVRAM_GPON_PASSWORD_LEN         11

#define NVRAM_WLAN_PARAMS_LEN      256
#define NVRAM_WPS_DEVICE_PIN_LEN   8

#define THREAD_NUM_ADDRESS_OFFSET       (NVRAM_DATA_OFFSET + 4 + NVRAM_BOOTLINE_LEN + NVRAM_BOARD_ID_STRING_LEN)
#define THREAD_NUM_ADDRESS              (0x80000000 + THREAD_NUM_ADDRESS_OFFSET)

#define DEFAULT_BOOTLINE    "e=192.168.0.1:ffffff00 h=192.168.0.100 g= r=f f=vmlinux i=bcm963xx_fs_kernel d=1 p=0 "
#define DEFAULT_BOARD_IP    "192.168.0.1"
#define DEFAULT_MAC_NUM     10
#define DEFAULT_BOARD_MAC   "00:10:18:00:00:00"
#define DEFAULT_TP_NUM      0
#if 1//__MTS__, RaynorChung: Support 963268 nand flash, patch form SVN#3597 on http://svn.zyxel.com.tw/svn/CPE_SW1/BCM96368/trunk/P-870HA/branches/cht/fttb8/4.11 
#define DEFAULT_PSI_SIZE    128
#else
#define DEFAULT_PSI_SIZE    24
#endif
#define DEFAULT_GPON_SN     "BRCM12345678"
#define DEFAULT_GPON_PW     "          "
#if 1//__MTS__, RaynorChung: Support 963268 nand flash, patch form SVN#3597 on http://svn.zyxel.com.tw/svn/CPE_SW1/BCM96368/trunk/P-870HA/branches/cht/fttb8/4.11 
#define DEFAULT_FLASHBLK_SIZE  64
#define MAX_FLASHBLK_SIZE      128
#define DEFAULT_AUXFS_PERCENT 0
#define MAX_AUXFS_PERCENT   80
#define DEFAUT_BACKUP_PSI  1     //jchang change to 1
#endif
#if 1//__MTS__, RaynorChung: Support 963268 nand flash, patch form SVN#3597 on http://svn.zyxel.com.tw/svn/CPE_SW1/BCM96368/trunk/P-870HA/branches/cht/fttb8/4.11 
#define NVRAM_VENDORNAME_LEN        32
#define NVRAM_PRODUCTNAME_LEN       32
#define NVRAM_BUILDINFO_LEN         10
#define NVRAM_FEATUREBITS_LEN       30
#define NVRAM_SERIALNUMBER_LEN      14
#define DEFAULT_VENDORNAME       "MitraStar Technology Corp."
#define DEFAULT_PRODUCTNAME      "DSL-491HNU-B1B V2"
#define DEFAULT_SERIALNUMBER     "S090Y00000000"
#define DEFAULT_DEBUGFLAG        1
#define DEFAULT_FEATUREBITS0     "ZY"
#define DEFAULT_FEATUREBITS2     0x00
#define DEFAULT_FEATUREBITS3     0x0e
#define DEFAULT_MAINFEATUREBIT   0
// __MSTC__, zongyue: reduce manufacture bootup time for wireless calibration
#define ATMT_FEATUREBITS_IDX	 4
#endif

#define DEFAULT_WPS_DEVICE_PIN     "12345670"

#define DEFAULT_VOICE_BOARD_ID     "NONE"

#define NVRAM_MAC_COUNT_MAX         32
#if 1//__MTS__, RaynorChung: Support 963268 nand flash, patch form SVN#3597 on http://svn.zyxel.com.tw/svn/CPE_SW1/BCM96368/trunk/P-870HA/branches/cht/fttb8/4.11 
#define NVRAM_MAX_PSI_SIZE  128
#else
#define NVRAM_MAX_PSI_SIZE  64
#endif
#define NVRAM_MAX_SYSLOG_SIZE       256
#define NVRAM_OPTICAL_PARAMS_SIZE   64

#define NP_BOOT             0
#define NP_ROOTFS_1         1
#define NP_ROOTFS_2         2
#define NP_DATA             3
#define NP_BBT              4
#define NP_TOTAL            5

#define NAND_DATA_SIZE_KB       4096
#define NAND_BBT_THRESHOLD_KB   (512 * 1024)
#define NAND_BBT_SMALL_SIZE_KB  1024
#define NAND_BBT_BIG_SIZE_KB    4096

/* __MSTC__, zongyue: for relayout flash partition */
#define NAND_RAW_SIZE_KB    1024
/* app size = 65536 - BBT_SIZE - DATA_SIZE - NAND_RAW_SIZE_KB*/
/* ----------------------------------------------- */

#define NAND_CFE_RAM_NAME   "cferam.000"

#define NAND_RFS_OFS_NAME   "NAND_RFS_OFS"
#if 1 //__CTLK__, Thief
#define NVRAM_FACORTYVERSION_LEN 11
#endif
#ifndef _LANGUAGE_ASSEMBLY
#pragma pack(1) /* __MSTC__, zongyue: Data structure alignment */
typedef struct
{
    unsigned long ulVersion;
    char szBootline[NVRAM_BOOTLINE_LEN];
    char szBoardId[NVRAM_BOARD_ID_STRING_LEN];
    unsigned long ulMainTpNum;
    unsigned long ulPsiSize;
    unsigned long ulNumMacAddrs;
    unsigned char ucaBaseMacAddr[NVRAM_MAC_ADDRESS_LEN];
    char pad;
    char backupPsi;  /**< if 0x01, allocate space for a backup PSI */
    unsigned long ulCheckSumV4;
    char gponSerialNumber[NVRAM_GPON_SERIAL_NUMBER_LEN];
    char gponPassword[NVRAM_GPON_PASSWORD_LEN];
    char wpsDevicePin[NVRAM_WPS_DEVICE_PIN_LEN];
    char wlanParams[NVRAM_WLAN_PARAMS_LEN];
    unsigned long ulSyslogSize; /**< number of KB to allocate for persistent syslog */
    unsigned long ulNandPartOfsKb[NP_TOTAL];
    unsigned long ulNandPartSizeKb[NP_TOTAL];
    char szVoiceBoardId[NVRAM_BOARD_ID_STRING_LEN];
    unsigned long afeId[2];
#if 0
    char OpticalParams[NVRAM_OPTICAL_PARAMS_SIZE];    
#endif
#if 1//__MTS__, RaynorChung: Support 963268 nand flash, patch form SVN#3597 on http://svn.zyxel.com.tw/svn/CPE_SW1/BCM96368/trunk/P-870HA/branches/cht/fttb8/4.11 
    char VendorName[NVRAM_VENDORNAME_LEN];
    char ProductName[NVRAM_PRODUCTNAME_LEN];
    char FeatureBits[NVRAM_FEATUREBITS_LEN];
    char BuildInfo[NVRAM_BUILDINFO_LEN];
    char SerialNumber[NVRAM_SERIALNUMBER_LEN];
    char FeatureBit;
    char EngDebugFlag;
    char countryCode;
#if 1 // __VERIZON__, ALLEN
    unsigned char ucFlashBlkSize;
    unsigned char ucAuxFSPercent;
#if 1//__MTS__, RaynorChung: Patch form SVN 4.06L03
    char ProductEXTName[NVRAM_PRODUCTNAME_LEN];
#endif
#if 1 //__MSTC__, Dennis, merge from Elina
    char updateConfig;  
#endif
#if 1
    char OpticalParams[NVRAM_OPTICAL_PARAMS_SIZE];    
#endif
#if 1 //__CTLK__, Thief
	char facortyimageversion[NVRAM_FACORTYVERSION_LEN];	
#endif
#if 1 /* __MSTC__, zongyue: additional partition information */
    unsigned long AppNandPartOfsKb;
    unsigned long AppNandPartSizeKb;
#endif
    char chUnused[125];
#else
#if 1//__MTS__, RaynorChung: Patch form SVN 4.06L03
    char ProductEXTName[NVRAM_PRODUCTNAME_LEN];
#endif
    char chUnused[203];
#endif
#else
#if 1//__MTS__, RaynorChung: Patch form SVN 4.06L03
    char countryCode;
#endif
    char chUnused[355];
#endif
    unsigned long ulCheckSum;
} NVRAM_DATA, *PNVRAM_DATA;
#pragma pack()
#endif

#if 1//__MTS__, RaynorChung: Support 963268 nand flash, patch form SVN#3597 on http://svn.zyxel.com.tw/svn/CPE_SW1/BCM96368/trunk/P-870HA/branches/cht/fttb8/4.11 
#define BOOT_LATEST_IMAGE   '0'
#define BOOT_PREVIOUS_IMAGE '1'
#endif
/*****************************************************************************/
/*       Misc Offsets                                                        */
/*****************************************************************************/
#define CFE_VERSION_OFFSET           0x0570
#define CFE_VERSION_MARK_SIZE        5
#define CFE_VERSION_SIZE             5

/*****************************************************************************/
/*       Scratch Pad Defines                                                 */
/*****************************************************************************/
/* SP - Persistent Scratch Pad format:
       sp header        : 32 bytes
       tokenId-1        : 8 bytes
       tokenId-1 len    : 4 bytes
       tokenId-1 data    
       ....
       tokenId-n        : 8 bytes
       tokenId-n len    : 4 bytes
       tokenId-n data    
*/

#define MAGIC_NUM_LEN       8
#define MAGIC_NUMBER        "gOGoBrCm"
#define TOKEN_NAME_LEN      16
#define SP_VERSION          1
#if 1//__MTS__, RaynorChung: Support 963268 nand flash, patch form SVN#3597 on http://svn.zyxel.com.tw/svn/CPE_SW1/BCM96368/trunk/P-870HA/branches/cht/fttb8/4.11 
#define SP_RESERVERD        20
#else
#define CFE_NVRAM_DATA2_LEN 20
#endif

#ifndef _LANGUAGE_ASSEMBLY
typedef struct _SP_HEADER
{
    char SPMagicNum[MAGIC_NUM_LEN];             // 8 bytes of magic number
    int SPVersion;                              // version number
#if 1//__MTS__, RaynorChung: Support 963268 nand flash, patch form SVN#3597 on http://svn.zyxel.com.tw/svn/CPE_SW1/BCM96368/trunk/P-870HA/branches/cht/fttb8/4.11 
    char SPReserved[SP_RESERVERD];              // reservied, total 32 bytes
#else
    char NvramData2[CFE_NVRAM_DATA2_LEN];       // not related to scratch pad
                                                // additional NVRAM_DATA
#endif
} SP_HEADER, *PSP_HEADER;                       // total 32 bytes

typedef struct _TOKEN_DEF
{
    char tokenName[TOKEN_NAME_LEN];
    int tokenLen;
} SP_TOKEN, *PSP_TOKEN;
#endif

/*****************************************************************************/
/*       Boot Loader Parameters                                              */
/*****************************************************************************/

#define BLPARMS_MAGIC               0x424c504d

#define BOOTED_IMAGE_ID_NAME        "boot_image"

#define BOOTED_NEW_IMAGE            1
#define BOOTED_OLD_IMAGE            2
#define BOOTED_ONLY_IMAGE           3
#define BOOTED_PART1_IMAGE          4
#define BOOTED_PART2_IMAGE          5

#define BOOT_SET_NEW_IMAGE          '0'
#define BOOT_SET_OLD_IMAGE          '1'
#define BOOT_SET_NEW_IMAGE_ONCE     '2'
#define BOOT_GET_IMAGE_VERSION      '3'
#define BOOT_GET_BOOTED_IMAGE_ID    '4'
#define BOOT_SET_PART1_IMAGE        '5'
#define BOOT_SET_PART2_IMAGE        '6'
#define BOOT_SET_PART1_IMAGE_ONCE   '7'
#define BOOT_SET_PART2_IMAGE_ONCE   '8'
#define BOOT_GET_BOOT_IMAGE_STATE   '9'

#define FLASH_PARTDEFAULT_REBOOT    0x00000000
#define FLASH_PARTDEFAULT_NO_REBOOT 0x00000001
#define FLASH_PART1_REBOOT          0x00010000
#define FLASH_PART1_NO_REBOOT       0x00010001
#define FLASH_PART2_REBOOT          0x00020000
#define FLASH_PART2_NO_REBOOT       0x00020001

#define FLASH_IS_NO_REBOOT(X)       ((X) & 0x0000ffff)
#define FLASH_GET_PARTITION(X)      ((unsigned long) (X) >> 16)

#ifdef __cplusplus
}
#endif

#endif /* _BCM_HWDEFS_H */

