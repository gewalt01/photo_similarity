//================================================================================
//      FILE
//--------------------------------------------------------------------------------
//      ファイル名      :       imaget.c
//      概要            :       imageのテストツール
//--------------------------------------------------------------------------------
#include <stdio.h>
#include "bmp.h"
#include "pnm.h"
#include "image.h"

int main(void)
{
	FILE *fp1;
	FILE *fp2;

	PNM *pnm1;
	PNM *pnm2;
	BMP *bmp1;
	BMP *bmp2;

	C3IMG *c3img;

	fp1 = fopen("./img/test.ppm", "rb");
	fp2 = fopen("./img/testimgo.bmp", "wb");


	pnm1 = read_pnm(fp1);
	c3img = pnm2c3img(pnm1);
	//c3img = read_image(fp1);
	bmp1 = c3img2bmp(c3img)	;

	write_bmp(fp2, bmp1);

	fclose(fp1);
	fclose(fp2);


	fp1 = fopen("./img/test.bmp", "rb");
	fp2 = fopen("./img/testimgo.ppm", "wb");

	bmp2 = read_bmp(fp1);
	c3img = bmp2c3img(bmp2);
	//c3img = read_image(fp1);
	pnm2 = c3img2pnm(c3img) ;

	write_pnm(pnm2, fp2);

	return 0;
}
