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

/** @addtogroup AT32F407_periph_examples
  * @{
  */

/** @addtogroup 407_BPR_data BPR_data
  * @{
  */

#define BPR_DR_NUMBER                    42

bpr_data_type bpr_addr_tab[BPR_DR_NUMBER] =
{
  BPR_DATA1,  BPR_DATA2,  BPR_DATA3,  BPR_DATA4,  BPR_DATA5,
  BPR_DATA6,  BPR_DATA7,  BPR_DATA8,  BPR_DATA9,  BPR_DATA10,
  BPR_DATA11, BPR_DATA12, BPR_DATA13, BPR_DATA14, BPR_DATA15,
  BPR_DATA16, BPR_DATA17, BPR_DATA18, BPR_DATA19, BPR_DATA20,
  BPR_DATA21, BPR_DATA22, BPR_DATA23, BPR_DATA24, BPR_DATA25,
  BPR_DATA26, BPR_DATA27, BPR_DATA28, BPR_DATA29, BPR_DATA30,
  BPR_DATA31, BPR_DATA32, BPR_DATA33, BPR_DATA34, BPR_DATA35,
  BPR_DATA36, BPR_DATA37, BPR_DATA38, BPR_DATA39, BPR_DATA40,
  BPR_DATA41, BPR_DATA42
};

void bpr_reg_write(void);
uint8_t bpr_reg_check(void);
void uart_init(uint32_t baudrate);

/**
  * @brief  write data to bpr dt register.
  * @param  none.
  * @retval none
  */
void bpr_reg_write(void)
{
  uint32_t index = 0;

  for(index = 0; index < BPR_DR_NUMBER; index++)
  {
    bpr_data_write(bpr_addr_tab[index], 0x5A00 | bpr_addr_tab[index]);
  }
}

/**
  * @brief  checks if the bpr dt register values are correct or not.
  * @param  none.
  * @retval
  *          - TRUE: all bpr dt register values are correct
  *          - FALSE: number of the bpr register
  *            which value is not correct
  */
uint8_t bpr_reg_check(void)
{
  uint32_t index = 0;

  for(index = 0; index < BPR_DR_NUMBER; index++)
  {
    if(bpr_data_read(bpr_addr_tab[index]) != (0x5A00 | bpr_addr_tab[index]))
    {
      return FALSE;
    }
  }

  return TRUE;
}

/**
  * @brief  main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  system_clock_config();

  uart_print_init(115200);

  /* enable pwc and bpr clock */
  crm_periph_clock_enable(CRM_PWC_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_BPR_PERIPH_CLOCK, TRUE);

  /* enable write access to bpr domain */
  pwc_battery_powered_domain_access(TRUE);

  /* clear tamper pin event pending flag */
  bpr_flag_clear(BPR_TAMPER_EVENT_FLAG);

  /* check data from bpr dt register */
  if(bpr_reg_check() == TRUE)
  {
    printf("bpr reg => none reset\r\n");
  }
  else
  {
    printf("bpr reg => reset\r\n");
  }

  /* reset bpr domain */
  bpr_reset();

  /* write data to bpr dt register */
  bpr_reg_write();

  /* check if the written data are correct */
  if(bpr_reg_check() == TRUE)
  {
    printf("write bpr reg ok\r\n");
  }
  else
  {
    printf("write bpr reg fail\r\n");
  }

  /* disable pwc and bpr clock for power saving */
  crm_periph_clock_enable(CRM_PWC_PERIPH_CLOCK, FALSE);
  crm_periph_clock_enable(CRM_BPR_PERIPH_CLOCK, FALSE);

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
