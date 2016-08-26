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

/*!\file flash_common.c
 * \brief This file contains NOR flash related functions used by both
 *        CFE and kernel.
 *
 */

/** Includes. */
#ifdef _CFE_                                                
#include "lib_types.h"
#include "lib_printf.h"
#include "lib_string.h"
#include "bcm_map.h"
#include "bcm63xx_util.h"
#if 1//__MTS__, RaynorChung: Support 963268 nand flash, patch form SVN#3597 on http://svn.zyxel.com.tw/svn/CPE_SW1/BCM96368/trunk/P-870HA/branches/cht/fttb8/4.11 
#include "lib_malloc.h"
#else  
#define printk  printf
#endif
#else // Linux
#include <linux/kernel.h>
#include "bcm_map_part.h"
#if 1//__MTS__, RaynorChung: Support 963268 nand flash, patch form SVN#3597 on http://svn.zyxel.com.tw/svn/CPE_SW1/BCM96368/trunk/P-870HA/branches/cht/fttb8/4.11 
#include <linux/sched.h>
#endif
#endif

#include "bcmtypes.h"
#include "bcm_hwdefs.h"
#include "flash_api.h"
#include "flash_common.h"

//#define DEBUG_FLASH

void flash_init_info(const NVRAM_DATA *nvRam, FLASH_ADDR_INFO *fInfo)
{
    int i = 0;
    int totalBlks = 0;
    int totalSize = 0;
    int psiStartAddr = 0;
    int spStartAddr = 0;
    int usedBlkSize = 0;
    int needBytes = 0;

    if (flash_get_flash_type() == FLASH_IFC_NAND)
    {
        /* When using NAND flash disable Bcm_flash */
        totalSize = 0;
    }
    else {
        totalBlks = flash_get_numsectors();
        totalSize = flash_get_total_size();
        printk("Total Flash size: %dK with %d sectors\n", totalSize/1024, totalBlks);
    }

    if (totalSize <= FLASH_LENGTH_BOOT_ROM) {
        /* NAND flash settings. NAND flash does not use raw flash partitioins
         * to store psi, backup psi, scratch pad and syslog.  These data items
         * are stored as files on a JFFS2 file system.
         */
        if ((nvRam->ulPsiSize != -1) && (nvRam->ulPsiSize != 0))
            fInfo->flash_persistent_length = nvRam->ulPsiSize * ONEK;
        else
            fInfo->flash_persistent_length = DEFAULT_PSI_SIZE * ONEK;

        fInfo->flash_persistent_start_blk = 0;
        fInfo->flash_rootfs_start_offset = 0;
        fInfo->flash_scratch_pad_length = SP_MAX_LEN;
        fInfo->flash_syslog_length = nvRam->ulSyslogSize * 1024;

        /* This is a boolean field for NAND flash. */
        fInfo->flash_backup_psi_number_blk = nvRam->backupPsi;
        return;
    }

    /*
    * calculate mandatory primary PSI size and set its fInfo parameters.
    */
    if ((nvRam->ulPsiSize != -1) && (nvRam->ulPsiSize != 0))
        fInfo->flash_persistent_length = nvRam->ulPsiSize * ONEK;
    else
        fInfo->flash_persistent_length = DEFAULT_PSI_SIZE * ONEK;

    psiStartAddr = totalSize - fInfo->flash_persistent_length;
    fInfo->flash_persistent_start_blk = flash_get_blk(FLASH_BASE+psiStartAddr);
    fInfo->flash_persistent_number_blk = totalBlks - fInfo->flash_persistent_start_blk;

    usedBlkSize = 0;
    for (i = fInfo->flash_persistent_start_blk; 
        i < (fInfo->flash_persistent_start_blk + fInfo->flash_persistent_number_blk); i++)
    {
        usedBlkSize += flash_get_sector_size((unsigned short) i);
    }
    fInfo->flash_persistent_blk_offset =  usedBlkSize - fInfo->flash_persistent_length;
    fInfo->flash_meta_start_blk = fInfo->flash_persistent_start_blk;

    /*
    * Next is the optional scratch pad, which is on top of the primary PSI.
    * Old code allowed scratch pad to share a sector with primary PSI.
    * That is retained for backward compatibility.  (Although depending on your
    * NOR flash sector sizes, they may still be in different sectors.)
    * If you have a new deployment, consider forcing separate sectors.
    */
    if ((fInfo->flash_persistent_blk_offset > 0) &&
        (fInfo->flash_persistent_blk_offset < SP_MAX_LEN))
    {
        /*
        * there is some room left in the first persistent sector, but it is
        * not big enough for the scratch pad. (Use this line unconditionally
        * if you want to guarentee scratch pad and primary PSI are on different
        * sectors.)
        */
        spStartAddr = psiStartAddr - fInfo->flash_persistent_blk_offset - SP_MAX_LEN;
    }
    else
    {
        /* either the primary PSI starts on a sector boundary, or there is
        * enough room at the top of the first sector for the scratch pad. */
        spStartAddr = psiStartAddr - SP_MAX_LEN ;
    }

    fInfo->flash_scratch_pad_start_blk = flash_get_blk(FLASH_BASE+spStartAddr);
    fInfo->flash_scratch_pad_length = SP_MAX_LEN;

    if (fInfo->flash_persistent_start_blk == fInfo->flash_scratch_pad_start_blk)  // share blk
    {
#if 0 /* do not used scratch pad unless it's in its own sector */
        printk("Scratch pad is not used for this flash part.\n");  
        fInfo->flash_scratch_pad_length = 0;     // no sp
#else /* allow scratch pad to share a sector with another section such as PSI */
        fInfo->flash_scratch_pad_number_blk = 1;
        fInfo->flash_scratch_pad_blk_offset = fInfo->flash_persistent_blk_offset - fInfo->flash_scratch_pad_length;
#endif
    }
    else // on different blk
    {
        fInfo->flash_scratch_pad_number_blk = fInfo->flash_persistent_start_blk - fInfo->flash_scratch_pad_start_blk;
        // find out the offset in the start_blk
        usedBlkSize = 0;
        for (i = fInfo->flash_scratch_pad_start_blk; 
            i < (fInfo->flash_scratch_pad_start_blk + fInfo->flash_scratch_pad_number_blk); i++)
            usedBlkSize += flash_get_sector_size((unsigned short) i);
        fInfo->flash_scratch_pad_blk_offset =  usedBlkSize - fInfo->flash_scratch_pad_length;
    }

    if (fInfo->flash_scratch_pad_length > 0) {

        fInfo->flash_meta_start_blk = fInfo->flash_scratch_pad_start_blk;
    }

    /*
    * Next is the optional backup PSI.
    */
    if (nvRam->backupPsi == 0x01)
    {
        needBytes = fInfo->flash_persistent_length;
        i = fInfo->flash_meta_start_blk;
        while (needBytes > 0)
        {
            i--;
            needBytes -= flash_get_sector_size((unsigned short) i);
        }
        fInfo->flash_backup_psi_start_blk = i;
        /* calclate how many blocks we actually consumed */
        needBytes = fInfo->flash_persistent_length;
        fInfo->flash_backup_psi_number_blk = 0;
        while (needBytes > 0)
        {
            needBytes -= flash_get_sector_size((unsigned short) i);
            i++;
            fInfo->flash_backup_psi_number_blk++;
        }

        fInfo->flash_meta_start_blk = fInfo->flash_backup_psi_start_blk;
    }
    else
    {
        fInfo->flash_backup_psi_number_blk = 0;
    }

    /*
    * Next is the optional persistent syslog.
    */
    if (nvRam->ulSyslogSize != 0 && nvRam->ulSyslogSize != -1)
    {
        fInfo->flash_syslog_length = nvRam->ulSyslogSize * 1024;
        needBytes = fInfo->flash_syslog_length;
        i = fInfo->flash_meta_start_blk;
        while (needBytes > 0)
        {
            i--;
            needBytes -= flash_get_sector_size((unsigned short) i);
        }
        fInfo->flash_syslog_start_blk = i;
        /* calclate how many blocks we actually consumed */
        needBytes = fInfo->flash_syslog_length;
        fInfo->flash_syslog_number_blk = 0;
        while (needBytes > 0)
        {
            needBytes -= flash_get_sector_size((unsigned short) i);
            i++;
            fInfo->flash_syslog_number_blk++;
        }

        fInfo->flash_meta_start_blk = fInfo->flash_syslog_start_blk;
    }
    else
    {
        fInfo->flash_syslog_length = 0;
        fInfo->flash_syslog_number_blk = 0;
    }

#ifdef DEBUG_FLASH_TOO_MUCH
    /* dump sizes of all sectors in flash */
    for (i=0; i<totalBlks; i++)
        printk("blk %03d: %d\n", i, flash_get_sector_size((unsigned short) i));
#endif

#if defined(DEBUG_FLASH)
    printk("FLASH_BASE                    =0x%08x\n\n", (unsigned int)FLASH_BASE);

    printk("fInfo->flash_rootfs_start_offset =0x%08x\n\n", (unsigned int)fInfo->flash_rootfs_start_offset);

    printk("fInfo->flash_meta_start_blk = %d\n\n", fInfo->flash_meta_start_blk);

    printk("fInfo->flash_syslog_start_blk  = %d\n", fInfo->flash_syslog_start_blk);
    printk("fInfo->flash_syslog_number_blk = %d\n", fInfo->flash_syslog_number_blk);
    printk("fInfo->flash_syslog_length=0x%x\n\n", (unsigned int)fInfo->flash_syslog_length);

    printk("fInfo->flash_backup_psi_start_blk = %d\n", fInfo->flash_backup_psi_start_blk);
    printk("fInfo->flash_backup_psi_number_blk = %d\n\n", fInfo->flash_backup_psi_number_blk);

    printk("sp startAddr = %x\n", (unsigned int) (FLASH_BASE+spStartAddr));
    printk("fInfo->flash_scratch_pad_start_blk = %d\n", fInfo->flash_scratch_pad_start_blk);
    printk("fInfo->flash_scratch_pad_number_blk = %d\n", fInfo->flash_scratch_pad_number_blk);
    printk("fInfo->flash_scratch_pad_length = 0x%x\n", fInfo->flash_scratch_pad_length);
    printk("fInfo->flash_scratch_pad_blk_offset = 0x%x\n\n", (unsigned int)fInfo->flash_scratch_pad_blk_offset);

    printk("psi startAddr = %x\n", (unsigned int) (FLASH_BASE+psiStartAddr));
    printk("fInfo->flash_persistent_start_blk = %d\n", fInfo->flash_persistent_start_blk);
    printk("fInfo->flash_persistent_number_blk = %d\n", fInfo->flash_persistent_number_blk);
    printk("fInfo->flash_persistent_length=0x%x\n", (unsigned int)fInfo->flash_persistent_length);
    printk("fInfo->flash_persistent_blk_offset = 0x%x\n\n", (unsigned int)fInfo->flash_persistent_blk_offset);
#endif
}

#if 1 //__MTS__, Paul Ho: Support 963268 nand flash, patch form SVN#3781 on http://svn.zyxel.com.tw/svn/CPE_SW1/BCM96368/trunk/P-870HA/branches/cht/fttb8/4.11
int getNandBlock (int addr, int order) {
    int num_blocks = flash_get_numsectors();
    int len = 0;
    int blk = flash_get_blk(addr);
    static unsigned char buf[256*ONEK];
    int i = 0;

    if ( order == 0 && blk > 0 && blk < num_blocks ) {
        for( i = blk; i < num_blocks; i++ )
        {
            len = flash_get_sector_size(i);
            if( flash_read_buf(i, 0, buf, len) != FLASH_API_ERROR )
            {
                return i;
            }
        }
    }
    else if ( order == 1 && blk > 0 && blk < num_blocks ) {
        for( i = blk; i > 0; i-- )
        {
            len = flash_get_sector_size(i);
            if( flash_read_buf(i, 0, buf, len) != FLASH_API_ERROR )
            {
                return i;
            }
        }
    }
    return FLASH_API_ERROR;
}

int getNandBlockSize (int blk) {
    int len = flash_get_sector_size(blk);
    static unsigned char buf[256*ONEK];

    if( flash_read_buf(blk, 0, buf, len) != FLASH_API_ERROR )
    {
        return len;
    }
    return 0;
}

void flash_init_nand_info(NVRAM_DATA *nvRam, FLASH_ADDR_INFO *fInfo)
{
    int i = 0;
    int totalBlks = 0;
    int totalSize = 0;
#if 0 /*__MSTC__, JhenYang; 20120402: zongyue mark to 0 */
    int psiStartAddr = 0;
#endif
#if 1 //__MSTC__, Dennis merge from Elina
   NVRAM_DATA nvramData;
   int change = 0;
#endif
    if (flash_get_flash_type() == FLASH_IFC_NAND)
    {
        /* When using NAND flash disable Bcm_flash */
        totalBlks = flash_get_numsectors();
        totalSize = 0;
    }
    else {
        totalBlks = flash_get_numsectors();
        totalSize = flash_get_total_size();
        printk("Total Flash size: %dK with %d sectors\n", totalSize/1024, totalBlks);
    }

    if (totalSize <= FLASH_LENGTH_BOOT_ROM) {
        /* NAND flash settings. NAND flash does not use raw flash partitioins
         * to store psi, backup psi, scratch pad and syslog.  These data items
         * are stored as files on a JFFS2 file system.
         */
        if ((nvRam->ulPsiSize != -1) && (nvRam->ulPsiSize != 0))
            fInfo->flash_persistent_length = nvRam->ulPsiSize * ONEK;
        else
            fInfo->flash_persistent_length = DEFAULT_PSI_SIZE * ONEK;

        fInfo->flash_persistent_start_blk = 0;
        fInfo->flash_rootfs_start_offset = 0;
        fInfo->flash_scratch_pad_length = SP_MAX_LEN;
        fInfo->flash_syslog_length = nvRam->ulSyslogSize * ONEK;

        /* This is a boolean field for NAND flash. */
        fInfo->flash_backup_psi_number_blk = nvRam->backupPsi;
    }

    unsigned long ulBlockSizeKb = (unsigned long)flash_get_sector_size(0)/1024;
    unsigned long ulTotalSizeKb = (unsigned long)flash_get_total_size() / 1024;
    unsigned long ulRootfsSizeKb;
    unsigned long ulBbtSizeKb = (ulTotalSizeKb > NAND_BBT_THRESHOLD_KB)
        ? NAND_BBT_BIG_SIZE_KB : NAND_BBT_SMALL_SIZE_KB;


    fInfo->flash_rootfs_start_offset = flash_get_sector_size(0);
    if( fInfo->flash_rootfs_start_offset < FLASH_LENGTH_BOOT_ROM )
        fInfo->flash_rootfs_start_offset = FLASH_LENGTH_BOOT_ROM;

    int blk = getNandBlock(FLASH_BASE+fInfo->flash_rootfs_start_offset, 0);
    fInfo->flash_rootfs_start_offset = 0;
    /* This can shift boot address when it occurs bad block on block#1 */
    for ( i = 0; i < blk ; i ++ ) {
      fInfo->flash_rootfs_start_offset += flash_get_sector_size(i);
    }

    /* The Boot partition is first and is one block in size. */
    nvramData.ulNandPartOfsKb[NP_BOOT] = 0;
    nvramData.ulNandPartSizeKb[NP_BOOT] = ulBlockSizeKb;

    /* The Bad Block Table partition is last and is a constant size. */
    nvramData.ulNandPartOfsKb[NP_BBT] = ulTotalSizeKb - ulBbtSizeKb;
    nvramData.ulNandPartSizeKb[NP_BBT] = ulBbtSizeKb;

    /* The Data partition is before the BBT and is a constant size. */
    nvramData.ulNandPartOfsKb[NP_DATA] = nvramData.ulNandPartOfsKb[NP_BBT] - NAND_DATA_SIZE_KB;
    nvramData.ulNandPartSizeKb[NP_DATA] = NAND_DATA_SIZE_KB;

#if 1 /* __MSTC__, zongyue: additional partition */
    /*
       Notice, the flash partition is different from NOR flash.
       We use DATA partition to store PSI, backupPSI, and scratchpad.
       And use flash_persistent partition to store updated configuration file 
       when we want to update configuration at cfe command mode.
       0~64MB: CFE(128KB) + RootFS1(32640KB) + RootFS2(32768KB-->32640KB)
       64~128MB: Reserved(1024KB) + APP(59392KB) + DATA(4096KB) + BBT(1024KB)
    */
    blk = flash_get_blk(FLASH_BASE + ulTotalSizeKb * 1024 / 2);
    fInfo->flash_persistent_start_blk = getNandBlock((FLASH_BASE + ulTotalSizeKb * 1024 / 2), 0);
    if (8 > (fInfo->flash_persistent_start_blk - blk))
    {
        fInfo->flash_persistent_number_blk = 1;
        fInfo->flash_persistent_length = fInfo->flash_persistent_number_blk * flash_get_sector_size(0);
        fInfo->flash_persistent_blk_offset = fInfo->flash_persistent_start_blk * flash_get_sector_size(0);
    }
    else
    {   /* __MSTC__, zongyue: no more reserved space */
        fInfo->flash_persistent_start_blk = blk;
        fInfo->flash_persistent_number_blk = 0;
        fInfo->flash_persistent_length = 0;
        fInfo->flash_persistent_blk_offset = blk * flash_get_sector_size(0);
    }

    /* __MSTC__, zongyue: 1st RootFS is starting after CFE block, size is 32640KB(32MB-128KB) */
    ulRootfsSizeKb = ((ulTotalSizeKb / 4) - ulBlockSizeKb);
	nvramData.ulNandPartOfsKb[NP_ROOTFS_1] = ulBlockSizeKb;
    nvramData.ulNandPartSizeKb[NP_ROOTFS_1] = ulRootfsSizeKb;

    blk = flash_get_blk(FLASH_BASE + ulTotalSizeKb * 1024 / 4);
    nvramData.ulNandPartOfsKb[NP_ROOTFS_2] = blk * flash_get_sector_size(0) / 1024;
    nvramData.ulNandPartSizeKb[NP_ROOTFS_2] = ulRootfsSizeKb;

    /* additional APP partition info */
	blk = flash_get_blk(FLASH_BASE + ulTotalSizeKb * 1024 / 2 + NAND_RAW_SIZE_KB * 1024);
    nvramData.AppNandPartOfsKb = blk * flash_get_sector_size(0) / 1024;
    nvramData.AppNandPartSizeKb = (ulTotalSizeKb / 2) - NAND_RAW_SIZE_KB - NAND_DATA_SIZE_KB - ulBbtSizeKb;

    /* update flash info structure */
    fInfo->flash_meta_start_blk = fInfo->flash_persistent_start_blk;
    fInfo->flash_rootfs_max_size = ulRootfsSizeKb * ONEK;
#else
#if 1 //__MSTC__,JhenYang
    fInfo->flash_persistent_number_blk = 1;
    psiStartAddr = nvramData.ulNandPartOfsKb[NP_DATA] * ONEK - (fInfo->flash_persistent_number_blk * ulBlockSizeKb * ONEK);
    fInfo->flash_persistent_start_blk = getNandBlock(FLASH_BASE+psiStartAddr, 1);
    fInfo->flash_persistent_blk_offset = fInfo->flash_persistent_start_blk * flash_get_sector_size(0);
    fInfo->flash_persistent_length = fInfo->flash_persistent_number_blk * flash_get_sector_size(0);
#else
#if 1 //__MSTC__, Dennis merge from Elina
   /*
      Notice, the flash partition is different from NOR flash. We use DATA partition to store PSI, backupPSI, and scratchpad.
      And use flash_persistent partition to store updated configuration file when we want to update configuration at cfe command mode.
   */
   psiStartAddr = flash_get_blk(FLASH_BASE + nvramData.ulNandPartOfsKb[NP_DATA] * 1024);
   fInfo->flash_persistent_start_blk = getNandBlock(FLASH_BASE + psiStartAddr, 1);
   fInfo->flash_persistent_number_blk = 1;
   fInfo->flash_persistent_length = fInfo->flash_persistent_number_blk * flash_get_sector_size(0);
   fInfo->flash_persistent_blk_offset = fInfo->flash_persistent_start_blk * flash_get_sector_size(0);
#endif
#endif

    /* The first rootfs partition starts at the second sector. */
    nvramData.ulNandPartOfsKb[NP_ROOTFS_1] = ulBlockSizeKb;

    /* The size of the two root file system partitions is whatever is left
     * after the Boot, Data and BBT paritions divided by 2 and evenly
     * divisible by the NAND flash block size.
     */
#if 1 //__MSTC__, Dennis
#else
    fInfo->flash_rootfs_max_size = 
        ((nvRam->ulNandPartOfsKb[NP_DATA]/ulBlockSizeKb) - totalBlks/2 - 1) * flash_get_sector_size(0);
    ulRootfsSizeKb = fInfo->flash_rootfs_max_size/1024;
#endif
#if 1 //__MSTC__, Dennis
    fInfo->flash_meta_start_blk = fInfo->flash_persistent_start_blk;
#else
    fInfo->flash_meta_start_blk = flash_get_blk(FLASH_BASE + nvramData.ulNandPartOfsKb[NP_DATA] * 1024);
#endif
#if 1 //__MSTC__, Dennis
    fInfo->flash_rootfs_max_size = 
        (fInfo->flash_meta_start_blk - totalBlks/2 - 1) * flash_get_sector_size(0);
    ulRootfsSizeKb = fInfo->flash_rootfs_max_size/1024;
#endif

    nvramData.ulNandPartSizeKb[NP_ROOTFS_1] = ulRootfsSizeKb;

    /* Record the bottom half first available block on nvRam->ulNandPartOfsKb[NP_ROOTFS_2] */	
    blk = getNandBlock(FLASH_BASE+ulTotalSizeKb*1024/2 , 0);
    nvramData.ulNandPartOfsKb[NP_ROOTFS_2] = blk * flash_get_sector_size(0)/1024;
#if 1 //__MSTC__, Dennis merge from Elina
    nvramData.ulNandPartSizeKb[NP_ROOTFS_2] = (fInfo->flash_persistent_blk_offset/ONEK) - nvramData.ulNandPartOfsKb[NP_ROOTFS_2];
#else
    nvramData.ulNandPartSizeKb[NP_ROOTFS_2] = nvramData.ulNandPartOfsKb[NP_DATA]- nvramData.ulNandPartOfsKb[NP_ROOTFS_2];
#endif
#endif

#if 1 // __MSTC__, zongyue: to fix ImageDefault start postion and RootFS size if NAND FLASH have bad blocks
    fInfo->flash_rootfs_bad_block_number = 0;
#endif

#ifdef _CFE_  
   if(nvramData.ulNandPartOfsKb[NP_BOOT] != nvRam->ulNandPartOfsKb[NP_BOOT])
   {
      nvRam->ulNandPartOfsKb[NP_BOOT] = nvramData.ulNandPartOfsKb[NP_BOOT];
      change++;
   }
   if(nvramData.ulNandPartSizeKb[NP_BOOT] != nvRam->ulNandPartSizeKb[NP_BOOT])
   {
      nvRam->ulNandPartSizeKb[NP_BOOT] = nvramData.ulNandPartSizeKb[NP_BOOT];
      change++;
   }
   if(nvramData.ulNandPartOfsKb[NP_DATA] != nvRam->ulNandPartOfsKb[NP_DATA])
   {
      nvRam->ulNandPartOfsKb[NP_DATA] = nvramData.ulNandPartOfsKb[NP_DATA];
      change++;
   }
   if(nvramData.ulNandPartSizeKb[NP_DATA] != nvRam->ulNandPartSizeKb[NP_DATA])
   {
      nvRam->ulNandPartSizeKb[NP_DATA] = nvramData.ulNandPartSizeKb[NP_DATA];
      change++;
   }
   if(nvramData.ulNandPartOfsKb[NP_ROOTFS_1] != nvRam->ulNandPartOfsKb[NP_ROOTFS_1])
   {
      nvRam->ulNandPartOfsKb[NP_ROOTFS_1] = nvramData.ulNandPartOfsKb[NP_ROOTFS_1];
      change++;
   }
   if(nvramData.ulNandPartSizeKb[NP_ROOTFS_1] != nvRam->ulNandPartSizeKb[NP_ROOTFS_1])
   {
      nvRam->ulNandPartSizeKb[NP_ROOTFS_1] = nvramData.ulNandPartSizeKb[NP_ROOTFS_1];
      change++;
   }
   if(nvramData.ulNandPartOfsKb[NP_ROOTFS_2] != nvRam->ulNandPartOfsKb[NP_ROOTFS_2])
   {
      nvRam->ulNandPartOfsKb[NP_ROOTFS_2] = nvramData.ulNandPartOfsKb[NP_ROOTFS_2];
      change++;
   }
   if(nvramData.ulNandPartSizeKb[NP_ROOTFS_2] != nvRam->ulNandPartSizeKb[NP_ROOTFS_2])
   {
      nvRam->ulNandPartSizeKb[NP_ROOTFS_2] = nvramData.ulNandPartSizeKb[NP_ROOTFS_2];
      change++;
   }
#if 1 /* __MSTC__, zongyue: update additional APP partition info, if different previous */
   if(nvramData.AppNandPartOfsKb != nvRam->AppNandPartOfsKb)
   {
      nvRam->AppNandPartOfsKb = nvramData.AppNandPartOfsKb;
      change++;
   }
   if(nvramData.AppNandPartSizeKb != nvRam->AppNandPartSizeKb)
   {
      nvRam->AppNandPartSizeKb = nvramData.AppNandPartSizeKb;
      change++;
   }
   /* __MSTC__, zongyue: why BBT did not update */
   if(nvramData.ulNandPartOfsKb[NP_BBT] != nvRam->ulNandPartOfsKb[NP_BBT])
   {
      nvRam->ulNandPartOfsKb[NP_BBT] = nvramData.ulNandPartOfsKb[NP_BBT];
      change++;
   }
   if(nvramData.ulNandPartSizeKb[NP_BBT] != nvRam->ulNandPartSizeKb[NP_BBT])
   {
      nvRam->ulNandPartSizeKb[NP_BBT] = nvramData.ulNandPartSizeKb[NP_BBT];
      change++;
   }
#endif

   if ( change > 0 ) 
   {
      writeNvramData(nvRam);
      printf("Nand flash partition is changed.\n");
   }
#endif

#if defined(DEBUG_FLASH)
    printk("flash partition table\n");
    printk("boot	offset=0x%8.8lx, size=0x%8.8lx\n",
    	nvRam->ulNandPartOfsKb[NP_BOOT] * 1024,
    	nvRam->ulNandPartSizeKb[NP_BOOT] * 1024);
    printk("rootfs1 offset=0x%8.8lx, size=0x%8.8lx\n",
    	nvRam->ulNandPartOfsKb[NP_ROOTFS_1] * 1024,
    	nvRam->ulNandPartSizeKb[NP_ROOTFS_1] * 1024);
    printk("rootfs2 offset=0x%8.8lx, size=0x%8.8lx\n",
    	nvRam->ulNandPartOfsKb[NP_ROOTFS_2] * 1024,
    	nvRam->ulNandPartSizeKb[NP_ROOTFS_2] * 1024);
    printk("data	offset=0x%8.8lx, size=0x%8.8lx\n",
    	nvRam->ulNandPartOfsKb[NP_DATA] * 1024,
    	nvRam->ulNandPartSizeKb[NP_DATA] * 1024);
    printk("bbt 	offset=0x%8.8lx, size=0x%8.8lx\n",
    	nvRam->ulNandPartOfsKb[NP_BBT] * 1024,
    	nvRam->ulNandPartSizeKb[NP_BBT] * 1024);
    /* additional APP partition info */
    printk("app 	offset=0x%8.8lx, size=0x%8.8lx\n\n",
    	nvRam->AppNandPartOfsKb * 1024,
    	nvRam->AppNandPartSizeKb * 1024);

    printk("FLASH_BASE                    =0x%08x\n\n", (unsigned int)FLASH_BASE);
    printk("fInfo->flash_rootfs_start_offset =0x%08x\n", (unsigned int)fInfo->flash_rootfs_start_offset);
    printk("fInfo->flash_rootfs_max_size =0x%08x\n", (unsigned int)fInfo->flash_rootfs_max_size);
    printk("fInfo->flash_meta_start_blk = %d\n", fInfo->flash_meta_start_blk);
    printk("fInfo->flash_syslog_length=0x%x\n", (unsigned int)fInfo->flash_syslog_length);
    printk("fInfo->flash_backup_psi_number_blk = %d\n", fInfo->flash_backup_psi_number_blk);
    printk("fInfo->flash_scratch_pad_length = 0x%x\n", fInfo->flash_scratch_pad_length);
    printk("fInfo->flash_persistent_start_blk = %d\n", fInfo->flash_persistent_start_blk);
    printk("fInfo->flash_persistent_number_blk = %d\n", fInfo->flash_persistent_number_blk);
    printk("fInfo->flash_persistent_length=0x%x\n", (unsigned int)fInfo->flash_persistent_length);
#endif

}
#endif

#ifdef _CFE_ /* __MSTC__, zongyue: to calculate rootfs parition bad block number */
#if 1
#if (INC_NAND_FLASH_DRIVER==1)
void flash_scan_bad_blocks(FLASH_ADDR_INFO *fInfo)
{
    PFILE_TAG pTag = getBootImageTag();
    int i;
    unsigned int start_block = getNandBlock(atoi(pTag->rootfsAddress) + BOOT_OFFSET, 1);
    unsigned int end_block = getNandBlock(atoi(pTag->rootfsAddress) + BOOT_OFFSET +
                                          atoi(pTag->rootfsLen), 1);

    for (i = start_block; i < end_block; i++)
    {
        if (0 == getNandBlockSize(i))
        {
            fInfo->flash_rootfs_bad_block_number++;
            end_block++;
        }
    }
}
#endif
#endif
#endif

unsigned int flash_get_reserved_bytes_at_end(const FLASH_ADDR_INFO *fInfo)
{
    unsigned int reserved=0;
    int i = fInfo->flash_meta_start_blk;
    int totalBlks = flash_get_numsectors();

    while (i < totalBlks)
    {
        reserved += flash_get_sector_size((unsigned short) i);
        i++;
    }

#if defined(DEBUG_FLASH)
    printk("reserved at bottom=%dKB\n", reserved/1024);
#endif

    return reserved;
}

