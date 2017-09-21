//================================================================================
//      FILE
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//      ファイル名      :       bmp.c
//      概要            :       BMP形式画像を扱うライブラリ
//--------------------------------------------------------------------------------
#include "bmp.h"
#include <math.h>

static BMP *_read_bmp_core_header(FILE *);

static BMP *_read_bmp_info_header(FILE *);

static BMP *_read_bmp_info2_header(FILE *);

static BMP *_read_bmp_v4_header(FILE *);

static BMP *_read_bmp_v5_header(FILE *);

static int _write_bmp_core_header(FILE *, BMP *);

static int _write_bmp_info_header(FILE *, BMP *);

static int _write_bmp_info2_header(FILE *, BMP *);

static int _write_bmp_v4_header(FILE *, BMP *);

static int _write_bmp_v5_header(FILE *, BMP *);

//----------------------------------------------------------------------------
// 関数名       ：read_bmp
// 概要         ：BMP画像の読み込み
// 戻り値       ：BMP *
// 引数         ：FILE *fp
//                画像ファイルのファイルポインタ
//----------------------------------------------------------------------------
BMP *read_bmp(FILE *fp){

	BMP *bmp;
	unsigned char buff[1024];

	/* 読み込み前の前処理 */
	if (fp == NULL) return NULL;

	/* BMPファイルヘッダー + 情報ヘッダー(ヘッダーサイズ)の抜き出し */
	fread(buff, BMPFILEHEADERSIZE + 4, 1, fp);

	/* BMPファイルではない */
	if( *(short *)buff != *(short *)"BM" ) {
		return NULL;
	}

	/* BMP情報ヘッダーごとに_read_bmp_*()に丸投げ */
	bmp = NULL;

	switch(*(int *)(buff + BMPFILEHEADERSIZE)){
	case BMPCOREHEADERSIZE:
		/* 未実装 */
		bmp = _read_bmp_core_header(fp);
		break;

	case BMPINFOHEADERSIZE:
		bmp = _read_bmp_info_header(fp);
		break;

	case BMPINFO2HEADERSIZE:
		/* 未実装 */
		bmp = _read_bmp_info2_header(fp);
		break;

	case BMPV4HEADERSIZE:
		bmp = _read_bmp_v4_header(fp);
		break;

	case BMPV5HEADERSIZE:
		/* 未実装 */
		bmp = _read_bmp_v5_header(fp);
		break;
	}

	return bmp;
}

//----------------------------------------------------------------------------
// 関数名       ：write_bmp
// 概要         ：BMP画像の保存
// 戻り値       ：int
// 引数         ：FILE *fp
//                保存ファイルのファイルポインタ
// 引数         ：BMP *bmp
//                BMPデータ
//----------------------------------------------------------------------------
int write_bmp(FILE *fp, BMP *bmp)
{
	int status_code = 1;

	if (fp == NULL) return 1;
	if (bmp == NULL) return 1;

	switch(bmp->bmpType){
	case BMPCOREHEADERSIZE:
		/* 未実装 */
		status_code = _write_bmp_core_header(fp, bmp);
		break;

	case BMPINFOHEADERSIZE:
		status_code = _write_bmp_info_header(fp, bmp);
		break;

	case BMPINFO2HEADERSIZE:
		/* 未実装 */
		status_code = _write_bmp_info2_header(fp, bmp);
		break;

	case BMPV4HEADERSIZE:
		status_code = _write_bmp_v4_header(fp, bmp);
		break;

	case BMPV5HEADERSIZE:
		/* 未実装 */
		status_code = _write_bmp_v5_header(fp, bmp);
		break;
	}

	return status_code;
}

//----------------------------------------------------------------------------
// 関数名       ：close_bmp
// 概要         ：BMPデータ領域開放
// 戻り値       ：int
// 引数         ：BMP *bmp
//                BMPデータ
//----------------------------------------------------------------------------
int close_bmp(BMP *bmp)
{
	int i;

	if(bmp == NULL) return 0;
	free(bmp->body);
	bmp->body = NULL;
	free(bmp->fileHeader);
	bmp->fileHeader = NULL;
	free(bmp->infoHeader);
	bmp->infoHeader = NULL;
	/*
	 * TODO: ここをどうにかしないと
	 * info_header->biClrUsed: 1<<bmp->palletBitSize
	 * COREHEADERの扱いは？
	 *
	 *  */
	for(i = 0 ;  i < (bmp->palletBitSize ? 1<<bmp->palletBitSize: 0); i++)
	{
		free(bmp->pallets[i]);
		bmp->pallets[i] = NULL;
	}
	free(bmp->pallets);
	bmp->pallets = NULL;
	free(bmp);

	return 1;
}

int get_bmp_width(BMP *bmp)
{
	switch(bmp->bmpType){
	case BMPCOREHEADERSIZE:
		return ((BMPCOREHEADER *)bmp->infoHeader)->bcWidth;
		break;

	case BMPINFOHEADERSIZE:
		return ((BMPINFOHEADER *)bmp->infoHeader)->bcWidth;
		break;

	case BMPINFO2HEADERSIZE:
		/* 未実装 */
		break;

	case BMPV4HEADERSIZE:
		return ((BMPV4HEADER *)bmp->infoHeader)->bV4Width;
		break;

	case BMPV5HEADERSIZE:
		return ((BMPV5HEADER *)bmp->infoHeader)->bV5Width;
		break;
	}
	return 0;
}

int get_bmp_height(BMP *bmp)
{
	switch(bmp->bmpType){
	case BMPCOREHEADERSIZE:
		return ((BMPCOREHEADER *)bmp->infoHeader)->bcHeight;
		break;

	case BMPINFOHEADERSIZE:
		return ((BMPINFOHEADER *)bmp->infoHeader)->bcHeight;
		break;

	case BMPINFO2HEADERSIZE:
		/* 未実装 */
		break;

	case BMPV4HEADERSIZE:
		return ((BMPV4HEADER *)bmp->infoHeader)->bV4Height;
		break;

	case BMPV5HEADERSIZE:
		return ((BMPV5HEADER *)bmp->infoHeader)->bV5Height;
		break;
	}
	return 0;
}

short get_bmp_bit_count(BMP *bmp)
{
	switch(bmp->bmpType){
	case BMPCOREHEADERSIZE:
		return ((BMPCOREHEADER *)bmp->infoHeader)->bcBitCount;
		break;

	case BMPINFOHEADERSIZE:
		return ((BMPINFOHEADER *)bmp->infoHeader)->bcBitCount;
		break;

	case BMPINFO2HEADERSIZE:
		/* 未実装 */
		break;

	case BMPV4HEADERSIZE:
		return ((BMPV4HEADER *)bmp->infoHeader)->bV4BitCount;
		break;

	case BMPV5HEADERSIZE:
		return ((BMPV5HEADER *)bmp->infoHeader)->bV5BitCount;
		break;
	}
	return 0;
	//return bmp->bcBitCount;
}

int get_compress_type(BMP *bmp)
{
	return bmp->bmpType;
}

static BMP *_read_bmp_core_header(FILE *fp)
{
	/* 未実装 */
	return NULL;
}

static BMP *_read_bmp_info_header(FILE *fp){
	int i;
	int fsize;
	int body_size;
	unsigned char *buff;
	BMP *bmp;
	BMPINFOHEADER *info_header;

	/* 未実装 */
	fseek(fp, 0, SEEK_END);
	fsize = ftell(fp);
	rewind(fp);
	buff = (unsigned char*)malloc(fsize);
	if (buff == NULL) return NULL;

	fread(buff, fsize, 1, fp);

	bmp = (BMP *)malloc(sizeof(BMP));

	if (bmp == NULL) {
		free(buff);
		return NULL;
	}

	bmp->bmpType = 0;
	bmp->body = NULL;
	bmp->fileHeader = NULL;
	bmp->infoHeader = NULL;
	bmp->palletBitSize = 0;
	bmp->pallets = NULL;

	/* BMPFILEHEADER処理 */
	bmp->fileHeader = (BMPFILEHEADER *)malloc(sizeof(BMPFILEHEADER));

	if(bmp->fileHeader == NULL) {
		close_bmp(bmp);
		free(buff);
		return NULL;
	}

	memcpy(&bmp->fileHeader->bfType, buff, 2);
	memcpy(&bmp->fileHeader->bfSize, buff + 2, 4);
	memcpy(&bmp->fileHeader->bfReserved1, buff + 6, 2);
	memcpy(&bmp->fileHeader->bfReserved2, buff + 8, 2);
	memcpy(&bmp->fileHeader->bfOffBits, buff + 10, 4);

	if (bmp->fileHeader->bfType != *(short *)"BM") {
		close_bmp(bmp);
		free(buff);
		return NULL;
	}

	/* BMPINFOHEADER */
	/*
	 * FIXME: おかしくね
	 */
	bmp->infoHeader = (BMPINFOHEADER *)malloc(sizeof(BMPINFOHEADER));

	if(bmp->infoHeader == NULL) {
		close_bmp(bmp);
		free(buff);
		return NULL;
	}

	info_header = (BMPINFOHEADER *) bmp->infoHeader;

	memcpy(&info_header->bcSize, buff + BMPFILEHEADERSIZE, 4);
	memcpy(&info_header->bcWidth, buff + BMPFILEHEADERSIZE + 4, 4);
	memcpy(&info_header->bcHeight, buff + BMPFILEHEADERSIZE + 8, 4);
	memcpy(&info_header->bcPlanes, buff + BMPFILEHEADERSIZE + 12, 2);
	memcpy(&info_header->bcBitCount, buff + BMPFILEHEADERSIZE + 14, 2);
	memcpy(&info_header->biCompression, buff + BMPFILEHEADERSIZE + 16, 4);
	memcpy(&info_header->biSizeImage, buff + BMPFILEHEADERSIZE + 20, 4);
	memcpy(&info_header->biXPixPerMeter, buff + BMPFILEHEADERSIZE + 24, 4);
	memcpy(&info_header->biYPixPerMeter, buff + BMPFILEHEADERSIZE + 28, 4);
	memcpy(&info_header->biClrUsed, buff + BMPFILEHEADERSIZE + 32, 4);
	memcpy(&info_header->biCirImportant, buff + BMPFILEHEADERSIZE + 36, 4);

	if (info_header->bcSize != BMPINFOHEADERSIZE) {
		close_bmp(bmp);
		free(buff);
		return NULL;
	}

	bmp->bmpType = info_header->bcSize;

	/* パレット処理 */
	if(info_header->bcBitCount != BMP24 && info_header->bcBitCount != BMP32 ) {
		bmp->palletBitSize = info_header->bcBitCount;
		bmp->pallets = (RGBQUAD **)malloc(sizeof(RGBQUAD *) * (info_header->biClrUsed? info_header->biClrUsed: 1<<info_header->bcBitCount));
		if (bmp->pallets == NULL) {
			close_bmp(bmp);
			free(buff);
			return NULL;
		}

		for(i=0; i < (info_header->biClrUsed? info_header->biClrUsed: 1<<info_header->bcBitCount); i++)
		{
			bmp->pallets[i] = (RGBQUAD *)malloc(sizeof(RGBQUAD));
			if (bmp->pallets[i] == NULL) {
				close_bmp(bmp);
				free(buff);
				return NULL;
			}
			memcpy(&bmp->pallets[i]->rgbBlue,     buff + BMPFILEHEADERSIZE + BMPINFOHEADERSIZE + i * 4 + 0, 1);
			memcpy(&bmp->pallets[i]->rgbGreen,    buff + BMPFILEHEADERSIZE + BMPINFOHEADERSIZE + i * 4 + 1, 1);
			memcpy(&bmp->pallets[i]->rgbRed,      buff + BMPFILEHEADERSIZE + BMPINFOHEADERSIZE + i * 4 + 2, 1);
			memcpy(&bmp->pallets[i]->rgbReserved, buff + BMPFILEHEADERSIZE + BMPINFOHEADERSIZE + i * 4 + 3, 1);
		}
	}
	/* データ部処理 */

	body_size= info_header->bcHeight * 4 * ceil(ceil(info_header->bcWidth * (float)info_header->bcBitCount / 8.0) / 4.0);
	bmp->body = (unsigned char *)malloc(sizeof(unsigned char) * body_size );

	if (bmp->body == NULL) {
		close_bmp(bmp);
		free(buff);
		return NULL;
	}

	memcpy(bmp->body, buff + bmp->fileHeader->bfOffBits, body_size);

	free(buff);

	return bmp;
}

static BMP *_read_bmp_info2_header(FILE *fp)
{
	/* 未実装 */

	return NULL;
}

static BMP *_read_bmp_v4_header(FILE *fp)
{
	int i;
	int fsize;
	int body_size;
	unsigned char *buff;
	BMP *bmp;
	BMPV4HEADER *info_header;

	fseek(fp, 0, SEEK_END);
	fsize = ftell(fp);
	rewind(fp);
	buff = (unsigned char*)malloc(fsize);
	if (buff == NULL) return NULL;

	fread(buff, fsize, 1, fp);

	bmp = (BMP *)malloc(sizeof(BMP));

	if (bmp == NULL) {
		free(buff);
		return NULL;
	}

	bmp->bmpType = 0;
	bmp->body = NULL;
	bmp->fileHeader = NULL;
	bmp->infoHeader = NULL;
	bmp->palletBitSize = 0;
	bmp->pallets = NULL;

	/* BMPFILEHEADER処理 */
	bmp->fileHeader = (BMPFILEHEADER *)malloc(sizeof(BMPFILEHEADER));

	if(bmp->fileHeader == NULL) {
		close_bmp(bmp);
		free(buff);
		return NULL;
	}

	memcpy(&bmp->fileHeader->bfType, buff, 2);
	memcpy(&bmp->fileHeader->bfSize, buff + 2, 4);
	memcpy(&bmp->fileHeader->bfReserved1, buff + 6, 2);
	memcpy(&bmp->fileHeader->bfReserved2, buff + 8, 2);
	memcpy(&bmp->fileHeader->bfOffBits, buff + 10, 4);

	if (bmp->fileHeader->bfType != *(short *)"BM") {
		close_bmp(bmp);
		free(buff);
		return NULL;
	}

	/* BMPV4HEADER */
	bmp->infoHeader = (BMPV4HEADER *)malloc(sizeof(BMPV4HEADER));

	if(bmp->infoHeader == NULL) {
		close_bmp(bmp);
		free(buff);
		return NULL;
	}

	info_header = (BMPV4HEADER *) bmp->infoHeader;

	memcpy(&info_header->bV4Size         , buff + BMPFILEHEADERSIZE     , 4);
	memcpy(&info_header->bV4Width        , buff + BMPFILEHEADERSIZE +4  , 4);
	memcpy(&info_header->bV4Height       , buff + BMPFILEHEADERSIZE +8  , 4);
	memcpy(&info_header->bV4Planes       , buff + BMPFILEHEADERSIZE +12 , 2);
	memcpy(&info_header->bV4BitCount     , buff + BMPFILEHEADERSIZE +14 , 2);
	memcpy(&info_header->bV4Compression, buff + BMPFILEHEADERSIZE +16 , 4);
	memcpy(&info_header->bV4SizeImage    , buff + BMPFILEHEADERSIZE +20 , 4);
	memcpy(&info_header->bV4XPelsPerMeter, buff + BMPFILEHEADERSIZE +24 , 4);
	memcpy(&info_header->bV4YPelsPerMeter, buff + BMPFILEHEADERSIZE +28 , 4);
	memcpy(&info_header->bV4ClrUsed      , buff + BMPFILEHEADERSIZE +32 , 4);
	memcpy(&info_header->bV4ClrImportant , buff + BMPFILEHEADERSIZE +36 , 4);
	memcpy(&info_header->bV4RedMask      , buff + BMPFILEHEADERSIZE +40 , 4);
	memcpy(&info_header->bV4GreenMask    , buff + BMPFILEHEADERSIZE +44 , 4);
	memcpy(&info_header->bV4BlueMask     , buff + BMPFILEHEADERSIZE +48 , 4);
	memcpy(&info_header->bV4AlphaMask    , buff + BMPFILEHEADERSIZE +52 , 4);
	memcpy(&info_header->bV4CSType       , buff + BMPFILEHEADERSIZE +56 , 4);
	memcpy(&info_header->bV4EndpointRX   , buff + BMPFILEHEADERSIZE +60 , 4);
	memcpy(&info_header->bV4EndpointRY   , buff + BMPFILEHEADERSIZE +64 , 4);
	memcpy(&info_header->bV4EndpointRZ   , buff + BMPFILEHEADERSIZE +68 , 4);
	memcpy(&info_header->bV4EndpointGX   , buff + BMPFILEHEADERSIZE +72 , 4);
	memcpy(&info_header->bV4EndpointGY   , buff + BMPFILEHEADERSIZE +76 , 4);
	memcpy(&info_header->bV4EndpointGZ   , buff + BMPFILEHEADERSIZE +80 , 4);
	memcpy(&info_header->bV4EndpointBX   , buff + BMPFILEHEADERSIZE +84 , 4);
	memcpy(&info_header->bV4EndpointBY   , buff + BMPFILEHEADERSIZE +88 , 4);
	memcpy(&info_header->bV4EndpointBZ   , buff + BMPFILEHEADERSIZE +92 , 4);
	memcpy(&info_header->bV4GammaRed     , buff + BMPFILEHEADERSIZE +96 , 4);
	memcpy(&info_header->bV4GammaGreen   , buff + BMPFILEHEADERSIZE +100, 4);
	memcpy(&info_header->bV4GammaBlue    , buff + BMPFILEHEADERSIZE +104, 4);


	if (info_header->bV4Size != BMPV4HEADERSIZE) {
		close_bmp(bmp);
		free(buff);
		return NULL;
	}

	bmp->bmpType = info_header->bV4Size;

	/* パレット処理 */
	if(info_header->bV4BitCount != BMP24 && info_header->bV4BitCount != BMP32 ) {
		bmp->palletBitSize = info_header->bV4BitCount;
		bmp->pallets = (RGBQUAD **)malloc(sizeof(RGBQUAD *) * (info_header->bV4ClrUsed ? info_header->bV4ClrUsed: 1<<info_header->bV4BitCount));
		if (bmp->pallets == NULL) {
			close_bmp(bmp);
			free(buff);
			return NULL;
		}

		for(i=0; i < (info_header->bV4ClrUsed ? info_header->bV4ClrUsed: 1<<info_header->bV4BitCount); i++)
		{
			bmp->pallets[i] = (RGBQUAD *)malloc(sizeof(RGBQUAD));
			if (bmp->pallets[i] == NULL) {
				close_bmp(bmp);
				free(buff);
				return NULL;
			}
			memcpy(&bmp->pallets[i]->rgbBlue,     buff + BMPFILEHEADERSIZE + BMPV4HEADERSIZE + i * 4 + 0, 1);
			memcpy(&bmp->pallets[i]->rgbGreen,    buff + BMPFILEHEADERSIZE + BMPV4HEADERSIZE + i * 4 + 1, 1);
			memcpy(&bmp->pallets[i]->rgbRed,      buff + BMPFILEHEADERSIZE + BMPV4HEADERSIZE + i * 4 + 2, 1);
			memcpy(&bmp->pallets[i]->rgbReserved, buff + BMPFILEHEADERSIZE + BMPV4HEADERSIZE + i * 4 + 3, 1);
		}
	}
	/* データ部処理 */
	body_size= info_header->bV4Height * 4 * ceil(ceil(info_header->bV4Width * (float)info_header->bV4BitCount / 8.0) / 4.0);
	bmp->body = (unsigned char *)malloc(sizeof(unsigned char) * body_size );

	if (bmp->body == NULL) {
		close_bmp(bmp);
		free(buff);
		return NULL;
	}

	memcpy(bmp->body, buff + bmp->fileHeader->bfOffBits, body_size);

	free(buff);

	return bmp;

}

static BMP *_read_bmp_v5_header(FILE *fp)
{
	int i;
	int fsize;
	int body_size;
	unsigned char *buff;
	BMP *bmp;
	BMPV5HEADER *info_header;

	fseek(fp, 0, SEEK_END);
	fsize = ftell(fp);
	rewind(fp);
	buff = (unsigned char*)malloc(fsize);
	if (buff == NULL) return NULL;

	fread(buff, fsize, 1, fp);

	bmp = (BMP *)malloc(sizeof(BMP));

	if (bmp == NULL) {
		free(buff);
		return NULL;
	}

	bmp->bmpType = 0;
	bmp->body = NULL;
	bmp->fileHeader = NULL;
	bmp->infoHeader = NULL;
	bmp->palletBitSize = 0;
	bmp->pallets = NULL;

	/* BMPFILEHEADER処理 */
	bmp->fileHeader = (BMPFILEHEADER *)malloc(sizeof(BMPFILEHEADER));

	if(bmp->fileHeader == NULL) {
		close_bmp(bmp);
		free(buff);
		return NULL;
	}

	memcpy(&bmp->fileHeader->bfType, buff, 2);
	memcpy(&bmp->fileHeader->bfSize, buff + 2, 4);
	memcpy(&bmp->fileHeader->bfReserved1, buff + 6, 2);
	memcpy(&bmp->fileHeader->bfReserved2, buff + 8, 2);
	memcpy(&bmp->fileHeader->bfOffBits, buff + 10, 4);

	if (bmp->fileHeader->bfType != *(short *)"BM") {
		close_bmp(bmp);
		free(buff);
		return NULL;
	}

	/* BMPV5HEADER */
	bmp->infoHeader = (BMPV5HEADER *)malloc(sizeof(BMPV5HEADER));

	if(bmp->infoHeader == NULL) {
		close_bmp(bmp);
		free(buff);
		return NULL;
	}

	info_header = (BMPV5HEADER *) bmp->infoHeader;

	memcpy(&info_header->bV5Size         , buff + BMPFILEHEADERSIZE     , 4);
	memcpy(&info_header->bV5Width        , buff + BMPFILEHEADERSIZE +4  , 4);
	memcpy(&info_header->bV5Height       , buff + BMPFILEHEADERSIZE +8  , 4);
	memcpy(&info_header->bV5Planes       , buff + BMPFILEHEADERSIZE +12 , 2);
	memcpy(&info_header->bV5BitCount     , buff + BMPFILEHEADERSIZE +14 , 2);
	memcpy(&info_header->bV5Compression  , buff + BMPFILEHEADERSIZE +16 , 4);
	memcpy(&info_header->bV5SizeImage    , buff + BMPFILEHEADERSIZE +20 , 4);
	memcpy(&info_header->bV5XPelsPerMeter, buff + BMPFILEHEADERSIZE +24 , 4);
	memcpy(&info_header->bV5YPelsPerMeter, buff + BMPFILEHEADERSIZE +28 , 4);
	memcpy(&info_header->bV5ClrUsed      , buff + BMPFILEHEADERSIZE +32 , 4);
	memcpy(&info_header->bV5ClrImportant , buff + BMPFILEHEADERSIZE +36 , 4);
	memcpy(&info_header->bV5RedMask      , buff + BMPFILEHEADERSIZE +40 , 4);
	memcpy(&info_header->bV5GreenMask    , buff + BMPFILEHEADERSIZE +44 , 4);
	memcpy(&info_header->bV5BlueMask     , buff + BMPFILEHEADERSIZE +48 , 4);
	memcpy(&info_header->bV5AlphaMask    , buff + BMPFILEHEADERSIZE +52 , 4);
	memcpy(&info_header->bV5CSType       , buff + BMPFILEHEADERSIZE +56 , 4);
	memcpy(&info_header->bV5EndpointRX   , buff + BMPFILEHEADERSIZE +60 , 4);
	memcpy(&info_header->bV5EndpointRY   , buff + BMPFILEHEADERSIZE +64 , 4);
	memcpy(&info_header->bV5EndpointRZ   , buff + BMPFILEHEADERSIZE +68 , 4);
	memcpy(&info_header->bV5EndpointGX   , buff + BMPFILEHEADERSIZE +72 , 4);
	memcpy(&info_header->bV5EndpointGY   , buff + BMPFILEHEADERSIZE +76 , 4);
	memcpy(&info_header->bV5EndpointGZ   , buff + BMPFILEHEADERSIZE +80 , 4);
	memcpy(&info_header->bV5EndpointBX   , buff + BMPFILEHEADERSIZE +84 , 4);
	memcpy(&info_header->bV5EndpointBY   , buff + BMPFILEHEADERSIZE +88 , 4);
	memcpy(&info_header->bV5EndpointBZ   , buff + BMPFILEHEADERSIZE +92 , 4);
	memcpy(&info_header->bV5GammaRed     , buff + BMPFILEHEADERSIZE +96 , 4);
	memcpy(&info_header->bV5GammaGreen   , buff + BMPFILEHEADERSIZE +100, 4);
	memcpy(&info_header->bV5GammaBlue    , buff + BMPFILEHEADERSIZE +104, 4);
	memcpy(&info_header->bV5Intent       , buff + BMPFILEHEADERSIZE +108, 4);
	memcpy(&info_header->bV5ProfileData  , buff + BMPFILEHEADERSIZE +112, 4);
	memcpy(&info_header->bV5ProfileSize  , buff + BMPFILEHEADERSIZE +116, 4);
	memcpy(&info_header->bV5Reserved     , buff + BMPFILEHEADERSIZE +120, 4);

	if (info_header->bV5Size != BMPV5HEADERSIZE) {
		close_bmp(bmp);
		free(buff);
		return NULL;
	}

	bmp->bmpType = info_header->bV5Size;

	/* パレット処理 */
	if(info_header->bV5BitCount != BMP24 && info_header->bV5BitCount != BMP32 ) {
		bmp->palletBitSize = info_header->bV5BitCount;
		bmp->pallets = (RGBQUAD **)malloc(sizeof(RGBQUAD *) * (info_header->bV5ClrUsed ? info_header->bV5ClrUsed: 1<<info_header->bV5BitCount));
		if (bmp->pallets == NULL) {
			close_bmp(bmp);
			free(buff);
			return NULL;
		}

		for(i=0; i < (info_header->bV5ClrUsed ? info_header->bV5ClrUsed: 1<<info_header->bV5BitCount); i++)
		{
			bmp->pallets[i] = (RGBQUAD *)malloc(sizeof(RGBQUAD));
			if (bmp->pallets[i] == NULL) {
				close_bmp(bmp);
				free(buff);
				return NULL;
			}
			memcpy(&bmp->pallets[i]->rgbBlue,     buff + BMPFILEHEADERSIZE + BMPV5HEADERSIZE + i * 4 + 0, 1);
			memcpy(&bmp->pallets[i]->rgbGreen,    buff + BMPFILEHEADERSIZE + BMPV5HEADERSIZE + i * 4 + 1, 1);
			memcpy(&bmp->pallets[i]->rgbRed,      buff + BMPFILEHEADERSIZE + BMPV5HEADERSIZE + i * 4 + 2, 1);
			memcpy(&bmp->pallets[i]->rgbReserved, buff + BMPFILEHEADERSIZE + BMPV5HEADERSIZE + i * 4 + 3, 1);
		}
	}
	/* データ部処理 */
	body_size= info_header->bV5Height * 4 * ceil(ceil(info_header->bV5Width * (float)info_header->bV5BitCount / 8.0) / 4.0);
	bmp->body = (unsigned char *)malloc(sizeof(unsigned char) * body_size );

	if (bmp->body == NULL) {
		close_bmp(bmp);
		free(buff);
		return NULL;
	}

	memcpy(bmp->body, buff + bmp->fileHeader->bfOffBits, body_size);

	free(buff);

	return bmp;
}

static int _write_bmp_core_header(FILE *fp, BMP *bmp)
{
	/* 未実装 */
	return 0;
}

static int _write_bmp_info_header(FILE *fp, BMP *bmp)
{
	/* 未実装 */
	int i;

	int header_size;
	int body_size;
	BMPINFOHEADER *info_header;
	unsigned char buff[1024];

	if (fp == NULL) return 1;
	if (bmp == NULL) return 1;
	if (bmp->bmpType != BMPINFOHEADERSIZE ) return 1;

	info_header = (BMPINFOHEADER *)bmp->infoHeader;

	header_size = BMPFILEHEADERSIZE + BMPINFOHEADERSIZE;
	body_size= info_header->bcHeight * 4 * ceil(ceil(info_header->bcWidth * (float)info_header->bcBitCount / 8.0) / 4.0);

	/* BMPヘッダーをバッファーにコピー&書き込み */
	memcpy(buff +                   + 0  , &bmp->fileHeader->bfType      , 2);
	memcpy(buff +                   + 2  , &bmp->fileHeader->bfSize      , 4);
	memcpy(buff +                   + 6  , &bmp->fileHeader->bfReserved1 , 2);
	memcpy(buff +                   + 8  , &bmp->fileHeader->bfReserved2 , 2);
	memcpy(buff +                   + 10 , &bmp->fileHeader->bfOffBits   , 4);
	memcpy(buff + BMPFILEHEADERSIZE + 0  , &info_header->bcSize         , 4);
	memcpy(buff + BMPFILEHEADERSIZE + 4  , &info_header->bcWidth        , 4);
	memcpy(buff + BMPFILEHEADERSIZE + 8  , &info_header->bcHeight       , 4);
	memcpy(buff + BMPFILEHEADERSIZE + 12 , &info_header->bcPlanes       , 2);
	memcpy(buff + BMPFILEHEADERSIZE + 14 , &info_header->bcBitCount     , 2);
	memcpy(buff + BMPFILEHEADERSIZE + 16 , &info_header->biCompression  , 4);
	memcpy(buff + BMPFILEHEADERSIZE + 20 , &info_header->biSizeImage    , 4);
	memcpy(buff + BMPFILEHEADERSIZE + 24 , &info_header->biXPixPerMeter , 4);
	memcpy(buff + BMPFILEHEADERSIZE + 28 , &info_header->biYPixPerMeter , 4);
	memcpy(buff + BMPFILEHEADERSIZE + 32 , &info_header->biClrUsed      , 4);
	memcpy(buff + BMPFILEHEADERSIZE + 36 , &info_header->biCirImportant , 4);

	fwrite(buff, header_size, 1, fp);

	/* カラーパレット書き込み */
	if(info_header->bcBitCount != BMP24 && info_header->bcBitCount != BMP32 ) {
		for(i=0; i < (info_header->biClrUsed? info_header->biClrUsed: 1<<info_header->bcBitCount); i++)
		{
			buff[i*4  ] = bmp->pallets[i]->rgbBlue;
			buff[i*4+1] = bmp->pallets[i]->rgbGreen;
			buff[i*4+2] = bmp->pallets[i]->rgbRed;
			buff[i*4+3] = bmp->pallets[i]->rgbReserved;
		}
		fwrite(buff, info_header->biClrUsed ? info_header->biClrUsed: 1<<info_header->bcBitCount, 4, fp);


	}

	/* BMPデータ部書き込み */
	fwrite(bmp->body, body_size, 1, fp);

	return 0;
}

static int _write_bmp_info2_header(FILE *fp, BMP *bmp)
{
	/* 未実装 */
	return 0;
}

static int _write_bmp_v4_header(FILE *fp, BMP *bmp)
{

	int i;
	int header_size;
	int body_size;
	BMPV4HEADER *info_header;
	unsigned char buff[1024];

	if (fp == NULL) return 1;
	if (bmp == NULL) return 1;
	if (bmp->bmpType != BMPV4HEADERSIZE ) return 1;

	info_header = (BMPV4HEADER *)bmp->infoHeader;

	header_size = BMPFILEHEADERSIZE + BMPV4HEADERSIZE;
	body_size= info_header->bV4Height * 4 * ceil(ceil(info_header->bV4Width * (float)info_header->bV4BitCount / 8.0) / 4.0);

	/* BMPヘッダーをバッファーにコピー&書き込み */
	memcpy(buff +                   + 0  , &bmp->fileHeader->bfType      , 2);
	memcpy(buff +                   + 2  , &bmp->fileHeader->bfSize      , 4);
	memcpy(buff +                   + 6  , &bmp->fileHeader->bfReserved1 , 2);
	memcpy(buff +                   + 8  , &bmp->fileHeader->bfReserved2 , 2);
	memcpy(buff +                   + 10 , &bmp->fileHeader->bfOffBits   , 4);
	memcpy(buff + BMPFILEHEADERSIZE + 0  , &info_header->bV4Size         , 4);
	memcpy(buff + BMPFILEHEADERSIZE + 4  , &info_header->bV4Width        , 4);
	memcpy(buff + BMPFILEHEADERSIZE + 8  , &info_header->bV4Height       , 4);
	memcpy(buff + BMPFILEHEADERSIZE + 12 , &info_header->bV4Planes       , 4);
	memcpy(buff + BMPFILEHEADERSIZE + 14 , &info_header->bV4BitCount     , 4);
	memcpy(buff + BMPFILEHEADERSIZE + 16 , &info_header->bV4Compression  , 4);
	memcpy(buff + BMPFILEHEADERSIZE + 20 , &info_header->bV4SizeImage    , 4);
	memcpy(buff + BMPFILEHEADERSIZE + 24 , &info_header->bV4XPelsPerMeter, 4);
	memcpy(buff + BMPFILEHEADERSIZE + 28 , &info_header->bV4YPelsPerMeter, 4);
	memcpy(buff + BMPFILEHEADERSIZE + 32 , &info_header->bV4ClrUsed      , 4);
	memcpy(buff + BMPFILEHEADERSIZE + 36 , &info_header->bV4ClrImportant , 4);
	memcpy(buff + BMPFILEHEADERSIZE + 40 , &info_header->bV4RedMask      , 4);
	memcpy(buff + BMPFILEHEADERSIZE + 44 , &info_header->bV4GreenMask    , 4);
	memcpy(buff + BMPFILEHEADERSIZE + 48 , &info_header->bV4BlueMask     , 4);
	memcpy(buff + BMPFILEHEADERSIZE + 52 , &info_header->bV4AlphaMask    , 4);
	memcpy(buff + BMPFILEHEADERSIZE + 56 , &info_header->bV4CSType       , 4);
	memcpy(buff + BMPFILEHEADERSIZE + 60 , &info_header->bV4EndpointRX   , 4);
	memcpy(buff + BMPFILEHEADERSIZE + 64 , &info_header->bV4EndpointRY   , 4);
	memcpy(buff + BMPFILEHEADERSIZE + 68 , &info_header->bV4EndpointRZ   , 4);
	memcpy(buff + BMPFILEHEADERSIZE + 72 , &info_header->bV4EndpointGX   , 4);
	memcpy(buff + BMPFILEHEADERSIZE + 76 , &info_header->bV4EndpointGY   , 4);
	memcpy(buff + BMPFILEHEADERSIZE + 80 , &info_header->bV4EndpointGZ   , 4);
	memcpy(buff + BMPFILEHEADERSIZE + 84 , &info_header->bV4EndpointBX   , 4);
	memcpy(buff + BMPFILEHEADERSIZE + 88 , &info_header->bV4EndpointBY   , 4);
	memcpy(buff + BMPFILEHEADERSIZE + 92 , &info_header->bV4EndpointBZ   , 4);
	memcpy(buff + BMPFILEHEADERSIZE + 96 , &info_header->bV4GammaRed     , 4);
	memcpy(buff + BMPFILEHEADERSIZE + 100, &info_header->bV4GammaGreen   , 4);
	memcpy(buff + BMPFILEHEADERSIZE + 104, &info_header->bV4GammaBlue    , 4);


	fwrite(buff, header_size, 1, fp);

	/* カラーパレット書き込み */
	if(info_header->bV4BitCount != BMP24 && info_header->bV4BitCount != BMP32 ) {
		for(i=0; i < (info_header->bV4ClrUsed ? info_header->bV4ClrUsed: 1<<info_header->bV4BitCount); i++)
		{
			buff[i*4  ] = bmp->pallets[i]->rgbBlue;
			buff[i*4+1] = bmp->pallets[i]->rgbGreen;
			buff[i*4+2] = bmp->pallets[i]->rgbRed;
			buff[i*4+3] = bmp->pallets[i]->rgbReserved;
		}

		fwrite(buff, (info_header->bV4ClrUsed ? info_header->bV4ClrUsed: 1<<info_header->bV4BitCount), 4, fp);

	}

	/* BMPデータ部書き込み */
	fwrite(bmp->body, body_size, 1, fp);

	return 0;
}

static int _write_bmp_v5_header(FILE *fp, BMP *bmp)
{
	int i;
	int header_size;
	int body_size;
	BMPV5HEADER *info_header;
	unsigned char buff[1024];

	if (fp == NULL) return 1;
	if (bmp == NULL) return 1;
	if (bmp->bmpType != BMPV5HEADERSIZE ) return 1;

	info_header = (BMPV5HEADER *)bmp->infoHeader;

	header_size = BMPFILEHEADERSIZE + BMPV5HEADERSIZE;
	body_size= info_header->bV5Height * 4 * ceil(ceil(info_header->bV5Width * (float)info_header->bV5BitCount / 8.0) / 4.0);

	/* BMPヘッダーをバッファーにコピー&書き込み */
	memcpy(buff +                   + 0  , &bmp->fileHeader->bfType      , 2);
	memcpy(buff +                   + 2  , &bmp->fileHeader->bfSize      , 4);
	memcpy(buff +                   + 6  , &bmp->fileHeader->bfReserved1 , 2);
	memcpy(buff +                   + 8  , &bmp->fileHeader->bfReserved2 , 2);
	memcpy(buff +                   + 10 , &bmp->fileHeader->bfOffBits   , 4);
	memcpy(buff + BMPFILEHEADERSIZE + 0  , &info_header->bV5Size         , 4);
	memcpy(buff + BMPFILEHEADERSIZE + 4  , &info_header->bV5Width        , 4);
	memcpy(buff + BMPFILEHEADERSIZE + 8  , &info_header->bV5Height       , 4);
	memcpy(buff + BMPFILEHEADERSIZE + 12 , &info_header->bV5Planes       , 2);
	memcpy(buff + BMPFILEHEADERSIZE + 14 , &info_header->bV5BitCount     , 2);
	memcpy(buff + BMPFILEHEADERSIZE + 16 , &info_header->bV5Compression  , 4);
	memcpy(buff + BMPFILEHEADERSIZE + 20 , &info_header->bV5SizeImage    , 4);
	memcpy(buff + BMPFILEHEADERSIZE + 24 , &info_header->bV5XPelsPerMeter, 4);
	memcpy(buff + BMPFILEHEADERSIZE + 28 , &info_header->bV5YPelsPerMeter, 4);
	memcpy(buff + BMPFILEHEADERSIZE + 32 , &info_header->bV5ClrUsed      , 4);
	memcpy(buff + BMPFILEHEADERSIZE + 36 , &info_header->bV5ClrImportant , 4);
	memcpy(buff + BMPFILEHEADERSIZE + 40 , &info_header->bV5RedMask      , 4);
	memcpy(buff + BMPFILEHEADERSIZE + 44 , &info_header->bV5GreenMask    , 4);
	memcpy(buff + BMPFILEHEADERSIZE + 48 , &info_header->bV5BlueMask     , 4);
	memcpy(buff + BMPFILEHEADERSIZE + 52 , &info_header->bV5AlphaMask    , 4);
	memcpy(buff + BMPFILEHEADERSIZE + 56 , &info_header->bV5CSType       , 4);
	memcpy(buff + BMPFILEHEADERSIZE + 60 , &info_header->bV5EndpointRX   , 4);
	memcpy(buff + BMPFILEHEADERSIZE + 64 , &info_header->bV5EndpointRY   , 4);
	memcpy(buff + BMPFILEHEADERSIZE + 68 , &info_header->bV5EndpointRZ   , 4);
	memcpy(buff + BMPFILEHEADERSIZE + 72 , &info_header->bV5EndpointGX   , 4);
	memcpy(buff + BMPFILEHEADERSIZE + 76 , &info_header->bV5EndpointGY   , 4);
	memcpy(buff + BMPFILEHEADERSIZE + 80 , &info_header->bV5EndpointGZ   , 4);
	memcpy(buff + BMPFILEHEADERSIZE + 84 , &info_header->bV5EndpointBX   , 4);
	memcpy(buff + BMPFILEHEADERSIZE + 88 , &info_header->bV5EndpointBY   , 4);
	memcpy(buff + BMPFILEHEADERSIZE + 92 , &info_header->bV5EndpointBZ   , 4);
	memcpy(buff + BMPFILEHEADERSIZE + 96 , &info_header->bV5GammaRed     , 4);
	memcpy(buff + BMPFILEHEADERSIZE + 100, &info_header->bV5GammaGreen   , 4);
	memcpy(buff + BMPFILEHEADERSIZE + 104, &info_header->bV5GammaBlue    , 4);
	memcpy(buff + BMPFILEHEADERSIZE + 108, &info_header->bV5Intent       , 4);
	memcpy(buff + BMPFILEHEADERSIZE + 112, &info_header->bV5ProfileData  , 4);
	memcpy(buff + BMPFILEHEADERSIZE + 116, &info_header->bV5ProfileSize  , 4);
	memcpy(buff + BMPFILEHEADERSIZE + 120, &info_header->bV5Reserved     , 4);


	fwrite(buff, header_size, 1, fp);

	/* カラーパレット書き込み */
	if(info_header->bV5BitCount != BMP24 && info_header->bV5BitCount != BMP32 ) {
		for(i=0; i < (info_header->bV5ClrUsed ? info_header->bV5ClrUsed: 1<<info_header->bV5BitCount); i++)
		{
			buff[i*4  ] = bmp->pallets[i]->rgbBlue;
			buff[i*4+1] = bmp->pallets[i]->rgbGreen;
			buff[i*4+2] = bmp->pallets[i]->rgbRed;
			buff[i*4+3] = bmp->pallets[i]->rgbReserved;
		}

		fwrite(buff, (info_header->bV5ClrUsed ? info_header->bV5ClrUsed: 1<<info_header->bV5BitCount), 4, fp);

	}

	/* BMPデータ部書き込み */
	fwrite(bmp->body, body_size, 1, fp);

	return 0;
}
