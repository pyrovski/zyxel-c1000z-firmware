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

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>

#include "cms.h"
#include "cms_util.h"
#include "../oal.h"

UBOOL8 oalFil_isFilePresent(const char *filename)
{
   struct stat statbuf;
   SINT32 rc;

   rc = stat(filename, &statbuf);

   if (rc == 0)
   {
      return TRUE;
   }
   else
   {
      return FALSE;
   }
}


SINT32 oalFil_getSize(const char *filename)
{
   struct stat statbuf;
   SINT32 rc;

   rc = stat(filename, &statbuf);

   if (rc == 0)
   {
      return ((SINT32) statbuf.st_size);
   }
   else
   {
      return -1;
   }
}


CmsRet oalFil_copyToBuffer(const char *filename, UINT8 *buf, UINT32 *bufSize)
{
   SINT32 actualFileSize;
   SINT32 fd, rc;

   actualFileSize = oalFil_getSize(filename);

   if (actualFileSize < 0) // Added by KJ
   {
      cmsLog_error("oalFil_getSize failed");
      return CMSRET_INTERNAL_ERROR;
   }

   if (*bufSize < (UINT32) actualFileSize)
   {
      cmsLog_error("user supplied buffer is %d, filename actual size is %d", *bufSize, actualFileSize);
      return CMSRET_RESOURCE_EXCEEDED;
   }

   *bufSize = 0;
       
   fd = open(filename, 0);
   if (fd < 0)
   {
      cmsLog_error("could not open file %s, errno=%d", filename, errno);
      return CMSRET_INTERNAL_ERROR;
   }

   rc = read(fd, buf, actualFileSize);
   if (rc != actualFileSize)
   {
      cmsLog_error("read error, got %d, expected %d", rc, actualFileSize);
      close(fd);
      return CMSRET_INTERNAL_ERROR;
   }

   close(fd);

   /* let user know how many bytes was actually copied */
   *bufSize = (UINT32) actualFileSize;
   return CMSRET_SUCCESS;
}


CmsRet oalFil_writeToProc(const char *procFilename, const char *s)
{
   CmsRet ret=CMSRET_SUCCESS;

#ifdef DESKTOP_LINUX

   cmsLog_debug("writing %s to %s", s, procFilename);

#else

   /* on the modem */
   SINT32 fd, rc;

   if ((fd = open(procFilename, O_RDWR)) < 0)
   {
      cmsLog_error("could not open %s", procFilename);
      return CMSRET_INTERNAL_ERROR;
   }

   rc = write(fd, s, strlen(s));

   if (rc < (SINT32) strlen(s))
   {
      cmsLog_error("write %s to %s failed, rc=%d", s, procFilename, rc);
      ret = CMSRET_INTERNAL_ERROR;
   }

   close(fd);

#endif  /* DESKTOP_LINUX */

   return ret;
}


CmsRet oalFil_writeBufferToFile(const char *filename, const UINT8 *buf, UINT32 bufLen)
{
   CmsRet ret=CMSRET_SUCCESS;
   SINT32 fd, rc;

   if ((fd = open(filename, O_RDWR|O_CREAT|O_TRUNC, S_IRWXU)) < 0)
   {
      cmsLog_error("could not open %s", filename);
      return CMSRET_INTERNAL_ERROR;
   }

   rc = write(fd, buf, bufLen);

   if (rc < (SINT32) bufLen)
   {
      cmsLog_error("write to %s failed, rc=%d", filename, rc);
      ret = CMSRET_INTERNAL_ERROR;
   }

   close(fd);

   return ret;
}


CmsRet oalFil_removeDir(const char *dirname)
{
   DIR *d;
   struct dirent *dent;

   cmsLog_debug("dirname=%s", dirname);
   /*
    * Remove all non-directories in this dir.
    * Recurse into any sub-dirs and remove them.
    */
   d = opendir(dirname);
   if (NULL == d)
   {
      /* dir must not exist, no need to remove */
      return CMSRET_SUCCESS;
   }

   while (NULL != (dent = readdir(d)))
   {
      char path[BUFLEN_1024]={0};

      if (!strcmp(dent->d_name, ".") || !strcmp(dent->d_name, ".."))
         continue;

      snprintf(path, sizeof(path)-1, "%s/%s", dirname, dent->d_name);
      if (DT_DIR == dent->d_type)
      {
         oalFil_removeDir(path);
      }
      else
      {
         unlink(path);
      }
   }

   closedir(d);

   if (0 != rmdir(dirname))
   {
      cmsLog_error("rmdir of %s failed!", dirname);
   }

   return CMSRET_SUCCESS;
}


CmsRet oalFil_makeDir(const char *dirname)
{

   if (0 != mkdir(dirname, S_IRWXU))
   {
      cmsLog_error("mkdir on %s failed", dirname);
      return CMSRET_INTERNAL_ERROR;
   }

   return CMSRET_SUCCESS;
}
