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
#include "at32_emac.h"
#include "netconf.h"
#include "tcp_client.h"

/** @addtogroup AT32F407_periph_examples
  * @{
  */

/** @addtogroup 407_EMAC_tcp_client EMAC_tcp_client
  * @{
  */

#define DELAY                            100
#define FAST                             1
#define SLOW                             4

#define TCP_CLIENT_TEST_DATA             "tcp client experiment!\n"
uint8_t g_speed = FAST;
volatile uint32_t local_time = 0;

/**
  * @brief  main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  struct tcp_pcb *pcb;
  error_status status;

  system_clock_config();

  at32_board_init();

  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);

  delay_init();

  status = emac_system_init();

  while(status == ERROR);

  tcpip_stack_init();

  tcp_client_init(TCP_LOCAL_PORT,TCP_SERVER_PORT,TCP_SERVER_IP);

  for(;;)
  {
    pcb = check_tcp_connect();

    if(pcb != NULL)
    {
      tcp_client_send_data(pcb, TCP_CLIENT_TEST_DATA, sizeof(TCP_CLIENT_TEST_DATA));      //Send data to TCP server actively
    }

    at32_led_toggle(LED2);
    delay_ms(g_speed * DELAY);
    at32_led_toggle(LED3);
    delay_ms(g_speed * DELAY);
    at32_led_toggle(LED4);
    delay_ms(g_speed * DELAY);

    delay_sec(1);
  }
}

/**
  * @}
  */

/**
  * @}
  */
