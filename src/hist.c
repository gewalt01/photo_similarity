//================================================================================
//      FILE
//--------------------------------------------------------------------------------
//      ファイル名      :       hist.c
//      概要            :       ヒストグラムを扱うライブラリ,勝手に正規化する。
//--------------------------------------------------------------------------------
#include "hist.h"

//----------------------------------------------------------------------------
// 関数名       ：create_histgram_d
// 概要         ：ヒストグラムデータ作成(double型)
// 戻り値       ：HIST *
// 引数         ：int size
//                次元数
// 引数         ：double *array
//                ベクトルデータ
//----------------------------------------------------------------------------
HIST *create_histgram_d(int size, double *array)
{
	HIST *hist;
	int i;

	hist = (HIST *)malloc(sizeof(HIST));
	if (hist == NULL) return NULL;

	hist->size = size;
	hist->histgram = (double *)malloc(sizeof(double) * hist->size);
	if (hist->histgram == NULL) {
		free(hist);
		return NULL;
	}

	for(i=0; i<hist->size; i++)
	{
		hist->histgram[i] = array[i] / size;
	}

	return hist;
}

//----------------------------------------------------------------------------
// 関数名       ：create_histgram_i
// 概要         ：ヒストグラムデータ作成(int型)
// 戻り値       ：HIST *
// 引数         ：int size
//                次元数
// 引数         ：int *array
//                ベクトルデータ
//----------------------------------------------------------------------------
HIST *create_histgram_i(int size, int *array)
{
	HIST *hist;
	int i;

	hist = (HIST *)malloc(sizeof(HIST));
	if (hist == NULL) return NULL;

	hist->size = size;
	hist->histgram = (double *)malloc(sizeof(double) * hist->size);
	if (hist->histgram == NULL) {
		free(hist);
		return NULL;
	}

	for(i=0; i<hist->size; i++)
	{
		hist->histgram[i] = (double) array[i] / size;
	}

	return hist;
}

//----------------------------------------------------------------------------
// 関数名       ：create_histgram_c
// 概要         ：ヒストグラムデータ作成(char型)
// 戻り値       ：HIST *
// 引数         ：int size
//                次元数
// 引数         ：char *array
//                ベクトルデータ
//----------------------------------------------------------------------------
HIST *create_histgram_c(int size, char *array)
{
	HIST *hist;
	int i;

	hist = (HIST *)malloc(sizeof(HIST));
	if (hist == NULL) return NULL;

	hist->size = size;
	hist->histgram = (double *)malloc(sizeof(double) * hist->size);
	if (hist->histgram == NULL) {
		free(hist);
		return NULL;
	}

	for(i=0; i<hist->size; i++)
	{
		hist->histgram[i] = (double) array[i] / size;
	}

	return hist;
}

//----------------------------------------------------------------------------
// 関数名       ：free_histgram
// 概要         ：ヒストグラムデータの領域開放
// 戻り値       ：int
// 引数         ：HIST *hist
//                ヒストグラムデータ
//----------------------------------------------------------------------------
int free_histgram(HIST *hist)
{
	free(hist->histgram);

	return 0;
}

//----------------------------------------------------------------------------
// 関数名       ：similaly
// 概要         ：類似度計算
// 戻り値       ：double
// 引数         ：HIST *hista
//                ヒストグラムデータ
// 引数         ：HIST *histb
//                ヒストグラムデータ
//----------------------------------------------------------------------------
double similaly(HIST *hista, HIST *histb)
{
	double suma = 0;
	double sumb = 0;
	int i;

	if (hista->size != histb->size) return -1;

	for(i=0; i<hista->size; i++)
	{
		suma += fminf(hista->histgram[i], histb->histgram[i]);
		sumb += hista->histgram[i];
	}

	return suma / sumb;
}
