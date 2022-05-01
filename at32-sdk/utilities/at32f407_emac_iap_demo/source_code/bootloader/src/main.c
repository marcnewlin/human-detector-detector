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
#include "stdio.h"
#include "stdlib.h"
#include "netconf.h"
#include "flash.h"
#include "iap.h"
#include "tmr.h"
#include "httpserver.h"
#include "tftpserver.h"

/** @addtogroup UTILITIES_examples
  * @{
  */

/** @addtogroup EMAC_IAP_bootloader
  * @{
  */

/**
  * @brief  main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  error_status status;
  uint32_t useraddr;
  system_clock_config();

  at32_board_init();

  delay_init();

  /* config nvic priority group */
  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);

   /* check iap upgrade */
  if(at32_button_press()==USER_BUTTON)
  {
    iap_command_handle();
  }

  /* check iap_upgrade_flag flag */
  if(flash_upgrade_flag_read() == RESET)
  {
    /* check app starting address whether 0x08xxxxxx */
    if(((*(uint32_t*)(APP_START_SECTOR_ADDR + 4)) & 0xFF000000) == 0x08000000)
      app_load(APP_START_SECTOR_ADDR);
  }

  status = emac_system_init();

  while(status == ERROR);

  tcpip_stack_init();

  tmr_init();

  /* init flash */
  flash_unlock();

  for(useraddr = APP_START_SECTOR_ADDR; useraddr <= APP_LAST_SECTOR_ADDR; useraddr += 2048)
  {
    flash_sector_erase(useraddr);
	 }
#ifdef USE_IAP_HTTP
  /* initialize the http server */
  iap_httpd_init();
#endif

#ifdef USE_IAP_TFTP
  /* initialize the tftp server */
  iap_tftpd_init();
#endif

  while(1);
}

/**
  * @}
  */

/**
  * @}
  */
