/**
  **************************************************************************
  * @file     fsdata.h
  * @version  v2.0.8
  * @date     2022-04-02
  * @brief    fsdata header file
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

#ifndef __FSDATA_H__
#define __FSDATA_H__

#ifdef __cplusplus
 extern "C" {
#endif

#define FS_ROOT                          file_uploaddone_html
#define FS_NUMFILES                      4

struct fsdata_file {
  const struct fsdata_file *next;
  const char *name;
  const char *data;
  const int len;
};

struct fsdata_file_noconst {
  struct fsdata_file *next;
  char *name;
  char *data;
  int len;
};

extern const struct fsdata_file file_index_html[];
extern const struct fsdata_file file_reset_html[];
extern const struct fsdata_file file_upload_html[];
extern const struct fsdata_file file_uploaddone_html[];

#ifdef __cplusplus
}
#endif

#endif /* __FSDATA_H__ */
