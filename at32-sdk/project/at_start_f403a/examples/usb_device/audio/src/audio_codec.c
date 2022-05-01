/**
  **************************************************************************
  * @file     audio_codec.c
  * @version  v2.0.8
  * @date     2022-04-02
  * @brief    audio codec function
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
#include "audio_codec.h"
#include "i2c_application.h"
#include "audio_conf.h"
#include <string.h>

/** @addtogroup AT32F403A_periph_examples
  * @{
  */

/** @addtogroup 403A_USB_device_audio
  * @{
  */

#define I2C_TIMEOUT                      0xFFFFFFFF

#define I2Cx_SPEED                       200000
#define I2Cx_ADDRESS                     0x00

#define I2Cx_PORT                        I2C1
#define I2Cx_CLK                         CRM_I2C1_PERIPH_CLOCK

#define I2Cx_SCL_PIN                     GPIO_PINS_8
#define I2Cx_SCL_GPIO_PORT               GPIOB
#define I2Cx_SCL_GPIO_CLK                CRM_GPIOB_PERIPH_CLOCK

#define I2Cx_SDA_PIN                     GPIO_PINS_9
#define I2Cx_SDA_GPIO_PORT               GPIOB
#define I2Cx_SDA_GPIO_CLK                CRM_GPIOB_PERIPH_CLOCK

/**
  * @brief  wm8988 freq
  */
#define WM8988_REG_FREQ48K               ((WM8988_R8_SAMPLE_RATE << 9) | 0x0081)
#define WM8988_REG_FREQ16K               ((WM8988_R8_SAMPLE_RATE << 9) | 0x0095)

/**
  * @brief  wm8988 bit width
  */
#define WM8988_REG_BITW16                ((WM8988_R7_AUDIO_INTERFACE << 9) | 0x0042)

/**
  * @brief  wm8988 register default value
  */
uint16_t reg_addr_data[] =
{
  (WM8988_R0_LEFT_INPUT_VOLUME << 9) |          0x012F,                /*Left Input Channel Volume*/
  (WM8988_R1_RIGHT_INPUT_VOLUME << 9) |         0x012F,                /*Right Input Channel Volume*/
  (WM8988_R2_LOUT1_VOLUME << 9) |               0x0179,                /*Left Output Channel Volume*/
  (WM8988_R3_ROUT1_VOLUME << 9) |               0x0179,                /*Right Output Channel Volume*/
  (WM8988_R5_ADC_DAC_CONTROL << 9) |            0x0006,                /*De-emphasis Control and Digital soft mute*/
  (WM8988_REG_BITW16),
  (WM8988_REG_FREQ16K),
  (WM8988_R10_LEFT_DAC_VOLUME << 9) |           0x01FF,                /*Left Digital DAC Volume Control*/
  (WM8988_R11_RIGHT_DAC_VOLUME << 9) |          0x01FF,                /*Right Digital DAC Volume Control*/
  (WM8988_R12_BASS_CONTROL << 9) |              0x000F,                /*Bass Control*/
  (WM8988_R13_TREBLE_CONTROL << 9) |            0x000F,                /*Treble Control*/
  (WM8988_R16_3D_CONTROL << 9) |                0x0000,                /*3D stereo Enhancment*/
  (WM8988_R21_LEFT_ADC_VOLUME << 9) |           0x01C3,                /*Left ADC Digital Volume*/
  (WM8988_R22_RIGHT_ADC_VOLUME << 9) |          0x01C3,                /*Right ADC Digital Volume*/
  (WM8988_R23_ADDITIONAL_1_CONTROL << 9) |      0x00C2,                /*Additional Control 1*/
  (WM8988_R24_ADDITIONAL_2_CONTROL << 9) |      0x0000,                /*Additional Control 2*/
  (WM8988_R27_ADDITIONAL_3_CONTROL << 9) |      0x0000,                /*Additional Control 3*/
  (WM8988_R31_ADC_INPUT_MODE << 9) |            0x0000,                /*ADC input mode*/
  (WM8988_R32_ADC_L_SIGNAL_PATH << 9) |         0x0000,                /*ADC Signal Path Control left*/
  (WM8988_R33_ADC_R_SIGNAL_PATH << 9) |         0x0000,                /*ADC Signal Path Control right*/
  (WM8988_R34_LEFT_OUT_MIX_1 << 9) |            0x0152,                /*Left DAC mixer Control*/
  (WM8988_R35_LEFT_OUT_MIX_2 << 9) |            0x0050,                /*Left DAC mixer Control*/
  (WM8988_R36_RIGHT_OUT_MIX_1 << 9) |           0x0052,                /*Right DAC mixer Control*/
  (WM8988_R37_RIGHT_OUT_MIX_2 << 9) |           0x0150,                /*Right DAC mixer Control*/
  (WM8988_R40_LOUT2_VOLUME << 9) |              0x01FF,                /*Output left channel volume*/
  (WM8988_R41_ROUT2_VOLUME << 9) |              0x01FF,                /*Output left channel volume*/
  (WM8988_R43_LOW_POWER_PALYBACK << 9) |        0x0008,                /*Output left channel volume*/
  (WM8988_R25_PWR_1_MGMT << 9) |                0x017C,                /*Power Management1*/
  (WM8988_R26_PWR_2_MGMT << 9) |                0x01F8,                /*Power Management2*/
};

i2c_handle_type hi2cx;

audio_codec_type audio_codec;
uint16_t spk_dma_buffer[DMA_BUFFER_SIZE];
uint16_t mic_dma_buffer[DMA_BUFFER_SIZE];

void memset16_buffer(uint16_t *buffer, uint32_t set, uint32_t len);
void codec_i2s_reset(void);
void codec_i2s_init(audio_codec_type *param);
void mclk_tmr1_init(void);
void copy_buff(uint16_t *dest, uint16_t *src, uint32_t len);

/**
  * @brief  audio codec set microphone freq
  * @param  freq: freq (wm8988 microphone and speaker must same freq)
  * @retval none
  */
void audio_codec_set_mic_freq(uint32_t freq)
{
  if(audio_codec.audio_freq != freq)
  {
    audio_codec.audio_freq = freq;
    codec_i2s_reset();
    audio_codec_modify_freq(freq);
    codec_i2s_init(&audio_codec);
  }
}

/**
  * @brief  audio codec set speaker freq
  * @param  freq: freq(wm8988 microphone and speaker must same freq)
  * @retval none
  */
void audio_codec_set_spk_freq(uint32_t freq)
{
  if(audio_codec.audio_freq != freq)
  {
    audio_codec.audio_freq = freq;
    codec_i2s_reset();
    audio_codec_modify_freq(freq);
    codec_i2s_init(&audio_codec);
  }
}


/**
  * @brief  audio codec speaker alt setting config
  * @param  none
  * @retval none
  */
void audio_codec_spk_alt_setting(uint32_t alt_seting)
{
  if(alt_seting == 0)
  {
  }
  else
  {
  }
}

/**
  * @brief  audio codec microphone alt setting config
  * @param  none
  * @retval none
  */
void audio_codec_mic_alt_setting(uint32_t alt_seting)
{
  if(alt_seting == 0)
  {
  }
  else
  {
  }
}

/**
  * @brief  audio codec set microphone mute
  * @param  mute: mute state
  * @retval none
  */
void audio_codec_set_mic_mute(uint8_t mute)
{
  audio_codec.mic_mute = mute;
}


/**
  * @brief  audio codec set speaker mute
  * @param  mute: mute state
  * @retval none
  */
void audio_codec_set_spk_mute(uint8_t mute)
{
  audio_codec.spk_mute = mute;
}


/**
  * @brief  audio codec set microphone volume
  * @param  volume: the new volume
  * @retval none
  */
void audio_codec_set_mic_volume(uint16_t volume)
{
  /* wm8988 adc have 256 steps */
  audio_codec.mic_volume = volume;
}

/**
  * @brief  audio codec set speaker volume
  * @param  volume: the new volume
  * @retval none
  */
void audio_codec_set_spk_volume(uint16_t volume)
{
  /* wm8988 dac have 80 steps */
  audio_codec.spk_volume = (volume * 0x50) / 0x64 + 0x30 - 1;
}

/**
  * @brief  codec speaker feedback
  * @param  feedback: data buffer
  * @retval feedback len
  */
uint8_t audio_codec_spk_feedback(uint8_t *feedback)
{
  uint32_t feedback_value = (audio_codec.spk_freq);
  feedback_value = ((feedback_value/1000)<<14)|((feedback_value%1000)<<4);
  feedback[0] = (uint8_t)(feedback_value);
  feedback[1] = (uint8_t)(feedback_value >> 8);
  feedback[2] = (uint8_t)(feedback_value >> 16);
  return 3;
}

/**
  * @brief  codec speaker write fifo
  * @param  data: data buffer
  * @param  len: data length
  * @retval none
  */
void audio_codec_spk_fifo_write(uint8_t *data, uint32_t len)
{
  uint16_t ulen = len / 2, i;
  uint16_t *u16data = (uint16_t *)data;

  switch(audio_codec.spk_stage)
  {
    case 0:
      audio_codec.spk_woff = audio_codec.spk_roff = audio_codec.spk_buffer;
      audio_codec.spk_wtotal = audio_codec.spk_rtotal = 0;
      audio_codec.spk_stage = 1;
      audio_codec.spk_threshold = SPK_BUFFER_SIZE/2;
      break;
    case 1:
      if( audio_codec.spk_wtotal >= SPK_BUFFER_SIZE/2 )
      {
        audio_codec.spk_stage = 2;
      }
      break;
    case 2:
      break;
  }
  for( i = 0; i < ulen; ++i )
  {
    *audio_codec.spk_woff++ = *u16data++;
    if( audio_codec.spk_woff >= audio_codec.spk_rend )
    {
      audio_codec.spk_woff = audio_codec.spk_buffer;
    }
  }
  audio_codec.spk_wtotal += ulen;
}

/**
  * @brief  codec microphone get data
  * @param  data: data buffer
  * @retval data len
  */
uint32_t audio_codec_mic_get_data(uint8_t *buffer)
{
  //copy_buff((uint16_t *)buffer, audio_codec.mic_buffer + audio_codec.mic_hf_status, audio_codec.mic_rx_size);
  uint16_t len = audio_codec.mic_rx_size << 1;
  uint16_t i;
  uint16_t* u16buf = (uint16_t*)buffer;
  switch(audio_codec.mic_stage)
  {
    case 0:
      audio_codec.mic_stage = 1;
      memset( buffer, 0, len );
      return len;
    case 1:
      if( audio_codec.mic_wtotal - audio_codec.mic_rtotal >= MIC_BUFFER_SIZE/2 )
      {
        audio_codec.mic_stage = 2;
      }
      return len;
  }
  switch( audio_codec.mic_adj_stage )
  {
    case 0:
      break;
    case 1:
      if( audio_codec.mic_adj_count >= 4 ){
        len += 4;
        audio_codec.mic_adj_count -= 4;
      }
      break;
    case 2:
      if( audio_codec.mic_adj_count >= 4 ){
        len -= 4;
        audio_codec.mic_adj_count -= 4;
      }
      break;
  }

  for( i = 0; i < len/2; ++i )
  {
    *u16buf++ = *audio_codec.mic_roff++;
    if( audio_codec.mic_roff >= audio_codec.mic_wend )
    {
      audio_codec.mic_roff = audio_codec.mic_buffer;
    }
  }
  if( audio_codec.mic_wtotal <= audio_codec.mic_rtotal )
  { //should not happen
    while(1);
  }

  audio_codec.mic_rtotal += len/2;
  audio_codec.mic_delta  += len/2;
  return len;
}

/**
  * @brief  audio codec modify freq
  * @param  freq: freq (wm8988 microphone and speaker must as same freq)
  * @retval none
  */
void audio_codec_modify_freq(uint32_t freq)
{
  uint16_t codec_cmd;
  uint8_t i2c_cmd[2];
  if(freq == AUDIO_FREQ_16K)
  {
    codec_cmd = WM8988_REG_FREQ16K;
  }
  else if(freq == AUDIO_FREQ_48K)
  {
    codec_cmd = WM8988_REG_FREQ48K;
  }
  i2c_cmd[0] = (uint8_t)(codec_cmd >> 8);
  i2c_cmd[1] = (uint8_t)codec_cmd & 0xFF;
  if(i2c_master_transmit(&hi2cx, WM8988_I2C_ADDR_CSB_LOW, (uint8_t *)i2c_cmd, 2, 0xFFFF) != I2C_OK)
  {
    while(1);
  }
}

/**
  * @brief  buffer memset
  * @param  buffer: buffer
  * @param  set: memset data
  * @param  len: data length
  * @retval none
  */
void memset16_buffer(uint16_t *buffer, uint32_t set, uint32_t len)
{
  uint32_t i_index = 0;
  for(i_index = 0; i_index < len; i_index ++)
  {
    buffer[i_index] = (uint16_t)set;
  }
}

/**
  * @brief  copy buffer
  * @param  dest: dest data buffer
  * @param  src: src data buffer
  * @param  len: data length
  * @retval none
  */
void copy_buff(uint16_t *dest, uint16_t *src, uint32_t len)
{
  uint16_t i_index;
  for(i_index = 0; i_index < len; i_index ++)
  {
    dest[i_index] = src[i_index];
  }
}

/**
  * @brief  initializes peripherals used by the i2c.
  * @param  none
  * @retval none
  */
void i2c_lowlevel_init(i2c_handle_type* hi2c)
{
  gpio_init_type gpio_initstructure;

  if(hi2c->i2cx == I2Cx_PORT)
  {
    /* i2c periph clock enable */
    crm_periph_clock_enable(I2Cx_CLK, TRUE);
    crm_periph_clock_enable(CRM_IOMUX_PERIPH_CLOCK, TRUE);
    gpio_pin_remap_config(I2C1_MUX, TRUE);
    crm_periph_clock_enable(I2Cx_SCL_GPIO_CLK, TRUE);
    crm_periph_clock_enable(I2Cx_SDA_GPIO_CLK, TRUE);

    /* gpio configuration */
    gpio_initstructure.gpio_out_type       = GPIO_OUTPUT_OPEN_DRAIN;
    gpio_initstructure.gpio_pull           = GPIO_PULL_UP;
    gpio_initstructure.gpio_mode           = GPIO_MODE_MUX;
    gpio_initstructure.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;

    /* configure i2c pins: scl */
    gpio_initstructure.gpio_pins = I2Cx_SCL_PIN;
    gpio_init(I2Cx_SCL_GPIO_PORT, &gpio_initstructure);

    /* configure i2c pins: sda */
    gpio_initstructure.gpio_pins = I2Cx_SDA_PIN;
    gpio_init(I2Cx_SDA_GPIO_PORT, &gpio_initstructure);

    i2c_init(hi2c->i2cx, I2C_FSMODE_DUTY_2_1, I2Cx_SPEED);

    i2c_own_address1_set(hi2c->i2cx, I2C_ADDRESS_MODE_7BIT, I2Cx_ADDRESS);
  }
}

/**
  * @brief  mclk clock conifg
  * @param  none
  * @retval none
  */
void mclk_tmr1_init(void)
{
  gpio_init_type gpio_init_struct;
  tmr_output_config_type tmr_oc_init_structure;
  uint16_t prescaler_value = (uint16_t)(system_core_clock / 24000000) - 1;

  crm_periph_clock_enable(CRM_TMR1_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_IOMUX_PERIPH_CLOCK, TRUE);

  gpio_default_para_init(&gpio_init_struct);

  gpio_init_struct.gpio_pins = GPIO_PINS_8;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  gpio_init(GPIOA, &gpio_init_struct);

  tmr_base_init(TMR1, 1, prescaler_value);
  tmr_cnt_dir_set(TMR1, TMR_COUNT_UP);
  tmr_clock_source_div_set(TMR1, TMR_CLOCK_DIV1);

  tmr_output_default_para_init(&tmr_oc_init_structure);
  tmr_oc_init_structure.oc_mode = TMR_OUTPUT_CONTROL_PWM_MODE_A;
  tmr_oc_init_structure.oc_idle_state = FALSE;
  tmr_oc_init_structure.oc_polarity = TMR_OUTPUT_ACTIVE_HIGH;
  tmr_oc_init_structure.oc_output_state = TRUE;
  tmr_output_channel_config(TMR1, TMR_SELECT_CHANNEL_1, &tmr_oc_init_structure);
  tmr_channel_value_set(TMR1, TMR_SELECT_CHANNEL_1, 1);
  tmr_output_channel_buffer_enable(TMR1, TMR_SELECT_CHANNEL_1, TRUE);

  /* tmr enable counter */
  tmr_counter_enable(TMR1, TRUE);
  tmr_output_enable(TMR1, TRUE);
}


/**
  * @brief  audio codec i2s reset
  * @param  none
  * @retval none
  */
void codec_i2s_reset(void)
{
  i2s_enable(SPI1, FALSE);
  i2s_enable(SPI2, FALSE);
  dma_channel_enable(DMA1_CHANNEL3, FALSE);
  dma_channel_enable(DMA1_CHANNEL4, FALSE);
}

/**
  * @brief  audio codec i2s init
  * @param  freq: audio sampling freq
  * @param  bitw_format: bit width
  * @retval error status
  */
void codec_i2s_init(audio_codec_type *param)
{
  gpio_init_type gpio_init_struct;
  dma_init_type dma_init_struct;
  i2s_init_type i2s_init_struct;
  i2s_data_channel_format_type format = I2S_DATA_16BIT_CHANNEL_16BIT;

  crm_periph_clock_enable(I2S1_GPIO_CRM_CLK, TRUE);
  crm_periph_clock_enable(I2S2_GPIO_CRM_CLK, TRUE);
  crm_periph_clock_enable(CRM_DMA1_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_SPI1_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_SPI2_PERIPH_CLOCK, TRUE);

  param->spk_freq = param->audio_freq;

  param->spk_tx_size = (param->audio_freq / 1000) * (param->audio_bitw / 8) * AUDIO_SPK_CHANEL_NUM / 2;
  param->mic_rx_size = (param->audio_freq / 1000) * (param->audio_bitw / 8) * AUDIO_MIC_CHANEL_NUM / 2;

  memset(param->spk_buffer, 0, SPK_BUFFER_SIZE*sizeof(uint16_t));
  memset(param->mic_buffer, 0, MIC_BUFFER_SIZE*sizeof(uint16_t));

  param->mic_wend = param->mic_woff = param->mic_roff = param->mic_buffer;
  param->spk_rend = param->spk_woff = param->spk_roff = param->spk_buffer;

  while(param->mic_wend < param->mic_buffer+MIC_BUFFER_SIZE)
  {
    param->mic_wend += param->mic_rx_size;
  }
  param->mic_wend -= param->mic_rx_size;
  while(param->spk_rend < param->spk_buffer+SPK_BUFFER_SIZE)
  {
    param->spk_rend += param->spk_tx_size;
  }
  param->spk_rend -= param->spk_tx_size;
  if(param->audio_bitw == 16)
  {
    format = I2S_DATA_16BIT_CHANNEL_16BIT;
  }
  else
  {
    format = I2S_DATA_24BIT_CHANNEL_32BIT;
  }
  gpio_default_para_init(&gpio_init_struct);

  /* i2s1 ws pins */
  gpio_init_struct.gpio_pull           = GPIO_PULL_UP;
  gpio_init_struct.gpio_mode           = GPIO_MODE_INPUT;
  gpio_init_struct.gpio_pins           = I2S1_WS_PIN;
  gpio_init(I2S1_GPIO, &gpio_init_struct);

  /* i2s1 ck pins */
  gpio_init_struct.gpio_pull           = GPIO_PULL_DOWN;
  gpio_init_struct.gpio_mode           = GPIO_MODE_INPUT;
  gpio_init_struct.gpio_pins           = I2S1_CK_PIN;
  gpio_init(I2S1_GPIO, &gpio_init_struct);

  /* i2s1 sd pins slave tx */
  gpio_init_struct.gpio_pull           = GPIO_PULL_UP;
  gpio_init_struct.gpio_mode           = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pins           = I2S1_SD_PIN;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  gpio_init(I2S1_GPIO, &gpio_init_struct);

  /* i2s2 ws pins */
  gpio_init_struct.gpio_pull           = GPIO_PULL_UP;
  gpio_init_struct.gpio_mode           = GPIO_MODE_INPUT;
  gpio_init_struct.gpio_pins           = I2S2_WS_PIN;
  gpio_init(I2S2_GPIO, &gpio_init_struct);

  /* i2s2 ck pins */
  gpio_init_struct.gpio_pull           = GPIO_PULL_DOWN;
  gpio_init_struct.gpio_mode           = GPIO_MODE_INPUT;
  gpio_init_struct.gpio_pins           = I2S2_CK_PIN;
  gpio_init(I2S2_GPIO, &gpio_init_struct);

  /* i2s2 sd pins slave rx */
  gpio_init_struct.gpio_pull           = GPIO_PULL_UP;
  gpio_init_struct.gpio_mode           = GPIO_MODE_INPUT;
  gpio_init_struct.gpio_pins           = I2S2_SD_PIN;
  gpio_init(I2S2_GPIO, &gpio_init_struct);

  /* dma config */
  dma_reset(DMA1_CHANNEL3);
  dma_reset(DMA1_CHANNEL4);

  /* dma1 channel3: speaker i2s1 tx */
  dma_default_para_init(&dma_init_struct);
  dma_init_struct.buffer_size = param->spk_tx_size << 1;
  dma_init_struct.direction = DMA_DIR_MEMORY_TO_PERIPHERAL;
  dma_init_struct.memory_base_addr = (uint32_t)spk_dma_buffer;
  dma_init_struct.memory_data_width = DMA_MEMORY_DATA_WIDTH_HALFWORD;
  dma_init_struct.memory_inc_enable = TRUE;
  dma_init_struct.peripheral_base_addr = (uint32_t)I2S1_DT_ADDRESS;
  dma_init_struct.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_HALFWORD;
  dma_init_struct.peripheral_inc_enable = FALSE;
  dma_init_struct.priority = DMA_PRIORITY_HIGH;
  dma_init_struct.loop_mode_enable = TRUE;
  dma_init(DMA1_CHANNEL3, &dma_init_struct);
  dma_interrupt_enable(DMA1_CHANNEL3, DMA_FDT_INT, TRUE);
  dma_interrupt_enable(DMA1_CHANNEL3, DMA_HDT_INT, TRUE);
  nvic_irq_enable(DMA1_Channel3_IRQn, 1, 0);

  /* dma1 channel4: microphone i2s2 rx */
  dma_default_para_init(&dma_init_struct);
  dma_init_struct.buffer_size = param->mic_rx_size << 1;
  dma_init_struct.direction = DMA_DIR_PERIPHERAL_TO_MEMORY;
  dma_init_struct.memory_base_addr = (uint32_t)mic_dma_buffer;
  dma_init_struct.memory_data_width = DMA_MEMORY_DATA_WIDTH_HALFWORD;
  dma_init_struct.memory_inc_enable = TRUE;
  dma_init_struct.peripheral_base_addr = (uint32_t)I2S2_DT_ADDRESS;
  dma_init_struct.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_HALFWORD;
  dma_init_struct.peripheral_inc_enable = FALSE;
  dma_init_struct.priority = DMA_PRIORITY_HIGH;
  dma_init_struct.loop_mode_enable = TRUE;
  dma_init(DMA1_CHANNEL4, &dma_init_struct);
  dma_interrupt_enable(DMA1_CHANNEL4, DMA_FDT_INT, TRUE);
  dma_interrupt_enable(DMA1_CHANNEL4, DMA_HDT_INT, TRUE);
  nvic_irq_enable(DMA1_Channel4_IRQn, 2, 0);

  /* i2s1 tx init */
  spi_i2s_reset(SPI1);
  i2s_default_para_init(&i2s_init_struct);
  i2s_init_struct.audio_protocol = I2S_AUDIO_PROTOCOL_PHILLIPS;
  i2s_init_struct.data_channel_format = format;
  i2s_init_struct.mclk_output_enable = FALSE;
  i2s_init_struct.audio_sampling_freq = (i2s_audio_sampling_freq_type)param->audio_freq;
  i2s_init_struct.clock_polarity = I2S_CLOCK_POLARITY_LOW;
  i2s_init_struct.operation_mode = I2S_MODE_SLAVE_TX;
  i2s_init(SPI1, &i2s_init_struct);

   /* i2s2 rx init */
  spi_i2s_reset(SPI2);
  i2s_default_para_init(&i2s_init_struct);
  i2s_init_struct.audio_protocol = I2S_AUDIO_PROTOCOL_PHILLIPS;
  i2s_init_struct.data_channel_format = format;
  i2s_init_struct.mclk_output_enable = FALSE;
  i2s_init_struct.audio_sampling_freq = (i2s_audio_sampling_freq_type)param->audio_freq;
  i2s_init_struct.clock_polarity = I2S_CLOCK_POLARITY_LOW;
  i2s_init_struct.operation_mode = I2S_MODE_SLAVE_RX;
  i2s_init(SPI2, &i2s_init_struct);


  spi_i2s_dma_transmitter_enable(SPI1, TRUE);
  spi_i2s_dma_receiver_enable(SPI2, TRUE);
  i2s_enable(SPI1, TRUE);
  i2s_enable(SPI2, TRUE);

  dma_channel_enable(DMA1_CHANNEL3, TRUE);
  dma_channel_enable(DMA1_CHANNEL4, TRUE);

}

/**
  * @brief  this function handles dma1 channel3 interrupt.
  * @param  none
  * @retval none
  */
void DMA1_Channel3_IRQHandler(void)
{
  uint16_t half_size = audio_codec.spk_tx_size;
  uint16_t *pdst;

  if(dma_flag_get(DMA1_HDT3_FLAG) == SET)
  {
    //copy_buff(audio_codec.spk_buffer, audio_codec.spk_tx_fifo + audio_codec.r_pos, half_size);
    pdst = spk_dma_buffer;
    dma_flag_clear(DMA1_HDT3_FLAG);
  }
  else if(dma_flag_get(DMA1_FDT3_FLAG) == SET)
  {
    //copy_buff(&audio_codec.spk_buffer[half_size], audio_codec.spk_tx_fifo + audio_codec.r_pos, half_size);
    pdst = spk_dma_buffer + half_size;
    dma_flag_clear(DMA1_FDT3_FLAG);
  }

  switch( audio_codec.spk_stage )
  {
    case 0:
    case 1:
      memset( pdst, 0, half_size << 1);
      break;
    case 2:
      if( audio_codec.spk_wtotal >= audio_codec.spk_rtotal + SPK_BUFFER_SIZE )
      {
        while(1);//should not happen;
      }
      if( audio_codec.spk_rtotal >= audio_codec.spk_wtotal )
      {
        audio_codec.spk_stage = 0;
        audio_codec.spk_woff = audio_codec.spk_roff = audio_codec.spk_buffer;
        audio_codec.spk_rtotal = audio_codec.spk_wtotal = 0;
      }
      else
      {
        memcpy( pdst, audio_codec.spk_roff, half_size << 1 );
        audio_codec.spk_roff   += half_size;
        audio_codec.spk_rtotal += half_size;
        if( audio_codec.spk_roff >= audio_codec.spk_rend )
        {
          audio_codec.spk_roff = audio_codec.spk_buffer;
        }
        if( ++audio_codec.spk_calc == 256 )
        {
          uint16_t delta = audio_codec.spk_wtotal - audio_codec.spk_rtotal;
          audio_codec.spk_calc = 0;
          if( delta < audio_codec.spk_threshold - half_size )
          {
            audio_codec.spk_threshold -= half_size;
            audio_codec.spk_freq += audio_codec.audio_freq/1024;
          }
          else if( delta > audio_codec.spk_threshold + half_size )
          {
            audio_codec.spk_threshold += half_size;
            audio_codec.spk_freq -= audio_codec.audio_freq/1024;
          }
          if(audio_codec.spk_rtotal > 0x20000000)
          {
            audio_codec.spk_rtotal -= 0x10000000;
            audio_codec.spk_wtotal -= 0x10000000;
          }
        }
      }
    break;
  }

}

/**
  * @brief  this function handles dma1 channel4 interrupt.
  * @param  none
  * @retval none
  */
void DMA1_Channel4_IRQHandler(void)
{
  uint16_t *psrc;
  uint16_t len = audio_codec.mic_rx_size << 1;

  if(dma_flag_get(DMA1_HDT4_FLAG) == SET)
  {
    dma_flag_clear(DMA1_HDT4_FLAG);
    psrc = mic_dma_buffer;
  }
  else if(dma_flag_get(DMA1_FDT4_FLAG) == SET)
  {
    psrc = mic_dma_buffer + audio_codec.mic_rx_size;
    dma_flag_clear(DMA1_FDT4_FLAG);
  }
  if( audio_codec.mic_stage )
  {
    memcpy( audio_codec.mic_woff, psrc, len );
    audio_codec.mic_woff   += len/2;
    audio_codec.mic_wtotal += len/2;
    if( audio_codec.mic_woff >= audio_codec.mic_wend )
    {
      audio_codec.mic_woff = audio_codec.mic_buffer;
    }
    if( audio_codec.mic_stage == 2 )
    {
      if( 1024 == ++audio_codec.mic_calc )
      {
        uint32_t size_estimate = 1024*audio_codec.mic_rx_size;
        audio_codec.mic_calc = 0;
        if( audio_codec.mic_delta > size_estimate )
        {
          audio_codec.mic_adj_count = audio_codec.mic_delta - size_estimate;
          audio_codec.mic_adj_stage = 2;
        }
        else if( audio_codec.mic_delta < size_estimate )
        {
          audio_codec.mic_adj_count = size_estimate - audio_codec.mic_delta;
          audio_codec.mic_adj_stage = 1;
        }
        else
        {
          audio_codec.mic_adj_count = 0;
          audio_codec.mic_adj_stage = 0;
        }
        audio_codec.mic_delta = 0;
        if( audio_codec.mic_rtotal >= 0x80000000 )
        {
          audio_codec.mic_rtotal -= 0x80000000;
          audio_codec.mic_wtotal -= 0x80000000;
        }
      }
      if( audio_codec.mic_wtotal >= audio_codec.mic_rtotal + MIC_BUFFER_SIZE )
      {
        audio_codec.mic_delta = audio_codec.mic_wtotal = audio_codec.mic_rtotal = 0;
        audio_codec.mic_woff = audio_codec.mic_roff = audio_codec.mic_buffer;
        audio_codec.mic_stage = 0;
        audio_codec.mic_adj_stage = 0;
        audio_codec.mic_adj_count = 0;
        audio_codec.mic_calc = 0;
      }
    }
  }
}

/**
  * @brief  audio codec init
  * @param  none
  * @retval error status
  */
error_status audio_codec_init(void)
{
  uint32_t reg_len = sizeof(reg_addr_data) / sizeof(uint16_t);
  uint32_t i_index = 0;
  uint8_t i2c_cmd[2];

  if(AUDIO_DEFAULT_FREQ == AUDIO_FREQ_16K)
  {
    reg_addr_data[6] = WM8988_REG_FREQ16K;
    audio_codec.audio_freq = AUDIO_FREQ_16K;
  }
  else if(AUDIO_DEFAULT_FREQ == AUDIO_FREQ_48K)
  {
    reg_addr_data[6] = WM8988_REG_FREQ48K;
    audio_codec.audio_freq = AUDIO_FREQ_48K;
  }
  else
  {
    return ERROR;
  }

  if(AUDIO_DEFAULT_BITW == AUDIO_BITW_16)
  {
    reg_addr_data[5] = WM8988_REG_BITW16;
    audio_codec.audio_bitw = AUDIO_BITW_16;
  }
  else
  {
    return ERROR;
  }

  /* i2c init */
  hi2cx.i2cx = I2Cx_PORT;

  i2c_config(&hi2cx);

  for(i_index = 0; i_index < reg_len; i_index ++)
  {
    i2c_cmd[0] = (uint8_t)(reg_addr_data[i_index] >> 8);
    i2c_cmd[1] = (uint8_t)reg_addr_data[i_index] & 0xFF;
    if(i2c_master_transmit(&hi2cx, WM8988_I2C_ADDR_CSB_LOW, (uint8_t *)i2c_cmd, 2, 0xFFFF) != I2C_OK)
    {
      return ERROR;
    }
  }

  /* timer init */
  mclk_tmr1_init();

  codec_i2s_init(&audio_codec);

  return SUCCESS;
}

/**
  * @brief  audio codec loop
  * @param  none
  * @retval none
  */
error_status audio_codec_loop(void)
{
  static uint8_t mic_mute = 0;
  static uint16_t mic_volume = 0;
  static uint8_t spk_mute = 0;
  static uint16_t spk_volume = 0;
  uint16_t audio_ctrl_l;
  uint16_t audio_ctrl_r;
  uint8_t i2c_cmd[2];

  if(mic_mute != audio_codec.mic_mute)
  {
    mic_mute = audio_codec.mic_mute;
    audio_ctrl_l = WM8988_R21_LEFT_ADC_VOLUME << 9 | 0x100;
    audio_ctrl_r = WM8988_R22_RIGHT_ADC_VOLUME << 9 | 0x100;
    if(audio_codec.mic_mute == 0)
    {
      audio_ctrl_l |= audio_codec.mic_volume;
      audio_ctrl_r |= audio_codec.mic_volume;
    }
    i2c_cmd[0] = (uint8_t)(audio_ctrl_l >> 8);
    i2c_cmd[1] = (uint8_t)audio_ctrl_l & 0xFF;
    if(i2c_master_transmit(&hi2cx, WM8988_I2C_ADDR_CSB_LOW, (uint8_t *)i2c_cmd, 2, 0xFFFF) != I2C_OK)
    {
      return ERROR;
    }
    i2c_cmd[0] = (uint8_t)(audio_ctrl_r >> 8);
    i2c_cmd[1] = (uint8_t)audio_ctrl_r & 0xFF;
    if(i2c_master_transmit(&hi2cx, WM8988_I2C_ADDR_CSB_LOW, (uint8_t *)i2c_cmd, 2, 0xFFFF) != I2C_OK)
    {
      return ERROR;
    }
  }

  if(mic_volume != audio_codec.mic_volume)
  {
    mic_volume = audio_codec.mic_volume;
    audio_ctrl_l = WM8988_R21_LEFT_ADC_VOLUME << 9 | 0x100;
    audio_ctrl_r = WM8988_R22_RIGHT_ADC_VOLUME << 9 | 0x100;
    audio_ctrl_l |= audio_codec.mic_volume;
    audio_ctrl_r |= audio_codec.mic_volume;
    i2c_cmd[0] = (uint8_t)(audio_ctrl_l >> 8);
    i2c_cmd[1] = (uint8_t)audio_ctrl_l & 0xFF;
    if(i2c_master_transmit(&hi2cx, WM8988_I2C_ADDR_CSB_LOW, (uint8_t *)i2c_cmd, 2, 0xFFFF) != I2C_OK)
    {
      return ERROR;
    }
    i2c_cmd[0] = (uint8_t)(audio_ctrl_r >> 8);
    i2c_cmd[1] = (uint8_t)audio_ctrl_r & 0xFF;
    if(i2c_master_transmit(&hi2cx, WM8988_I2C_ADDR_CSB_LOW, (uint8_t *)i2c_cmd, 2, 0xFFFF) != I2C_OK)
    {
      return ERROR;
    }
  }

  if(spk_mute != audio_codec.spk_mute)
  {
    spk_mute = audio_codec.spk_mute;
    audio_ctrl_l = (WM8988_R5_ADC_DAC_CONTROL << 9) | 0x0006 | (audio_codec.spk_mute << 3);
    i2c_cmd[0] = (uint8_t)(audio_ctrl_l >> 8);
    i2c_cmd[1] = (uint8_t)audio_ctrl_l & 0xFF;
    if(i2c_master_transmit(&hi2cx, WM8988_I2C_ADDR_CSB_LOW, (uint8_t *)i2c_cmd, 2, 0xFFFF) != I2C_OK)
    {
      return ERROR;
    }
  }

  if(spk_volume != audio_codec.spk_volume)
  {
    spk_volume = audio_codec.spk_volume;
    audio_ctrl_l = WM8988_R2_LOUT1_VOLUME << 9 | 0x100;
    audio_ctrl_r = WM8988_R3_ROUT1_VOLUME << 9 | 0x100;
    audio_ctrl_l |= audio_codec.spk_volume;
    audio_ctrl_r |= audio_codec.spk_volume;

    i2c_cmd[0] = (uint8_t)(audio_ctrl_l >> 8);
    i2c_cmd[1] = (uint8_t)audio_ctrl_l & 0xFF;

    if(i2c_master_transmit(&hi2cx, WM8988_I2C_ADDR_CSB_LOW, (uint8_t *)i2c_cmd, 2, 0xFFFF) != I2C_OK)
    {
      return ERROR;
    }
    i2c_cmd[0] = (uint8_t)(audio_ctrl_r >> 8);
    i2c_cmd[1] = (uint8_t)audio_ctrl_r & 0xFF;
    if(i2c_master_transmit(&hi2cx, WM8988_I2C_ADDR_CSB_LOW, (uint8_t *)i2c_cmd, 2, 0xFFFF) != I2C_OK)
    {
      return ERROR;
    }
  }
  return SUCCESS;
}

/**
  * @}
  */

/**
  * @}
  */




