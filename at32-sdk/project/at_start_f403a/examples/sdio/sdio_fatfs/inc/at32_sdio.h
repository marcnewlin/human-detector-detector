/**
  **************************************************************************
  * @file     at32_sdio.h
  * @version  v2.0.8
  * @date     2022-04-02
  * @brief    this file contains all the functions prototypes for the sd/mmc
  *           card at32_sdio driver firmware library.
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

/* define to prevent recursive inclusion -------------------------------------*/
#ifndef __AT32_SDIO_H
#define __AT32_SDIO_H

#ifdef __cplusplus
extern "C" {
#endif

/* includes ------------------------------------------------------------------*/
#include "at32f403a_407.h"

/** @addtogroup AT32F403A_periph_examples
  * @{
  */

/** @addtogroup 403A_SDIO_sdio_fatfs
  * @{
  */

/** @defgroup SDIO_sdio_fatfs_instance_definition
  * @{
  */

#define SDIOx                            SDIO1

/**
  * @}
  */

/** @defgroup SDIO_sdio_fatfs_exported_types
  * @{
  */

/**
  * sdio data transfer mode
  */
typedef enum
{
  SD_TRANSFER_POLLING_MODE               = 0,
  SD_TRANSFER_DMA_MODE                   = 1
} sd_data_transfer_mode_type;

/**
  * sdio error defines
  */
typedef enum
{
  /* sdio specific error defines ------------------------------------------*/
  SD_CMD_FAIL                            = 1,    /*!< command response received (but crc check failed) */
  SD_DATA_FAIL                           = 2,    /*!< data bock sent/received (crc check failed) */
  SD_CMD_RSP_TIMEOUT                     = 3,    /*!< command response timeout */
  SD_DATA_TIMEOUT                        = 4,    /*!< data time out */
  SD_TX_UNDERRUN                         = 5,    /*!< transmit fifo under-run */
  SD_RX_OVERRUN                          = 6,    /*!< receive fifo over-run */
  SD_START_BIT_ERR                       = 7,    /*!< start bit not detected on all data signals in wide bus mode */
  SD_CMD_OUT_OF_RANGE                    = 8,    /*!< cmd's argument was out of range. */
  SD_ADDR_MISALIGNED                     = 9,    /*!< misaligned address */
  SD_BLK_LEN_ERR                         = 10,   /*!< transferred block length is not allowed for the card or the number of transferred bytes does not match the block length */
  SD_ERASE_SEQ_ERR                       = 11,   /*!< an error in the sequence of erase command occurs. */
  SD_INVALID_ERASE_PARAM                 = 12,   /*!< an invalid selection for erase groups */
  SD_WR_PROTECT_VIOLATION                = 13,   /*!< attempt to program a write protect block */
  SD_LOCK_UNLOCK_FAILED                  = 14,   /*!< sequence or password error has been detected in unlock command or if there was an attempt to access a locked card */
  SD_CMD_CRC_FAILED                      = 15,   /*!< crc check of the previous command failed */
  SD_ILLEGAL_CMD                         = 16,   /*!< command is not legal for the card state */
  SD_CARD_ECC_FAILED                     = 17,   /*!< card internal ecc was applied but failed to correct the data */
  SD_CARD_CONTROLLER_ERR                 = 18,   /*!< internal card controller error */
  SD_GENERAL_UNKNOWN_ERROR               = 19,   /*!< general or unknown error */
  SD_STREAM_RD_UNDERRUN                  = 20,   /*!< the card could not sustain data transfer in stream read operation. */
  SD_STREAM_WR_OVERRUN                   = 21,   /*!< the card could not sustain data programming in stream mode */
  SD_CID_CSD_OVERWRITE                   = 22,   /*!< cid/csd overwrite error */
  SD_WP_ERASE_SKIP                       = 23,   /*!< only partial address space was erased */
  SD_CARD_ECC_DISABLED                   = 24,   /*!< command has been executed without using internal ecc */
  SD_ERASE_RESET                         = 25,   /*!< erase sequence was cleared before executing because an out of erase sequence command was received */
  SD_AKE_SEQ_ERROR                       = 26,   /*!< error in sequence of authentication. */
  SD_INVALID_VOLTRANGE                   = 27,   /*!< invalid voltage range */
  SD_ADDR_OUT_OF_RANGE                   = 28,   /*!< address out of range */
  SD_SWITCH_ERROR                        = 29,   /*!< switch error */
  SD_SDIO_DISABLED                       = 30,   /*!< sdio disabled */
  SD_SDIO_FUNC_BUSY                      = 31,   /*!< function busy */
  SD_SDIO_FUNC_FAILED                    = 32,   /*!< function failed */
  SD_SDIO_UNKNOWN_FUNC                   = 33,   /*!< unknown function */

  /* standard error defines --------------------------------------------*/
  SD_INTERNAL_ERROR,                             /*!< internal error */
  SD_NOT_CONFIGURED,                             /*!< sdio doesn't configuration */
  SD_REQ_PENDING,                                /*!< request pending */
  SD_REQ_NOT_APPLICABLE,                         /*!< request isn't applicable */
  SD_INVALID_PARAMETER,                          /*!< invalid parameter */
  SD_UNSUPPORTED_FEATURE,                        /*!< unsupported feature */
  SD_UNSUPPORTED_HW,                             /*!< unsupported hardware */
  SD_ERROR,                                      /*!< error */
  SD_OK = 0                                      /*!< pass */
} sd_error_status_type;

/**
  * card specific data: csd register
  */
typedef struct
{
  uint8_t  csd_struct;                           /*!< csd structure */
  uint8_t  spec_version;                         /*!< system specification version */
  uint8_t  reserved1;                            /*!< reserved */
  uint8_t  taac;                                 /*!< data read access-time 1 */
  uint8_t  nsac;                                 /*!< data read access-time 2 in clk cycles */
  uint8_t  max_bus_clk_freq;                     /*!< max. bus clock frequency */
  uint16_t card_cmd_classes;                     /*!< card command classes */
  uint8_t  max_read_blk_length;                  /*!< max. read data block length */
  uint8_t  part_blk_read;                        /*!< partial blocks for read allowed */
  uint8_t  write_blk_misalign;                   /*!< write block misalignment */
  uint8_t  read_blk_misalign;                    /*!< read block misalignment */
  uint8_t  dsr_implemented;                      /*!< dsr implemented */
  uint8_t  reserved2;                            /*!< reserved */
  uint32_t device_size;                          /*!< device size */
  uint8_t  max_read_current_vdd_min;             /*!< max. read current @ vdd min */
  uint8_t  max_read_current_vdd_max;             /*!< max. read current @ vdd max */
  uint8_t  max_write_current_vdd_min;            /*!< max. write current @ vdd min */
  uint8_t  max_write_current_vdd_max;            /*!< max. write current @ vdd max */
  uint8_t  device_size_mult;                     /*!< device size multiplier */
  uint8_t  erase_group_size;                     /*!< erase group size */
  uint8_t  erase_group_size_mult;                /*!< erase group size multiplier */
  uint8_t  write_protect_group_size;             /*!< write protect group size */
  uint8_t  write_protect_group_enable;           /*!< write protect group enable */
  uint8_t  manufacturer_default_ecc;             /*!< manufacturer default ecc */
  uint8_t  write_speed_factor;                   /*!< write speed factor */
  uint8_t  max_write_blk_length;                 /*!< max. write data block length */
  uint8_t  part_blk_write;                       /*!< partial blocks for write allowed */
  uint8_t  reserved3;                            /*!< reserded */
  uint8_t  content_protect_app;                  /*!< content protection application */
  uint8_t  file_format_group;                    /*!< file format group */
  uint8_t  copy_flag;                            /*!< copy flag (otp) */
  uint8_t  permanent_write_protect;              /*!< permanent write protection */
  uint8_t  temp_write_protect;                   /*!< temporary write protection */
  uint8_t  file_formart;                         /*!< file format */
  uint8_t  ecc_code;                             /*!< ecc code */
  uint8_t  csd_crc;                              /*!< csd crc */
  uint8_t  reserved4;                            /*!< always */
} sd_csd_struct_type;

/**
  * card identification data: cid register
  */
typedef struct
{
  uint8_t  manufacturer_id;                      /*!< manufacturer id */
  uint16_t oem_app_id;                           /*!< oem/application id */
  uint32_t product_name1;                        /*!< product name part1 */
  uint8_t  product_name2;                        /*!< product name part2 */
  uint8_t  product_reversion;                    /*!< product revision */
  uint32_t product_sn;                           /*!< product serial number */
  uint8_t  reserved1;                            /*!< reserved1 */
  uint16_t manufact_date;                        /*!< manufacturing date */
  uint8_t  cid_crc;                              /*!< cid crc */
  uint8_t  reserved2;                            /*!< always 1 */
} sd_cid_struct_type;

/**
  * sd card status
  */
typedef enum
{
  SD_CARD_READY                          = ((uint32_t)0x00000001),
  SD_CARD_IDENTIFICATION                 = ((uint32_t)0x00000002),
  SD_CARD_STANDBY                        = ((uint32_t)0x00000003),
  SD_CARD_TRANSFER                       = ((uint32_t)0x00000004),
  SD_CARD_SENDING                        = ((uint32_t)0x00000005),
  SD_CARD_RECEIVING                      = ((uint32_t)0x00000006),
  SD_CARD_PROGRAMMING                    = ((uint32_t)0x00000007),
  SD_CARD_DISCONNECTED                   = ((uint32_t)0x00000008),
  SD_CARD_ERROR                          = ((uint32_t)0x000000FF)
} sd_card_state_type;

/**
  * supported sd memory cards
  */
typedef enum
{
  SDIO_STD_CAPACITY_SD_CARD_V1_1         = 0,
  SDIO_STD_CAPACITY_SD_CARD_V2_0         = 1,
  SDIO_HIGH_CAPACITY_SD_CARD             = 2,
  SDIO_MULTIMEDIA_CARD                   = 3,
  SDIO_SECURE_DIGITAL_IO_CARD            = 4,
  SDIO_HIGH_SPEED_MULTIMEDIA_CARD        = 5,
  SDIO_SECURE_DIGITAL_IO_COMBO_CARD      = 6,
  SDIO_HIGH_CAPACITY_MMC_CARD            = 7,
  SDIO_SDIO_CARD                         = 8
} sd_memory_card_type;

/**
  * sd card scr information
  */
typedef struct
{
  uint32_t sd_spec                       :4;     /* [59:56] */
  uint32_t scr_structure                 :4;     /* [60:63] */
  uint32_t sd_bus_width                  :4;     /* [51:48] */
  uint32_t sd_security                   :3;     /* [52:54] */
  uint32_t data_stat_after_erase         :1;     /* [55:55] */
  uint32_t reserved1                     :7;     /* [46:40] */
  uint32_t sd_spec3                      :1;     /* [47:47] */
  uint32_t cmd20_support                 :1;     /* [32:32] */
  uint32_t cmd23_support                 :1;     /* [33:33] */
  uint32_t reserverd2                    :6;     /* [34:39] */
  uint32_t reserverd3;                           /* [31:0] */
} sd_scr_struct_type;

/**
  * sd card information
  */
typedef struct
{
  sd_csd_struct_type sd_csd_reg;
  sd_cid_struct_type sd_cid_reg;
  sd_scr_struct_type sd_scr_reg;
  long long card_capacity;
  uint32_t card_blk_size;
  uint16_t rca;
  uint8_t card_type;
} sd_card_info_struct_type;

/**
  * @}
  */

/** @defgroup SDIO_sdio_fatfs_command_index_definition
  * @{
  */

/**
  * sdio commands index
  */
#define SD_CMD_GO_IDLE_STATE             ((uint8_t)0)
#define SD_CMD_SEND_OP_COND              ((uint8_t)1)
#define SD_CMD_ALL_SEND_CID              ((uint8_t)2)
#define SD_CMD_SET_REL_ADDR              ((uint8_t)3)
#define SD_CMD_SET_DSR                   ((uint8_t)4)
#define SD_CMD_SDIO_SEN_OP_COND          ((uint8_t)5)
#define SD_CMD_HS_SWITCH                 ((uint8_t)6)
#define SD_CMD_SEL_DESEL_CARD            ((uint8_t)7)
#define SD_CMD_HS_SEND_EXT_CSD           ((uint8_t)8)
#define SD_CMD_SEND_CSD                  ((uint8_t)9)
#define SD_CMD_SEND_CID                  ((uint8_t)10)
#define SD_CMD_READ_DAT_UNTIL_STOP       ((uint8_t)11)
#define SD_CMD_STOP_TRANSMISSION         ((uint8_t)12)
#define SD_CMD_SEND_STATUS               ((uint8_t)13)
#define SD_CMD_HS_BUSTEST_READ           ((uint8_t)14)
#define SD_CMD_GO_INACTIVE_STATE         ((uint8_t)15)
#define SD_CMD_SET_BLOCKLEN              ((uint8_t)16)
#define SD_CMD_READ_SINGLE_BLOCK         ((uint8_t)17)
#define SD_CMD_READ_MULT_BLOCK           ((uint8_t)18)
#define SD_CMD_HS_BUSTEST_WRITE          ((uint8_t)19)
#define SD_CMD_WRITE_DAT_UNTIL_STOP      ((uint8_t)20)
#define SD_CMD_SET_BLOCK_COUNT           ((uint8_t)23)
#define SD_CMD_WRITE_SINGLE_BLOCK        ((uint8_t)24)
#define SD_CMD_WRITE_MULT_BLOCK          ((uint8_t)25)
#define SD_CMD_PROG_CID                  ((uint8_t)26)
#define SD_CMD_PROG_CSD                  ((uint8_t)27)
#define SD_CMD_SET_WRITE_PROT            ((uint8_t)28)
#define SD_CMD_CLR_WRITE_PROT            ((uint8_t)29)
#define SD_CMD_SEND_WRITE_PROT           ((uint8_t)30)
#define SD_CMD_SD_ERASE_GRP_START        ((uint8_t)32)
#define SD_CMD_SD_ERASE_GRP_END          ((uint8_t)33)
#define SD_CMD_ERASE_GRP_START           ((uint8_t)35)
#define SD_CMD_ERASE_GRP_END             ((uint8_t)36)
#define SD_CMD_ERASE                     ((uint8_t)38)
#define SD_CMD_FAST_IO                   ((uint8_t)39)
#define SD_CMD_GO_IRQ_STATE              ((uint8_t)40)
#define SD_CMD_LOCK_UNLOCK               ((uint8_t)42)
#define SD_CMD_APP_CMD                   ((uint8_t)55)
#define SD_CMD_GEN_CMD                   ((uint8_t)56)
#define SD_CMD_NO_CMD                    ((uint8_t)64)

/**
  * following commands are sd card specific commands.
  * should be sent before sending these commands.
  */
#define SD_CMD_APP_SD_SET_BUSWIDTH       ((uint8_t)6)
#define SD_CMD_SD_APP_STAUS              ((uint8_t)13)
#define SD_CMD_SD_APP_SEND_NUM_WRITE_BLOCKS ((uint8_t)22)
#define SD_CMD_SD_APP_OP_COND            ((uint8_t)41)
#define SD_CMD_SD_APP_SET_CLR_CARD_DETECT ((uint8_t)42)
#define SD_CMD_SD_APP_SEND_SCR           ((uint8_t)51)
#define SD_CMD_SDIO_RW_DIRECT            ((uint8_t)52)
#define SD_CMD_SDIO_RW_EXTENDED          ((uint8_t)53)

/**
  * following commands are sd card specific security commands.
  * sdio_app_cmd should be sent before sending these commands.
  */
#define SD_CMD_SD_APP_GET_MKB            ((uint8_t)43)
#define SD_CMD_SD_APP_GET_MID            ((uint8_t)44)
#define SD_CMD_SD_APP_SET_CER_RN1        ((uint8_t)45)
#define SD_CMD_SD_APP_GET_CER_RN2        ((uint8_t)46)
#define SD_CMD_SD_APP_SET_CER_RES2       ((uint8_t)47)
#define SD_CMD_SD_APP_GET_CER_RES1       ((uint8_t)48)
#define SD_CMD_SD_APP_SECURE_READ_MULTIPLE_BLOCK ((uint8_t)18)
#define SD_CMD_SD_APP_SECURE_WRITE_MULTIPLE_BLOCK ((uint8_t)25)
#define SD_CMD_SD_APP_SECURE_ERASE       ((uint8_t)38)
#define SD_CMD_SD_APP_CHANGE_SECURE_AREA ((uint8_t)49)
#define SD_CMD_SD_APP_SECURE_WRITE_MKB   ((uint8_t)48)

/**
  * @}
  */

/** @defgroup SDIO_sdio_fatfs_paremeters_definition
  * @{
  */

/**
  * sdio paremeters
  */
#define NULL                             0
#define SDIO_STATIC_FLAGS                ((uint32_t)0x000005FF)
#define SDIO_CMD0TIMEOUT                 ((uint32_t)0x00010000)
#define SDIO_DATATIMEOUT                 ((uint32_t)0xFFFFFFFF)
#define SDIO_FIFO_Address                ((uint32_t)0x40018080)

/**
  * @}
  */

/** @defgroup SDIO_sdio_fatfs_response_definition
  * @{
  */

/**
  * mask for errors card status r1 (ocr register)
  */
#define SD_OCR_ADDR_OUT_OF_RANGE         ((uint32_t)0x80000000)
#define SD_OCR_ADDR_MISALIGNED           ((uint32_t)0x40000000)
#define SD_OCR_BLK_LEN_ERR               ((uint32_t)0x20000000)
#define SD_OCR_ERASE_SEQ_ERR             ((uint32_t)0x10000000)
#define SD_OCR_INVALID_ERASE_PARAM       ((uint32_t)0x08000000)
#define SD_OCR_WR_PROTECT_VIOLATION      ((uint32_t)0x04000000)
#define SD_OCR_LOCK_UNLOCK_FAILED        ((uint32_t)0x01000000)
#define SD_OCR_CMD_CRC_FAILED            ((uint32_t)0x00800000)
#define SD_OCR_ILLEGAL_CMD               ((uint32_t)0x00400000)
#define SD_OCR_CARD_ECC_FAILED           ((uint32_t)0x00200000)
#define SD_OCR_CARD_CONTROLLER_ERR       ((uint32_t)0x00100000)
#define SD_OCR_GENERAL_UNKNOWN_ERROR     ((uint32_t)0x00080000)
#define SD_OCR_STREAM_RD_UNDERRUN        ((uint32_t)0x00040000)
#define SD_OCR_STREAM_WR_OVERRUN         ((uint32_t)0x00020000)
#define SD_OCR_CID_CSD_OVERWRIETE        ((uint32_t)0x00010000)
#define SD_OCR_WP_ERASE_SKIP             ((uint32_t)0x00008000)
#define SD_OCR_CARD_ECC_DISABLED         ((uint32_t)0x00004000)
#define SD_OCR_ERASE_RESET               ((uint32_t)0x00002000)
#define MMC_SWITCH_ERROR                 ((uint32_t)0x00000080)
#define SD_OCR_AKE_SEQ_ERROR             ((uint32_t)0x00000008)
#define SD_OCR_ERRORBITS                 ((uint32_t)0xFDFFE008)

/**
  * masks for r5 response
  */
#define SD_R5_OUT_OF_RANGE               ((uint32_t)0x00000100)
#define SD_R5_FUNCTION_NUMBER            ((uint32_t)0x00000200)
#define SD_R5_ERROR                      ((uint32_t)0x00000800)

/**
  * masks for r6 response
  */
#define SD_R6_GENERAL_UNKNOWN_ERROR      ((uint32_t)0x00002000)
#define SD_R6_ILLEGAL_CMD                ((uint32_t)0x00004000)
#define SD_R6_CMD_CRC_FAILED             ((uint32_t)0x00008000)
#define SD_VOLTAGE_WINDOW_SD             ((uint32_t)0x80100000)
#define SD_HIGH_CAPACITY                 ((uint32_t)0x40000000)
#define SD_STD_CAPACITY                  ((uint32_t)0x00000000)
#define SD_CHECK_PATTERN                 ((uint32_t)0x000001AA)
#define SD_VOLTAGE_WINDOW_MMC            ((uint32_t)0x80FF8000)
#define SD_MAX_VOLT_TRIAL                ((uint32_t)0x0000FFFF)
#define SD_ALLZERO                       ((uint32_t)0x00000000)
#define SD_WIDE_BUS_SUPPORT              ((uint32_t)0x00040000)
#define SD_SINGLE_BUS_SUPPORT            ((uint32_t)0x00010000)
#define SD_CARD_LOCKED                   ((uint32_t)0x02000000)
#define SD_CARD_PROGRAMMING              ((uint32_t)0x00000007)
#define SD_CARD_RECEIVING                ((uint32_t)0x00000006)
#define SD_DATATIMEOUT                   ((uint32_t)0xFFFFFFFF)
#define SD_0TO7BITS                      ((uint32_t)0x000000FF)
#define SD_8TO15BITS                     ((uint32_t)0x0000FF00)
#define SD_16TO23BITS                    ((uint32_t)0x00FF0000)
#define SD_24TO31BITS                    ((uint32_t)0xFF000000)
#define SD_MAX_DATA_LENGTH               ((uint32_t)0x01FFFFFF)
#define SD_HALFFIFO                      ((uint32_t)0x00000008)
#define SD_HALFFIFOBYTES                 ((uint32_t)0x00000020)

/**
  * @}
  */

/** @defgroup SDIO_sdio_fatfs_command_class_definition
  * @{
  */

/**
  * command class supported
  */
#define SD_CCCC_LOCK_UNLOCK              ((uint32_t)0x00000080)
#define SD_CCCC_WRITE_PROT               ((uint32_t)0x00000040)
#define SD_CCCC_ERASE                    ((uint32_t)0x00000020)

/**
  * @}
  */

/** @defgroup SDIO_sdio_fatfs_cmd8_definition
  * @{
  */

/**
  * cmd8
  */
#define SDIO_SEND_IF_COND                ((uint32_t)0x00000008)

/**
  * @}
  */

/** @defgroup SDIO_sdio_fatfs_mmc_extend_definition
  * @{
  */

/**
  * mmc ext_csd operation
  */
#define EXT_CSD_Command_set              0x0
#define EXT_CSD_Set_bit                  0x1
#define EXT_CSD_Clear_byte               0x2
#define EXT_CSD_Write_byte               0x3

#define EXT_CSD_CMD_SET_NORMAL           (1<<0)
#define EXT_CSD_CMD_SET_SECURE           (1<<1)
#define EXT_CSD_CMD_SET_CPSECURE         (1<<2)

/**
  * mmc ext_csd offset
  */
#define EXT_CSD_BUS_WIDTH                183
#define EXT_CSD_HS_TIMING                185

/**
  * @}
  */

/** @defgroup SDIO_sdio_fatfs_interrupt_flags_definition
  * @{
  */

#define SDIO_INTR_STS_WRITE_MASK         (SDIO_DTFAIL_FLAG | SDIO_DTTIMEOUT_FLAG | SDIO_TXERRU_FLAG | \
                                          SDIO_DTCMPL_FLAG | SDIO_SBITERR_FLAG)
#define SDIO_INTR_STS_READ_MASK          (SDIO_DTFAIL_FLAG | SDIO_DTTIMEOUT_FLAG | SDIO_RXERRO_FLAG | \
                                          SDIO_DTCMPL_FLAG | SDIO_SBITERR_FLAG)
/**
  * @}
  */

extern sd_card_info_struct_type sd_card_info;

/** @defgroup SDIO_sdio_fatfs_exported_functions
  * @{
  */

/* sd exported functions ----------------------------------------*/
sd_error_status_type sd_init(void);
void sdio_clock_set(uint32_t clkdiv);
sd_error_status_type sd_power_on(void);
sd_error_status_type sd_power_off(void);
sd_error_status_type sd_card_init(void);
sd_error_status_type sd_card_info_get(sd_card_info_struct_type *card_info);
sd_error_status_type sd_wide_bus_operation_config(sdio_bus_width_type mode);
sd_error_status_type sd_device_mode_set(uint32_t mode);
sd_error_status_type sd_deselect_select(uint32_t addr);
sd_error_status_type sd_status_send(uint32_t *p_card_status);
sd_card_state_type sd_state_get(void);
sd_error_status_type sd_blocks_erase(long long addr, uint32_t nblks);
sd_error_status_type sd_block_read(uint8_t *buf, long long addr, uint16_t blk_size);
sd_error_status_type sd_mult_blocks_read(uint8_t *buf, long long addr, uint16_t blk_size, uint32_t nblks);
sd_error_status_type sd_block_write(const uint8_t *buf, long long addr, uint16_t blk_size);
sd_error_status_type sd_mult_blocks_write(const uint8_t *buf, long long addr, uint16_t blk_size, uint32_t nblks);
sd_error_status_type mmc_stream_read(uint8_t *buf, long long addr, uint32_t len);
sd_error_status_type mmc_stream_write(uint8_t *buf, long long addr, uint32_t len);
sd_error_status_type sd_irq_service(void);
void sd_dma_config(uint32_t *mbuf, uint32_t buf_size, dma_dir_type dir);

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

#endif /* __AT32_SDIO_H */
