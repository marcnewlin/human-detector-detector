/**
  **************************************************************************
  * @file     spi_flash.h
  * @version  v2.0.8
  * @date     2022-04-02
  * @brief    header file of spi_flash
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

#ifndef __SPI_FLASH_H
#define __SPI_FLASH_H

#ifdef __cplusplus
extern "C" {
#endif

#include "at32f403a_407.h"

/** @addtogroup AT32F407_periph_examples
  * @{
  */

/** @addtogroup 407_SPI_w25q_flash
  * @{
  */


 /* use dma transfer spi data */
//#define SPI_TRANS_DMA

/** @defgroup SPI_flash_cs_pin_definition
  * @{
  */

#define FLASH_CS_HIGH()                  gpio_bits_set(GPIOA, GPIO_PINS_4)
#define FLASH_CS_LOW()                   gpio_bits_reset(GPIOA, GPIO_PINS_4)

/**
  * @}
  */

/** @defgroup SPI_flash_id_definition
  * @{
  */

/*
 * flash define
 */
#define W25Q80                           0xEF13
#define W25Q16                           0xEF14
#define W25Q32                           0xEF15
#define W25Q64                           0xEF16
/* 16mb, the range of address:0~0xFFFFFF */
#define W25Q128                          0xEF17

/**
  * @}
  */

/** @defgroup SPI_flash_operation_definition
  * @{
  */

#define SPIF_CHIP_SIZE                   0x1000000
#define SPIF_SECTOR_SIZE                 4096
#define SPIF_PAGE_SIZE                   256

#define SPIF_WRITEENABLE                 0x06
#define SPIF_WRITEDISABLE                0x04
/* s7-s0 */
#define SPIF_READSTATUSREG1              0x05
#define SPIF_WRITESTATUSREG1             0x01
/* s15-s8 */
#define SPIF_READSTATUSREG2              0x35
#define SPIF_WRITESTATUSREG2             0x31
/* s23-s16 */
#define SPIF_READSTATUSREG3              0x15
#define SPIF_WRITESTATUSREG3             0x11
#define SPIF_READDATA                    0x03
#define SPIF_FASTREADDATA                0x0B
#define SPIF_FASTREADDUAL                0x3B
#define SPIF_PAGEPROGRAM                 0x02
/* block size:64kb */
#define SPIF_BLOCKERASE                  0xD8
#define SPIF_SECTORERASE                 0x20
#define SPIF_CHIPERASE                   0xC7
#define SPIF_POWERDOWN                   0xB9
#define SPIF_RELEASEPOWERDOWN            0xAB
#define SPIF_DEVICEID                    0xAB
#define SPIF_MANUFACTDEVICEID            0x90
#define SPIF_JEDECDEVICEID               0x9F
#define FLASH_SPI_DUMMY_BYTE             0xA5

/**
  * @}
  */

/** @defgroup SPI_flash_exported_functions
  * @{
  */

void spiflash_init(void);
void spiflash_write(uint8_t *pbuffer, uint32_t write_addr, uint32_t length);
void spiflash_read(uint8_t *pbuffer, uint32_t read_addr, uint32_t length);
void spiflash_sector_erase(uint32_t erase_addr);
void spiflash_write_nocheck(uint8_t *pbuffer, uint32_t write_addr, uint32_t length);
void spiflash_page_write(uint8_t *pbuffer, uint32_t write_addr, uint32_t length);
void spi_bytes_write(uint8_t *pbuffer, uint32_t length);
void spi_bytes_read(uint8_t *pbuffer, uint32_t length);
void spiflash_wait_busy(void);
uint8_t spiflash_read_sr1(void);
void spiflash_write_enable(void);
uint16_t spiflash_read_id(void);
uint8_t spi_byte_write(uint8_t data);
uint8_t spi_byte_read(void);

/**
  * @}
  */

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

