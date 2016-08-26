/***********************************************************************
 *
 *  Copyright (c) 2006-2007  Broadcom Corporation
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

#include "cms.h"
#include "cms_util.h"
#include "cms_boardioctl.h"

#if 1 //__CTL__, KuanJung
#include "boardparms.h"
#endif

/*
 * See:
 * bcmdrivers/opensource/include/bcm963xx/board.h
 * bcmdrivers/opensource/char/board/bcm963xx/impl1/board.c and bcm63xx_led.c
 */

void cmsLed_setWanConnected(void)
{
   devCtl_boardIoctl(BOARD_IOCTL_LED_CTRL, 0, NULL, kLedWanData, kLedStateOn, NULL);
}


void cmsLed_setWanDisconnected(void)
{
   devCtl_boardIoctl(BOARD_IOCTL_LED_CTRL, 0, NULL, kLedWanData, kLedStateOff, NULL);
}


void cmsLed_setWanFailed(void)
{
   devCtl_boardIoctl(BOARD_IOCTL_LED_CTRL, 0, NULL, kLedWanData, kLedStateFail, NULL);
}

#if 1 // __CenturyLink__, Hong-Yu
void cmsLed_setInternetLed(char *color)
{
   if (strcmp(color, "red") == 0)
   {
	  devCtl_boardIoctl(BOARD_IOCTL_LED_CTRL, PERSISTENT, "", kLedInetR, kLedStateOn, "");
	  devCtl_boardIoctl(BOARD_IOCTL_SET_GPIO, PERSISTENT, "", BP_GPIO_39_AH, kLedStateOff, "");
	  devCtl_boardIoctl(BOARD_IOCTL_LED_CTRL, PERSISTENT, "", kLedWanData, kLedStateOn, "");
   }
   else if (strcmp(color, "green") == 0)
   {
	  devCtl_boardIoctl(BOARD_IOCTL_LED_CTRL, PERSISTENT, "", kLedInetR, kLedStateOff, "");
      devCtl_boardIoctl(BOARD_IOCTL_LED_CTRL, PERSISTENT, "", kLedWanData, kLedStateBlinkByPackets, "");
	  devCtl_boardIoctl(BOARD_IOCTL_SET_GPIO, PERSISTENT, "", BP_GPIO_39_AH, kLedStateOn, "");
   }
   else if (strcmp(color, "amber-blink-with-packet") == 0)
   {
	  devCtl_boardIoctl(BOARD_IOCTL_LED_CTRL, PERSISTENT, "", kLedWanData, kLedStateBlinkByPackets, "");
	  devCtl_boardIoctl(BOARD_IOCTL_SET_GPIO, PERSISTENT, "", BP_GPIO_39_AH, kLedStateOn, "");
      devCtl_boardIoctl(BOARD_IOCTL_LED_CTRL, PERSISTENT, "", kLedInetR, kLedStateBlinkByPackets, "");
   }
   else if (strcmp(color, "amber") == 0)
   {
	  devCtl_boardIoctl(BOARD_IOCTL_LED_CTRL, PERSISTENT, "", kLedWanData, kLedStateOn, "");
	  devCtl_boardIoctl(BOARD_IOCTL_SET_GPIO, PERSISTENT, "", BP_GPIO_39_AH, kLedStateOn, "");
      devCtl_boardIoctl(BOARD_IOCTL_LED_CTRL, PERSISTENT, "", kLedInetR, kLedStateOn, "");
   }
   else if (strcmp(color, "off") == 0)
   {
	  devCtl_boardIoctl(BOARD_IOCTL_SET_GPIO, PERSISTENT, "", BP_GPIO_39_AH, kLedStateOff, "");
	  devCtl_boardIoctl(BOARD_IOCTL_LED_CTRL, PERSISTENT, "", kLedInetR, kLedStateOff, "");
	  devCtl_boardIoctl(BOARD_IOCTL_LED_CTRL, PERSISTENT, "", kLedWanData, kLedStateOn, "");
   }
   else if (strcmp(color, "red-blink") == 0)
   {
	  devCtl_boardIoctl(BOARD_IOCTL_LED_CTRL, PERSISTENT, "", kLedInetR, kLedStateOn, "");
	  devCtl_boardIoctl(BOARD_IOCTL_SET_GPIO, PERSISTENT, "", BP_GPIO_39_AH, kLedStateOff, "");
	  devCtl_boardIoctl(BOARD_IOCTL_LED_CTRL, PERSISTENT, "", kLedWanData, kLedStateLongBlinkContinues, "");
   }
   else if (strcmp(color, "green-blink") == 0)
   {
	  devCtl_boardIoctl(BOARD_IOCTL_LED_CTRL, PERSISTENT, "", kLedInetR, kLedStateOff, "");
	  devCtl_boardIoctl(BOARD_IOCTL_SET_GPIO, PERSISTENT, "", BP_GPIO_39_AH, kLedStateOn, "");
      devCtl_boardIoctl(BOARD_IOCTL_LED_CTRL, PERSISTENT, "", kLedWanData, kLedStateLongBlinkContinues, "");
   }
   else if (strcmp(color, "amber-blink") == 0)
   {
	  devCtl_boardIoctl(BOARD_IOCTL_LED_CTRL, PERSISTENT, "", kLedInetR, kLedStateOn, "");
	  devCtl_boardIoctl(BOARD_IOCTL_SET_GPIO, PERSISTENT, "", BP_GPIO_39_AH, kLedStateOn, "");
      devCtl_boardIoctl(BOARD_IOCTL_LED_CTRL, PERSISTENT, "", kLedWanData, kLedStateLongBlinkContinues, "");
   }
   else if (strcmp(color, "red-green-alternate") == 0)
   {
	  devCtl_boardIoctl(BOARD_IOCTL_SET_GPIO, PERSISTENT, "", BP_GPIO_39_AH, kLedStateOff, "");
	  devCtl_boardIoctl(BOARD_IOCTL_LED_CTRL, PERSISTENT, "", kLedWanData, kLedStateOn, "");	  
	  devCtl_boardIoctl(BOARD_IOCTL_LED_CTRL, PERSISTENT, "", kLedInetR, kLedStateAlternateBlinkContinues, "");
   }
   else if (strcmp(color, "red-flash") == 0)
   {
	  devCtl_boardIoctl(BOARD_IOCTL_LED_CTRL, PERSISTENT, "", kLedInetR, kLedStateOn, "");
	  devCtl_boardIoctl(BOARD_IOCTL_SET_GPIO, PERSISTENT, "", BP_GPIO_39_AH, kLedStateOff, "");
	  devCtl_boardIoctl(BOARD_IOCTL_LED_CTRL, PERSISTENT, "", kLedWanData, kLedStateFlashBlinkContinues, "");
   }
   else if (strcmp(color, "green-flash") == 0)
   {
  	  devCtl_boardIoctl(BOARD_IOCTL_LED_CTRL, PERSISTENT, "", kLedInetR, kLedStateOff, "");
	  devCtl_boardIoctl(BOARD_IOCTL_SET_GPIO, PERSISTENT, "", BP_GPIO_39_AH, kLedStateOn, "");
      devCtl_boardIoctl(BOARD_IOCTL_LED_CTRL, PERSISTENT, "", kLedWanData, kLedStateFlashBlinkContinues, "");
   }
   else if (strcmp(color, "amber-flash") == 0)
   {
  	  devCtl_boardIoctl(BOARD_IOCTL_LED_CTRL, PERSISTENT, "", kLedInetR, kLedStateOn, "");
	  devCtl_boardIoctl(BOARD_IOCTL_SET_GPIO, PERSISTENT, "", BP_GPIO_39_AH, kLedStateOn, "");
      devCtl_boardIoctl(BOARD_IOCTL_LED_CTRL, PERSISTENT, "", kLedWanData, kLedStateFlashBlinkContinues, "");
   }
   else
   {
      cmsLog_error("Unknown LED color: %s", color);
      return;
   }
}
#endif
