/*
    Copyright 2000-2010 Broadcom Corporation

    <:label-BRCM:2011:DUAL/GPL:standard
    
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License, version 2, as published by
    the Free Software Foundation (the "GPL").
    
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    
    A copy of the GPL is available at http://www.broadcom.com/licenses/GPLv2.php, or by
    writing to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.
    
    :>
*/                       

/**************************************************************************
 * File Name  : boardparms.h
 *
 * Description: This file contains definitions and function prototypes for
 *              the BCM63xx board parameter access functions.
 *
 * Updates    : 07/14/2003  Created.
 ***************************************************************************/

#if !defined(_BOARDPARMS_H)
#define _BOARDPARMS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Return codes. */
#define BP_SUCCESS                              0
#define BP_BOARD_ID_NOT_FOUND                   1
#define BP_VALUE_NOT_DEFINED                    2
#define BP_BOARD_ID_NOT_SET                     3

/* Values for EthernetMacInfo PhyType. */
#define BP_ENET_NO_PHY                          0
#define BP_ENET_INTERNAL_PHY                    1
#define BP_ENET_EXTERNAL_SWITCH                 2
#define BP_ENET_SWITCH_VIA_INTERNAL_PHY         3      /* it is for cpu internal phy connects to port 4 of 5325e */

/* Values for EthernetMacInfo Configuration type. */
#define BP_ENET_CONFIG_MDIO                     0       /* Internal PHY, External PHY, Switch+(no GPIO, no SPI, no MDIO Pseudo phy */
#define BP_ENET_CONFIG_MDIO_PSEUDO_PHY          1
#define BP_ENET_CONFIG_SPI_SSB_0                2
#define BP_ENET_CONFIG_SPI_SSB_1                3
#define BP_ENET_CONFIG_SPI_SSB_2                4
#define BP_ENET_CONFIG_SPI_SSB_3                5
#define BP_ENET_CONFIG_MMAP                     6
#define BP_ENET_CONFIG_GPIO_MDIO                7       /* use GPIO to simulate MDC/MDIO */
#define BP_ENET_CONFIG_HS_SPI_SSB_0             8
#define BP_ENET_CONFIG_HS_SPI_SSB_1             9
#define BP_ENET_CONFIG_HS_SPI_SSB_2             10
#define BP_ENET_CONFIG_HS_SPI_SSB_3             11
#define BP_ENET_CONFIG_HS_SPI_SSB_4             12
#define BP_ENET_CONFIG_HS_SPI_SSB_5             13
#define BP_ENET_CONFIG_HS_SPI_SSB_6             14
#define BP_ENET_CONFIG_HS_SPI_SSB_7             15


/* Values for VoIPDSPInfo DSPType. */
#define BP_VOIP_NO_DSP                          0
#define BP_VOIP_DSP                             1
#define BP_VOIP_MIPS                            2

/* Values for GPIO pin assignments (AH = Active High, AL = Active Low). */
#define BP_GPIO_NUM_MASK                        0x00FF
#define BP_ACTIVE_MASK                          0x8000
#define BP_ACTIVE_HIGH                          0x0000
#define BP_ACTIVE_LOW                           0x8000
#define BP_GPIO_SERIAL                          0x4000

#define BP_GPIO_0_AH                            (0)
#define BP_GPIO_0_AL                            (0  | BP_ACTIVE_LOW)
#define BP_GPIO_1_AH                            (1)
#define BP_GPIO_1_AL                            (1  | BP_ACTIVE_LOW)
#define BP_GPIO_2_AH                            (2)
#define BP_GPIO_2_AL                            (2  | BP_ACTIVE_LOW)
#define BP_GPIO_3_AH                            (3)
#define BP_GPIO_3_AL                            (3  | BP_ACTIVE_LOW)
#define BP_GPIO_4_AH                            (4)
#define BP_GPIO_4_AL                            (4  | BP_ACTIVE_LOW)
#define BP_GPIO_5_AH                            (5)
#define BP_GPIO_5_AL                            (5  | BP_ACTIVE_LOW)
#define BP_GPIO_6_AH                            (6)
#define BP_GPIO_6_AL                            (6  | BP_ACTIVE_LOW)
#define BP_GPIO_7_AH                            (7)
#define BP_GPIO_7_AL                            (7  | BP_ACTIVE_LOW)
#define BP_GPIO_8_AH                            (8)
#define BP_GPIO_8_AL                            (8  | BP_ACTIVE_LOW)
#define BP_GPIO_9_AH                            (9)
#define BP_GPIO_9_AL                            (9  | BP_ACTIVE_LOW)
#define BP_GPIO_10_AH                           (10)
#define BP_GPIO_10_AL                           (10 | BP_ACTIVE_LOW)
#define BP_GPIO_11_AH                           (11)
#define BP_GPIO_11_AL                           (11 | BP_ACTIVE_LOW)
#define BP_GPIO_12_AH                           (12)
#define BP_GPIO_12_AL                           (12 | BP_ACTIVE_LOW)
#define BP_GPIO_13_AH                           (13)
#define BP_GPIO_13_AL                           (13 | BP_ACTIVE_LOW)
#define BP_GPIO_14_AH                           (14)
#define BP_GPIO_14_AL                           (14 | BP_ACTIVE_LOW)
#define BP_GPIO_15_AH                           (15)
#define BP_GPIO_15_AL                           (15 | BP_ACTIVE_LOW)
#define BP_GPIO_16_AH                           (16)
#define BP_GPIO_16_AL                           (16 | BP_ACTIVE_LOW)
#define BP_GPIO_17_AH                           (17)
#define BP_GPIO_17_AL                           (17 | BP_ACTIVE_LOW)
#define BP_GPIO_18_AH                           (18)
#define BP_GPIO_18_AL                           (18 | BP_ACTIVE_LOW)
#define BP_GPIO_19_AH                           (19)
#define BP_GPIO_19_AL                           (19 | BP_ACTIVE_LOW)
#define BP_GPIO_20_AH                           (20)
#define BP_GPIO_20_AL                           (20 | BP_ACTIVE_LOW)
#define BP_GPIO_21_AH                           (21)
#define BP_GPIO_21_AL                           (21 | BP_ACTIVE_LOW)
#define BP_GPIO_22_AH                           (22)
#define BP_GPIO_22_AL                           (22 | BP_ACTIVE_LOW)
#define BP_GPIO_23_AH                           (23)
#define BP_GPIO_23_AL                           (23 | BP_ACTIVE_LOW)
#define BP_GPIO_24_AH                           (24)
#define BP_GPIO_24_AL                           (24 | BP_ACTIVE_LOW)
#define BP_GPIO_25_AH                           (25)
#define BP_GPIO_25_AL                           (25 | BP_ACTIVE_LOW)
#define BP_GPIO_26_AH                           (26)
#define BP_GPIO_26_AL                           (26 | BP_ACTIVE_LOW)
#define BP_GPIO_27_AH                           (27)
#define BP_GPIO_27_AL                           (27 | BP_ACTIVE_LOW)
#define BP_GPIO_28_AH                           (28)
#define BP_GPIO_28_AL                           (28 | BP_ACTIVE_LOW)
#define BP_GPIO_29_AH                           (29)
#define BP_GPIO_29_AL                           (29 | BP_ACTIVE_LOW)
#define BP_GPIO_30_AH                           (30)
#define BP_GPIO_30_AL                           (30 | BP_ACTIVE_LOW)
#define BP_GPIO_31_AH                           (31)
#define BP_GPIO_31_AL                           (31 | BP_ACTIVE_LOW)
#define BP_GPIO_32_AH                           (32)
#define BP_GPIO_32_AL                           (32 | BP_ACTIVE_LOW)
#define BP_GPIO_33_AH                           (33)
#define BP_GPIO_33_AL                           (33 | BP_ACTIVE_LOW)
#define BP_GPIO_34_AH                           (34)
#define BP_GPIO_34_AL                           (34 | BP_ACTIVE_LOW)
#define BP_GPIO_35_AH                           (35)
#define BP_GPIO_35_AL                           (35 | BP_ACTIVE_LOW)
#define BP_GPIO_36_AH                           (36)
#define BP_GPIO_36_AL                           (36 | BP_ACTIVE_LOW)
#define BP_GPIO_37_AH                           (37)
#define BP_GPIO_37_AL                           (37 | BP_ACTIVE_LOW)
#define BP_GPIO_38_AH                           (38)
#define BP_GPIO_38_AL                           (38 | BP_ACTIVE_LOW)
#define BP_GPIO_39_AH                           (39)
#define BP_GPIO_39_AL                           (39 | BP_ACTIVE_LOW)
#define BP_GPIO_40_AH                           (40)
#define BP_GPIO_40_AL                           (40 | BP_ACTIVE_LOW)
#define BP_GPIO_41_AH                           (41)
#define BP_GPIO_41_AL                           (41 | BP_ACTIVE_LOW)
#define BP_GPIO_42_AH                           (42)
#define BP_GPIO_42_AL                           (42 | BP_ACTIVE_LOW)
#define BP_GPIO_43_AH                           (43)
#define BP_GPIO_43_AL                           (43 | BP_ACTIVE_LOW)
#define BP_GPIO_44_AH                           (44)
#define BP_GPIO_44_AL                           (44 | BP_ACTIVE_LOW)
#define BP_GPIO_45_AH                           (45)
#define BP_GPIO_45_AL                           (45 | BP_ACTIVE_LOW)
#define BP_GPIO_46_AH                           (46)
#define BP_GPIO_46_AL                           (46 | BP_ACTIVE_LOW)
#define BP_GPIO_47_AH                           (47)
#define BP_GPIO_47_AL                           (47 | BP_ACTIVE_LOW)
#define BP_GPIO_50_AH                           (50)
#define BP_GPIO_50_AL                           (50 | BP_ACTIVE_LOW)
#define BP_GPIO_51_AH                           (51)
#define BP_GPIO_51_AL                           (51 | BP_ACTIVE_LOW)

#define BP_SERIAL_GPIO_0_AH                     (0  | BP_GPIO_SERIAL)
#define BP_SERIAL_GPIO_0_AL                     (0  | BP_GPIO_SERIAL | BP_ACTIVE_LOW)
#define BP_SERIAL_GPIO_1_AH                     (1  | BP_GPIO_SERIAL)
#define BP_SERIAL_GPIO_1_AL                     (1  | BP_GPIO_SERIAL | BP_ACTIVE_LOW)
#define BP_SERIAL_GPIO_2_AH                     (2  | BP_GPIO_SERIAL)
#define BP_SERIAL_GPIO_2_AL                     (2  | BP_GPIO_SERIAL | BP_ACTIVE_LOW)
#define BP_SERIAL_GPIO_3_AH                     (3  | BP_GPIO_SERIAL)
#define BP_SERIAL_GPIO_3_AL                     (3  | BP_GPIO_SERIAL | BP_ACTIVE_LOW)
#define BP_SERIAL_GPIO_4_AH                     (4  | BP_GPIO_SERIAL)
#define BP_SERIAL_GPIO_4_AL                     (4  | BP_GPIO_SERIAL | BP_ACTIVE_LOW)
#define BP_SERIAL_GPIO_5_AH                     (5  | BP_GPIO_SERIAL)
#define BP_SERIAL_GPIO_5_AL                     (5  | BP_GPIO_SERIAL | BP_ACTIVE_LOW)
#define BP_SERIAL_GPIO_6_AH                     (6  | BP_GPIO_SERIAL)
#define BP_SERIAL_GPIO_6_AL                     (6  | BP_GPIO_SERIAL | BP_ACTIVE_LOW)
#define BP_SERIAL_GPIO_7_AH                     (7  | BP_GPIO_SERIAL)
#define BP_SERIAL_GPIO_7_AL                     (7  | BP_GPIO_SERIAL | BP_ACTIVE_LOW)
#define BP_SERIAL_GPIO_8_AH                     (8  | BP_GPIO_SERIAL)
#define BP_SERIAL_GPIO_8_AL                     (8  | BP_GPIO_SERIAL | BP_ACTIVE_LOW)
#define BP_SERIAL_GPIO_9_AH                     (9  | BP_GPIO_SERIAL)
#define BP_SERIAL_GPIO_9_AL                     (9  | BP_GPIO_SERIAL | BP_ACTIVE_LOW)
#define BP_SERIAL_GPIO_10_AH                    (10 | BP_GPIO_SERIAL)
#define BP_SERIAL_GPIO_10_AL                    (10 | BP_GPIO_SERIAL | BP_ACTIVE_LOW)
#define BP_SERIAL_GPIO_11_AH                    (11 | BP_GPIO_SERIAL)
#define BP_SERIAL_GPIO_11_AL                    (11 | BP_GPIO_SERIAL | BP_ACTIVE_LOW)
#define BP_SERIAL_GPIO_12_AH                    (12 | BP_GPIO_SERIAL)
#define BP_SERIAL_GPIO_12_AL                    (12 | BP_GPIO_SERIAL | BP_ACTIVE_LOW)
#define BP_SERIAL_GPIO_13_AH                    (13 | BP_GPIO_SERIAL)
#define BP_SERIAL_GPIO_13_AL                    (13 | BP_GPIO_SERIAL | BP_ACTIVE_LOW)
#define BP_SERIAL_GPIO_14_AH                    (14 | BP_GPIO_SERIAL)
#define BP_SERIAL_GPIO_14_AL                    (14 | BP_GPIO_SERIAL | BP_ACTIVE_LOW)
#define BP_SERIAL_GPIO_15_AH                    (15 | BP_GPIO_SERIAL)
#define BP_SERIAL_GPIO_15_AL                    (15 | BP_GPIO_SERIAL | BP_ACTIVE_LOW)
#define BP_SERIAL_GPIO_16_AH                    (16 | BP_GPIO_SERIAL)
#define BP_SERIAL_GPIO_16_AL                    (16 | BP_GPIO_SERIAL | BP_ACTIVE_LOW)
#define BP_SERIAL_GPIO_17_AH                    (17 | BP_GPIO_SERIAL)
#define BP_SERIAL_GPIO_17_AL                    (17 | BP_GPIO_SERIAL | BP_ACTIVE_LOW)
#define BP_SERIAL_GPIO_18_AH                    (18 | BP_GPIO_SERIAL)
#define BP_SERIAL_GPIO_18_AL                    (18 | BP_GPIO_SERIAL | BP_ACTIVE_LOW)
#define BP_SERIAL_GPIO_19_AH                    (19 | BP_GPIO_SERIAL)
#define BP_SERIAL_GPIO_19_AL                    (19 | BP_GPIO_SERIAL | BP_ACTIVE_LOW)
#define BP_SERIAL_GPIO_20_AH                    (20 | BP_GPIO_SERIAL)
#define BP_SERIAL_GPIO_20_AL                    (20 | BP_GPIO_SERIAL | BP_ACTIVE_LOW)
#define BP_SERIAL_GPIO_21_AH                    (21 | BP_GPIO_SERIAL)
#define BP_SERIAL_GPIO_21_AL                    (21 | BP_GPIO_SERIAL | BP_ACTIVE_LOW)
#define BP_SERIAL_GPIO_22_AH                    (22 | BP_GPIO_SERIAL)
#define BP_SERIAL_GPIO_22_AL                    (22 | BP_GPIO_SERIAL | BP_ACTIVE_LOW)
#define BP_SERIAL_GPIO_23_AH                    (23 | BP_GPIO_SERIAL)
#define BP_SERIAL_GPIO_23_AL                    (23 | BP_GPIO_SERIAL | BP_ACTIVE_LOW)

/* Values for external interrupt assignments. */
#define BP_EXT_INTR_0                           0
#define BP_EXT_INTR_1                           1
#define BP_EXT_INTR_2                           2
#define BP_EXT_INTR_3                           3
#define BP_EXT_INTR_4                           4
#define BP_EXT_INTR_5                           5

/* Values for chip select assignments. */
#define BP_CS_0                                 0
#define BP_CS_1                                 1
#define BP_CS_2                                 2
#define BP_CS_3                                 3

#define BP_OVERLAY_GPON_TX_EN_L                 (1<<0)
#define BP_OVERLAY_PCI                          (1<<0)
#define BP_OVERLAY_PCIE_CLKREQ                  (1<<0)
#define BP_OVERLAY_CB                           (1<<1) // Unused
#define BP_OVERLAY_SPI_EXT_CS                   (1<<2)
#define BP_OVERLAY_UART1                        (1<<3) // Unused
#define BP_OVERLAY_PHY                          (1<<4)
#define BP_OVERLAY_SERIAL_LEDS                  (1<<5)
#define BP_OVERLAY_EPHY_LED_0                   (1<<6)
#define BP_OVERLAY_EPHY_LED_1                   (1<<7)
#define BP_OVERLAY_EPHY_LED_2                   (1<<8)
#define BP_OVERLAY_EPHY_LED_3                   (1<<9)
#define BP_OVERLAY_GPHY_LED_0                   (1<<10)
#define BP_OVERLAY_GPHY_LED_1                   (1<<11)
#define BP_OVERLAY_INET_LED                     (1<<12)
#define BP_OVERLAY_MOCA_LED                     (1<<13)
#define BP_OVERLAY_USB_LED                      (1<<14)
#define BP_OVERLAY_USB_DEVICE                   (1<<15)
#define BP_OVERLAY_SPI_SSB2_EXT_CS              (1<<16)
#define BP_OVERLAY_SPI_SSB3_EXT_CS              (1<<17)
#define BP_OVERLAY_HS_SPI_SSB4_EXT_CS           (1<<18)
#define BP_OVERLAY_HS_SPI_SSB5_EXT_CS           (1<<19)
#define BP_OVERLAY_HS_SPI_SSB6_EXT_CS           (1<<20)
#define BP_OVERLAY_HS_SPI_SSB7_EXT_CS           (1<<21)

/* Value for GPIO and external interrupt fields that are not used. */
#define BP_NOT_DEFINED                          0xffff

/* Maximum size of the board id string. */
#define BP_BOARD_ID_LEN                         16

/* Maximum size of the board id string. */
#define BP_OPTICAL_PARAMS_LEN                   64

/* Maximum number of Ethernet MACs. */
#define BP_MAX_ENET_MACS                        2
#define BP_MAX_SWITCH_PORTS                     8
#define BP_MAX_ENET_INTERNAL                    2
/* Maximum number of VoIP DSPs. */
#define BP_MAX_VOIP_DSP                         2

/* Wireless Antenna Settings. */
#define BP_WLAN_ANT_MAIN                        0
#define BP_WLAN_ANT_AUX                         1
#define BP_WLAN_ANT_BOTH                        3

/* Wireless FLAGS */
#define BP_WLAN_MAC_ADDR_OVERRIDE               0x0001   /* use kerSysGetMacAddress for mac address */
#define BP_WLAN_EXCLUDE_ONBOARD                 0x0002   /* exclude onboard wireless  */
#define BP_WLAN_EXCLUDE_ONBOARD_FORCE           0x0004   /* force exclude onboard wireless even without addon card*/
#define BP_WLAN_USE_OTP                         0x0008   /* don't use sw srom map, may fall to OTP or uninitialzed */

#define BP_WLAN_NVRAM_NAME_LEN      16
#define BP_WLAN_MAX_PATCH_ENTRY     32

/* AFE IDs */
#define BP_AFE_DEFAULT                  0

#define BP_AFE_CHIP_INT                 (1 << 28)
#define BP_AFE_CHIP_6505                (2 << 28)
#define BP_AFE_CHIP_6306                (3 << 28)

#define BP_AFE_LD_ISIL1556              (1 << 21)
#define BP_AFE_LD_6301                  (2 << 21)
#define BP_AFE_LD_6302                  (3 << 21)
#define BP_AFE_LD_6303                  (4 << 21)

#define BP_AFE_FE_ANNEXA                (1 << 15)
#define BP_AFE_FE_ANNEXB                (2 << 15)
#define BP_AFE_FE_ANNEXJ                (3 << 15)
#define BP_AFE_FE_ANNEXBJ               (4 << 15)
#define BP_AFE_FE_ANNEXM                (5 << 15)

#define BP_AFE_FE_AVMODE_COMBO          (0 << 13)
#define BP_AFE_FE_AVMODE_ADSL           (1 << 13)
#define BP_AFE_FE_AVMODE_VDSL           (2 << 13)

/* VDSL only */
#define BP_AFE_FE_REV_ISIL_REV1         (1 << 8)
#define BP_AFE_FE_REV_12_20             BP_AFE_FE_REV_ISIL_REV1
#define BP_AFE_FE_REV_12_21             (2 << 8)

/* Combo */
#define BP_AFE_FE_REV_6302_REV1         (1 << 8)
#define BP_AFE_FE_REV_6302_REV_7_12     (1 << 8)
#define BP_AFE_FE_REV_6302_REV_7_2_21   (2 << 8)

#define BP_AFE_FE_REV_6302_REV_7_2_1    (3 << 8)
#define BP_AFE_FE_REV_6302_REV_7_2      (4 << 8)
#define BP_AFE_FE_REV_6302_REV_7_2_UR2  (5 << 8)
#define BP_AFE_FE_REV_6302_REV_7_2_2    (6 << 8)
#define BP_AFE_FE_REV_6302_REV_7_2_30    (7 << 8)
#define BP_AFE_6302_6306_REV_A_12_40    (8 << 8)

/* ADSL only*/
#define BP_AFE_FE_REV_6302_REV_5_2_1    (1 << 8)
#define BP_AFE_FE_REV_6302_REV_5_2_2    (2 << 8)
#define BP_AFE_FE_REV_6301_REV_5_1_1    (1 << 8)
#define BP_AFE_FE_REV_6301_REV_5_1_2    (2 << 8)

#define BP_GET_EXT_AFE_DEFINED
#define BP_GET_INT_AFE_DEFINED

#if !defined(__ASSEMBLER__)

typedef struct {
  unsigned short duplexLed;
  unsigned short speedLed100;
  unsigned short speedLed1000;
} LED_INFO;

/* Information about Ethernet switch */
typedef struct {
  unsigned long port_map;
  unsigned long phy_id[BP_MAX_SWITCH_PORTS];
  LED_INFO ledInfo[BP_MAX_ENET_INTERNAL];
} ETHERNET_SW_INFO;

#define BP_PHY_ID_0                            (0)
#define BP_PHY_ID_1                            (1)
#define BP_PHY_ID_2                            (2)
#define BP_PHY_ID_3                            (3)
#define BP_PHY_ID_4                            (4)
#define BP_PHY_ID_5                            (5)
#define BP_PHY_ID_6                            (6)
#define BP_PHY_ID_7                            (7)
#define BP_PHY_ID_8                            (8)
#define BP_PHY_ID_9                            (9)
#define BP_PHY_ID_10                           (10)
#define BP_PHY_ID_11                           (11)
#define BP_PHY_ID_12                           (12)
#define BP_PHY_ID_13                           (13)
#define BP_PHY_ID_14                           (14)
#define BP_PHY_ID_15                           (15)
#define BP_PHY_ID_16                           (16)
#define BP_PHY_ID_17                           (17)
#define BP_PHY_ID_18                           (18)
#define BP_PHY_ID_19                           (19)
#define BP_PHY_ID_20                           (20)
#define BP_PHY_ID_21                           (21)
#define BP_PHY_ID_22                           (22)
#define BP_PHY_ID_23                           (23)
#define BP_PHY_ID_24                           (24)
#define BP_PHY_ID_25                           (25)
#define BP_PHY_ID_26                           (26)
#define BP_PHY_ID_27                           (27)
#define BP_PHY_ID_28                           (28)
#define BP_PHY_ID_29                           (29)
#define BP_PHY_ID_30                           (30)
#define BP_PHY_ID_31                           (31)
#define BP_PHY_ID_NOT_SPECIFIED                (0xFF)
#define BP_PHY_NOT_PRESENT                     (0)


/* Phy config info embedded into phy_id of ETHERNET_SW_INFO */
#define PHY_LNK_CFG_M           0x7
#define PHY_LNK_CFG_S           8
#define ATONEG_FOR_LINK         (0 << PHY_LNK_CFG_S)
#define FORCE_LINK_DOWN         (1 << PHY_LNK_CFG_S)
#define FORCE_LINK_10HD         (2 << PHY_LNK_CFG_S)
#define FORCE_LINK_10FD         (3 << PHY_LNK_CFG_S)
#define FORCE_LINK_100HD        (4 << PHY_LNK_CFG_S)
#define FORCE_LINK_100FD        (5 << PHY_LNK_CFG_S)
#define FORCE_LINK_1000FD       (6 << PHY_LNK_CFG_S)

#define PHY_LNK_CFG_VALID_M     1
#define PHY_LNK_CFG_VALID_S     11 
#define PHY_LNK_CFG_VALID       (PHY_LNK_CFG_VALID_M << PHY_LNK_CFG_VALID_S)

#define PHY_ADV_CAP_CFG_M       0x3F
#define PHY_ADV_CAP_CFG_S       12
#define ADVERTISE_10HD          (1 << PHY_ADV_CAP_CFG_S)
#define ADVERTISE_10FD          (2 << PHY_ADV_CAP_CFG_S)
#define ADVERTISE_100HD         (4 << PHY_ADV_CAP_CFG_S)
#define ADVERTISE_100FD         (8 << PHY_ADV_CAP_CFG_S)
#define ADVERTISE_1000HD        (16 << PHY_ADV_CAP_CFG_S)
#define ADVERTISE_1000FD        (32 << PHY_ADV_CAP_CFG_S)
#define ADVERTISE_ALL_GMII      (ADVERTISE_10HD | ADVERTISE_10FD | ADVERTISE_100HD | ADVERTISE_100FD | ADVERTISE_1000HD | ADVERTISE_1000FD)
#define ADVERTISE_ALL_MII       (ADVERTISE_10HD | ADVERTISE_10FD | ADVERTISE_100HD | ADVERTISE_100FD)

#define PHY_ADV_CFG_VALID_M     1
#define PHY_ADV_CFG_VALID_S     18 
#define PHY_ADV_CFG_VALID       (PHY_ADV_CFG_VALID_M << PHY_ADV_CFG_VALID_S)

#define PHY_INTEGRATED_M        0x1
#define PHY_INTEGRATED_S        19
#define PHY_INTERNAL            (0 << PHY_INTEGRATED_S)
#define PHY_EXTERNAL            (1 << PHY_INTEGRATED_S)

#define PHY_INTEGRATED_VALID_M  1
#define PHY_INTEGRATED_VALID_S  20
#define PHY_INTEGRATED_VALID    (PHY_INTEGRATED_VALID_M << PHY_INTEGRATED_VALID_S)

#define MAC_CONN_M              0x1
#define MAC_CONN_S              21
#define MAC_CONNECTION          (MAC_CONN_M << MAC_CONN_S)
#define MAC_PHY_IF              (0 << MAC_CONN_S)
#define MAC_MAC_IF              (1 << MAC_CONN_S)

#define MAC_CONN_VALID_M        1
#define MAC_CONN_VALID_S        22 
#define MAC_CONN_VALID          (MAC_CONN_VALID_M << MAC_CONN_VALID_S)

#define MAC_IFACE_M             0x7
#define MAC_IFACE_S             23
#define MAC_IFACE               (MAC_IFACE_M << MAC_IFACE_S)
#define MAC_IF_MII              (0 << MAC_IFACE_S)
#define MAC_IF_RvMII            (1 << MAC_IFACE_S)
#define MAC_IF_GMII             (2 << MAC_IFACE_S)
#define MAC_IF_RGMII            (3 << MAC_IFACE_S)

#define MAC_IFACE_VALID_M        1
#define MAC_IFACE_VALID_S        26
#define MAC_IFACE_VALID          (MAC_IFACE_VALID_M << MAC_IFACE_VALID_S)

#define EXTSW_CONNECTED_M       1
#define EXTSW_CONNECTED_S       27
#define EXTSW_CONNECTED         (EXTSW_CONNECTED_M << EXTSW_CONNECTED_S)

#define CONNECTED_TO_EPON_MAC_M 1
#define CONNECTED_TO_EPON_MAC_S 28
#define CONNECTED_TO_EPON_MAC  (CONNECTED_TO_EPON_MAC_M << CONNECTED_TO_EPON_MAC_S)


/* Currently used for qualifying WAN_PORT and MII_OVER_GPIO. Can be split into 2 if needed. */
#define PHYCFG_VALID_M           1 
#define PHYCFG_VALID_S           31 
#define PHYCFG_VALID             (PHYCFG_VALID_M << PHYCFG_VALID_S)

/* MII over GPIO config info embedded into phy_id of ETHERNET_SW_INFO */
#define MII_OVER_GPIO_M          1 
#define MII_OVER_GPIO_S          30
#define MII_OVER_GPIO_VALID      (MII_OVER_GPIO_M << MII_OVER_GPIO_S)

#define PHYID_LSBYTE_M           0xFF
#define BCM_PHY_ID_M             0x1F

/* MII - RvMII connection. Force Link to 100FD */
#define MII_DIRECT               (PHY_LNK_CFG_VALID | FORCE_LINK_100FD | MAC_CONN_VALID | MAC_MAC_IF | MAC_IFACE_VALID | MAC_IF_MII)
#define RGMII_DIRECT             (PHY_LNK_CFG_VALID | FORCE_LINK_1000FD | MAC_CONN_VALID | MAC_MAC_IF | MAC_IFACE_VALID | MAC_IF_RGMII)

/* WAN port flag in the phy_id of ETHERNET_SW_INFO */
#define BCM_WAN_PORT        0x40
#define IsWanPort(id)       ((id & PHYCFG_VALID)?(id & BCM_WAN_PORT):((id & BCM_WAN_PORT) && ((id & PHYID_LSBYTE_M) != 0xFF)))
#define IsPhyConnected(id)  ((id & MAC_CONN_VALID)?((id & MAC_CONNECTION) != MAC_MAC_IF):((id & PHYID_LSBYTE_M) != 0xFF))
#define IsExtPhyId(id)      ((id & PHY_INTEGRATED_VALID)?(id & PHY_EXTERNAL):((id & BCM_PHY_ID_M) >= 0x10))
#define IsRGMII(id)         ((id & MAC_IFACE_VALID)?((id & MAC_IFACE) == MAC_IF_RGMII):0)
#define IsRvMII(id)         ((id & MAC_IFACE_VALID)?((id & MAC_IFACE) == MAC_IF_RvMII):0)
#define IsPortConnectedToExternalSwitch(id)  ((id & EXTSW_CONNECTED)?1:0) 


#define c0(n) (((n) & 0x55555555) + (((n) >> 1) & 0x55555555))
#define c1(n) (((n) & 0x33333333) + (((n) >> 2) & 0x33333333))
#define c2(n) (((n) & 0x0f0f0f0f) + (((n) >> 4) & 0x0f0f0f0f))
#define bitcount(r, n) {r = n; r = c0(r); r = c1(r); r = c2(r); r %= 255;}

/* GPON Optics Type Settings */
#define BP_GPON_OPTICS_TYPE_LEGACY              0
#define BP_GPON_OPTICS_TYPE_BOSA                1

/* Information about an Ethernet MAC.  If ucPhyType is BP_ENET_NO_PHY,
 * then the other fields are not valid.
 */
typedef struct EthernetMacInfo
{
    unsigned char ucPhyType;                    /* BP_ENET_xxx             */
    unsigned char ucPhyAddress;                 /* 0 to 31                 */
    unsigned short usConfigType;                /* Configuration type */
    ETHERNET_SW_INFO sw;                        /* switch information */
    unsigned short usGpioMDC;                   /* GPIO pin to simulate MDC */
    unsigned short usGpioMDIO;                  /* GPIO pin to simulate MDIO */
} ETHERNET_MAC_INFO, *PETHERNET_MAC_INFO;

typedef struct WlanSromEntry {
    unsigned short wordOffset;
    unsigned short value;
} WLAN_SROM_ENTRY;

typedef struct WlanSromPatchInfo {
    char szboardId[BP_BOARD_ID_LEN];
    unsigned short usWirelessChipId;
    unsigned short usNeededSize;
    WLAN_SROM_ENTRY entries[BP_WLAN_MAX_PATCH_ENTRY];
} WLAN_SROM_PATCH_INFO, *PWLAN_SROM_PATCH_INFO;

typedef struct WlanPciEntry {
    char name[BP_WLAN_NVRAM_NAME_LEN];
    unsigned int dwordOffset;
    unsigned int value;
} WLAN_PCI_ENTRY;

typedef struct WlanPciPatchInfo {
    char szboardId[BP_BOARD_ID_LEN];
    unsigned int usWirelessPciId;
    int usNeededSize;
    WLAN_PCI_ENTRY entries[BP_WLAN_MAX_PATCH_ENTRY];
} WLAN_PCI_PATCH_INFO, *PWLAN_PCI_PATCH_INFO;

/* Information about VoIP DSPs.  If ucDspType is BP_VOIP_NO_DSP,
 * then the other fields are not valid.
 */
typedef struct VoIPDspInfo
{
    unsigned char  ucDspType;
    unsigned char  ucDspAddress;
    unsigned short usGpioLedVoip;
    unsigned short usGpioVoip1Led;
    unsigned short usGpioVoip1LedFail;
    unsigned short usGpioVoip2Led;
    unsigned short usGpioVoip2LedFail;
    unsigned short usGpioPotsLed;
    unsigned short usGpioDectLed;

} VOIP_DSP_INFO;


/***********************************************************************
 * SMP locking notes for BoardParm functions
 *
 * No locking is needed for any of these boardparm functions as long
 * as the following conditions/assumptions are not violated.
 *
 * 1. Initialization functions such as BpSetBoardId() are only called
 *    during startup when no other BoardParam functions are in progress.
 *    BpSetBoardId() modifies the internal global pointer g_CurrentBp,
 *    which other functions deference multiple times inside their
 *    functions.  So if g_CurrentBp changes in the middle of a function,
 *    inconsistent data could be returned.
 *    Actually, BpSetBoardId is also called when cfe or whole image is
 *    being written to flash, but this is when system is about to shut
 *    down, so should also be OK.
 *
 * 2. Callers to functions which return a pointer to the boardparm data
 *    (currently there is only 1: BpGetVoipDspConfig) should not modify
 *    the boardparm data.  All other functions are well written
 *    in this regard, they only return a copy of the requested data and
 *    not a pointer to the data itself.
 *
 *
 ************************************************************************/

int BpSetBoardId( char *pszBoardId );
int BpGetBoardId( char *pszBoardId);
int BpGetBoardIds( char *pszBoardIds, int nBoardIdsSize );

int BpGetComment( char **pcpValue );

int BpGetGPIOverlays( unsigned long *pusValue );

int BpGetRj11InnerOuterPairGpios( unsigned short *pusInner, unsigned short *pusOuter );
int BpGetRtsCtsUartGpios( unsigned short *pusRts, unsigned short *pusCts );

int BpGetAdslLedGpio( unsigned short *pusValue );
int BpGetAdslFailLedGpio( unsigned short *pusValue );
int BpGetSecAdslLedGpio( unsigned short *pusValue );
int BpGetSecAdslFailLedGpio( unsigned short *pusValue );

#if 1 //__MSTC__, RaynorChung: Ssupport VDSL LED
int BpGetVdslLedGpio( unsigned short *pusValue );
int BpGetVdslFailLedGpio( unsigned short *pusValue );
int BpGetSecVdslLedGpio( unsigned short *pusValue );
int BpGetSecVdslFailLedGpio( unsigned short *pusValue );
#endif
#if 1 //__MSTC__, RaynorChung: Ssupport USB LED
int BpGetUSB0LedGpio( unsigned short *pusValue );
int BpGetUSB0FailLedGpio( unsigned short *pusValue );
int BpGetUSB1LedGpio( unsigned short *pusValue );
int BpGetUSB1FailLedGpio( unsigned short *pusValue );
#endif
#if 1//__MSTC__,JhenYang,LED
int BpGetWLANREDLedGpio( unsigned short *pusValue );
int BpGetWLANREDFailLedGpio( unsigned short *pusValue );
int BpGetADSLREDLedGpio( unsigned short *pusValue );
int BpGetADSLORANGELedGpio( unsigned short *pusValue );
int BpGetWLANORANGELedGpio( unsigned short *pusValue );
int BpGetWPSREDLedGpio( unsigned short *pusValue );
int BpGetPppLedGpio( unsigned short *pusValue );
int BpGetADSLREDFailLedGpio( unsigned short *pusValue );
#endif
int BpGetWirelessSesLedGpio( unsigned short *pusValue );
int BpGetWanDataLedGpio( unsigned short *pusValue );
int BpGetWanErrorLedGpio( unsigned short *pusValue );
int BpGetBootloaderPowerOnLedGpio( unsigned short *pusValue );
int BpGetBootloaderStopLedGpio( unsigned short *pusValue );
int BpGetPassDyingGaspGpio( unsigned short *pusValue );
int BpGetFpgaResetGpio( unsigned short *pusValue );
int BpGetGponLedGpio( unsigned short *pusValue );
int BpGetGponFailLedGpio( unsigned short *pusValue );
int BpGetMoCALedGpio( unsigned short *pusValue );
int BpGetMoCAFailLedGpio( unsigned short *pusValue );

int BpGetResetToDefaultExtIntr( unsigned short *pusValue );
int BpGetWirelessSesExtIntr( unsigned short *pusValue );
int BpGetWirelessAntInUse( unsigned short *pusValue );
int BpGetWirelessFlags( unsigned short *pusValue );
int BpGetWirelessPowerDownGpio( unsigned short *pusValue );
int BpUpdateWirelessSromMap(unsigned short chipID, unsigned short* pBase, int sizeInWords);
int BpUpdateWirelessPciConfig (unsigned long pciID, unsigned long* pBase, int sizeInDWords);

int BpGetEthernetMacInfo( PETHERNET_MAC_INFO pEnetInfos, int nNumEnetInfos );
#if defined(CONFIG_BCM963268) && (CONFIG_BCM_EXT_SWITCH)
int BpGetPortConnectedToExtSwitch(void);
#endif
int BpGet6829PortInfo( unsigned char *portInfo6829 );
int BpGetDslPhyAfeIds( unsigned long *pulValues );
int BpGetExtAFEResetGpio( unsigned short *pulValues );
int BpGetExtAFELDPwrGpio( unsigned short *pulValues );
int BpGetExtAFELDModeGpio( unsigned short *pulValues );
int BpGetIntAFELDPwrGpio( unsigned short *pusValue );
int BpGetIntAFELDModeGpio( unsigned short *pulValues );
int BpGetAFELDRelayGpio( unsigned short *pusValue );
int BpGetExtAFELDDataGpio( unsigned short *pusValue );
int BpGetExtAFELDClkGpio( unsigned short *pusValue );

int BpGetUart2SdoutGpio( unsigned short *pusValue );
int BpGetUart2SdinGpio( unsigned short *pusValue );

int BpGetEthSpdLedGpio( unsigned short port, unsigned short enetIdx,
                         unsigned short ledIdx, unsigned short *pusValue );

VOIP_DSP_INFO *BpGetVoipDspConfig( unsigned char dspNum );
int BpGetVoipLedGpio( unsigned short *pusValue );
int BpGetVoip1LedGpio( unsigned short *pusValue );
int BpGetVoip1FailLedGpio( unsigned short *pusValue );
int BpGetVoip2LedGpio( unsigned short *pusValue );
int BpGetVoip2FailLedGpio( unsigned short *pusValue );
int BpGetPotsLedGpio( unsigned short *pusValue );
int BpGetDectLedGpio( unsigned short *pusValue );

int BpGetLaserDisGpio( unsigned short *pusValue );
int BpGetLaserTxPwrEnGpio( unsigned short *pusValue );
int BpGetVregSel1P2( unsigned short *pusValue );
int BpGetMiiOverGpioFlag( unsigned long* pMiiOverGpioFlag );
int BpGetFemtoResetGpio( unsigned short *pusValue );
int BpGetEphyBaseAddress( unsigned short *pusValue );

int bpstrcmp(const char *dest,const char *src);
int BpGetGponOpticsType( unsigned short *pusValue );
int BpGetDefaultOpticalParams( unsigned char *pOpticalParams );


int BpGetSpiSlaveResetGpio( unsigned short *pusValue );
int BpGetSpiSlaveBusNum( unsigned short *pusValue );
int BpGetSpiSlaveSelectNum( unsigned short *pusValue );
int BpGetSpiSlaveMode( unsigned short *pusValue );
int BpGetSpiSlaveCtrlState( unsigned long *pulValue );
int BpGetSpiSlaveMaxFreq( unsigned long *pulValue );
int BpGetSpiSlaveProtoRev( unsigned short *pusValue );

#endif /* __ASSEMBLER__ */

#ifdef __cplusplus
}
#endif

#endif /* _BOARDPARMS_H */

