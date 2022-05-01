/**
  **************************************************************************
  * @file     main.c
  * @version  v2.0.8
  * @date     2022-04-02
  * @brief    main program
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

#include "at32f403a_407_board.h"
#include "at32f403a_407_clock.h"
#include "smartcard_config.h"

/** @addtogroup AT32F403A_periph_examples
  * @{
  */

/** @addtogroup 403A_USART_smartcard USART_smartcard
  * @{
  */

#define T0_PROTOCOL                      0x00  /* t0 protocol */
#define SETUP_LENGTH                     20
#define HIST_LENGTH                      20
#define SC_RECEIVE_TIMEOUT               0x4000 /* direction to reader */

/* atr structure - answer to reset */
typedef struct
{
  uint8_t ts;              /* bit convention */
  uint8_t t0;              /* high nibble = n. of setup byte; low nibble = n. of historical byte */
  uint8_t t[SETUP_LENGTH]; /* setup array */
  uint8_t h[HIST_LENGTH];  /* historical array */
  uint8_t tlength;         /* setup array dimension */
  uint8_t hlength;         /* historical array dimension */
} sc_atr_type;

sc_atr_type sc_a2r_struct;
uint32_t counter = 0;
error_status atr_decode_status = ERROR;
uint32_t card_inserted = 0, card_protocol = 1;
uint8_t dst_buffer[50]= {
                         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                         0, 0, 0, 0, 0, 0, 0, 0, 0, 0
                        };

/**
  * @brief  config usart
  * @param  none
  * @retval none
  */
void usart_configuration(void)
{
  gpio_init_type gpio_init_struct;
  exint_init_type exint_init_struct;

  /* enable gpio_3_5v, sc_usart_gpio_clk, gpio_reset, gpio_cmdvcc, gpio_off and iomux clocks */
  crm_periph_clock_enable(SC_3_5V_CLK, TRUE);
  crm_periph_clock_enable(SC_USART_GPIO_CLK, TRUE);
  crm_periph_clock_enable(SC_RESET_CLK, TRUE);
  crm_periph_clock_enable(SC_CMDVCC_CLK, TRUE);
  crm_periph_clock_enable(SC_OFF_CLK, TRUE);
  crm_periph_clock_enable(CRM_IOMUX_PERIPH_CLOCK, TRUE);

  /* enable sc_usart clocks */
  crm_periph_clock_enable(SC_USART_CLK, TRUE);

  gpio_default_para_init(&gpio_init_struct);
  /* configure sc_usart ck pin */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pins = SC_USART_CLK_PIN;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(SC_USART_CLK_PORT, &gpio_init_struct);

  /* configure sc_usart tx pin */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_OPEN_DRAIN;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pins = SC_USART_TX_PIN;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(SC_USART_TX_PORT, &gpio_init_struct);

  /* configure smartcard reset pin */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
  gpio_init_struct.gpio_pins = SC_RESET_PIN;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(SC_RESET_PORT, &gpio_init_struct);

  /* set reset pin high */
  gpio_bits_set(SC_RESET_PORT, SC_RESET_PIN);

  /* configure smartcard 3/5v pin */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
  gpio_init_struct.gpio_pins = SC_3_5V_PIN;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(SC_3_5V_PORT, &gpio_init_struct);

  /* select 5 v */
  gpio_bits_set(SC_3_5V_PORT, SC_3_5V_PIN);

  /* configure smartcard cmdvcc pin */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
  gpio_init_struct.gpio_pins = SC_CMDVCC_PIN;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(SC_CMDVCC_PORT, &gpio_init_struct);

  /* set smartcard cmdvcc */
  gpio_bits_set(SC_CMDVCC_PORT, SC_CMDVCC_PIN);

  /* configure smartcard off pin */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
  gpio_init_struct.gpio_pins = SC_OFF_PIN;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(SC_OFF_PORT, &gpio_init_struct);

  /* nvic interrupt init */
  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);
  NVIC_ClearPendingIRQ(SC_OFF_EXINT_IRQ);
  nvic_irq_enable(SC_OFF_EXINT_IRQ, 0, 0);
  nvic_irq_enable(SC_USART_IRQn, 1, 0);

  /* configure smartcard off exint */
  gpio_exint_line_config(SC_OFF_PORTSOURCE, SC_OFF_PINSOURCE);

  exint_default_para_init(&exint_init_struct);
  exint_init_struct.line_mode = EXINT_LINE_INTERRUPUT;
  exint_init_struct.line_polarity = EXINT_TRIGGER_RISING_EDGE;
  exint_init_struct.line_select = SC_OFF_EXINT;
  exint_init_struct.line_enable = TRUE;
  exint_init(&exint_init_struct);

  /* clear sc exint line pending bit */
  exint_flag_clear(SC_OFF_EXINT);

  /* sc_usart clock set to 4.28mhz (apbclk = 120 mhz / 28) */
  usart_irda_smartcard_division_set(SC_USART, 14);
  /* sc_usart guard time set to 2 bit */
  usart_smartcard_guard_time_set(SC_USART, 0x2);

  /* configure sc usart clk */
  usart_clock_config(SC_USART, USART_CLOCK_POLARITY_LOW, USART_CLOCK_PHASE_1EDGE, USART_CLOCK_LAST_BIT_OUTPUT);
  usart_clock_enable(SC_USART, TRUE);

  /* configure sc usart param */
  usart_init(SC_USART, 12096, USART_DATA_9BITS, USART_STOP_1_5_BIT);
  usart_parity_selection_config(SC_USART, USART_PARITY_EVEN);
  usart_transmitter_enable(SC_USART, TRUE);
  usart_receiver_enable(SC_USART, TRUE);

  /* enable the sc_usart parity error interrupt */
  usart_interrupt_enable(SC_USART, USART_PERR_INT, TRUE);
  usart_enable(SC_USART, TRUE);

  /* enable the nack transmission */
  usart_smartcard_nack_set(SC_USART, TRUE);

  /* enable the smartcard mode */
  usart_smartcard_mode_enable(SC_USART, TRUE);
}

/**
  * @brief  decode the card atr response.
  * @param  card: pointer to the buffer containing the card atr.
  * @retval card protocol
  */
uint8_t sc_response_decode(__IO uint8_t *card)
{
  uint32_t i = 0, flag = 0, buf = 0, protocol = 0;

  sc_a2r_struct.ts = 0;
  sc_a2r_struct.t0 = 0;
  for(i = 0; i < SETUP_LENGTH; i++)
  {
    sc_a2r_struct.t[i] = 0;
  }
  for(i = 0;i < HIST_LENGTH; i++)
  {
    sc_a2r_struct.h[i] = 0;
  }
  sc_a2r_struct.tlength = 0;
  sc_a2r_struct.hlength = 0;

  /* initial character */
  sc_a2r_struct.ts = card[0];

  /* format character */
  sc_a2r_struct.t0 = card[1];

  sc_a2r_struct.hlength = sc_a2r_struct.t0 & 0x0F;

  if((sc_a2r_struct.t0 & 0x80) == 0x80) flag = 1;

  for(i = 0; i < 4; i++)
  {
    sc_a2r_struct.tlength = sc_a2r_struct.tlength + (((sc_a2r_struct.t0 & 0xF0) >> (4 + i)) & 0x1);
  }

  for(i = 0; i < sc_a2r_struct.tlength; i++)
  {
    sc_a2r_struct.t[i] = card[i + 2];
  }

  protocol = sc_a2r_struct.t[sc_a2r_struct.tlength - 1] & 0x0F;

  while(flag)
  {
    if((sc_a2r_struct.t[sc_a2r_struct.tlength - 1] & 0x80)== 0x80)
    {
      flag = 1;
    }
    else
    {
      flag = 0;
    }
    buf = sc_a2r_struct.tlength;
    sc_a2r_struct.tlength = 0;

    for(i = 0; i < 4; i++)
    {
      sc_a2r_struct.tlength = sc_a2r_struct.tlength + (((sc_a2r_struct.t[buf - 1] & 0xF0) >> (4 + i)) & 0x1);
    }
    for(i = 0; i < sc_a2r_struct.tlength; i++)
    {
      sc_a2r_struct.t[buf + i] = card[i + 2 + buf];
    }
    sc_a2r_struct.tlength += buf;
  }

  for(i = 0;i < sc_a2r_struct.hlength; i++)
  {
    sc_a2r_struct.h[i] = card[i + 2 + sc_a2r_struct.tlength];
  }

  return ((uint8_t)protocol);
}

/**
  * @brief  main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  uint32_t index = 0;
  system_clock_config();
  at32_board_init();
  usart_configuration();

  /* loop while no smartcard is detected */
  while(card_inserted == 0)
  {
  }

  /* read smartcard atr response */
  for(index = 0; index < 40; index++, counter = 0)
  {
    while((usart_flag_get(SC_USART, USART_RDBF_FLAG) == RESET) && (counter != SC_RECEIVE_TIMEOUT))
    {
      counter++;
    }

    if(counter != SC_RECEIVE_TIMEOUT)
    {
      dst_buffer[index] = usart_data_receive(SC_USART);
    }
  }

  /* decode atr */
  card_protocol = sc_response_decode(dst_buffer);

  /* test if the inserted card is iso7816-3 t=0 compatible */
  if(card_protocol == 0)
  {
    /* inserted card is iso7816-3 t=0 compatible */
    atr_decode_status = SUCCESS;
  }
  else
  {
    /* inserted smartcard is not iso7816-3 t=0 compatible */
    atr_decode_status = ERROR;
  }

  /* judge whether the result is correct */
  if(atr_decode_status == SUCCESS)
  {
    /* trun on the led */
    at32_led_on(LED2);
    at32_led_on(LED3);
    at32_led_on(LED4);
  }

  while(1)
  {
  }
}

/**
  * @}
  */

/**
  * @}
  */
