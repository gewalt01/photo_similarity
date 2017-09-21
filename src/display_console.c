//================================================================================
//      FILE
//--------------------------------------------------------------------------------
//      ファイル名      :       display_console.c
//      概要            :       データのコンソール出力する。display.cの下位モジュール。
//--------------------------------------------------------------------------------
#include "display_console.h"

static int _print_ex(char *, int, int);

//----------------------------------------------------------------------------
// 関数名       ：display_console
// 概要         ：コンソールに実行結果を出力
// 戻り値       ：int
// 引数         ：RESULT *result
//                RESILTデータ
// 引数         ：OPTION *option
//                OPTIONデータ
//----------------------------------------------------------------------------
int display_console(RESULT *result, OPTION *option)
{
	/*
	 * XXX:苦肉の策でUTF-8にのみ対応
	 *
	 */
	int i;
	int j;
	char buff[1024];
	int tmpno;
	int fnamelen;
	double tmpsimil;
	unsigned char *tmpfname;
	int max_row1_len;
	int max_row2_len;
	int max_row3_len; //類似度はどーせ6文字以内だし…


	char *hname1 = "original file";
	char *hname2 = "similar file";
	char *hname3 = "similarity";

	/*
	 * こう表示させたい
	 *
	 * +-------------+------------+----------+
	 * |original file|similar file|similarity|
	 * +-------------+------------+----------+
	 * |./test.bmp   |./test.ppm  |25.0%    |
	 * +-------------+------------+----------+
	 */



	//列1の幅
	max_row1_len = count_chars_utf8(hname1) * 2;
	max_row1_len -= count_halfwidth_chars_utf8(hname1);
	for(i=0; i<get_result_length(result);i++)
	{
		tmpfname = get_result_filename(i, result);
		//半角文字換算での文字の長さ計算
		//とりあえずUTF8で。システムの内部エンコードの取得が大変。
		fnamelen = count_chars_utf8(tmpfname) * 2;
		fnamelen -= count_halfwidth_chars_utf8(tmpfname);
		max_row1_len = (max_row1_len < fnamelen) ? fnamelen: max_row1_len;
	}

	//列2の幅
	max_row2_len = count_chars_utf8(hname2) * 2;
	max_row2_len -= count_halfwidth_chars_utf8(hname2);
	for(i=0; i<get_result_length(result);i++)
	{
		tmpno = get_result_similfileno(i, result);
		tmpfname = get_result_filename(tmpno, result);
		//半角文字換算での文字の長さ計算
		//とりあえずUTF8で。システムの内部エンコードの取得が大変。
		fnamelen = count_chars_utf8(tmpfname) * 2;
		fnamelen -= count_halfwidth_chars_utf8(tmpfname);
		max_row2_len = (max_row2_len < fnamelen) ? fnamelen: max_row2_len;
	}

	//列3の幅
	max_row3_len = count_chars_utf8(hname3) * 2;
	max_row3_len -= count_halfwidth_chars_utf8(hname3);


	//表ヘッダー線
	printf("+");
	//列1線
	printf("-");
	for(i=0; i<max_row1_len; i++)
	{
		printf("-");
	}
	printf("-");
	printf("+");
	//列2線
	printf("-");
	for(i=0; i<max_row2_len; i++)
	{
		printf("-");
	}
	printf("-");
	printf("+");
	//列3線
	printf("-");
	for(i=0; i<max_row3_len; i++)
	{
		printf("-");
	}
	printf("-");
	printf("+");
	puts("");


	//表ヘッダー中
	printf("+");
	//列1中身
	printf(" ");
	fnamelen = count_chars_utf8(hname1) * 2;
	fnamelen -= count_halfwidth_chars_utf8(hname1);
	_print_ex(hname1, max_row1_len, fnamelen);
	printf(" ");
	printf("+");
	//列2中身
	printf(" ");
	fnamelen = count_chars_utf8(hname2) * 2;
	fnamelen -= count_halfwidth_chars_utf8(hname2);
	_print_ex(hname2, max_row2_len, fnamelen);
	printf(" ");
	printf("+");
	//列3中身
	printf(" ");
	fnamelen = count_chars_utf8(hname3) * 2;
	fnamelen -= count_halfwidth_chars_utf8(hname3);
	_print_ex(hname3, max_row3_len, fnamelen);
	printf(" ");
	printf("+");

	puts("");

	//線
	printf("+");
	//列1線
	printf("-");
	for(i=0; i<max_row1_len; i++)
	{
		printf("-");
	}
	printf("-");
	printf("+");
	//列2線
	printf("-");
	for(i=0; i<max_row2_len; i++)
	{
		printf("-");
	}
	printf("-");
	printf("+");
	//列3線
	printf("-");
	for(i=0; i<max_row3_len; i++)
	{
		printf("-");
	}
	printf("-");
	printf("+");
	puts("");


	//実行結果
	for(i=0; i<get_result_length(result);i++)
	{
		printf("+");
		//列1
		printf(" ");
		tmpfname = get_result_filename(i, result);
		fnamelen = count_chars_utf8(tmpfname) * 2;
		fnamelen -= count_halfwidth_chars_utf8(tmpfname);
		_print_ex(tmpfname, max_row1_len, fnamelen);
		printf(" ");
		printf("+");

		//列2
		printf(" ");
		tmpno = get_result_similfileno(i, result);
		tmpfname = get_result_filename(tmpno, result);
		fnamelen = count_chars_utf8(tmpfname) * 2;
		fnamelen -= count_halfwidth_chars_utf8(tmpfname);
		_print_ex(tmpfname, max_row2_len, fnamelen);
		printf(" ");
		printf("+");

		//列3
		printf(" ");
		tmpsimil = get_result_similaly(i, result);
		snprintf(buff, 10, "%3.1f%%", tmpsimil * 100);
		fnamelen = count_chars_utf8(buff) * 2;
		fnamelen -= count_halfwidth_chars_utf8(buff);
		_print_ex(buff, max_row3_len, fnamelen);
		printf(" ");
		printf("+");
		puts("");

		//中間線
		printf("+");
		//列1線
		printf("-");
		for(j=0; j<max_row1_len; j++)
		{
			printf("-");
		}
		printf("-");
		printf("+");
		//列2線
		printf("-");
		for(j=0; j<max_row2_len; j++)
		{
			printf("-");
		}
		printf("-");
		printf("+");
		//列3線
		printf("-");
		for(j=0; j<max_row3_len; j++)
		{
			printf("-");
		}
		printf("-");
		printf("+");
		puts("");

	}


	return 0;
}


//----------------------------------------------------------------------------
// 関数名       ：_print_ex
// 概要         ：マルチバイト文字対応化の画面出力補助
// 戻り値       ：int
// 引数         ：int size
//                マルチバイト文字列
// 引数         ：int size
//                最大幅
// 引数         ：int size
//                半角文字換算のマルチバイト文字列の幅
//----------------------------------------------------------------------------
static int _print_ex(char *fname, int mbmaxhlen, int mbhlen)
{
	char buff[0x7FFF];
	snprintf(buff, 0x3FFF, "%%-%ds", strlen(fname) + mbmaxhlen - mbhlen);
	printf(buff, fname);

	return 0;
}
