/**
  ******************************************************************************
  * @file     xmc_nand.h
  * @version  v2.0.8
  * @date     2022-04-02
  * @brief    header file for the nand configuration.
  ******************************************************************************
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

/*-- define to prevent recursive inclusion -------------------------------------*/
#ifndef __XMC_NAND_H
#define __XMC_NAND_H

#ifdef __cplusplus
extern "C" {
#endif

#include "at32f403a_407_board.h"

/** @addtogroup AT32F407_periph_examples
  * @{
  */

/** @addtogroup 407_XMC_nand
  * @{
  */

/** @defgroup NAND_id_define
  * @{
  */

typedef struct
{
  uint8_t maker_id;      /*!< maker id */
  uint8_t device_id;     /*!< device id */
  uint8_t third_id;      /*!< third id */
  uint8_t fourth_id;     /*!< fourth id */
  uint8_t fifth_id;      /*!< fifth id */
  uint8_t sixth_id;      /*!< sixth id */
} nand_id_type;

/**
  * @}
  */

/** @defgroup NAND_address_define
  * @{
  */

typedef struct
{
  uint16_t zone;
  uint16_t block;
  uint16_t page;
  uint16_t byte;
} nand_address_type;

/**
  * @}
  */


#define ROW_ADDRESS                      (address_struct.page + (address_struct.block + (address_struct.zone * NAND_ZONE_SIZE)) * NAND_BLOCK_SIZE)
#define COL_ADDRESS                      address_struct.byte

#define BUFFER_SIZE                      NAND_PAGE_SIZE

#define XMC_Bank_NAND                    XMC_Bank2_NAND
#define Bank_NAND_ADDR                   Bank2_NAND_ADDR
#define Bank2_NAND_ADDR                  ((uint32_t)0x70000000)

#ifdef H27U1G8F2CTR
#define NAND_AT_MakerID                  0xAD
#define NAND_AT_DeviceID                 0xF1
#elif defined K9GAG08U0E
#define NAND_AT_MakerID                  0xEC
#define NAND_AT_DeviceID                 0xD5
#endif

/** @defgroup NAND_area_define
  * @{
  */
/* a16 = cle  high */
#define CMD_AREA                         (uint32_t)(1<<16)
/* a17 = ale  high */
#define ADDR_AREA                        (uint32_t)(1<<17)

#define DATA_AREA                        ((uint32_t)0x00000000)

/**
  * @}
  */

/** @defgroup XMC_nand_memory_command
  * @{
  */

/*-- xmc nand memory command ------------------------------------*/
#define NAND_CMD_AREA_A                  ((uint8_t)0x00)
#define NAND_CMD_AREA_B                  ((uint8_t)0x01)
#define NAND_CMD_AREA_C                  ((uint8_t)0x50)
#define NAND_CMD_AREA_TRUE1              ((uint8_t)0x30)

#define NAND_CMD_WRITE0                  ((uint8_t)0x80)
#define NAND_CMD_WRITE_TRUE1             ((uint8_t)0x10)

#define NAND_CMD_ERASE0                  ((uint8_t)0x60)
#define NAND_CMD_ERASE1                  ((uint8_t)0xD0)

#define NAND_CMD_READID                  ((uint8_t)0x90)
#define NAND_CMD_STATUS                  ((uint8_t)0x70)
#define NAND_CMD_LOCK_STATUS             ((uint8_t)0x7A)
#define NAND_CMD_RESET                   ((uint8_t)0xFF)

/**
  * @}
  */

/** @defgroup XMC_nand_status
  * @{
  */

/*-- nand memory status -----------      -------------------------*/
#define NAND_VALID_ADDRESS               ((uint32_t)0x00000100)
#define NAND_INVALID_ADDRESS             ((uint32_t)0x00000200)
#define NAND_TIMEOUT_ERROR               ((uint32_t)0x00000400)
#define NAND_BUSY                        ((uint32_t)0x00000000)
#define NAND_ERROR                       ((uint32_t)0x00000001)
#define NAND_READY                       ((uint32_t)0x00000040)

/**
  * @}
  */

/** @defgroup XMC_nand_parameters
  * @{
  */

/*-- xmc nand memory parameters ------------------------------------*/
#ifdef H27U1G8F2CTR
  /* 2k bytes per page w/o Spare Area */
  #define NAND_PAGE_SIZE                 ((uint16_t)0x0800)
  /* 64x2k bytes pages per block */
  #define NAND_BLOCK_SIZE                ((uint16_t)0x0040)
  /* 1024 Block per zone */
  #define NAND_ZONE_SIZE                 ((uint16_t)0x0400)
  /* last 64 bytes as spare area */
  #define NAND_SPARE_AREA_SIZE           ((uint16_t)0x0040)
  /* 1 zones of 1024 block */
  #define NAND_MAX_ZONE                  ((uint16_t)0x0001)
#elif defined K9GAG08U0E
  /* 8k bytes per page w/o Spare Area */
  #define NAND_PAGE_SIZE                 ((uint16_t)0x2000)
  /* 128x8k bytes pages per block */
  #define NAND_BLOCK_SIZE                ((uint16_t)0x0080)
  /* 2048 Block per zone */
  #define NAND_ZONE_SIZE                 ((uint16_t)0x0800)
  /* last 436 bytes as spare area */
  #define NAND_SPARE_AREA_SIZE           ((uint16_t)0x01B4)
  /* 1 zones of 1024 block */
  #define NAND_MAX_ZONE                  ((uint16_t)0x0001)
#endif

/**
  * @}
  */

/** @defgroup XMC_nand_address_computation
  * @{
  */

/*-- xmc nand memory address computation ------------------------------------*/
/* 1st addressing cycle */
#define addr_1st_cycle(addr)             (uint8_t)((addr) & 0x000000FF)
/* 2nd addressing cycle */
#define addr_2nd_cycle(addr)             (uint8_t)(((addr) & 0x0000FF00) >> 8)
/* 3rd addressing cycle */
#define addr_3rd_cycle(addr)             (uint8_t)(((addr) & 0x00FF0000) >> 16)
/* 4th addressing cycle */
#define addr_4th_cycle(addr)             (uint8_t)(((addr) & 0xFF000000) >> 24)

/**
  * @}
  */

/** @defgroup XMC_nand_functions
  * @{
  */

void nand_init(void);
void nand_read_id(nand_id_type* nand_id_struct);
uint32_t nand_write_small_page(uint8_t *pbuffer, nand_address_type address_struct, uint32_t num_page_to_write);
uint32_t nand_read_small_page(uint8_t *pbuffer, nand_address_type address_struct, uint32_t num_page_to_read);
uint32_t nand_write_spare_area(uint8_t *pbuffer, nand_address_type address_struct, uint32_t num_spare_area_to_write);
uint32_t nand_read_spare_area(uint8_t *pbuffer, nand_address_type address_struct, uint32_t num_spare_area_to_read);
uint32_t nand_erase_block(nand_address_type address_struct);
uint32_t nand_reset(void);
uint32_t nand_get_status(void);
uint32_t nand_read_status(void);
uint32_t nand_address_increment(nand_address_type* address_struct);

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


