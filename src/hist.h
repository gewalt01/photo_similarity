//================================================================================
//      FILE
//--------------------------------------------------------------------------------
//      ファイル名      :       hist.c
//      概要            :       ヒストグラムを扱うライブラリ,勝手に正規化する。
//--------------------------------------------------------------------------------
#ifndef INCLUDE_GUARD_57EA1687_D72A_47E6_A663_83E0502537BA
#define INCLUDE_GUARD_57EA1687_D72A_47E6_A663_83E0502537BA

#include "image.h"
#include "hist.h"


typedef struct {
	int size;
	double *histgram;
} HIST;

HIST *create_histgram_d(int, double *);

HIST *create_histgram_i(int, int *);

HIST *create_histgram_c(int, char *);

int free_histgram(HIST *);

double similaly(HIST *, HIST*);

#endif
