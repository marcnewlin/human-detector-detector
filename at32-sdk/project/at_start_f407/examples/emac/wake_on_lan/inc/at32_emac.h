/**
  **************************************************************************
  * @file     at32_emac.h
  * @version  v2.0.8
  * @date     2022-04-02
  * @brief    header file of emac config program.
  **************************************************************************
  *                       Copyright notice & Disclaimer
  *
  * The software Board Support Package (BSP) that is made available to
  * download from Artery official website is the copyrighted work of Artery.
  * Artery authorizes customers to use, copy, and distribute the BSP
  * software and its related documentation for the purpose of design and
  * development in conjunction with Artery microcontrollers. Use of the
  * software is governed by this copyright notice and the following disclaimer.
  *
  * THIS SOFTWARE IS PROVIDED ON "AS IS" BASIS WITHOUT WARRANTIES,
  * GUARANTEES OR REPRESENTATIONS OF ANY KIND. ARTERY EXPRESSLY DISCLAIMS,
  * TO THE FULLEST EXTENT PERMITTED BY LAW, ALL EXPRESS, IMPLIED OR
  * STATUTORY OR OTHER WARRANTIES, GUARANTEES OR REPRESENTATIONS,
  * INCLUDING BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.
  *
  **************************************************************************
  */

/* define to prevent recursive inclusion -------------------------------------*/
#ifndef __AT32_EMAC_H
#define __AT32_EMAC_H

#ifdef __cplusplus
 extern "C" {
#endif

/* includes ------------------------------------------------------------------*/
#include "at32f403a_407.h"

/** @addtogroup AT32F407_periph_examples
  * @{
  */

/** @addtogroup 407_EMAC_wake_on_lan
  * @{
  */

#define RX_REMAP                         (1)
#define CRYSTAL_ON_PHY                   (0)

//#define MII_MODE
#define RMII_MODE

#define DM9162
//#define DP83848

#if defined (DM9162)
  #define PHY_ADDRESS                      (0x00)       /*!< relative to at32 board */
  #define PHY_CONTROL_REG                  (0x00)       /*!< basic mode control register */
  #define PHY_STATUS_REG                   (0x01)       /*!< basic mode status register */
  #define PHY_SPECIFIED_CS_REG             (0x11)       /*!< specified configuration and status register */
  /* phy control register */
  #define PHY_AUTO_NEGOTIATION_BIT         (0x1000)     /*!< enable auto negotiation */
  #define PHY_LOOPBACK_BIT                 (0x4000)     /*!< enable loopback */
  #define PHY_RESET_BIT                    (0x8000)     /*!< reset phy */
  /* phy status register */
  #define PHY_LINKED_STATUS_BIT            (0x0004)     /*!< link status */
  #define PHY_NEGO_COMPLETE_BIT            (0x0020)     /*!< auto negotiation complete */
  /* phy specified control/status register */
  #define PHY_FULL_DUPLEX_100MBPS_BIT      (0x8000)     /*!< full duplex 100 mbps */
  #define PHY_HALF_DUPLEX_100MBPS_BIT      (0x4000)     /*!< half duplex 100 mbps */
  #define PHY_FULL_DUPLEX_10MBPS_BIT       (0x2000)     /*!< full duplex 10 mbps */
  #define PHY_HALF_DUPLEX_10MBPS_BIT       (0x1000)     /*!< half duplex 10 mbps */
#elif defined (DP83848)
  #define PHY_ADDRESS                      (0x01)       /*!< relative to at32 board */
  #define PHY_CONTROL_REG                  (0x00)       /*!< basic mode control register */
  #define PHY_STATUS_REG                   (0x01)       /*!< basic mode status register */
  #define PHY_SPECIFIED_CS_REG             (0x10)       /*!< phy status register */
  /* phy control register */
  #define PHY_AUTO_NEGOTIATION_BIT         (0x1000)     /*!< enable auto negotiation */
  #define PHY_LOOPBACK_BIT                 (0x4000)     /*!< enable loopback */
  #define PHY_RESET_BIT                    (0x8000)     /*!< reset phy */
  /* phy status register */
  #define PHY_LINKED_STATUS_BIT            (0x0004)     /*!< link status */
  #define PHY_NEGO_COMPLETE_BIT            (0x0020)     /*!< auto negotiation complete */

  #define PHY_DUPLEX_MODE                  (0x0004)     /*!< full duplex mode */
  #define PHY_SPEED_MODE                   (0x0002)     /*!< 10 mbps */
#endif

error_status emac_system_init(void);
void emac_nvic_configuration(void);
void emac_pins_configuration(void);
error_status emac_layer2_configuration(void);
void static reset_phy(void);
error_status emac_phy_register_reset(void);
error_status emac_speed_config(emac_auto_negotiation_type nego, emac_duplex_type mode, emac_speed_type speed);
error_status emac_phy_init(emac_control_config_type *control_para);

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif
