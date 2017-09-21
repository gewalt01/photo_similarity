//================================================================================
//      FILE
//--------------------------------------------------------------------------------
//      ファイル名      :       bmp.h
//      概要            :       BMP形式画像を扱うライブラリ
//--------------------------------------------------------------------------------
#ifndef INCLUDE_GUARD_D52B9472_DF7E_11E3_B2B2_9FD320A51558
#define INCLUDE_GUARD_D52B9472_DF7E_11E3_B2B2_9FD320A51558

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


#define BMPFILEHEADERSIZE 14

#define BMP1 1
#define BMP4 4
#define BMP8 8
#define BMP16 16
#define BMP24 24
#define BMP32 32

#define BMPRGB 0

#define BMPCOREHEADERSIZE 12
#define BMPINFOHEADERSIZE 40
#define BMPINFO2HEADERSIZE 64
#define BMPV4HEADERSIZE 108
#define BMPV5HEADERSIZE 124

typedef struct{
        short        bfType;
        unsigned int bfSize;
        short        bfReserved1;
        short        bfReserved2;
        unsigned int bfOffBits;
} BMPFILEHEADER;

typedef struct {
  unsigned int   bcSize;
  unsigned short bcWidth;
  unsigned short bcHeight;
  unsigned short bcPlanes;
  unsigned short bcBitCount;
} BMPCOREHEADER;

typedef struct{
        unsigned int bcSize;
        int bcWidth;
        int bcHeight;
        unsigned short bcPlanes;
        unsigned short bcBitCount;
        unsigned int biCompression;
        unsigned int biSizeImage;
        int biXPixPerMeter;
        int biYPixPerMeter;
        unsigned int biClrUsed;
        unsigned int biCirImportant;
} BMPINFOHEADER;

typedef struct {
  unsigned int      bV4Size;
  int               bV4Width;
  int               bV4Height;
  unsigned short    bV4Planes;
  unsigned short    bV4BitCount;
  unsigned int      bV4Compression;
  unsigned int      bV4SizeImage;
  int               bV4XPelsPerMeter;
  int               bV4YPelsPerMeter;
  unsigned int      bV4ClrUsed;
  unsigned int      bV4ClrImportant;
  unsigned int      bV4RedMask;
  unsigned int      bV4GreenMask;
  unsigned int      bV4BlueMask;
  unsigned int      bV4AlphaMask;
  unsigned int      bV4CSType;
  unsigned int      bV4EndpointRX;
  unsigned int      bV4EndpointRY;
  unsigned int      bV4EndpointRZ;
  unsigned int      bV4EndpointGX;
  unsigned int      bV4EndpointGY;
  unsigned int      bV4EndpointGZ;
  unsigned int      bV4EndpointBX;
  unsigned int      bV4EndpointBY;
  unsigned int      bV4EndpointBZ;
  unsigned int      bV4GammaRed;
  unsigned int      bV4GammaGreen;
  unsigned int      bV4GammaBlue;
} BMPV4HEADER;

typedef struct {
  unsigned int      bV5Size;
  int               bV5Width;
  int               bV5Height;
  unsigned short    bV5Planes;
  unsigned short    bV5BitCount;
  unsigned int      bV5Compression;
  unsigned int      bV5SizeImage;
  int               bV5XPelsPerMeter;
  int               bV5YPelsPerMeter;
  unsigned int      bV5ClrUsed;
  unsigned int      bV5ClrImportant;
  unsigned int      bV5RedMask;
  unsigned int      bV5GreenMask;
  unsigned int      bV5BlueMask;
  unsigned int      bV5AlphaMask;
  unsigned int      bV5CSType;
  unsigned int      bV5EndpointRX;
  unsigned int      bV5EndpointRY;
  unsigned int      bV5EndpointRZ;
  unsigned int      bV5EndpointGX;
  unsigned int      bV5EndpointGY;
  unsigned int      bV5EndpointGZ;
  unsigned int      bV5EndpointBX;
  unsigned int      bV5EndpointBY;
  unsigned int      bV5EndpointBZ;
  unsigned int      bV5GammaRed;
  unsigned int      bV5GammaGreen;
  unsigned int      bV5GammaBlue;
  unsigned int      bV5Intent;
  unsigned int      bV5ProfileData;
  unsigned int      bV5ProfileSize;
  unsigned int      bV5Reserved;
} BMPV5HEADER;

typedef struct RGBQUAD{
  unsigned char rgbBlue;
  unsigned char rgbGreen;
  unsigned char rgbRed;
  unsigned char rgbReserved;
} RGBQUAD;


typedef struct{
	int bmpType;
	int palletBitSize;
	BMPFILEHEADER *fileHeader;
	void *infoHeader;
	RGBQUAD **pallets;
	unsigned char *body;
} BMP;

short get_bmp_bit_count(BMP *);

int get_bmp_height(BMP *);

int get_bmp_type();

int get_bmp_width(BMP *);

int get_compres_type(BMP *);

int close_bmp(BMP *);

BMP *read_bmp(FILE *);

int write_bmp(FILE *, BMP *);


#endif
