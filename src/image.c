//================================================================================
//      FILE
//--------------------------------------------------------------------------------
//      ファイル名      :       image.c
//      概要            :       画像データの抽象化レイヤー
//--------------------------------------------------------------------------------
#include "image.h"

//----------------------------------------------------------------------------
// 関数名       ：read_image
// 概要         ：画像データ読み込み
// 戻り値       ：C3IMG *
// 引数         ：FILE *fp
//                画像ファイルのファイルポインタ
//----------------------------------------------------------------------------
C3IMG *read_image(FILE *fp)
{
	C3IMG *img;
	BMP *bmp;
	PNM *pnm;
	char buff[1024];

	img = NULL;

	fread(buff, 1, 2, fp);
	if (buff[0] == 'B' && buff[1] == 'M') { // BMP FILE
		rewind(fp);
		bmp = read_bmp(fp);
		if (bmp == NULL) return NULL;
		img = bmp2c3img(bmp);
		close_bmp(bmp);
		if (img == NULL) return NULL;
	} else if (buff[0] == 'P') { // PNM FILE
		rewind(fp);
		pnm = read_pnm(fp);
		if (pnm == NULL) return NULL;
		img = pnm2c3img(pnm);
		close_pnm(pnm);
		if (img == NULL) return NULL;
	}

	return img;
}

//----------------------------------------------------------------------------
// 関数名       ：get_c3img_width
// 概要         ：抽象化画像データの幅を返す
// 戻り値       ：int
// 引数         ：C3IMG *img
//                抽象化画像データ
//----------------------------------------------------------------------------
int get_c3img_width(C3IMG *img)
{
	return img->width;
}

//----------------------------------------------------------------------------
// 関数名       ：get_c3img_height
// 概要         ：抽象化画像データの高さを返す
// 戻り値       ：int
// 引数         ：C3IMG *img
//                抽象化画像データ
//----------------------------------------------------------------------------
int get_c3img_height(C3IMG *img)
{
	return img->height;
}

//----------------------------------------------------------------------------
// 関数名       ：bmp2c3img
// 概要         ：BMPデータを抽象化画像データに変換
// 戻り値       ：C3IMG *
// 引数         ：BMP *bmp
//                BMPデータ
//----------------------------------------------------------------------------
C3IMG *bmp2c3img(BMP *bmp)
{
	C3IMG *img;
	int i;
	int j;
	int width;
	int height;
	int tmppix;

	img = (C3IMG *)malloc(sizeof(C3IMG));
	if(img == NULL) return NULL;

	img->width = get_bmp_width(bmp);
	img->height = get_bmp_height(bmp);

	img->r = (unsigned char *)malloc(sizeof(unsigned char) * img->width * img->height);
	img->g = (unsigned char *)malloc(sizeof(unsigned char) * img->width * img->height);
	img->b = (unsigned char *)malloc(sizeof(unsigned char) * img->width * img->height);
	img->a = (unsigned char *)malloc(sizeof(unsigned char) * img->width * img->height);

	if(img->r == NULL || img->g == NULL || img->b == NULL || img->a == NULL) return NULL;

	//データ部
	switch(get_bmp_bit_count(bmp)){
	case BMP1:
		/* 1bitカラー, 未実装 */
		return NULL;
		break;

	case BMP4:
		/* 4bitカラー, 未実装 */
		return NULL;
		break;

	case BMP8:
		/* 8bitカラー, 未実装 */
		return NULL;
		break;

	case BMP16:
		/* 16bitカラー, 未実装 */
		return NULL;
		break;

	case BMP24:
		/* 24bitカラー */

		img->level = get_bmp_bit_count(bmp);
		for(i=img->height-1; 0 <= i; i--)
		{

			for(j=0; j < img->width; j++)
			{
				/* FIXME: 長い。治らぬ病と仏が匙投げる。 */
				tmppix = bmp->body[ (int) ( i * 4 * ceil( ( (double) 3 * img->width) / 4 ) + 3 * j + 2 ) ];
				img->r[ ( img->height-1 - i ) * img->width + j ] = tmppix;

				tmppix = bmp->body[ (int) ( i * 4 * ceil( ( (double) 3 * img->width) / 4 ) + 3 * j + 1 ) ];
				img->g[ ( img->height-1 - i ) * img->width + j ] = tmppix;

				tmppix = bmp->body[ (int) ( i * 4 * ceil( ( (double) 3 * img->width) / 4 ) + 3 * j + 0 ) ];
				img->b[ ( img->height-1 - i ) * img->width + j ] = tmppix;
			}

		}

		img->type = C3IMGCOLOR;
		img->level = C3IMGCOLOR24BIT;

		break;

	case BMP32:
		/* 32bitカラー, 未実装 */
		return NULL;
		break;
	}

	return img;

}

//----------------------------------------------------------------------------
// 関数名       ：c3img2bmp
// 概要         ：抽象化画像データをBMPデータに変換
// 戻り値       ：BMP *
// 引数         ：C3IMG *img
//                抽象化画像データ
//----------------------------------------------------------------------------
BMP *c3img2bmp(C3IMG *img)
{
	/*
	 * とりあえずINFOHEADERで実装
	 */
	int i;
	int j;
	int bmp_line_width;
	int fsize;
	int body_size;
	unsigned char *buff;
	BMP *bmp;
	BMPINFOHEADER *info_header;

	bmp = (BMP *)malloc(sizeof(BMP));

	if (bmp == NULL) return NULL;

	info_header = (BMPINFOHEADER *)malloc(sizeof(BMPINFOHEADER));
	if (info_header == NULL) return NULL;

	bmp->bmpType = BMPINFOHEADERSIZE;
	bmp->body = NULL;
	bmp->fileHeader = (BMPFILEHEADER *)malloc(sizeof(BMPFILEHEADER));
	bmp->infoHeader = info_header;
	bmp->palletBitSize = 0;
	bmp->pallets = NULL;

	bmp->fileHeader->bfType      = *(short *)"BM";
	bmp->fileHeader->bfSize      = BMPFILEHEADERSIZE + BMPINFOHEADERSIZE + 4 * ceil((double)img->width * 3 / 4.0 ) * img->height;
	bmp->fileHeader->bfReserved1 = 0;
	bmp->fileHeader->bfReserved2 = 0;
	bmp->fileHeader->bfOffBits   = BMPFILEHEADERSIZE + BMPINFOHEADERSIZE;

	info_header->bcSize           = BMPINFOHEADERSIZE;
	info_header->bcWidth          = img->width;
	info_header->bcHeight         = img->height;
	info_header->bcPlanes         = 1;
	info_header->bcBitCount       = BMP24;
	info_header->biCompression    = 0;
	info_header->biSizeImage      = BMPRGB;
	info_header->biXPixPerMeter   = 0;
	info_header->biYPixPerMeter   = 0;
	info_header->biClrUsed        = 0;
	info_header->biCirImportant   = 0;

	bmp_line_width = 4 * ceil((double)img->width * 3 / 4.0 );
	bmp->body = (unsigned char*)malloc(sizeof(unsigned char) * bmp_line_width * img->height);

	for(i=0; i < img->height; i++)
	{
		for(j=0; j < img->width; j++)
		{
			bmp->body[(img->height - i - 1) * bmp_line_width + 3*j + 0] = img->b[i*img->width + j];
			bmp->body[(img->height - i - 1) * bmp_line_width + 3*j + 1] = img->g[i*img->width + j];
			bmp->body[(img->height - i - 1) * bmp_line_width + 3*j + 2] = img->r[i*img->width + j];
		}
	}

	return bmp;
}

//----------------------------------------------------------------------------
// 関数名       ：pnm2c3img
// 概要         ：PNMデータを抽象化画像データに変換
// 戻り値       ：C3IMG *
// 引数         ：PNM *pnm
//                PNMデータ
//----------------------------------------------------------------------------
C3IMG *pnm2c3img(PNM *pnm)
{
	C3IMG *img;

	img = (C3IMG *)malloc(sizeof(C3IMG));
	img->width = pnm->width;
	img->height = pnm->height;

	img->r = (unsigned char *)malloc(sizeof(unsigned char) * img->width * img->height);
	img->g = (unsigned char *)malloc(sizeof(unsigned char) * img->width * img->height);
	img->b = (unsigned char *)malloc(sizeof(unsigned char) * img->width * img->height);
	img->a = (unsigned char *)malloc(sizeof(unsigned char) * img->width * img->height);

	if(img->r == NULL || img->g == NULL || img->b == NULL || img->a == NULL) return NULL;

	switch(pnm->type) {
	case PBMASCII:
		img->type = C3IMGGRAY;
		img->level = C3IMGCOLORGRAY8BIT;
		break;

	case PGMASCII:
		img->type = C3IMGGRAY;
		img->level = C3IMGCOLORGRAY8BIT;
		break;

	case PPMASCII:
		img->type = C3IMGCOLOR;
		img->level = C3IMGCOLOR24BIT;
		break;

	case PBMBIN:
		img->type = C3IMGGRAY;
		img->level = C3IMGCOLORGRAY1BIT;
		break;

	case PGMBIN:
		img->type = C3IMGGRAY;
		img->level = C3IMGCOLORGRAY8BIT;
		break;

	case PPMBIN:
		img->type = C3IMGCOLOR;
		img->level = C3IMGCOLOR24BIT;
		break;
	}

	memcpy(img->r, pnm->r, pnm->width * pnm->height);
	memcpy(img->g, pnm->g, pnm->width * pnm->height);
	memcpy(img->b, pnm->b, pnm->width * pnm->height);

	return img;

}

//----------------------------------------------------------------------------
// 関数名       ：c3img2pnm
// 概要         ：抽象化データをPNMデータに変換
// 戻り値       ：PNM *
// 引数         ：C3IMG *img
//                抽象化画像データ
//----------------------------------------------------------------------------
PNM *c3img2pnm(C3IMG *img)
{
	PNM *pnm;
	pnm = (PNM *)malloc(sizeof(PNM));

	pnm->type = PPMASCII;
	pnm->width = img->width;
	pnm->height = img->height;

	pnm->r = (unsigned char *)malloc(sizeof(unsigned char) * pnm->width * pnm->height);
	pnm->g = (unsigned char *)malloc(sizeof(unsigned char) * pnm->width * pnm->height);
	pnm->b = (unsigned char *)malloc(sizeof(unsigned char) * pnm->width * pnm->height);

	if(pnm->r == NULL || pnm->g == NULL || pnm->b == NULL) return NULL;

	memcpy(pnm->r, img->r, img->width * img->height);
	memcpy(pnm->g, img->g, img->width * img->height);
	memcpy(pnm->b, img->b, img->width * img->height);

	return pnm;
}

//----------------------------------------------------------------------------
// 関数名       ：posterize_c3img
// 概要         ：抽象化画像データの減色処理
// 戻り値       ：int
// 引数         ：C3IMG *img
//                抽象化画像データ
// 引数         ：int level
//                変換後の色レベル
//----------------------------------------------------------------------------
int posterize_c3img(C3IMG *img, int level)
{
	int i;
	int size;
	int tmpcol;

	size = get_c3img_width(img) * get_c3img_height(img);

	if(!(level == C3IMGCOLOR3BIT
			|| level == C3IMGCOLOR6BIT
			|| level == C3IMGCOLOR12BIT
			|| level == C3IMGCOLOR24BIT
			|| level == C3IMGCOLORGRAY1BIT
			|| level == C3IMGCOLORGRAY2BIT
			|| level == C3IMGCOLORGRAY4BIT
			|| level == C3IMGCOLORGRAY8BIT) ) {

		return -1;//非対応色レベル以外は拒否
	}

	for(i=0; i<size; i++)
	{
		switch(level){
		//カラー8色
		case C3IMGCOLOR3BIT:
			img->r[i] = (img->r[i]/128)*128;
			img->g[i] = (img->g[i]/128)*128;
			img->b[i] = (img->b[i]/128)*128;
			img->type = C3IMGCOLOR;
			img->level = C3IMGCOLOR3BIT;
			break;

		//カラー32色
		case C3IMGCOLOR6BIT:
			img->r[i] = (img->r[i]/64)*128;
			img->g[i] = (img->g[i]/64)*128;
			img->b[i] = (img->b[i]/64)*128;
			img->type = C3IMGCOLOR;
			img->level = C3IMGCOLOR6BIT;
			break;

		//カラー4096色
		case C3IMGCOLOR12BIT:
			img->r[i] = (img->r[i]/16)*128;
			img->g[i] = (img->g[i]/16)*128;
			img->b[i] = (img->b[i]/16)*128;
			img->type = C3IMGCOLOR;
			img->level = C3IMGCOLOR12BIT;
			break;

		//フルカラー
		case C3IMGCOLOR24BIT:
			//色変更する必要無し
			img->type = C3IMGCOLOR;
			img->level = C3IMGCOLOR24BIT;
			break;

		//グレースケール2階調(2値化)
		case C3IMGCOLORGRAY1BIT:
			tmpcol = (img->r[i] + img->g[i] + img->b[i])/3;
			img->r[i] = img->g[i] = img->b[i] = tmpcol/128;
			img->type = C3IMGGRAY;
			img->level = C3IMGCOLORGRAY1BIT;
			break;

		//グレースケール4階調(
		case C3IMGCOLORGRAY2BIT:
			tmpcol = (img->r[i] + img->g[i] + img->b[i])/3;
			img->r[i] = img->g[i] = img->b[i] = tmpcol/64;
			img->type = C3IMGGRAY;
			img->level = C3IMGCOLORGRAY2BIT;
			break;

		//グレースケール16階調(
		case C3IMGCOLORGRAY4BIT:
			tmpcol = (img->r[i] + img->g[i] + img->b[i])/3;
			img->r[i] = img->g[i] = img->b[i] = tmpcol/16;
			img->type = C3IMGGRAY;
			img->level = C3IMGCOLORGRAY4BIT;
			break;

		//グレースケール256階調(
		case C3IMGCOLORGRAY8BIT:
			tmpcol = (img->r[i] + img->g[i] + img->b[i])/3;
			img->r[i] = img->g[i] = img->b[i] = tmpcol;
			img->type = C3IMGGRAY;
			img->level = C3IMGCOLORGRAY8BIT;
			break;

		}
	}
	return 0;
}


//----------------------------------------------------------------------------
// 関数名       ：delete_c3img
// 概要         ：抽象化画像データの領域開放
// 戻り値       ：int
// 引数         ：C3IMG *img
//                抽象化画像データ
//----------------------------------------------------------------------------
int delete_c3img(C3IMG *img)
{
	free(img->r);
	free(img->g);
	free(img->b);
	free(img);

	return 0;
}
