//--------------------------------------------------------------------------------
//      ファイル名      :       pnm.c
//      概要            :       PNM形式(PBM, PGM, PPM形式)画像を扱うライブラリ
//--------------------------------------------------------------------------------
#ifndef INCLUDE_GUARD_ABE7BA4A_DF7F_11E3_8D79_33BC9EA6AB1E
#define INCLUDE_GUARD_ABE7BA4A_DF7F_11E3_8D79_33BC9EA6AB1E

#include <stdlib.h>
#include <stdio.h>


#define PBMASCII 1
#define PGMASCII 2
#define PPMASCII 3
#define PBMBIN 4
#define PGMBIN 5
#define PPMBIN 6


typedef struct{
	unsigned short type;
	int width;
	int height;
	unsigned char *r;
	unsigned char *g;
	unsigned char *b;
} PNM;

int close_pnm(PNM *);

PNM *read_pnm(FILE *);

int write_pnm(PNM *, FILE *);

#endif
