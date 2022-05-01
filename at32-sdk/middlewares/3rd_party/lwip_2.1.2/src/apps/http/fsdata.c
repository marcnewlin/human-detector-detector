#include "lwip/apps/fs.h"
#include "lwip/def.h"


#define file_NULL (struct fsdata_file *) NULL


#ifndef FS_FILE_FLAGS_HEADER_INCLUDED
#define FS_FILE_FLAGS_HEADER_INCLUDED 1
#endif
#ifndef FS_FILE_FLAGS_HEADER_PERSISTENT
#define FS_FILE_FLAGS_HEADER_PERSISTENT 0
#endif
/* FSDATA_FILE_ALIGNMENT: 0=off, 1=by variable, 2=by include */
#ifndef FSDATA_FILE_ALIGNMENT
#define FSDATA_FILE_ALIGNMENT 0
#endif
#ifndef FSDATA_ALIGN_PRE
#define FSDATA_ALIGN_PRE
#endif
#ifndef FSDATA_ALIGN_POST
#define FSDATA_ALIGN_POST
#endif
#if FSDATA_FILE_ALIGNMENT==2
#include "fsdata_alignment.h"
#endif
#if FSDATA_FILE_ALIGNMENT==1
static const unsigned int dummy_align__img_sics_gif = 0;
#endif

const unsigned char main_page_name[] = "/AT32F407.html";
const unsigned char led_page_name[] = "/AT32F407LED.html";
const unsigned char adc_page_name[] = "/AT32F407ADC.html";

static const unsigned char FSDATA_ALIGN_PRE data__img_sics_gif[] FSDATA_ALIGN_POST = {
/* /img/sics.gif (14 chars) */
0x2f,0x69,0x6d,0x67,0x2f,0x73,0x69,0x63,0x73,0x2e,0x67,0x69,0x66,0x00,0x00,0x00,

/* HTTP header */
/* "HTTP/1.0 200 OK
" (17 bytes) */
0x48,0x54,0x54,0x50,0x2f,0x31,0x2e,0x30,0x20,0x32,0x30,0x30,0x20,0x4f,0x4b,0x0d,
0x0a,
/* "Server: lwIP/2.0.3d (http://savannah.nongnu.org/projects/lwip)
" (64 bytes) */
0x53,0x65,0x72,0x76,0x65,0x72,0x3a,0x20,0x6c,0x77,0x49,0x50,0x2f,0x32,0x2e,0x30,
0x2e,0x33,0x64,0x20,0x28,0x68,0x74,0x74,0x70,0x3a,0x2f,0x2f,0x73,0x61,0x76,0x61,
0x6e,0x6e,0x61,0x68,0x2e,0x6e,0x6f,0x6e,0x67,0x6e,0x75,0x2e,0x6f,0x72,0x67,0x2f,
0x70,0x72,0x6f,0x6a,0x65,0x63,0x74,0x73,0x2f,0x6c,0x77,0x69,0x70,0x29,0x0d,0x0a,

/* "Content-Length: 724
" (18+ bytes) */
0x43,0x6f,0x6e,0x74,0x65,0x6e,0x74,0x2d,0x4c,0x65,0x6e,0x67,0x74,0x68,0x3a,0x20,
0x37,0x32,0x34,0x0d,0x0a,
/* "Content-Type: image/gif

" (27 bytes) */
0x43,0x6f,0x6e,0x74,0x65,0x6e,0x74,0x2d,0x54,0x79,0x70,0x65,0x3a,0x20,0x69,0x6d,
0x61,0x67,0x65,0x2f,0x67,0x69,0x66,0x0d,0x0a,0x0d,0x0a,
/* raw file data (724 bytes) */
0x47,0x49,0x46,0x38,0x39,0x61,0x46,0x00,0x22,0x00,0xa5,0x00,0x00,0xd9,0x2b,0x39,
0x6a,0x6a,0x6a,0xbf,0xbf,0xbf,0x93,0x93,0x93,0x0f,0x0f,0x0f,0xb0,0xb0,0xb0,0xa6,
0xa6,0xa6,0x80,0x80,0x80,0x76,0x76,0x76,0x1e,0x1e,0x1e,0x9d,0x9d,0x9d,0x2e,0x2e,
0x2e,0x49,0x49,0x49,0x54,0x54,0x54,0x8a,0x8a,0x8a,0x60,0x60,0x60,0xc6,0xa6,0x99,
0xbd,0xb5,0xb2,0xc2,0xab,0xa1,0xd9,0x41,0x40,0xd5,0x67,0x55,0xc0,0xb0,0xaa,0xd5,
0x5e,0x4e,0xd6,0x50,0x45,0xcc,0x93,0x7d,0xc8,0xa1,0x90,0xce,0x8b,0x76,0xd2,0x7b,
0x65,0xd1,0x84,0x6d,0xc9,0x99,0x86,0x3a,0x3a,0x3a,0x00,0x00,0x00,0xb8,0xb8,0xb8,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x2c,0x00,0x00,
0x00,0x00,0x46,0x00,0x22,0x00,0x00,0x06,0xfe,0x40,0x90,0x70,0x48,0x2c,0x1a,0x8f,
0xc8,0xa4,0x72,0xc9,0x6c,0x3a,0x9f,0xd0,0xa8,0x74,0x4a,0xad,0x5a,0xaf,0xd8,0xac,
0x76,0xa9,0x40,0x04,0xbe,0x83,0xe2,0x60,0x3c,0x50,0x20,0x0d,0x8e,0x6f,0x00,0x31,
0x28,0x1c,0x0d,0x07,0xb5,0xc3,0x60,0x75,0x24,0x3e,0xf8,0xfc,0x87,0x11,0x06,0xe9,
0x3d,0x46,0x07,0x0b,0x7a,0x7a,0x7c,0x43,0x06,0x1e,0x84,0x78,0x0b,0x07,0x6e,0x51,
0x01,0x8a,0x84,0x08,0x7e,0x79,0x80,0x87,0x89,0x91,0x7a,0x93,0x0a,0x04,0x99,0x78,
0x96,0x4f,0x03,0x9e,0x79,0x01,0x94,0x9f,0x43,0x9c,0xa3,0xa4,0x05,0x77,0xa3,0xa0,
0x4e,0x98,0x79,0x0b,0x1e,0x83,0xa4,0xa6,0x1f,0x96,0x05,0x9d,0xaa,0x78,0x01,0x07,
0x84,0x04,0x1e,0x1e,0xbb,0xb8,0x51,0x84,0x0e,0x43,0x05,0x07,0x77,0xa5,0x7f,0x42,
0xb1,0xb2,0x01,0x63,0x08,0x0d,0xbb,0x01,0x0c,0x7a,0x0d,0x44,0x0e,0xd8,0xaf,0x4c,
0x05,0x7a,0x04,0x47,0x07,0x07,0xb7,0x80,0xa2,0xe1,0x7d,0x44,0x05,0x01,0x04,0x01,
0xd0,0xea,0x87,0x93,0x4f,0xe0,0x9a,0x49,0xce,0xd8,0x79,0x04,0x66,0x20,0x15,0x10,
0x10,0x11,0x92,0x29,0x80,0xb6,0xc0,0x91,0x15,0x45,0x1e,0x90,0x19,0x71,0x46,0xa8,
0x5c,0x04,0x0e,0x00,0x22,0x4e,0xe8,0x40,0x24,0x9f,0x3e,0x04,0x06,0xa7,0x58,0xd4,
0x93,0xa0,0x1c,0x91,0x3f,0xe8,0xf0,0x88,0x03,0xb1,0x21,0xa2,0x49,0x00,0x19,0x86,
0xfc,0x52,0x44,0xe0,0x01,0x9d,0x29,0x21,0x15,0x25,0x50,0xf7,0x67,0x25,0x1e,0x06,
0xfd,0x4e,0x9a,0xb4,0x90,0xac,0x15,0xfa,0xcb,0x52,0x53,0x1e,0x8c,0xf2,0xf8,0x07,
0x92,0x2d,0x08,0x3a,0x4d,0x12,0x49,0x95,0x49,0xdb,0x14,0x04,0xc4,0x14,0x85,0x29,
0xaa,0xe7,0x01,0x08,0xa4,0x49,0x01,0x14,0x51,0xe0,0x53,0x91,0xd5,0x29,0x06,0x1a,
0x64,0x02,0xf4,0xc7,0x81,0x9e,0x05,0x20,0x22,0x64,0xa5,0x30,0xae,0xab,0x9e,0x97,
0x53,0xd8,0xb9,0xfd,0x50,0xef,0x93,0x02,0x42,0x74,0x34,0xe8,0x9c,0x20,0x21,0xc9,
0x01,0x68,0x78,0xe6,0x55,0x29,0x20,0x56,0x4f,0x4c,0x40,0x51,0x71,0x82,0xc0,0x70,
0x21,0x22,0x85,0xbe,0x4b,0x1c,0x44,0x05,0xea,0xa4,0x01,0xbf,0x22,0xb5,0xf0,0x1c,
0x06,0x51,0x38,0x8f,0xe0,0x22,0xec,0x18,0xac,0x39,0x22,0xd4,0xd6,0x93,0x44,0x01,
0x32,0x82,0xc8,0xfc,0x61,0xb3,0x01,0x45,0x0c,0x2e,0x83,0x30,0xd0,0x0e,0x17,0x24,
0x0f,0x70,0x85,0x94,0xee,0x05,0x05,0x53,0x4b,0x32,0x1b,0x3f,0x98,0xd3,0x1d,0x29,
0x81,0xb0,0xae,0x1e,0x8c,0x7e,0x68,0xe0,0x60,0x5a,0x54,0x8f,0xb0,0x78,0x69,0x73,
0x06,0xa2,0x00,0x6b,0x57,0xca,0x3d,0x11,0x50,0xbd,0x04,0x30,0x4b,0x3a,0xd4,0xab,
0x5f,0x1f,0x9b,0x3d,0x13,0x74,0x27,0x88,0x3c,0x25,0xe0,0x17,0xbe,0x7a,0x79,0x45,
0x0d,0x0c,0xb0,0x8b,0xda,0x90,0xca,0x80,0x06,0x5d,0x17,0x60,0x1c,0x22,0x4c,0xd8,
0x57,0x22,0x06,0x20,0x00,0x98,0x07,0x08,0xe4,0x56,0x80,0x80,0x1c,0xc5,0xb7,0xc5,
0x82,0x0c,0x36,0xe8,0xe0,0x83,0x10,0x46,0x28,0xe1,0x84,0x14,0x56,0x68,0xa1,0x10,
0x41,0x00,0x00,0x3b,};

#if FSDATA_FILE_ALIGNMENT==1
static const unsigned int dummy_align__404_html = 1;
#endif
static const unsigned char FSDATA_ALIGN_PRE data__404_html[] FSDATA_ALIGN_POST = {
/* /404.html (10 chars) */
0x2f,0x34,0x30,0x34,0x2e,0x68,0x74,0x6d,0x6c,0x00,0x00,0x00,

/* HTTP header */
/* "HTTP/1.0 404 File not found
" (29 bytes) */
0x48,0x54,0x54,0x50,0x2f,0x31,0x2e,0x30,0x20,0x34,0x30,0x34,0x20,0x46,0x69,0x6c,
0x65,0x20,0x6e,0x6f,0x74,0x20,0x66,0x6f,0x75,0x6e,0x64,0x0d,0x0a,
/* "Server: lwIP/2.0.3d (http://savannah.nongnu.org/projects/lwip)
" (64 bytes) */
0x53,0x65,0x72,0x76,0x65,0x72,0x3a,0x20,0x6c,0x77,0x49,0x50,0x2f,0x32,0x2e,0x30,
0x2e,0x33,0x64,0x20,0x28,0x68,0x74,0x74,0x70,0x3a,0x2f,0x2f,0x73,0x61,0x76,0x61,
0x6e,0x6e,0x61,0x68,0x2e,0x6e,0x6f,0x6e,0x67,0x6e,0x75,0x2e,0x6f,0x72,0x67,0x2f,
0x70,0x72,0x6f,0x6a,0x65,0x63,0x74,0x73,0x2f,0x6c,0x77,0x69,0x70,0x29,0x0d,0x0a,

/* "Content-Length: 565
" (18+ bytes) */
0x43,0x6f,0x6e,0x74,0x65,0x6e,0x74,0x2d,0x4c,0x65,0x6e,0x67,0x74,0x68,0x3a,0x20,
0x35,0x36,0x35,0x0d,0x0a,
/* "Content-Type: text/html

" (27 bytes) */
0x43,0x6f,0x6e,0x74,0x65,0x6e,0x74,0x2d,0x54,0x79,0x70,0x65,0x3a,0x20,0x74,0x65,
0x78,0x74,0x2f,0x68,0x74,0x6d,0x6c,0x0d,0x0a,0x0d,0x0a,
/* raw file data (565 bytes) */
0x3c,0x68,0x74,0x6d,0x6c,0x3e,0x0d,0x0a,0x3c,0x68,0x65,0x61,0x64,0x3e,0x3c,0x74,
0x69,0x74,0x6c,0x65,0x3e,0x6c,0x77,0x49,0x50,0x20,0x2d,0x20,0x41,0x20,0x4c,0x69,
0x67,0x68,0x74,0x77,0x65,0x69,0x67,0x68,0x74,0x20,0x54,0x43,0x50,0x2f,0x49,0x50,
0x20,0x53,0x74,0x61,0x63,0x6b,0x3c,0x2f,0x74,0x69,0x74,0x6c,0x65,0x3e,0x3c,0x2f,
0x68,0x65,0x61,0x64,0x3e,0x0d,0x0a,0x3c,0x62,0x6f,0x64,0x79,0x20,0x62,0x67,0x63,
0x6f,0x6c,0x6f,0x72,0x3d,0x22,0x77,0x68,0x69,0x74,0x65,0x22,0x20,0x74,0x65,0x78,
0x74,0x3d,0x22,0x62,0x6c,0x61,0x63,0x6b,0x22,0x3e,0x0d,0x0a,0x0d,0x0a,0x20,0x20,
0x20,0x20,0x3c,0x74,0x61,0x62,0x6c,0x65,0x20,0x77,0x69,0x64,0x74,0x68,0x3d,0x22,
0x31,0x30,0x30,0x25,0x22,0x3e,0x0d,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x3c,0x74,
0x72,0x20,0x76,0x61,0x6c,0x69,0x67,0x6e,0x3d,0x22,0x74,0x6f,0x70,0x22,0x3e,0x3c,
0x74,0x64,0x20,0x77,0x69,0x64,0x74,0x68,0x3d,0x22,0x38,0x30,0x22,0x3e,0x09,0x20,
0x20,0x0d,0x0a,0x09,0x20,0x20,0x3c,0x61,0x20,0x68,0x72,0x65,0x66,0x3d,0x22,0x68,
0x74,0x74,0x70,0x3a,0x2f,0x2f,0x77,0x77,0x77,0x2e,0x73,0x69,0x63,0x73,0x2e,0x73,
0x65,0x2f,0x22,0x3e,0x3c,0x69,0x6d,0x67,0x20,0x73,0x72,0x63,0x3d,0x22,0x2f,0x69,
0x6d,0x67,0x2f,0x73,0x69,0x63,0x73,0x2e,0x67,0x69,0x66,0x22,0x0d,0x0a,0x09,0x20,
0x20,0x62,0x6f,0x72,0x64,0x65,0x72,0x3d,0x22,0x30,0x22,0x20,0x61,0x6c,0x74,0x3d,
0x22,0x53,0x49,0x43,0x53,0x20,0x6c,0x6f,0x67,0x6f,0x22,0x20,0x74,0x69,0x74,0x6c,
0x65,0x3d,0x22,0x53,0x49,0x43,0x53,0x20,0x6c,0x6f,0x67,0x6f,0x22,0x3e,0x3c,0x2f,
0x61,0x3e,0x0d,0x0a,0x09,0x3c,0x2f,0x74,0x64,0x3e,0x3c,0x74,0x64,0x20,0x77,0x69,
0x64,0x74,0x68,0x3d,0x22,0x35,0x30,0x30,0x22,0x3e,0x09,0x20,0x20,0x0d,0x0a,0x09,
0x20,0x20,0x3c,0x68,0x31,0x3e,0x6c,0x77,0x49,0x50,0x20,0x2d,0x20,0x41,0x20,0x4c,
0x69,0x67,0x68,0x74,0x77,0x65,0x69,0x67,0x68,0x74,0x20,0x54,0x43,0x50,0x2f,0x49,
0x50,0x20,0x53,0x74,0x61,0x63,0x6b,0x3c,0x2f,0x68,0x31,0x3e,0x0d,0x0a,0x09,0x20,
0x20,0x3c,0x68,0x32,0x3e,0x34,0x30,0x34,0x20,0x2d,0x20,0x50,0x61,0x67,0x65,0x20,
0x6e,0x6f,0x74,0x20,0x66,0x6f,0x75,0x6e,0x64,0x3c,0x2f,0x68,0x32,0x3e,0x0d,0x0a,
0x09,0x20,0x20,0x3c,0x70,0x3e,0x0d,0x0a,0x09,0x20,0x20,0x20,0x20,0x53,0x6f,0x72,
0x72,0x79,0x2c,0x20,0x74,0x68,0x65,0x20,0x70,0x61,0x67,0x65,0x20,0x79,0x6f,0x75,
0x20,0x61,0x72,0x65,0x20,0x72,0x65,0x71,0x75,0x65,0x73,0x74,0x69,0x6e,0x67,0x20,
0x77,0x61,0x73,0x20,0x6e,0x6f,0x74,0x20,0x66,0x6f,0x75,0x6e,0x64,0x20,0x6f,0x6e,
0x20,0x74,0x68,0x69,0x73,0x0d,0x0a,0x09,0x20,0x20,0x20,0x20,0x73,0x65,0x72,0x76,
0x65,0x72,0x2e,0x20,0x0d,0x0a,0x09,0x20,0x20,0x3c,0x2f,0x70,0x3e,0x0d,0x0a,0x09,
0x3c,0x2f,0x74,0x64,0x3e,0x3c,0x74,0x64,0x3e,0x0d,0x0a,0x09,0x20,0x20,0x26,0x6e,
0x62,0x73,0x70,0x3b,0x0d,0x0a,0x09,0x3c,0x2f,0x74,0x64,0x3e,0x3c,0x2f,0x74,0x72,
0x3e,0x0d,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x3c,0x2f,0x74,0x61,0x62,0x6c,0x65,
0x3e,0x0d,0x0a,0x3c,0x2f,0x62,0x6f,0x64,0x79,0x3e,0x0d,0x0a,0x3c,0x2f,0x68,0x74,
0x6d,0x6c,0x3e,0x0d,0x0a,};

#if FSDATA_FILE_ALIGNMENT==1
static const unsigned int dummy_align__index_html = 2;
#endif

static const unsigned char FSDATA_ALIGN_PRE data_AT32F407_html[] FSDATA_ALIGN_POST = " \
<html>\
    <head>\
        <title>Technology Corp. Demo Web Page</title>\
    </head>\
    <body>\
        <div style=\"text-align: center; margin-left: 40px; width: 977px;\">\
            <span>\
                <br />\
                <big>\
                    <big>\
                        <big style=\"font-weight: bold;\"><big>AT32F407 web server demo </big></big>\
                    </big>\
                </big>\
            </span>\
            <small>\
                <small>\
                    <big>\
                        <big>\
                            <big style=\"font-weight: bold;\">\
                                <big>\
                                    <font size=\"6\">\
                                        <big>\
                                            <big>\
                                                <big>\
                                                    <big>\
                                                        <big><strong></strong></big>\
                                                    </big>\
                                                </big>\
                                            </big>\
                                        </big>\
                                    </font>\
                                </big>\
                            </big>\
                        </big>\
                    </big>\
                    <font size=\"6\">\
                        <big>&nbsp;</big>\
                        <small style=\"font-family: Verdana;\">\
                            <small>\
                                <big>\
                                    <big>\
                                        <big>\
                                            <big style=\"font-weight: bold; color: rgb(51, 51, 255);\">\
                                                <big>\
                                                    <strong><span style=\"font-style: italic;\"></span></strong>\
                                                </big>\
                                                <span style=\"color: rgb(51, 51, 255);\">\
                                                    <font size=\"8\">\
                                                        <br />\
                                                        ****************************<br />\
                                                        ****************************\
                                                    </font>\
                                                </span>\
                                            </big>\
                                        </big>\
                                    </big>\
                                </big>\
                            </small>\
                        </small>\
                    </font>\
                </small>\
            </small>\
            <hr style=\"width: 100%; height: 2px;\" />\
            <table style=\"width: 976px; height: 60px;\" border=\"1\" cellpadding=\"2\" cellspacing=\"2\">\
                <tbody>\
                    <tr>\
                        <td style=\"font-family: Verdana; font-weight: bold; font-style: italic; background-color: rgb(51, 51, 255); width: 317px;\">\
                            <big>\
                                <big>\
                                    <a href=\"AT32F407.html\"><span style=\"color: red;\">Main Page</span></a>\
                                </big>\
                            </big>\
                        </td>\
                        <td style=\"font-family: Verdana; font-weight: bold; background-color: rgb(51, 51, 255); width: 317px;\">\
                            &nbsp;\
                            <big>\
                                <big>\
                                    <a href=\"AT32F407LED.html\"><span style=\"color: white;\">LED Control</span></a>\
                                </big>\
                            </big>\
                        </td>\
                        <td style=\"font-family: Verdana; font-weight: bold; background-color: rgb(51, 51, 255); width: 317px;\">\
                            <big>\
                                <big>\
                                    <a href=\"AT32F407ADC.html\"><span style=\"color: white;\">ADC Sampling</span></a>\
                                </big>\
                            </big>\
                        </td>\
                    </tr>\
                </tbody>\
            </table>\
            <table style=\"background-color: rgb(255, 255, 255); width: 766px; text-align: left; margin-left: auto; margin-right: auto;\" border=\"0\" cellpadding=\"0\" cellspacing=\"0\">\
                <tbody>\
                    <tr valign=\"top\">\
                        <td class=\"text\">\
                            <table border=\"0\" cellpadding=\"3\" cellspacing=\"0\" width=\"100%\">\
                                <tbody>\
                                    <tr>\
                                        <td>\
                                            <table border=\"0\" cellpadding=\"3\" cellspacing=\"0\" width=\"100%\">\
                                                <tbody>\
                                                    <tr>\
                                                        <td valign=\"top\" width=\"72%\">\
                                                            <font size=\"6\"> </font>\
                                                            <p align=\"center\">\
                                                                <font size=\"6\">Board Resource: <br /> </font>\
                                                            </p>\
                                                            <li style=\"text-align: left; margin-left: 80px; font-family: Times New Roman;\">\
                                                                <big><big>Main Chip: AT32F407VCT7 (RAM:96KB; FLASH:256KB)</big></big>\
                                                            </li>\
                                                            <li style=\"text-align: left; margin-left: 80px; font-family: Times New Roman;\">\
                                                                <big>\
                                                                    <big>Communication I/O: <br /></big>\
                                                                </big>\
                                                            </li>\
                                                            <li style=\"text-align: left; margin-left: 120px; font-family: Times New Roman;\">\
                                                                <big><big>3 I2C</big></big>\
                                                            </li>\
                                                            <li style=\"text-align: left; margin-left: 120px; font-family: Times New Roman;\">\
                                                                <big><big>8 USART</big></big>\
                                                            </li>\
                                                            <li style=\"text-align: left; margin-left: 120px; font-family: Times New Roman;\">\
                                                                <big><big>4 SPI</big></big>\
                                                            </li>\
                                                            <li style=\"text-align: left; margin-left: 120px; font-family: Times New Roman;\">\
                                                                <big><big>2 CAN</big></big>\
                                                            </li>\
                                                            <li style=\"text-align: left; margin-left: 120px; font-family: Times New Roman;\">\
                                                                <big><big>Crystal-less USB 2.0</big></big>\
                                                            </li>\
                                                            <li style=\"text-align: left; margin-left: 120px;\">\
                                                                <big>\
                                                                    <big><span style=\"font-family: Times New Roman;\">2 SDIO</span><br /> </big>\
                                                                </big>\
                                                            </li>\
                                                            <font size=\"6\">\
                                                                <font size=\"5\">\
                                                                    <font size=\"6\">\
                                                                        <font color=\"red\">\
                                                                            <font color=\"black\">\
                                                                                <font size=\"5\">\
                                                                                    <font color=\"red\"><font color=\"black\"> </font> </font>\
                                                                                </font>\
                                                                            </font>\
                                                                        </font>\
                                                                    </font>\
                                                                </font>\
                                                            </font>\
                                                        </td>\
                                                    </tr>\
                                                </tbody>\
                                            </table>\
                                        </td>\
                                    </tr>\
                                </tbody>\
                            </table>\
                        </td>\
                    </tr>\
                </tbody>\
            </table>\
        </div>\
    </body>\
</html>\
"
;

static const unsigned char FSDATA_ALIGN_PRE data_AT32F407LED_html[] FSDATA_ALIGN_POST = " \
<html>\
<head>\
    <title>Technology Corp. Demo Web Page</title>\
</head>\
<body>\
    <div style=\"text-align: center; margin-left: 40px; width: 977px;\">\
        <span><br>\
        <big><big><big style=\"font-weight: bold;\"><big>AT32F407 web server demo</big></big></big></big></span> <small><small><big><big><big style=\"font-weight: bold;\"><big><font size=\"6\"><big><big><big><big><big><strong></strong></big></big></big></big></big></font></big></big></big></big><font size=\"6\"><big>&nbsp;</big><small style=\"font-family: Verdana;\"><small><big><big><big><big style=\"font-weight: bold; color: rgb(51, 51, 255);\"><big><strong><span style=\"font-style: italic;\"></span></strong></big><span style=\"color: rgb(51, 51, 255);\"><font size=\"8\"><br>\
        ****************************<br>\
        ****************************</font></span></big></big></big></big></small></small></font></small></small>\
        <hr style=\"width: 100%; height: 2px;\">\
        <table border=\"1\" cellpadding=\"2\" cellspacing=\"2\" style=\"width: 976px; height: 60px;\">\
            <tbody>\
                <tr>\
                    <td style=\"font-family: Verdana; font-weight: bold; background-color: rgb(51, 51, 255); width: 316px;\"><big><big><a href=\"AT32F407.html\"><span style=\"color: white;\">Main Page</span></a></big></big></td>\
                    <td style=\"font-family: Verdana; font-weight: bold; font-style: italic; background-color: rgb(51, 51, 255); width: 317px;\">\
                        <a href=\"AT32F407LED.html\"><span style=\"color: red;\">&nbsp;<big><big>LED Control</big></big></span></a>\
                    </td>\
                    <td style=\"font-family: Verdana; font-weight: bold; background-color: rgb(51, 51, 255); width: 317px;\"><big><big><a href=\"AT32F407ADC.html\"><span style=\"color: white;\">ADC Sampling</span></a></big></big></td>\
                </tr>\
            </tbody>\
        </table>\
        <table border=\"0\" cellpadding=\"0\" cellspacing=\"0\" style=\"background-color: rgb(255, 255, 255); width: 766px; text-align: left; margin-left: auto; margin-right: auto;\">\
            <tbody></tbody>\
        </table><br>\
        <span style=\"font-family: Consolas;\"><big><big>On this page, you can control 4 LEDs on developing board:</big></big></span> <span style=\"font-family: Verdana;\"></span>\
        <form action=\"method=get\">\
            <span style=\"font-family: Verdana;\"></span>\
            <div style=\"text-align: center;\">\
                <span style=\"font-family: Verdana;\"><font size=\"5\" style=\"font-family: Consolas;\"></font></span>\
                <div style=\"text-align: center; font-family: Consolas;\">\
                    <span style=\"font-family: Verdana;\"><font size=\"5\"><input name=\"led\" type=\"checkbox\" value=\"2\"> LED2</font><br></span>\
                </div><span style=\"font-family: Verdana;\"><font size=\"5\" style=\"font-family: Consolas;\"></font></span>\
                <div style=\"text-align: center; font-family: Consolas;\">\
                    <span style=\"font-family: Verdana;\"><font size=\"5\"><input name=\"led\" type=\"checkbox\" value=\"3\"> LED3</font><br></span>\
                </div><span style=\"font-family: Verdana;\"><font size=\"5\" style=\"font-family: Consolas;\"></font></span>\
                <div style=\"text-align: center; font-family: Consolas;\">\
                    <span style=\"font-family: Verdana;\"><font size=\"5\"><input name=\"led\" type=\"checkbox\" value=\"4\"> LED4</font><br></span>\
                </div><span style=\"font-family: Verdana;\"><font size=\"5\" style=\"font-family: Consolas;\"></font></span>\
            </div><span style=\"font-family: Verdana;\"><font size=\"5\" style=\"font-family: Arial Unicode MS;\"><font size=\"5\"><input type=\"submit\" value=\"Send Command\"></font></font></span>\
        </form><span style=\"font-family: Verdana;\"></span>\
        <h3><span style=\"font-family: Verdana;\"><font size=\"5\"><font size=\"5\"><font size=\"-1\"><span style=\"color: black;\"></span></font></font></font></span></h3>\
        <h3 style=\"font-family: Verdana;\"><font size=\"5\"><font size=\"5\"><font size=\"-1\"><br></font></font></font></h3><font size=\"5\"><font size=\"5\"><span style=\"font-family: ����;\"></span><a href=\"/AT32F407.html\" style=\"font-family: ����;\"><font size=\"-1\"><big><span style=\"font-weight: bold;\"></span></big></font></a> <font size=\"-1\"><span style=\"color: blue;\"><font size=\"5\"><span style=\"font-family: ����;\"><a href=\"http://item.taobao.com/item.htm?spm=0.0.0.0.nObYQP&amp;id=17524242055\" style=\"font-weight: bold;\"><font size=\"5\"><span style=\"font-family: ����;\"></span></font></a> <font size=\"5\"></font></span></font></span></font></font></font>\
    </div>\
</body>\
</html>\
"
;

static const unsigned char FSDATA_ALIGN_PRE data_AT32F407ADC_html[] FSDATA_ALIGN_POST = " \
<html>\
<head>\
    <title>Technology Corp. Demo Web Page</title>\
    <meta content=\"1\" http-equiv=\"refresh\">\
</head>\
<body>\
    <div style=\"text-align: center; margin-left: 40px; width: 977px;\">\
        <span><br>\
        <big><big><big style=\"font-weight: bold;\"><big>AT32F407 web server demo</big></big></big></big></span> <small><small><big><big><big style=\"font-weight: bold;\"><big><font size=\"6\"><big><big><big><big><big><strong></strong></big></big></big></big></big></font></big></big></big></big><font size=\"6\"><big>&nbsp;</big><small style=\"font-family: Verdana;\"><small><big><big><big><big style=\"font-weight: bold; color: rgb(51, 51, 255);\"><big><strong><span style=\"font-style: italic;\"></span></strong></big><span style=\"color: rgb(51, 51, 255);\"><font size=\"8\"><br>\
        ****************************<br>\
        ****************************</font></span></big></big></big></big></small></small></font></small></small>\
        <hr style=\"height: 2px;\">\
        <table border=\"1\" cellpadding=\"2\" cellspacing=\"2\" style=\"width: 976px; height: 60px;\">\
            <tbody>\
                <tr>\
                    <td style=\"font-family: Verdana; font-weight: bold; background-color: rgb(51, 51, 255); width: 316px;\"><big><big><a href=\"AT32F407.html\"><span style=\"color: white;\">Main Page</span></a></big></big></td>\
                    <td style=\"font-family: Verdana; font-weight: bold; background-color: rgb(51, 51, 255); width: 316px;\">\
                        <a href=\"AT32F407LED.html\"><span style=\"color: white;\">&nbsp;<big><big>LED Control</big></big></span></a>\
                    </td>\
                    <td style=\"font-family: Verdana; font-weight: bold; font-style: italic; background-color: rgb(51, 51, 255); width: 316px;\"><big><big><a href=\"AT32F407ADC.html\"><span style=\"color: red;\">ADC Sampling</span></a></big></big></td>\
                </tr>\
            </tbody>\
        </table>\
        <table border=\"0\" cellpadding=\"0\" cellspacing=\"0\" style=\"background-color: rgb(255, 255, 255); width: 766px; text-align: left; margin-left: auto; margin-right: auto;\">\
            <tbody></tbody>\
        </table>\
        <table border=\"0\" cellpadding=\"0\" cellspacing=\"0\" style=\"background-color: rgb(255, 255, 255); width: 766px; text-align: left; margin-left: auto; margin-right: auto;\">\
            <tbody></tbody>\
        </table>\
        <p align=\"center\"><font size=\"6\">This page implement real-time sampling of voltage, ADC channel is 4<br></font> <font size=\"6\"><br></font></p>\
        <div style=\"text-align: center;\">\
            <big><span><big>Voltage:</big> <span style=\"color: red;\"><big>%.2fV</big></span></span></big>\
        </div>\
        <table border=\"3\" cellpadding=\"0\" cellspacing=\"0\" style=\"background-color: rgb(204, 204, 204); width: 520px; text-align: left; margin-left: auto; margin-right: auto;\">\
            <tbody>\
                <tr>\
                    <td>\
                        <div>\
                            <table border=\"0\" cellpadding=\"0\" cellspacing=\"0\" width=\"%d\">\
                                <tbody>\
                                    <tr>\
                                        <td bgcolor=\"#33FF00\" style=\"text-align: right;\">&nbsp;</td>\
                                    </tr>\
                                </tbody>\
                            </table>\
                        </div>\
                    </td>\
                </tr>\
            </tbody>\
        </table>\
        <table border=\"0\" style=\"width: 520px; text-align: left; margin-left: auto; margin-right: auto;\">\
            <tbody>\
                <tr>\
                    <td style=\"color: rgb(51, 51, 153); font-family: Times New Roman;\" width=\"14\">0V</td>\
                    <td style=\"font-family: Times New Roman;\" width=\"14\">0.5V</td>\
                    <td style=\"font-family: Times New Roman;\" width=\"14\">1V</td>\
                    <td style=\"font-family: Times New Roman;\" width=\"14\">1.5V</td>\
                    <td style=\"font-family: Times New Roman;\" width=\"14\">2V</td>\
                    <td style=\"font-family: Times New Roman;\" width=\"14\">2.5V</td>\
                    <td style=\"width: 14px; font-family: Times New Roman;\">3V</td>\
                </tr>\
            </tbody>\
        </table><font size=\"6\"><font size=\"-1\"><span style=\"color: black;\"></span></font><font size=\"-1\"><span style=\"color: black;\"></span></font><font size=\"-1\"><span style=\"color: black;\"></span></font></font><a href=\"/AT32F407.html\" style=\"font-family: Verdana;\"><big><span style=\"font-weight: bold;\"></span></big></a><span style=\"color: blue;\"><br>\
        <font size=\"5\"><span style=""></span></font></span>\
    </div>\
</body>\
</html>\
"
;

const struct fsdata_file file__img_sics_gif[] = { {
file_NULL,
data__img_sics_gif,
data__img_sics_gif + 16,
sizeof(data__img_sics_gif) - 16,
FS_FILE_FLAGS_HEADER_INCLUDED | FS_FILE_FLAGS_HEADER_PERSISTENT,
}};

const struct fsdata_file file__404_html[] = { {
file__img_sics_gif,
data__404_html,
data__404_html + 12,
sizeof(data__404_html) - 12,
FS_FILE_FLAGS_HEADER_INCLUDED | FS_FILE_FLAGS_HEADER_PERSISTENT,
}};

const struct fsdata_file file_AT32F407_html[] = { {
file_NULL,
main_page_name,
data_AT32F407_html,
sizeof(data_AT32F407_html),
FS_FILE_FLAGS_HEADER_INCLUDED | FS_FILE_FLAGS_HEADER_PERSISTENT,
}};

const struct fsdata_file file_AT32F407ADC_html[] = { {
file_AT32F407_html,
adc_page_name,
data_AT32F407ADC_html,
sizeof(data_AT32F407ADC_html),
FS_FILE_FLAGS_HEADER_INCLUDED | FS_FILE_FLAGS_HEADER_PERSISTENT,
}};

const struct fsdata_file file_AT32F407LED_html[] = { {
file_AT32F407ADC_html,
led_page_name,
data_AT32F407LED_html,
sizeof(data_AT32F407LED_html),
FS_FILE_FLAGS_HEADER_INCLUDED | FS_FILE_FLAGS_HEADER_PERSISTENT,
}};

#define FS_ROOT file_AT32F407LED_html
#define FS_NUMFILES 3

