//================================================================================
//      FILE
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//      ファイル名      :       pnmt.c
//      概要            :       PNM形式(PBM, PGM, PPM形式)画像を扱うライブラリpnmのテストツール
//--------------------------------------------------------------------------------
#include <stdio.h>
#include "pnm.h"

int main(void)
{
	FILE *fp1;
	FILE *fp2;
	PNM *pnm;

	/* PBMASCII ./img/test */
	fp1 = fopen("./img/test.pbm", "rb");
	fp2 = fopen("./img/testo.pbm", "wb");
	pnm = read_pnm(fp1);
	write_pnm(pnm, fp2);
	close_pnm(pnm);
	fclose(fp1);
	fclose(fp2);

	/* PGMASCII ./img/test */
	fp1 = fopen("./img/test.pgm", "rb");
	fp2 = fopen("./img/testo.pgm", "wb");
	pnm = read_pnm(fp1);
	write_pnm(pnm, fp2);
	close_pnm(pnm);
	fclose(fp1);
	fclose(fp2);

	/* PPMASCII ./img/test */
	fp1 = fopen("./img/test.ppm", "rb");
	fp2 = fopen("./img/testo.ppm", "wb");
	pnm = read_pnm(fp1);
	write_pnm(pnm, fp2);
	close_pnm(pnm);
	fclose(fp1);
	fclose(fp2);

	/* PBMBIN ./img/test */
	fp1 = fopen("./img/testb.pbm", "rb");
	fp2 = fopen("./img/testbo.pbm", "wb");
	pnm = read_pnm(fp1);
	write_pnm(pnm, fp2);
	close_pnm(pnm);
	fclose(fp1);
	fclose(fp2);

	/* PGMBIN ./img/test */
	fp1 = fopen("./img/testb.pgm", "rb");
	fp2 = fopen("./img/testbo.pgm", "wb");
	pnm = read_pnm(fp1);
	write_pnm(pnm, fp2);
	close_pnm(pnm);
	fclose(fp1);
	fclose(fp2);

	/* PPMBIN ./img/test */
	fp1 = fopen("./img/testb.ppm", "rb");
	fp2 = fopen("./img/testbo.ppm", "wb");
	pnm = read_pnm(fp1);
	write_pnm(pnm, fp2);
	close_pnm(pnm);
	fclose(fp1);
	fclose(fp2);

}
