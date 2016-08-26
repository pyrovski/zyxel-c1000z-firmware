/***********************************************************************
 *
 *  Copyright (c) 2007  Broadcom Corporation
 *  All Rights Reserved
 *
# 
# 
# Unless you and Broadcom execute a separate written software license 
# agreement governing use of this software, this software is licensed 
# to you under the terms of the GNU General Public License version 2 
# (the "GPL"), available at http://www.broadcom.com/licenses/GPLv2.php, 
# with the following added to such license:
# 
#    As a special exception, the copyright holders of this software give 
#    you permission to link this software with independent modules, and 
#    to copy and distribute the resulting executable under terms of your 
#    choice, provided that you also meet, for each linked independent 
#    module, the terms and conditions of the license of that module. 
#    An independent module is a module which is not derived from this
#    software.  The special exception does not apply to any modifications 
#    of the software.  
# 
# Not withstanding the above, under no circumstances may you combine 
# this software in any way with any other Broadcom software provided 
# under a license other than the GPL, without Broadcom's express prior 
# written consent. 
#
 *
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <net/if.h>
#include <fcntl.h>

#include "cms.h"
#include "cms_util.h"
#include "cms_msg.h"
#include "cms_boardcmds.h"
#include "cms_boardioctl.h"

#include "bcmTag.h" /* in shared/opensource/include/bcm963xx, for FILE_TAG */
#include "board.h" /* in bcmdrivers/opensource/include/bcm963xx, for BCM_IMAGE_CFE */

static UBOOL8 matchChipId(const char *strTagChipId, const char *signature2);
#if 1 //__MTS__, RaynorChung: Support 963268 nand flash, patch form SVN#3597 on http://svn.zyxel.com.tw/svn/CPE_SW1/BCM96368/trunk/P-870HA/branches/cht/fttb8/4.11 
static CmsRet verifyBroadcomFileTag(PFILE_TAG pTag);
#else
CmsRet verifyBroadcomFileTag(FILE_TAG *pTag, int imageLen);
#endif
#if 1 //__CTLK__, Thief
static CmsRet flashImage(char *imagePtr, UINT32 imageLen, CmsImageFormat format, void* msgHandle);
#else
static CmsRet flashImage(char *imagePtr, UINT32 imageLen, CmsImageFormat format);
#endif
static CmsRet sendConfigMsg(const char *imagePtr, UINT32 imageLen, void *msgHandle, CmsMsgType msgType);
#ifdef SUPPORT_MOD_SW_UPDATE
static void sendStartModupdtdMsg(void *msgHandle);
#endif
#if 1 //__MTS__, RaynorChung: Support 963268 nand flash, patch form SVN#3597 on http://svn.zyxel.com.tw/svn/CPE_SW1/BCM96368/trunk/P-870HA/branches/cht/fttb8/4.11 
extern int sysGetNVRAMFromFlash(PNVRAM_DATA nvramData);
#endif

/**
 * @return TRUE if the modem's chip id matches that of the image.
 */
UBOOL8 matchChipId(const char *strTagChipId, const char *signature2 __attribute__((unused)))
{
    UINT32 tagChipId = 0;
    UINT32 chipId; 
    UBOOL8 match;

    /* this is the image's chip id */
    tagChipId = (UINT32) strtol(strTagChipId, NULL, 16);
    
    /* get the system's chip id */
    devCtl_getChipId(&chipId);

    if (tagChipId == chipId)
    {
        match = TRUE;
    }
    else
    {
        cmsLog_error("Chip Id error.  Image Chip Id = %04x, Board Chip Id = %04x.", tagChipId, chipId);
        match = FALSE;
    }

    return match;
}

#if 1 //__MSTC__, RaynorChung: Support 963268 nand flash, patch form SVN#3597 on http://svn.zyxel.com.tw/svn/CPE_SW1/BCM96368/trunk/P-870HA/branches/cht/fttb8/4.11 
// verify the tag of the image
CmsRet verifyModelId_MSTC(FILE_TAG *pTag)
{
#if 1//__COMMON__, jchang for model id check
    NVRAM_DATA nvramData;
    char ModelIdPrefix[3]={0};  /* Should be "ZY" Prefix */
    char ModelIdMRD[5]={0};  /* Should be 4 digits */
    char ModelIdTAGIdentity[6]={0};  /* Should be ZyXEL */	
    char ModelIdTAG[5]={0};  /* Should be 4 digits */
    char ModelIdCustomTAG[5]={0};  /* Should be 4 digits */

/* 
Here, we only check files with BCM TAG in the begining i.e. fs_kernel and cfe_fs_kernel.
For w image, we skip the model id check so user can still upgrade bootloader with new model id by w image.
*/
    if (!sysGetNVRAMFromFlash( &nvramData )){
        strncpy(ModelIdPrefix, nvramData.FeatureBits, 2);
        sprintf(ModelIdMRD, "%02x%02x",*((char*)(nvramData.FeatureBits+2)),*((char*)(nvramData.FeatureBits+3)));
        strncpy(ModelIdTAGIdentity, pTag->signiture_1, 4);
        strncpy(ModelIdTAG, pTag->signiture_1+5, 4);
        ModelIdPrefix[2] = '\0';
        ModelIdMRD[4] = '\0';	  
        ModelIdTAGIdentity[4] = '\0';	
        ModelIdTAG[4] = '\0';	   
        /* For Custom Tag, suppose everyone follow the naming rule ZyXEL_xxxx_yyyy */
        memcpy(ModelIdCustomTAG, pTag->signiture_1+10, 4);
        ModelIdCustomTAG[4] = '\0';

        cmsLog_debug(" ModelIdPrefix = %s", ModelIdPrefix);
        cmsLog_debug(" ModelIdMRD = %s",ModelIdMRD);   
        cmsLog_debug(" ModelIdTAGIdentity = %s", ModelIdTAGIdentity);
        cmsLog_debug(" ModelIdTAG = %s",ModelIdTAG);   
        cmsLog_debug(" ModelIdCustomTAG = %s",(0 < strlen(ModelIdCustomTAG))?ModelIdCustomTAG:"N/A");
        
        if(!strcmp(ModelIdPrefix,"MS") && !strcmp(ModelIdTAGIdentity,"MSTC")){
            cmsLog_debug("Finds ZyXEL identity, start checking model ID");
            if(strcmp(ModelIdMRD,ModelIdTAG) && strcmp(ModelIdMRD,ModelIdCustomTAG)){
                cmsLog_error("Check model from TAG is different with MRD");
                return CMSRET_INVALID_MODELID;	   	 
            }else{
                cmsLog_debug("Check modelID successfully");		
            }
        }else{
           cmsLog_error("Not find MSTC identify, just fall through");	
        }
    }else{
       cmsLog_error("Get MRD information fails, just fall through");
    }
#endif

    return CMSRET_SUCCESS;
}

// verify the tag of the image
CmsRet verifyBroadcomFileTag(FILE_TAG *pTag)
{
    UINT32 crc;
    int totalImageSize;
    int tagVer, curVer;
    UINT32 tokenCrc, imageCrc;

        
    cmsLog_debug("start of pTag=%p tagversion %02x %02x %02x %02x", pTag,
                  pTag->tagVersion[0], 
                  pTag->tagVersion[1], 
                  pTag->tagVersion[2], 
                  pTag->tagVersion[3]);

    tokenCrc = *((UINT32 *)pTag->tagValidationToken);
    imageCrc = *((UINT32 *)pTag->imageValidationToken);                  
#ifdef DESKTOP_LINUX
    /* assume desktop is running on little endien intel, but the CRC has been
     * written for big endien mips, so swap before compare.
     */
    tokenCrc = htonl(tokenCrc);
    imageCrc = htonl(imageCrc);
#endif
                  
                  
    // check tag validate token first
    crc = CRC_INITIAL_VALUE;
    crc = cmsCrc_getCrc32((UINT8 *) pTag, (UINT32)TAG_LEN-TOKEN_LEN, crc);
    if (crc != tokenCrc)
    {
        /* this function is called even when we are not sure the image is
         * a broadcom image.  So if crc fails, it is not a big deal.  It just
         * means this is not a broadcom image.
         */
        cmsLog_debug("token crc failed, this is not a valid broadcom image");
        cmsLog_debug("calculated crc=0x%x tokenCrc=0x%x", crc, tokenCrc);
        return CMSRET_INVALID_IMAGE;
    }
    cmsLog_debug("header CRC is OK.");

    
    // check imageCrc
    totalImageSize = atoi((char *) pTag->totalImageLen);
    cmsLog_debug("totalImageLen=%d", totalImageSize);

    crc = CRC_INITIAL_VALUE;
#if 1 // __Verizon__, zongyue
    crc = cmsCrc_getCrc32(((UINT8 *)pTag + TAG_BLOCK_LEN), (UINT32) totalImageSize, crc);
#else
    crc = cmsCrc_getCrc32(((UINT8 *)pTag + TAG_LEN), (UINT32) totalImageSize, crc);
#endif
    if (crc != imageCrc)
    {
        /*
         * This should not happen.  We already passed the crc check on the header,
         * so we should pass the crc check on the image.  If this fails, something
         * is wrong.
         */
        cmsLog_error("image crc failed after broadcom header crc succeeded");
        cmsLog_error("calculated crc=0x%x imageCrc=0x%x totalImageSize=%d", crc, imageCrc, totalImageSize); 
        return CMSRET_INVALID_IMAGE;
    }
    cmsLog_debug("image crc is OK");


    tagVer = atoi((char *) pTag->tagVersion);
    curVer = atoi(BCM_TAG_VER);

    if (tagVer != curVer)

    {
       cmsLog_error("Firmware tag version [%d] is not compatible with the current Tag version [%d]", tagVer, curVer);
       return CMSRET_INVALID_IMAGE;
    }
    cmsLog_debug("tarVer=%d, curVar=%d", tagVer, curVer);

    if (!matchChipId((char *) pTag->chipId, (char *) pTag->signiture_2))
    {
       cmsLog_error("chipid check failed");
       return CMSRET_INVALID_IMAGE;
    }

    cmsLog_debug("Good broadcom image");

    return CMSRET_SUCCESS;
}

#else
// verify the tag of the image
CmsRet verifyBroadcomFileTag(FILE_TAG *pTag, int imageLen)
{
    UINT32 crc;
    int totalImageSize;
    int tagVer, curVer;
    UINT32 tokenCrc, imageCrc;

        
    cmsLog_debug("start of pTag=%p tagversion %02x %02x %02x %02x", pTag,
                  pTag->tagVersion[0], 
                  pTag->tagVersion[1], 
                  pTag->tagVersion[2], 
                  pTag->tagVersion[3]);

    tokenCrc = *((UINT32 *)pTag->tagValidationToken);
    imageCrc = *((UINT32 *)pTag->imageValidationToken);                  
#ifdef DESKTOP_LINUX
    /* assume desktop is running on little endien intel, but the CRC has been
     * written for big endien mips, so swap before compare.
     */
    tokenCrc = htonl(tokenCrc);
    imageCrc = htonl(imageCrc);
#endif
                  
                  
    // check tag validate token first
    crc = CRC_INITIAL_VALUE;
    crc = cmsCrc_getCrc32((UINT8 *) pTag, (UINT32)TAG_LEN-TOKEN_LEN, crc);
    if (crc != tokenCrc)
    {
        /* this function is called even when we are not sure the image is
         * a broadcom image.  So if crc fails, it is not a big deal.  It just
         * means this is not a broadcom image.
         */
        cmsLog_debug("token crc failed, this is not a valid broadcom image");
        cmsLog_debug("calculated crc=0x%x tokenCrc=0x%x", crc, tokenCrc);
        return CMSRET_INVALID_IMAGE;
    }
    cmsLog_debug("header CRC is OK.");

    
    // check imageCrc
    totalImageSize = atoi((char *) pTag->totalImageLen);
    cmsLog_debug("totalImageLen=%d, imageLen=%d, TAG_LEN=%d\n", totalImageSize, imageLen, TAG_LEN);

    if (totalImageSize > (imageLen -TAG_LEN)) {
	 cmsLog_error("invalid size\n");
        return CMSRET_INVALID_IMAGE;
	}
    crc = CRC_INITIAL_VALUE;
    crc = cmsCrc_getCrc32(((UINT8 *)pTag + TAG_LEN), (UINT32) totalImageSize, crc);      
    if (crc != imageCrc)
    {
        /*
         * This should not happen.  We already passed the crc check on the header,
         * so we should pass the crc check on the image.  If this fails, something
         * is wrong.
         */
        cmsLog_error("image crc failed after broadcom header crc succeeded");
        cmsLog_error("calculated crc=0x%x imageCrc=0x%x totalImageSize", crc, imageCrc, totalImageSize); 
        return CMSRET_INVALID_IMAGE;
    }
    cmsLog_debug("image crc is OK");


    tagVer = atoi((char *) pTag->tagVersion);
    curVer = atoi(BCM_TAG_VER);

    if (tagVer != curVer)

    {
       cmsLog_error("Firmware tag version [%d] is not compatible with the current Tag version [%d]", tagVer, curVer);
       return CMSRET_INVALID_IMAGE;
    }
    cmsLog_debug("tarVer=%d, curVar=%d", tagVer, curVer);

    if (!matchChipId((char *) pTag->chipId, (char *) pTag->signiture_2))
    {
       cmsLog_error("chipid check failed");
       return CMSRET_INVALID_IMAGE;
    }

    cmsLog_debug("Good broadcom image");

    return CMSRET_SUCCESS;
}
#endif

// depending on the image type, do the brcm image or whole flash image
#if 1 //__CTLK__, Thief
CmsRet flashImage(char *imagePtr, UINT32 imageLen, CmsImageFormat format, void* msgHandle)
#else
CmsRet flashImage(char *imagePtr, UINT32 imageLen, CmsImageFormat format)
#endif
{
    FILE_TAG *pTag = (FILE_TAG *) imagePtr;
#if 1 //__MTS__, RaynorChung: Support 963268 nand flash, patch form SVN#3597 on http://svn.zyxel.com.tw/svn/CPE_SW1/BCM96368/trunk/P-870HA/branches/cht/fttb8/4.11 
    int cfeSize, rootfsSize, kernelSize;
#else
    int cfeSize, rootfsSize, kernelSize, noReboot;
#endif
    unsigned long cfeAddr, rootfsAddr, kernelAddr;
    CmsRet ret;
#if 1 //__CTLK__, Thief
	int eid = cmsMsg_getHandleEid(msgHandle);
	char handle[BUFLEN_4] = {0};
	FILE_TAG curtagData, tagData1, tagData2;
	struct tm *tm_time;
	time_t cur_time;
	int fd;
	char str[BUFLEN_64] = {0};
	char str_curtime[BUFLEN_24];
	char nextimage = '\0';
	int hour;

	if (format == CMS_IMAGE_FORMAT_BROADCOM || format == CMS_IMAGE_FORMAT_FLASH){
	
		if ( !sysGetTAGFromFlash( &curtagData ) ){
			if ( !sysGetTAGFromFlashByPartition( &tagData1, 1 ) ) {
			    if ( strcmp ( tagData1.imageSequence, curtagData.imageSequence) == 0 ) {
				    nextimage = '2';
				}
			}
		    if ( !sysGetTAGFromFlashByPartition( &tagData2, 2 ) ) {
			    if ( strcmp ( tagData2.imageSequence, curtagData.imageSequence) == 0 ) {
					nextimage = '1';
			    }
			}
		}
		sprintf(str, "%c|", nextimage);

		if(eid == EID_TR69C){
			strcat(str, "ACS|");
			strcpy(handle, "ACS");
		}
		else if (eid == EID_HTTPD || eid == EID_HTTPD_SSL){
			strcat(str, "LAN|");
			strcpy(handle, "LAN");
	    }

		time(&cur_time);
		tm_time = localtime(&cur_time);
		strftime(str_curtime, sizeof(str_curtime), "%m/%d/%Y|%I:%M %p|", tm_time);
        strcat(str, str_curtime);

		strcat(str, pTag->externalversion);
		strcat(str, "|");
	
		if((fd = open("/data/fw_upgrade_history_tmp" , O_CREAT | O_RDWR)) >= 0) {
			write(fd, str, sizeof(str));
			fsync(fd);
			close(fd);
		}
	}
#endif

#if 0 //__MTS__, RaynorChung: Support 963268 nand flash, patch form SVN#3597 on http://svn.zyxel.com.tw/svn/CPE_SW1/BCM96368/trunk/P-870HA/branches/cht/fttb8/4.11 
    if( (format & CMS_IMAGE_FORMAT_PART1) == CMS_IMAGE_FORMAT_PART1 )
    {
        noReboot = ((format & CMS_IMAGE_FORMAT_NO_REBOOT) == 0)
            ? FLASH_PART1_REBOOT : FLASH_PART1_NO_REBOOT;
    }
    else if( (format & CMS_IMAGE_FORMAT_PART2) == CMS_IMAGE_FORMAT_PART2 )
    {
        noReboot = ((format & CMS_IMAGE_FORMAT_NO_REBOOT) == 0)
            ? FLASH_PART2_REBOOT : FLASH_PART2_NO_REBOOT;
    }
    else
        noReboot = ((format & CMS_IMAGE_FORMAT_NO_REBOOT) == 0)
            ? FLASH_PARTDEFAULT_REBOOT : FLASH_PARTDEFAULT_NO_REBOOT;
 
    format &= ~(CMS_IMAGE_FORMAT_NO_REBOOT | CMS_IMAGE_FORMAT_PART1 |
       CMS_IMAGE_FORMAT_PART2); 
#endif

    if (format != CMS_IMAGE_FORMAT_FLASH && format != CMS_IMAGE_FORMAT_BROADCOM)
    {
       cmsLog_error("invalid image format %d", format);
       return CMSRET_INVALID_IMAGE;
    }

    if (format == CMS_IMAGE_FORMAT_FLASH)  
    {
        cmsLog_notice("Flash whole image...");
        // Pass zero for the base address of whole image flash. It will be filled by kernel code
        // was sysFlashImageSet
        ret = devCtl_boardIoctl(BOARD_IOCTL_FLASH_WRITE,
                                BCM_IMAGE_WHOLE,
                                imagePtr,
                                imageLen-TOKEN_LEN,
#if 1 //__CTLK__, Thief
                                0, handle);
#else
                                0, 0);
#endif
        if (ret != CMSRET_SUCCESS)
        {
           cmsLog_error("Failed to flash whole image");
           return CMSRET_IMAGE_FLASH_FAILED;
        }
        else
        {
           return CMSRET_SUCCESS;
        }
    }

    /* this must be a broadcom format image */
    // check imageCrc
    cfeSize = rootfsSize = kernelSize = 0;

    // check cfe's existence
    cfeAddr = (unsigned long) strtoul((char *) pTag->cfeAddress, NULL, 10);
    cfeSize = atoi((char *) pTag->cfeLen);
    // check kernel existence
    kernelAddr = (unsigned long) strtoul((char *) pTag->kernelAddress, NULL, 10);
    kernelSize = atoi((char *) pTag->kernelLen);
    // check root filesystem existence
    rootfsAddr = (unsigned long) strtoul((char *) pTag->rootfsAddress, NULL, 10);
    rootfsSize = atoi((char *) pTag->rootfsLen);
    cmsLog_debug("cfeSize=%d kernelSize=%d rootfsSize=%d", cfeSize, kernelSize, rootfsSize);
    
    if (cfeAddr) 
    {
        printf("Flashing CFE...\n");
        ret = devCtl_boardIoctl(BOARD_IOCTL_FLASH_WRITE,
                                BCM_IMAGE_CFE,
#if 1 //__MTS__, RaynorChung: Support 963268 nand flash, patch form SVN#3597 on http://svn.zyxel.com.tw/svn/CPE_SW1/BCM96368/trunk/P-870HA/branches/cht/fttb8/4.11 
                                imagePtr+TAG_BLOCK_LEN,
#else
                                imagePtr+TAG_LEN,
#endif
                                cfeSize,
#if 1 //__CTLK__, Thief
                                (int) cfeAddr, handle);
#else
                                (int) cfeAddr, 0);
#endif
        if (ret != CMSRET_SUCCESS)
        {
            cmsLog_error("Failed to flash CFE");
            return CMSRET_IMAGE_FLASH_FAILED;
        }
    }
#if (INC_NAND_FLASH_DRIVER==1)    //__MTS__, RaynorChung: Support 963268 nand flash, patch form SVN#3597 on http://svn.zyxel.com.tw/svn/CPE_SW1/BCM96368/trunk/P-870HA/branches/cht/fttb8/4.11 
    if (rootfsAddr) 
#else
    if (rootfsAddr && kernelAddr) 
#endif
    {
        char *tagFs = imagePtr;

        // tag is alway at the sector start of fs
        if (cfeAddr)
        {
            tagFs = imagePtr + cfeSize;       // will trash cfe memory, but cfe is already flashed
#if 1 //__MTS__, RaynorChung: Support 963268 nand flash, patch form SVN#3597 on http://svn.zyxel.com.tw/svn/CPE_SW1/BCM96368/trunk/P-870HA/branches/cht/fttb8/4.11 
            memcpy(tagFs, imagePtr, TAG_BLOCK_LEN);
#else
            memcpy(tagFs, imagePtr, TAG_LEN);
#endif
        }

        printf("Flashing root file system and kernel...\n");
        /* only the buf pointer and length is needed, the offset parameter
         * was present in the legacy code, but is not used. */
        ret = devCtl_boardIoctl(BOARD_IOCTL_FLASH_WRITE,
                                BCM_IMAGE_FS,
                                tagFs,
#if 1 //__MTS__, RaynorChung: Support 963268 nand flash, patch form SVN#3597 on http://svn.zyxel.com.tw/svn/CPE_SW1/BCM96368/trunk/P-870HA/branches/cht/fttb8/4.11 
                                TAG_BLOCK_LEN+rootfsSize+kernelSize,
#if 1 //__CTLK__, Thief
                                0, handle); 
#else
                                0, 0); 
#endif
#else
                                TAG_LEN+rootfsSize+kernelSize,
                                noReboot, 0);
#endif
        if (ret != CMSRET_SUCCESS)
        {
            cmsLog_error("Failed to flash root file system and kernel");
            return CMSRET_IMAGE_FLASH_FAILED;
        }
    }

    cmsLog_notice("Image flash done.");
    
    return CMSRET_SUCCESS;
}

#if 1 //__MTS__, RaynorChung: Support 963268 nand flash, patch form SVN#3597 on http://svn.zyxel.com.tw/svn/CPE_SW1/BCM96368/trunk/P-870HA/branches/cht/fttb8/4.11 
UINT32 cmsImg_getImageFlashSizeLimit(void)
{   
   return FLASH_IMAGE_DOWNLOAD_SIZE;
}
#endif

UINT32 cmsImg_getImageFlashSize(void)
{
   UINT32 flashSize=0;
   CmsRet ret;
   
   ret = devCtl_boardIoctl(BOARD_IOCTL_FLASH_READ,
                           FLASH_SIZE,
                           0, 0, 0, &flashSize);
   if (ret != CMSRET_SUCCESS)
   {
      cmsLog_error("Could not get flash size, return 0");
      flashSize = 0;
   }
   
   return flashSize;
}


UINT32 cmsImg_getBroadcomImageTagSize(void)
{
   return TOKEN_LEN;
}


UINT32 cmsImg_getConfigFlashSize(void)
{
   UINT32 realSize;

   realSize = cmsImg_getRealConfigFlashSize();

#ifdef COMPRESSED_CONFIG_FILE   
   /*
    * A multiplier of 2 is now too small for some of the big voice and WLAN configs,   
    * so allow for the possibility of 4x compression ratio.  In a recent test on the
    * 6368 with wireless enabled, I got a compression ratio of 3.5.
    * The real test comes in management.c: after we do the
    * compression, writeValidatedConfigBuf will verify that the compressed buffer can
    * fit into the flash.
    * A 4x multiplier should be OK for small memory systems such as the 6338.
    * The kernel does not allocate physical pages until they are touched.
    * However, allocating an overly large buffer could be rejected immediately by the
    * kernel because it does not know we don't actually plan to use the entire buffer.
    * So if this is a problem on the 6338, we could improve this algorithm to
    * use a smaller factor on low memory systems.
    */
   realSize = realSize * 4;
#endif

   return realSize;
}


UINT32 cmsImg_getRealConfigFlashSize(void)
{
   CmsRet ret;
   UINT32 size=0;

   ret = devCtl_boardIoctl(BOARD_IOCTL_GET_PSI_SIZE, 0, NULL, 0, 0, (void *)&size);
   if (ret != CMSRET_SUCCESS)
   {
      cmsLog_error("boardIoctl to get config flash size failed.");
      size = 0;
   }

   return size;
}


UBOOL8 cmsImg_willFitInFlash(UINT32 imageSize)
{
   UINT32 flashSize;
   
   flashSize = cmsImg_getImageFlashSize();

   cmsLog_debug("flash size is %u bytes, imageSize=%u bytes", flashSize, imageSize);
                     
   return (flashSize > (imageSize + CMS_IMAGE_OVERHEAD));
}


UBOOL8 cmsImg_isBackupConfigFlashAvailable(void)
{
   static UBOOL8 firstTime=TRUE;
   static UBOOL8 avail=FALSE;
   UINT32 size=0;
   CmsRet ret;

   if (firstTime)
   {
      ret = devCtl_boardIoctl(BOARD_IOCTL_GET_BACKUP_PSI_SIZE, 0, NULL, 0, 0, (void *)&size);
      if (ret == CMSRET_SUCCESS && size > 0)
      {
         avail = TRUE;
      }

      firstTime = FALSE;
   }

   return avail;
}


UBOOL8 cmsImg_isConfigFileLikely(const char *buf)
{
   const char *header = "<?xml version";
   const char *dslCpeConfig = "<DslCpeConfig";
   UINT32 len, i;
   UBOOL8 likely=FALSE;
   
   if (strncmp(buf, "<?xml version", strlen(header)) == 0)
   {
      len = strlen(dslCpeConfig);
      for (i=20; i<50 && !likely; i++)
      {
         if (strncmp(&(buf[i]), dslCpeConfig, len) == 0)
         {
            likely = TRUE;
         }
      }
   }

//__MTS__, Paul Ho, 2011.06.24, to compress configuration file
#if defined(COMPRESSED_CONFIG_FILE) && defined(SUPPORT_MTS_COMPRESS_CONF_FILE)
   else if ((!strncmp(buf, COMPRESSED_CONFIG_HEADER, strlen(COMPRESSED_CONFIG_HEADER))))
   {
      likely = TRUE;
   }
#endif
   
   cmsLog_debug("returning likely=%d", likely);
   
   return likely;
}


/** General entry point for writing the image.
 *  The image can be a flash image or a config file.
 *  This function will first determine the image type, which has the side
 *  effect of validating it.
 */
CmsRet cmsImg_writeImage(char *imagePtr, UINT32 imageLen, void *msgHandle)
{
   CmsImageFormat format;
   CmsRet ret;

   if ((format = cmsImg_validateImage(imagePtr, imageLen, msgHandle)) == CMS_IMAGE_FORMAT_INVALID)
   {
      ret = CMSRET_INVALID_IMAGE;
   }
   else
   {
      ret = cmsImg_writeValidatedImage(imagePtr, imageLen, format, msgHandle);
   }

   return ret;
}


CmsRet cmsImg_writeValidatedImage(char *imagePtr, UINT32 imageLen, CmsImageFormat format, void *msgHandle)
{
   CmsRet ret=CMSRET_SUCCESS;
   
   switch(format & ~(CMS_IMAGE_FORMAT_NO_REBOOT | CMS_IMAGE_FORMAT_PART1 | CMS_IMAGE_FORMAT_PART2))
   {
   case CMS_IMAGE_FORMAT_BROADCOM:
   case CMS_IMAGE_FORMAT_FLASH:
      // BcmNtwk_unInit(); mwang_todo: is it important to let Wireless do some
      // uninit before we write the flash image?
#if 1 //__CTLK__, Thief
      ret = flashImage(imagePtr, imageLen, format, msgHandle);
#else
      ret = flashImage(imagePtr, imageLen, format);
#endif
      break;
      
   case CMS_IMAGE_FORMAT_XML_CFG:
      ret = sendConfigMsg(imagePtr, imageLen, msgHandle, CMS_MSG_WRITE_CONFIG_FILE);
      if (ret == CMSRET_SUCCESS)
      {
         /*
          * Emulate the behavior of the driver when a flash image is written.
          * When we write a config image, also request immediate reboot
          * because we don't want to let any other app save the config file
          * to flash, thus wiping out what we just written.
          */
         cmsLog_debug("config file download written, request reboot");
         cmsUtil_sendRequestRebootMsg(msgHandle);
      }
      break;
      
#ifdef SUPPORT_MOD_SW_UPDATE
   case CMS_IMAGE_FORMAT_MODSW_PKG:
   {
      char filename[BUFLEN_1024]={0};

      cmsFil_removeDir(SW_UPDATE_DATA_DIR);
      cmsFil_makeDir(SW_UPDATE_DATA_DIR);

      snprintf(filename, sizeof(filename)-1, "%s/%s",
                         SW_UPDATE_DATA_DIR, SW_UPDATE_PKG_FILE);
      cmsFil_writeBufferToFile(filename, (UINT8 *)imagePtr, imageLen);
      /*
       * modupdtd will unpack the sw package, and then send a graceful
       * shutdown msg to smd.
       */
      sendStartModupdtdMsg(msgHandle);
      break;
   }
#endif

   default:
       cmsLog_error("Unrecognized image format=%d", format);
       ret = CMSRET_INVALID_IMAGE;
       break;
    }
   
   return ret;
}

 
CmsImageFormat cmsImg_validateImage(const char *imageBuf, UINT32 imageLen, void *msgHandle)
{
   CmsImageFormat result = CMS_IMAGE_FORMAT_INVALID;
   CmsRet ret;
   
   if (imageBuf == NULL)
   {
      return CMS_IMAGE_FORMAT_INVALID;
   }
   
   if (imageLen > CMS_CONFIG_FILE_DETECTION_LENGTH &&
       cmsImg_isConfigFileLikely(imageBuf))
   {
      cmsLog_debug("possible CMS XML config file format detected");
      ret = sendConfigMsg(imageBuf, imageLen, msgHandle, CMS_MSG_VALIDATE_CONFIG_FILE);
      if (ret == CMSRET_SUCCESS)
      { 
         cmsLog_debug("CMS XML config format verified.");
         return CMS_IMAGE_FORMAT_XML_CFG;
      }
   } 
   
   cmsLog_debug("not a config file");
   
#ifdef SUPPORT_MOD_SW_UPDATE
   if (cmsImg_isModSwPkg((unsigned char *) imageBuf, imageLen))
   {
      cmsLog_debug("detected modular sw pkg format!");
      return CMS_IMAGE_FORMAT_MODSW_PKG;
   }

   cmsLog_debug("not a modular sw pkg");
#endif

#if 1 //__MTS__, RaynorChung: Support 963268 nand flash, patch form SVN#3597 on http://svn.zyxel.com.tw/svn/CPE_SW1/BCM96368/trunk/P-870HA/branches/cht/fttb8/4.11 
   if ((imageLen > sizeof(FILE_TAG)) 
        && (verifyBroadcomFileTag((FILE_TAG *) imageBuf) == CMSRET_SUCCESS))
#else
   if ((imageLen > sizeof(FILE_TAG)) 
        && (verifyBroadcomFileTag((FILE_TAG *) imageBuf, imageLen) == CMSRET_SUCCESS))
#endif
   {
      UINT32 maxLen;
      
      /* Found a valid Broadcom defined TAG record at the beginning of the image */
      cmsLog_debug("Broadcom format verified.");
      maxLen = cmsImg_getImageFlashSize() + cmsImg_getBroadcomImageTagSize();
      if (imageLen > maxLen)
      {
         cmsLog_error("broadcom image is too large for flash, got %u, max %u", imageLen, maxLen);
      }
      else
      {
         result = CMS_IMAGE_FORMAT_BROADCOM;
      }
   }
   else
   {
      /* if It is not a Broadcom flash format file.  Now check if it is a
       * flash image format file.  A flash image format file must have a
       * CRC at the end of the image.
       */
      UINT32 crc = CRC_INITIAL_VALUE;
      UINT32 imageCrc;
      UINT8 *crcPtr;
      
      if (imageLen > TOKEN_LEN)
      {
         crcPtr = (UINT8 *) (imageBuf + (imageLen - TOKEN_LEN));
         /*
          * CRC may not be word aligned, so extract the bytes out one-by-one.
          * Whole image CRC is calculated, then htonl, then written out using
          * fwrite (see addvtoken.c in hostTools).  Because of the way we are
          * extracting the CRC here, we don't have to swap for endieness when
          * doing compares on desktop Linux and modem (?).
          */
         imageCrc = (crcPtr[0] << 24) | (crcPtr[1] << 16) | (crcPtr[2] << 8) | crcPtr[3];
      
         crc = cmsCrc_getCrc32((unsigned char *) imageBuf, imageLen - TOKEN_LEN, crc);      
         if (crc == imageCrc)
         {
            UINT32 maxLen;
          
            cmsLog_debug("Whole flash image format [%ld bytes] verified.", imageLen);
            maxLen = cmsImg_getImageFlashSize();
            if (imageLen > maxLen)
            {
               cmsLog_error("whole image is too large for flash, got %u, max %u", imageLen, maxLen);
            }
            else
            {
               result = CMS_IMAGE_FORMAT_FLASH;
            }
         }
         else
         {
#if defined(EPON_SDK_BUILD)
            cmsLog_debug("Could not determine image format [%d bytes]", imageLen);
#else
            cmsLog_error("Could not determine image format [%d bytes]", imageLen);
#endif
            cmsLog_debug("calculated crc=0x%x image crc=0x%x", crc, imageCrc);
         }
      }
   }

#if 1 //__MSTC__, RaynorChung: Support 963268 nand flash, patch form SVN#3597 on http://svn.zyxel.com.tw/svn/CPE_SW1/BCM96368/trunk/P-870HA/branches/cht/fttb8/4.11 
   if ( result == CMS_IMAGE_FORMAT_BROADCOM ) {
      if ( verifyModelId_MSTC((FILE_TAG *) imageBuf) != CMSRET_SUCCESS ) {
	     result = CMS_IMAGE_MODELID_INVALID;
      }
   }
#endif
   cmsLog_debug("returning image format %d", result);

   return result;
}

CmsRet sendConfigMsg(const char *imagePtr, UINT32 imageLen, void *msgHandle, CmsMsgType msgType)
{
   char *buf=NULL;
   char *body=NULL;
   CmsMsgHeader *msg;
   CmsRet ret;

   if ((buf = cmsMem_alloc(sizeof(CmsMsgHeader) + imageLen, ALLOC_ZEROIZE)) == NULL)
   {
      cmsLog_error("failed to allocate %d bytes for msg 0x%x", 
                   sizeof(CmsMsgHeader) + imageLen, msgType);
      return CMSRET_RESOURCE_EXCEEDED;
   }
   
   msg = (CmsMsgHeader *) buf;
   body = (char *) (msg + 1);
    
   msg->type = msgType;
   msg->src = cmsMsg_getHandleEid(msgHandle);
   msg->dst = EID_SMD;
   msg->flags_request = 1;
   msg->dataLength = imageLen;
   
   memcpy(body, imagePtr, imageLen);

   ret = cmsMsg_sendAndGetReply(msgHandle, msg);
   
   cmsMem_free(buf);
   
   return ret;
}


void cmsImg_sendLoadStartingMsg(void *msgHandle, const char *connIfName)
{
   CmsMsgHeader *msg;
   char *data;
   void *msgBuf;
   UINT32 msgDataLen=0;

   /* for the msg and the connIfName */
   if (connIfName)
   {
      msgDataLen = strlen(connIfName) + 1;
      msgBuf = cmsMem_alloc(sizeof(CmsMsgHeader) + msgDataLen, ALLOC_ZEROIZE);
   } 
   else
   {
      cmsLog_error("msg without connIfName");
      msgBuf = cmsMem_alloc(sizeof(CmsMsgHeader), ALLOC_ZEROIZE);
   }
   
   msg = (CmsMsgHeader *)msgBuf;
   msg->src = cmsMsg_getHandleEid(msgHandle);
   msg->dst = EID_SMD;
   msg->flags_request = 1;
   msg->type = CMS_MSG_LOAD_IMAGE_STARTING;

   if (connIfName)
   {
      data = (char *) (msg + 1);
      msg->dataLength = msgDataLen;
      memcpy(data, (char *)connIfName, msgDataLen);      
   }
   
   cmsLog_debug("connIfName=%s", connIfName);

   cmsMsg_sendAndGetReply(msgHandle, msg);

   CMSMEM_FREE_BUF_AND_NULL_PTR(msgBuf);

}


void cmsImg_sendLoadDoneMsg(void *msgHandle)
{
   CmsMsgHeader msg = EMPTY_MSG_HEADER;

   msg.type = CMS_MSG_LOAD_IMAGE_DONE;
   msg.src = cmsMsg_getHandleEid(msgHandle);
   msg.dst = EID_SMD;
   msg.flags_request = 1;
   
   cmsMsg_sendAndGetReply(msgHandle, &msg);
}


#ifdef SUPPORT_MOD_SW_UPDATE
void sendStartModupdtdMsg(void *msgHandle)
{
   CmsMsgHeader msg = EMPTY_MSG_HEADER;

   msg.type = CMS_MSG_START_APP;
   msg.src = cmsMsg_getHandleEid(msgHandle);
   msg.dst = EID_SMD;
   msg.wordData = EID_MODUPDTD;
   msg.flags_request = 1;

   cmsMsg_sendAndGetReply(msgHandle, &msg);
}
#endif


/* using a cmsUtil_ prefix because this can be used in non-upload scenarios */
void cmsUtil_sendRequestRebootMsg(void *msgHandle)
{
   CmsMsgHeader msg = EMPTY_MSG_HEADER;

   msg.type = CMS_MSG_REBOOT_SYSTEM;
   msg.src = cmsMsg_getHandleEid(msgHandle);
   msg.dst = EID_SMD;
   msg.flags_request = 1;

   cmsMsg_sendAndGetReply(msgHandle, &msg);
}


CmsRet cmsImg_saveIfNameFromSocket(SINT32 socketfd, char *connIfName)
{
  
   SINT32 i = 0;
   SINT32 fd = 0;
   SINT32 numifs = 0;
   UINT32 bufsize = 0;
   struct ifreq *all_ifr = NULL;
   struct ifconf ifc;
#if 1 // __CTL__, KuanJung
   struct sockaddr_storage local_addr;
   socklen_t local_len = sizeof(local_addr);
   char ipAddrBuf[BUFLEN_40]= {0};
   
   if (socketfd < 0 || connIfName == NULL)
   {
      cmsLog_error("cmsImg_saveIfNameFromSocket: Invalid parameters: socket=%d, connIfName=%s", socketfd, connIfName);
      return CMSRET_INTERNAL_ERROR;
   }
   memset(&ifc, 0, sizeof(struct ifconf));
   memset(&local_addr, 0, sizeof(local_addr));
   
   if (getsockname(socketfd, &local_addr,&local_len) < 0) 
   {
      cmsLog_error("cmsImg_saveIfNameFromSocket: Error in getsockname.");
      return CMSRET_INTERNAL_ERROR;
   }

   // cmsLog_error("cmsImg_saveIfNameFromSocket: Session comes from: %s", inet_ntop(AF_INET6, ((struct sockaddr_in6 *)&local_addr)->sin6_addr));
   if (local_addr.ss_family == AF_INET)
   {
	inet_ntop(local_addr.ss_family, &(((struct sockaddr_in *)&local_addr)->sin_addr), ipAddrBuf, sizeof(ipAddrBuf));
   }
   else
   	inet_ntop(local_addr.ss_family, &(((struct sockaddr_in6 *)&local_addr)->sin6_addr), ipAddrBuf, sizeof(ipAddrBuf));

   cmsLog_debug("cmsImg_saveIfNameFromSocket: Session comes from: %s , local_addr.ss_family %d", ipAddrBuf, local_addr.ss_family);
   if (local_addr.ss_family == AF_INET || (strchr(ipAddrBuf, '.') && strstr(ipAddrBuf, ":ffff:")))
   {
       /* IPv4 client */
       char *v4addr;

	   if (local_addr.ss_family == AF_INET)
	   	v4addr = ipAddrBuf;
	   else
	   {
       	/* convert address to clean ipv4 address */
       	v4addr = strrchr(ipAddrBuf, ':') + 1;
	   }
       
	   if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
	   {
	      cmsLog_error("cmsImg_saveIfNameFromSocket: Error openning socket when getting socket intface info");
	      return CMSRET_INTERNAL_ERROR;
	   }
	   
	   numifs = 16;

	   bufsize = numifs*sizeof(struct ifreq);
	   all_ifr = (struct ifreq *)cmsMem_alloc(bufsize, ALLOC_ZEROIZE);
	   if (all_ifr == NULL) 
	   {
	      cmsLog_error("cmsImg_saveIfNameFromSocket: out of memory");
	      close(fd);
	      return CMSRET_INTERNAL_ERROR;
	   }

	   ifc.ifc_len = bufsize;
	   ifc.ifc_buf = (char *)all_ifr;
	   if (ioctl(fd, SIOCGIFCONF, &ifc) < 0) 
	   {
	      cmsLog_error("cmsImg_saveIfNameFromSocket: Error getting interfaces\n");
	      close(fd);
	      cmsMem_free(all_ifr);
	      return CMSRET_INTERNAL_ERROR;
	   }

	   numifs = ifc.ifc_len/sizeof(struct ifreq);
	   // cmsLog_error("cmsImg_saveIfNameFromSocket: numifs=%d\n",numifs); 
	   for (i = 0; i < numifs; i ++) 
	   {
		  char *localip;
		  localip = inet_ntoa(((struct sockaddr_in *)&(all_ifr[i].ifr_addr))->sin_addr);
		  if (!cmsUtl_strcmp(localip, v4addr))
		  {
		      strcpy(connIfName, all_ifr[i].ifr_name);
		  	break;
		  }
	   }

	   close(fd);
	   cmsMem_free(all_ifr);
   }
   else
   {
#ifdef DMP_X_404A03_IPV6_1 /* aka SUPPORT_IPV6 */
      /* delete the link local address of the interface */
      UINT32 addrIdx = 0;
      UINT32 ifIndex;
      UINT32 prefixLen = 0;
      UINT32 scope     = 0;
      UINT32 ifaFlags  = 0;
      char   ipAddr[BUFLEN_40];

      while (cmsNet_getIfAddr6("br0", addrIdx,
                               ipAddr, &ifIndex, &prefixLen, &scope, &ifaFlags) == CMSRET_SUCCESS)
      {
      	
         if (cmsNet_areIp6AddrEqual(ipAddr, ipAddrBuf) == 0)
         {
         	strcpy(connIfName, "br0");
            break;
         }
         else
         {
            /* get the next ip address of the interface */
            addrIdx++;
         }
      }
#endif
   }

#else

   struct sockaddr local_addr;
   socklen_t local_len = sizeof(struct sockaddr_in);

   if (socketfd < 0 || connIfName == NULL)
   {
      cmsLog_error("cmsImg_saveIfNameFromSocket: Invalid parameters: socket=%d, connIfName=%s", socketfd, connIfName);
      return CMSRET_INTERNAL_ERROR;
   }
   memset(&ifc, 0, sizeof(struct ifconf));
   memset(&local_addr, 0, sizeof(struct sockaddr));
   
   if (getsockname(socketfd, &local_addr,&local_len) < 0) 
   {
      cmsLog_error("cmsImg_saveIfNameFromSocket: Error in getsockname.");
      return CMSRET_INTERNAL_ERROR;
   }

   /* cmsLog_error("cmsImg_saveIfNameFromSocket: Session comes from: %s", inet_ntoa(((struct sockaddr_in *)&local_addr)->sin_addr)); */
   
   if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
   {
      cmsLog_error("cmsImg_saveIfNameFromSocket: Error openning socket when getting socket intface info");
      return CMSRET_INTERNAL_ERROR;
   }
   
   numifs = 16;

   bufsize = numifs*sizeof(struct ifreq);
   all_ifr = (struct ifreq *)cmsMem_alloc(bufsize, ALLOC_ZEROIZE);
   if (all_ifr == NULL) 
   {
      cmsLog_error("cmsImg_saveIfNameFromSocket: out of memory");
      close(fd);
      return CMSRET_INTERNAL_ERROR;
   }

   ifc.ifc_len = bufsize;
   ifc.ifc_buf = (char *)all_ifr;
   if (ioctl(fd, SIOCGIFCONF, &ifc) < 0) 
   {
      cmsLog_error("cmsImg_saveIfNameFromSocket: Error getting interfaces\n");
      close(fd);
      cmsMem_free(all_ifr);
      return CMSRET_INTERNAL_ERROR;
   }

   numifs = ifc.ifc_len/sizeof(struct ifreq);
   /* cmsLog_error("cmsImg_saveIfNameFromSocket: numifs=%d\n",numifs); */
   for (i = 0; i < numifs; i ++) 
   {
	  /* cmsLog_error("cmsImg_saveIfNameFromSocket: intface name=%s\n", all_ifr[i].ifr_name); */
	  struct in_addr addr1,addr2;
	  addr1 = ((struct sockaddr_in *)&(local_addr))->sin_addr;
	  addr2 = ((struct sockaddr_in *)&(all_ifr[i].ifr_addr))->sin_addr;
	  if (addr1.s_addr == addr2.s_addr) 
	  {
	      strcpy(connIfName, all_ifr[i].ifr_name);
	  	break;
	  }
   }

   close(fd);
   cmsMem_free(all_ifr);

#endif
   cmsLog_debug("connIfName=%s", connIfName);

   return CMSRET_SUCCESS;
   
}

