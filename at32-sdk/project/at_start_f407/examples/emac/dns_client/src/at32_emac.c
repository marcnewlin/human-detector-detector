/**
  **************************************************************************
  * @file     at32_emac.c
  * @version  v2.0.8
  * @date     2022-04-02
  * @brief    emac config program
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

/* includes ------------------------------------------------------------------*/
#include "at32f403a_407_board.h"
#include "at32_emac.h"

/** @addtogroup AT32F407_periph_examples
  * @{
  */

/** @addtogroup 407_EMAC_dns_client
  * @{
  */

/**
  * @brief  enable emac clock and gpio clock
  * @param  none
  * @retval success or error
  */
error_status emac_system_init(void)
{
  error_status status;

  emac_nvic_configuration();

  /* emac periph clock enable */
  crm_periph_clock_enable(CRM_EMAC_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_EMACTX_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_EMACRX_PERIPH_CLOCK, TRUE);

  emac_pins_configuration();
  status = emac_layer2_configuration();

  return status;
}

/**
  * @brief  configures emac irq channel.
  * @param  none
  * @retval none
  */
void emac_nvic_configuration(void)
{
  nvic_irq_enable(EMAC_IRQn, 1, 0);
}

/**
  * @brief  configures emac required pins.
  * @param  none
  * @retval none
  */
void emac_pins_configuration(void)
{
  gpio_init_type gpio_init_struct = {0};

  /* emac pins clock enable */
  crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOC_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOD_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOE_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_IOMUX_PERIPH_CLOCK, TRUE);

  gpio_default_para_init(&gpio_init_struct);

  #if RX_REMAP
  gpio_pin_remap_config(EMAC_MUX, TRUE);
  #endif

  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;

  /* pa2 -> mdio */
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pins = GPIO_PINS_2;
  gpio_init(GPIOA, &gpio_init_struct);

  /* pc1 -> mdc */
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pins = GPIO_PINS_1;
  gpio_init(GPIOC, &gpio_init_struct);

  #ifdef MII_MODE
  /*
    pb12 -> tx_d0
    pb13 -> tx_d1
    pc2  -> tx_d2
    pb8  -> tx_d3
    pb11 -> tx_en
    pc3  -> tx_clk
  */
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pins = GPIO_PINS_8 | GPIO_PINS_11 | GPIO_PINS_12 | GPIO_PINS_13;
  gpio_init(GPIOB, &gpio_init_struct);

  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pins = GPIO_PINS_2;
  gpio_init(GPIOC, &gpio_init_struct);

  gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
  gpio_init_struct.gpio_pins = GPIO_PINS_3;
  gpio_init(GPIOC, &gpio_init_struct);
  #if RX_REMAP
  /*
    pd8  -> rx_dv
    pd9  -> rx_d0
    pd10 -> rx_d1
    pd11 -> rx_d2
    pd12 -> rx_d3
  */
  gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
  gpio_init_struct.gpio_pins = GPIO_PINS_8 | GPIO_PINS_9 | GPIO_PINS_10 | GPIO_PINS_11 | GPIO_PINS_12;
  gpio_init(GPIOD, &gpio_init_struct);
  #else
  /*
    pa7  -> rx_dv
    pc4  -> rx_d0
    pc5  -> rx_d1
    pb0  -> rx_d2
    pb1  -> rx_d3
  */
  gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
  gpio_init_struct.gpio_pins = GPIO_PINS_7;
  gpio_init(GPIOA, &gpio_init_struct);

  gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
  gpio_init_struct.gpio_pins = GPIO_PINS_4 | GPIO_PINS_5;
  gpio_init(GPIOC, &gpio_init_struct);

  gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
  gpio_init_struct.gpio_pins = GPIO_PINS_0 | GPIO_PINS_1;
  gpio_init(GPIOB, &gpio_init_struct);
  #endif  /* RX_REMAP */
  /*
    pa1  -> rx_clk
    pa0  -> crs
    pa3  -> col
    pb10 -> rx_er
  */
  gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
  gpio_init_struct.gpio_pins = GPIO_PINS_0 | GPIO_PINS_1 | GPIO_PINS_3;
  gpio_init(GPIOA, &gpio_init_struct);

  gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
  gpio_init_struct.gpio_pins = GPIO_PINS_10;
  gpio_init(GPIOB, &gpio_init_struct);
  #endif  /* MII_MODE */

  #ifdef RMII_MODE
  /*
    pb12 -> tx_d0
    pb13 -> tx_d1
    pb11 -> tx_en
  */
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pins = GPIO_PINS_11 | GPIO_PINS_12 | GPIO_PINS_13;
  gpio_init(GPIOB, &gpio_init_struct);
  #if RX_REMAP
  /*
    pd8  -> rx_dv
    pd9  -> rx_d0
    pd10 -> rx_d1
  */
  gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
  gpio_init_struct.gpio_pins = GPIO_PINS_8 | GPIO_PINS_9 | GPIO_PINS_10;
  gpio_init(GPIOD, &gpio_init_struct);

  #else
  /*
    pa7  -> rx_dv
    pc4  -> rx_d0
    pc5  -> rx_d1
    pa1  -> ref_clk
  */
  gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
  gpio_init_struct.gpio_pins = GPIO_PINS_1 | GPIO_PINS_7;
  gpio_init(GPIOA, &gpio_init_struct);

  gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
  gpio_init_struct.gpio_pins = GPIO_PINS_4 | GPIO_PINS_5;
  gpio_init(GPIOC, &gpio_init_struct);
  #endif  /* RX_REMAP */
  #endif  /* RMII_MODE */
  /*
    pa1  -> ref_clk
  */
  gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
  gpio_init_struct.gpio_pins = GPIO_PINS_1;
  gpio_init(GPIOA, &gpio_init_struct);

  #if !CRYSTAL_ON_PHY
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pins = GPIO_PINS_8;
  gpio_init(GPIOA, &gpio_init_struct);
  #endif
}

/**
  * @brief  configures emac layer2
  * @param  none
  * @retval error or success
  */
error_status emac_layer2_configuration(void)
{
  emac_control_config_type mac_control_para;
  emac_dma_config_type dma_control_para;
  #ifdef MII_MODE
  gpio_pin_remap_config(MII_RMII_SEL_GMUX, FALSE);
  #elif defined RMII_MODE
  gpio_pin_remap_config(MII_RMII_SEL_GMUX, TRUE);
  #endif
  crm_clock_out_set(CRM_CLKOUT_SCLK);
  crm_clkout_div_set(CRM_CLKOUT_DIV_8);

  /* reset phy */
  reset_phy();
  /* reset emac ahb bus */
  emac_reset();

  /* software reset emac dma */
  emac_dma_software_reset_set();

  while(emac_dma_software_reset_get() == SET);

  emac_control_para_init(&mac_control_para);

  mac_control_para.auto_nego = EMAC_AUTO_NEGOTIATION_ON;

  if(emac_phy_init(&mac_control_para) == ERROR)
  {
    return ERROR;
  }

  emac_dma_para_init(&dma_control_para);

  dma_control_para.rsf_enable = TRUE;
  dma_control_para.tsf_enable = TRUE;
  dma_control_para.osf_enable = TRUE;
  dma_control_para.aab_enable = TRUE;
  dma_control_para.usp_enable = TRUE;
  dma_control_para.fb_enable = TRUE;
  dma_control_para.flush_rx_disable = TRUE;
  dma_control_para.rx_dma_pal = EMAC_DMA_PBL_32;
  dma_control_para.tx_dma_pal = EMAC_DMA_PBL_32;
  dma_control_para.priority_ratio = EMAC_DMA_2_RX_1_TX;

  emac_dma_config(&dma_control_para);
  emac_dma_interrupt_enable(EMAC_DMA_INTERRUPT_NORMAL_SUMMARY, TRUE);
  emac_dma_interrupt_enable(EMAC_DMA_INTERRUPT_RX, TRUE);

  return SUCCESS;
}

/**
  * @brief  reset layer 1
  * @param  none
  * @retval none
  */
void static reset_phy(void)
{
  gpio_init_type gpio_init_struct = {0};

  gpio_default_para_init(&gpio_init_struct);

  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pins = GPIO_PINS_8;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOC, &gpio_init_struct);

  gpio_bits_reset(GPIOC, GPIO_PINS_8);
  delay_ms(2);
  gpio_bits_set(GPIOC, GPIO_PINS_8);
  delay_us(2);
}

/**
  * @brief  reset phy register
  * @param  none
  * @retval SUCCESS or ERROR
  */
error_status emac_phy_register_reset(void)
{
  uint16_t data = 0;
  uint32_t timeout = 0;
  uint32_t i = 0;

  if(emac_phy_register_write(PHY_ADDRESS, PHY_CONTROL_REG, PHY_RESET_BIT) == ERROR)
  {
    return ERROR;
  }

  for(i = 0; i < 0x000FFFFF; i++);

  do
  {
    timeout++;
    if(emac_phy_register_read(PHY_ADDRESS, PHY_CONTROL_REG, &data) == ERROR)
    {
      return ERROR;
    }
  } while((data & PHY_RESET_BIT) && (timeout < PHY_TIMEOUT));

  for(i = 0; i < 0x00FFFFF; i++);
  if(timeout == PHY_TIMEOUT)
  {
    return ERROR;
  }
  return SUCCESS;
}

/**
  * @brief  set mac speed related parameters
  * @param  nego: auto negotiation on or off.
  *         this parameter can be one of the following values:
  *         - EMAC_AUTO_NEGOTIATION_OFF
  *         - EMAC_AUTO_NEGOTIATION_ON.
  * @param  mode: half-duplex or full-duplex.
  *         this parameter can be one of the following values:
  *         - EMAC_HALF_DUPLEX
  *         - EMAC_FULL_DUPLEX.
  * @param  speed: 10 mbps or 100 mbps
  *         this parameter can be one of the following values:
  *         - EMAC_SPEED_10MBPS
  *         - EMAC_SPEED_100MBPS.
  * @retval none
  */
error_status emac_speed_config(emac_auto_negotiation_type nego, emac_duplex_type mode, emac_speed_type speed)
{
  uint16_t data = 0;
  uint32_t timeout = 0;
  if(nego == EMAC_AUTO_NEGOTIATION_ON)
  {
    do
    {
      timeout++;
      if(emac_phy_register_read(PHY_ADDRESS, PHY_STATUS_REG, &data) == ERROR)
      {
        return ERROR;
      }
    } while(!(data & PHY_LINKED_STATUS_BIT) && (timeout < PHY_TIMEOUT));

    if(timeout == PHY_TIMEOUT)
    {
      return ERROR;
    }

    timeout = 0;

    if(emac_phy_register_write(PHY_ADDRESS, PHY_CONTROL_REG, PHY_AUTO_NEGOTIATION_BIT) == ERROR)
    {
      return ERROR;
    }


    do
    {
      timeout++;
      if(emac_phy_register_read(PHY_ADDRESS, PHY_STATUS_REG, &data) == ERROR)
      {
        return ERROR;
      }
    } while(!(data & PHY_NEGO_COMPLETE_BIT) && (timeout < PHY_TIMEOUT));

    if(timeout == PHY_TIMEOUT)
    {
      return ERROR;
    }

    if(emac_phy_register_read(PHY_ADDRESS, PHY_SPECIFIED_CS_REG, &data) == ERROR)
    {
      return ERROR;
    }
    #ifdef DM9162
    if(data & PHY_FULL_DUPLEX_100MBPS_BIT)
    {
      emac_fast_speed_set(EMAC_SPEED_100MBPS);
      emac_duplex_mode_set(EMAC_FULL_DUPLEX);
    }
    else if(data & PHY_HALF_DUPLEX_100MBPS_BIT)
    {
      emac_fast_speed_set(EMAC_SPEED_100MBPS);
      emac_duplex_mode_set(EMAC_HALF_DUPLEX);
    }
    else if(data & PHY_FULL_DUPLEX_10MBPS_BIT)
    {
      emac_fast_speed_set(EMAC_SPEED_10MBPS);
      emac_duplex_mode_set(EMAC_FULL_DUPLEX);
    }
    else if(data & PHY_HALF_DUPLEX_10MBPS_BIT)
    {
      emac_fast_speed_set(EMAC_SPEED_10MBPS);
      emac_duplex_mode_set(EMAC_HALF_DUPLEX);
    }
    #else
    if(data & PHY_DUPLEX_MODE)
    {
      emac_duplex_mode_set(EMAC_FULL_DUPLEX);
    }
    else
    {
      emac_duplex_mode_set(EMAC_HALF_DUPLEX);
    }
    if(data & PHY_SPEED_MODE)
    {
      emac_fast_speed_set(EMAC_SPEED_10MBPS);
    }
    else
    {
      emac_fast_speed_set(EMAC_SPEED_100MBPS);
    }
    #endif
  }
  else
  {
    if(emac_phy_register_write(PHY_ADDRESS, PHY_CONTROL_REG, (uint16_t)((mode << 8) | (speed << 13))) == ERROR)
    {
      return ERROR;
    }
    if(speed == EMAC_SPEED_100MBPS)
    {
      emac_fast_speed_set(EMAC_SPEED_100MBPS);
    }
    else
    {
      emac_fast_speed_set(EMAC_SPEED_10MBPS);
    }
    if(mode == EMAC_FULL_DUPLEX)
    {
      emac_duplex_mode_set(EMAC_FULL_DUPLEX);
    }
    else
    {
      emac_duplex_mode_set(EMAC_HALF_DUPLEX);
    }
  }

  return SUCCESS;
}

/**
  * @brief  initialize emac phy
  * @param  none
  * @retval SUCCESS or ERROR
  */
error_status emac_phy_init(emac_control_config_type *control_para)
{
  emac_clock_range_set();
  if(emac_phy_register_reset() == ERROR)
  {
    return ERROR;
  }
  if(emac_speed_config(control_para->auto_nego, control_para->duplex_mode, control_para->fast_ethernet_speed) == ERROR)
  {
    return ERROR;
  }

  emac_control_config(control_para);
  return SUCCESS;
}

/**
  * @}
  */

/**
  * @}
  */
