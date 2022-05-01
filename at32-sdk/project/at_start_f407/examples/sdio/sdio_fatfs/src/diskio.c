/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/


#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */

#include <string.h>
#include "at32_sdio.h"		/* Example: Header file of existing MMC/SDC contorl module */

/* Definitions of physical drive number for each drive */
#define DEV_RAM		0	/* Example: Map Ramdisk to physical drive 0 */
#define DEV_MMC		1	/* Example: Map MMC/SD card to physical drive 1 */
#define DEV_USB		2	/* Example: Map USB MSD to physical drive 2 */

__align(4) uint8_t sdio_data_buffer[512]; /* buf for sd_read_disk/sd_write_disk function used. */

sd_error_status_type sd_read_disk(uint8_t *buf, uint32_t sector, uint8_t cnt);
sd_error_status_type sd_write_disk(const uint8_t *buf, uint32_t sector, uint8_t cnt);

/**
  * @brief  read sd card sector
  * @param  buf: read data buf
  * @param  sector: sector address
  * @param  cnt: sector count
  * @retval sd_error_status_type: sd card error code.
  */
sd_error_status_type sd_read_disk(uint8_t *buf, uint32_t sector, uint8_t cnt)
{
  sd_error_status_type sta = SD_OK;
  long long lsector = sector;
  uint8_t n;

  /* data address is in block (512 byte) units. */
  lsector <<= 9;

  if((uint32_t)buf % 4 != 0)
  {
    for(n = 0; n < cnt; n++)
    {
      sta = sd_block_read(sdio_data_buffer, lsector + 512 * n, 512);
      memcpy(buf, sdio_data_buffer, 512);
      buf += 512;
    }
  }
  else
  {
    if(cnt == 1)
    {
      sta = sd_block_read(buf, lsector, 512);
    }
    else
    {
      sta = sd_mult_blocks_read(buf, lsector, 512, cnt);
    }
  }

  return sta;
}

/**
  * @brief  write sd card sector
  * @param  buf: write data buf
  * @param  sector: sector address
  * @param  cnt: sector count
  * @retval sd_error_status_type: sd card error code.
  */
sd_error_status_type sd_write_disk(const uint8_t *buf, uint32_t sector, uint8_t cnt)
{
  sd_error_status_type sta = SD_OK;
  uint8_t n;
  long long lsector = sector;

  /* data address is in block (512 byte) units. */
  lsector <<= 9;

  if((uint32_t)buf % 4 != 0)
  {
    for(n = 0; n < cnt; n++)
    {
      memcpy(sdio_data_buffer, buf, 512);
      sta = sd_block_write(sdio_data_buffer, lsector + 512*n, 512);
      buf += 512;
    }
  }
  else
  {
    if(cnt == 1)
    {
      sta = sd_block_write(buf, lsector, 512);
    }
    else
    {
      sta = sd_mult_blocks_write(buf, lsector, 512, cnt);
    }
  }

  return sta;
}

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/
DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	int result;

	switch (pdrv) {
	case DEV_RAM :
    result = 1;
    stat = (DSTATUS)result;
		return stat;

	case DEV_MMC :
    result = 0;
    stat = (DSTATUS)result;
		return stat;

	case DEV_USB :
    result = 1;
    stat = (DSTATUS)result;
		return stat;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/
DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	int result;

	switch (pdrv) {
	case DEV_RAM :
    result = 1;
    stat = (DSTATUS)result;
		return stat;

	case DEV_MMC :
    result = sd_init();
    stat = (DSTATUS)result;
		return stat;

	case DEV_USB :
    result = 1;
    stat = (DSTATUS)result;
		return stat;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/
DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	LBA_t sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	DRESULT res;
	int result;

	switch (pdrv) {
	case DEV_RAM :
    result = 1;
    res = (DRESULT)result;
		return res;

	case DEV_MMC :
    result = sd_read_disk(buff, sector, count);
    res = (DRESULT)result;
		return res;

	case DEV_USB :
    result = 1;
    res = (DRESULT)result;
		return res;
	}

	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/
#if FF_FS_READONLY == 0

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	LBA_t sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	DRESULT res;
	int result;

	switch (pdrv) {
	case DEV_RAM :
    result = 1;
    res = (DRESULT)result;
		return res;

	case DEV_MMC :
    result = sd_write_disk(buff, sector, count);
    res = (DRESULT)result;
		return res;

	case DEV_USB :
    result = 1;
    res = (DRESULT)result;
		return res;
	}

	return RES_PARERR;
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/
DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;
	int result;

	switch (pdrv) {
	case DEV_RAM :
    result = 1;
    res = (DRESULT)result;
		return res;

	case DEV_MMC :
    switch(cmd){
      case CTRL_SYNC:
        result = RES_OK;
        break;
      case GET_SECTOR_SIZE:
        *(DWORD*)buff = 512;
        result = RES_OK;
        break;
      case GET_SECTOR_COUNT:
        *(DWORD*)buff = sd_card_info.card_capacity/512;
        result = RES_OK;
        break;
      case GET_BLOCK_SIZE:
        *(DWORD*)buff = sd_card_info.card_blk_size;
        result = RES_OK;
        break;
      default:
        result = RES_PARERR;
        break;
    }
    res = (DRESULT)result;
		return res;

	case DEV_USB :
    result = 1;
    res = (DRESULT)result;
		return res;
	}

	return RES_PARERR;
}
