//================================================================================
//      FILE
//--------------------------------------------------------------------------------
//      ファイル名      :       image.h
//      概要            :       画像データの抽象化レイヤー
//--------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bmp.h"
#include "pnm.h"


#ifndef INCLUDE_GUARD_2905C266_DF7F_11E3_99DE_535E209A0D6B
#define INCLUDE_GUARD_2905C266_DF7F_11E3_99DE_535E209A0D6B

#define C3IMGCOLOR 1
#define C3IMGGRAY  2

#define C3IMGCOLOR3BIT  1
#define C3IMGCOLOR6BIT  2
#define C3IMGCOLOR12BIT  3
#define C3IMGCOLOR24BIT  4
#define C3IMGCOLORGRAY1BIT 101
#define C3IMGCOLORGRAY2BIT 102
#define C3IMGCOLORGRAY4BIT 103
#define C3IMGCOLORGRAY8BIT 104


typedef struct{
	unsigned int width;
	unsigned int height;
	unsigned int type;
	unsigned int level;
	unsigned char *r;
	unsigned char *g;
	unsigned char *b;
	unsigned char *a;
}C3IMG;

C3IMG *read_image(FILE *fp);

C3IMG *bmp2c3img(BMP *);

BMP *c3img2bmp(C3IMG *);

C3IMG *pnm2c3img(PNM *);

PNM *c3img2pnm(C3IMG *);

int posterize_c3img(C3IMG *, int);

int delete_c3img(C3IMG *);

#endif
