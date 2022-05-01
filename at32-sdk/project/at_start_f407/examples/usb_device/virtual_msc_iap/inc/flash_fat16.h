/**
  **************************************************************************
  * @file     flash_fat16.h
  * @version  v2.0.8
  * @date     2022-04-02
  * @brief    fat16 file system header file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FLASH_FAT16_H
#define __FLASH_FAT16_H

#ifdef __cplusplus
extern "C" {
#endif


#include "usb_conf.h"

/** @addtogroup AT32F407_periph_examples
  * @{
  */

/** @addtogroup 407_USB_device_virtual_msc_iap
  * @{
  */

/** @defgroup USB_device_virtual_msc_iap_definition
  * @{
  */
#define FAT16_SECTOR_SIZE                62
#define FAT16_TABLE_SIZE                 4
#define FAT16_BYTE_PER_SIZE              2048
#define FAT16_FILENAME_SIZE              11
#define FAT16_DIR_SIZE                   47

#define FLASH_FAT16_BOOT_SECTOR_ADDR     0x00
#define FLASH_FAT16_1_ADDR               0x1000
#define FLASH_FAT16_2_ADDR               0x7800
#define FLASH_FAT16_ROOT_ADDR            0xE000
#define FLASH_FAT16_FILE_START_ADDR      0x12000

#define FLASH_APP_START_ADDR             0x08005000

#define FLASH_SECTOR_1K_SIZE             0x400
#define FLASH_SECTOR_2K_SIZE             0x800
#define FLASH_SECTOR_4K_SIZE             0x1000

#define FLASH_SECTOR_1K_ALLGNED          0x3FF
#define FLASH_SECTOR_2K_ALLGNED          0x7FF
#define FLASH_SECTOR_4K_ALLGNED          0xFFF

#define FILE_SUFFIX1_LEN                 3
#define FILE_SUFFIX2_LEN                 3
#define FILE_SUFFIX1_NAME                "BIN"
#define FILE_SUFFIX2_NAME                "bin"

#define INVAILD_OFFSET_ADDR              0xFFFFFFFF
#define IAP_UPGRADE_COMPLETE_FLAG        0x41544B38
#define IAP_SUCCESS                      0
#define IAP_FAILED                       1

typedef enum
{
  UPGRADE_READY                          = 0,
  UPGRAGE_ONGOING,
  UPGRADE_SUCCESS,
  UPGRADE_FAILED,
  UPGRADE_LARGE,
  UPGRADE_UNKNOWN,
  UPGRADE_DONE,
  UPGRADE_JUMP
}upgrade_status_type;

typedef struct
{
  uint8_t file_name[11];
  uint8_t attr;
  uint8_t res;
  uint8_t create_time_tenth;
  uint16_t create_time;
  uint16_t create_data;
  uint16_t last_access_data;
  uint16_t clus_high;
  uint16_t write_time;
  uint16_t write_data;
  uint16_t clus_low;
  uint32_t file_size;
}fat_dir_type;

typedef struct
{
  uint32_t flash_base_addr;
  uint32_t flash_app_addr;
  uint32_t flash_size;
  uint32_t flash_app_size;
  uint32_t sector_size;
  uint32_t sector_mask;
  uint32_t firmware_length;
  uint32_t write_crc;
  uint32_t read_crc;

  uint32_t write_addr;
  uint32_t file_write_nr;
  upgrade_status_type msc_up_status;
}flash_iap_type;

extern flash_iap_type flash_iap;

uint32_t flash_fat16_write(uint32_t fat_lbk, uint8_t *data, uint32_t len);
uint32_t flash_fat16_read(uint32_t fat_lbk, uint8_t *data, uint32_t len);

void flash_fat16_init(void);
void flash_fat16_loop_status(void);
uint32_t flash_fat16_set_name(const uint8_t *file_name, uint8_t len);
uint8_t flash_fat16_get_upgrade_flag(void);

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

