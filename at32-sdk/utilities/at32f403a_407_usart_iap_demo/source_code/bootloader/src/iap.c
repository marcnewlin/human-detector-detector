/**
  **************************************************************************
  * @file     iap.c
  * @version  v2.0.8
  * @date     2022-04-02
  * @brief    iap program
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

#include "iap.h"
#include "usart.h"
#include "flash.h"
#include "tmr.h"

/** @addtogroup UTILITIES_examples
  * @{
  */

/** @addtogroup USART_IAP_bootloader
  * @{
  */

cmd_data_step_type cmd_data_step = CMD_DATA_IDLE;
cmd_ctr_step_type cmd_ctr_step = CMD_CTR_IDLE;
cmd_data_group_type cmd_data_group_struct;
update_status_type update_status = UPDATE_PRE;
static uint8_t cmd_addr_cnt = 0;
static uint32_t cmd_data_cnt = 0;
iapfun jump_to_app;

/* app_load don't optimize */
#if defined (__CC_ARM)
  #pragma O0
#elif defined (__ICCARM__)
  #pragma optimize=s none
#endif
/**
  * @brief  app load.
  * @param  app_addr
  *         app code starting address
  * @retval none
  */
void app_load(uint32_t app_addr)
{
  /* check the address of stack */
  if(((*(uint32_t*)app_addr) - 0x20000000) < (SRAM_SIZE * 1024))
  {
    /* disable periph clock */
    crm_periph_clock_enable(CRM_TMR3_PERIPH_CLOCK, FALSE);
    crm_periph_clock_enable(CRM_USART1_PERIPH_CLOCK, FALSE);
    crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, FALSE);

    /* disable nvic irq and clear pending */
    nvic_irq_disable(USART1_IRQn);
    nvic_irq_disable(TMR3_GLOBAL_IRQn);
    __NVIC_ClearPendingIRQ(USART1_IRQn);
    __NVIC_ClearPendingIRQ(TMR3_GLOBAL_IRQn);

    jump_to_app = (iapfun)*(uint32_t*)(app_addr + 4);        /* code second word is reset address */
    __set_MSP(*(uint32_t*)app_addr);                        /* init app stack pointer(code first word is stack address) */
    jump_to_app();                                          /* jump to user app */
  }
}

/**
  * @brief  take data from usart buf.
  * @param  app_addr
  *         app code starting address
  * @retval val
  *         took data
  */
uint8_t data_take(void)
{
  uint8_t val;
  usart_interrupt_enable(USART1, USART_RDBF_INT, FALSE);
  usart_group_struct.count--;
  usart_interrupt_enable(USART1, USART_RDBF_INT, TRUE);
  val = usart_group_struct.buf[usart_group_struct.tail++];
  if(usart_group_struct.tail > (USART_REC_LEN-1))
    usart_group_struct.tail = 0;
  return val;
}

/**
  * @brief  command analysis handle.
  * @param  none
  * @retval none
  */
void command_handle(void)
{
  uint8_t val, checksum;
  uint16_t index;
  /* check whether received usart data */
  if(usart_group_struct.count > 0)
    val = data_take();
  else
    return;

  if(update_status == UPDATE_PRE)
  {
    if(cmd_ctr_step == CMD_CTR_IDLE)
    {
      if(val == 0x5A)
        cmd_ctr_step = CMD_CTR_INIT;
    }
    else if(cmd_ctr_step == CMD_CTR_INIT)
    {
      if(val == 0x01)
      {
        cmd_ctr_step = CMD_CTR_DONE;
      }
      else if(val == 0xA5)
      {
        cmd_ctr_step = CMD_CTR_APP;
      }
      else
      {
        cmd_ctr_step = CMD_CTR_ERR;
      }
    }
  }
  else if(update_status == UPDATE_ING)
  {
    switch(cmd_data_step)
    {
      case CMD_DATA_IDLE:
        if(val == 0x31)
        {
          cmd_data_step = CMD_DATA_ADDR;
          cmd_addr_cnt = 0;
          cmd_data_cnt = 0;
        }
        if(val == 0x5A)
        {
          if(cmd_ctr_step == CMD_CTR_IDLE)
          {
            cmd_ctr_step = CMD_CTR_INIT;
            update_status = UPDATE_DONE;
          }
        }
        break;
      case CMD_DATA_ADDR:
        cmd_data_group_struct.cmd_addr[cmd_addr_cnt] = val;
        cmd_addr_cnt++;
        if(cmd_addr_cnt >= 4)
        {
          cmd_addr_cnt = 0;
          cmd_data_step = CMD_DATA_BUF;
        }
        break;
      case CMD_DATA_BUF:
        cmd_data_group_struct.cmd_buf[cmd_data_cnt] = val;
        cmd_data_cnt++;
        if(cmd_data_cnt >= 2048)
        {
          cmd_data_cnt = 0;
          cmd_data_step = CMD_DATA_CHACK;
        }
        break;
      case CMD_DATA_CHACK:
        checksum = 0;
        for(index = 0;index < 4;index++)
        {
          checksum += cmd_data_group_struct.cmd_addr[index];
        }
        for(index = 0;index < 0x800; index++)
        {
          checksum += cmd_data_group_struct.cmd_buf[index];
        }
        if(checksum == val)
        {
          cmd_data_step = CMD_DATA_DONE;
        }
        else
        {
          cmd_data_step = CMD_DATA_ERR;
        }
        break;
      default:
        break;
    }
  }
  else if(update_status == UPDATE_DONE)
  {
    if(cmd_ctr_step == CMD_CTR_IDLE)
    {
      if(val == 0x5A)
      {
        cmd_ctr_step = CMD_CTR_INIT;
      }
    }
    else if(cmd_ctr_step == CMD_CTR_INIT)
    {
      if(val == 0x02)
      {
        cmd_ctr_step = CMD_CTR_DONE;
      }
      else
      {
        cmd_ctr_step = CMD_CTR_ERR;
      }
    }
  }
}

/**
  * @brief  command response ok.
  * @param  none
  * @retval none
  */
void back_ok(void)
{
  usart_data_transmit(USART1, 0xCC);
  while(usart_flag_get(USART1, USART_TDC_FLAG) == RESET);
  usart_data_transmit(USART1, 0xDD);
  while(usart_flag_get(USART1, USART_TDC_FLAG) == RESET);
}

/**
  * @brief  command response error.
  * @param  none
  * @retval none
  */
void back_err(void)
{
  usart_data_transmit(USART1, 0xEE);
  while(usart_flag_get(USART1, USART_TDC_FLAG) == RESET);
  usart_data_transmit(USART1, 0xFF);
  while(usart_flag_get(USART1, USART_TDC_FLAG) == RESET);
  /* reset state machine of udgrade flow */
  cmd_data_step = CMD_DATA_IDLE;
  cmd_ctr_step = CMD_CTR_IDLE;
  update_status = UPDATE_PRE;
  cmd_addr_cnt = 0;
  cmd_data_cnt = 0;
  time_ira_cnt = 0;
  get_data_from_usart_flag = 0;
}

/**
  * @brief  app update flow handle.
  * @param  none
  * @retval none
  */
void app_update_handle(void)
{
  uint32_t write_addr=0;
  if(update_status == UPDATE_PRE)
  {
    if(cmd_ctr_step == CMD_CTR_DONE)
    {
      cmd_ctr_step = CMD_CTR_IDLE;
      update_status = UPDATE_CLEAR_FLAG;
    }
    else if(cmd_ctr_step == CMD_CTR_APP)
    {
      cmd_ctr_step = CMD_CTR_IDLE;
      back_ok();
    }
    else if(cmd_ctr_step == CMD_CTR_ERR)
    {
      cmd_ctr_step = CMD_CTR_IDLE;
      back_err();
    }
  }
  else if(update_status == UPDATE_CLEAR_FLAG)
  {
    get_data_from_usart_flag = 1;
    update_status = UPDATE_ING;
    back_ok();
  }
  else if(update_status == UPDATE_ING)
  {
    if(cmd_data_step == CMD_DATA_DONE)
    {
      write_addr = (cmd_data_group_struct.cmd_addr[0] << 24) + (cmd_data_group_struct.cmd_addr[1] << 16) + \
                   (cmd_data_group_struct.cmd_addr[2] << 8) + cmd_data_group_struct.cmd_addr[3];
      if((write_addr >= APP_START_ADDR) && (write_addr < FLASH_BASE + 1024 * FLASH_SIZE))
      {
        flash_2kb_write(write_addr, cmd_data_group_struct.cmd_buf);
        cmd_data_step = CMD_DATA_IDLE;
        back_ok();
      }
      else
      {
        cmd_data_step = CMD_DATA_IDLE;
        back_err();
      }
    }
    else if(cmd_data_step == CMD_DATA_ERR)
    {
      cmd_data_step = CMD_DATA_IDLE;
      back_err();
    }
  }
  else if(update_status == UPDATE_DONE)
  {
    if(cmd_ctr_step == CMD_CTR_DONE)
    {
      cmd_ctr_step = CMD_CTR_IDLE;
      back_ok();
      /* check app starting address whether 0x08xxxxxx */
      if(((*(uint32_t*)(APP_START_ADDR + 4)) & 0xFF000000) == 0x08000000)
      {
        crm_reset();
        /* jump and run in app */
        app_load(APP_START_ADDR);
      }
      else
      {
        cmd_ctr_step = CMD_CTR_ERR;
      }
    }
    else if(cmd_ctr_step == CMD_CTR_ERR)
    {
      cmd_ctr_step = CMD_CTR_IDLE;
      back_err();
    }
  }
}

/**
  * @brief  iap upgrade app handle.
  * @param  none
  * @retval none
  */
void iap_upgrade_app_handle(void)
{
  command_handle();
  app_update_handle();
}

/**
  * @}
  */

/**
  * @}
  */
