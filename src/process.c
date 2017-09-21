//================================================================================
//      FILE
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//      ファイル名      :       process.c
//      概要            :       実際の計算処理を司る。MVCのM?
//--------------------------------------------------------------------------------
#include "process.h"
#include <math.h>


static HIST *_c3img2hist(C3IMG *);

static int _get_option_colorlevel(OPTION *);

//----------------------------------------------------------------------------
// 関数名       ：new_result
// 概要         ：データの出力
// 戻り値       ：RESULT *
// 引数         ：void
//----------------------------------------------------------------------------
RESULT *new_result()
{
	RESULT *result;

	result = (RESULT *)malloc(sizeof(RESULT));
	if (result == NULL) return NULL;

	result->state = 0;
	result->length = 0;
	result->files = new_hashtable(127);
	result->similfileno= new_hashtable(127);
	result->similars= new_hashtable(127);
	if(result->files == NULL || result->similfileno == NULL || result->similars == NULL) {
		free_result(result);
	}

	return result;
}

//----------------------------------------------------------------------------
// 関数名       ：free_result
// 概要         ：RESULTデータ領域開放
// 戻り値       ：int
// 引数         ：RESULT *result
//                RESULTデータ
//----------------------------------------------------------------------------
int free_result(RESULT *result)
{
	del_hashtable(result->files);
	del_hashtable(result->similfileno);
	del_hashtable(result->similars);
	free(result);

	return 0;
}

//----------------------------------------------------------------------------
// 関数名       ：process
// 概要         ：実際の計算処理を行う。
//                重要。かなり重要。
// 戻り値       ：RESULT *
// 引数         ：OPTION *option
//                OPTIONデータ
//----------------------------------------------------------------------------
RESULT *process(OPTION *option)
{
	RESULT *result;
	FILE *fp1;
	FILE *fp2;
	C3IMG *img1;
	C3IMG *img2;
	HIST *hist1;
	HIST *hist2;
	char buff[0x7FFF];
	int i;
	int j;
	int length;
	double tmpsimil;
	int collv;

	result = new_result();
	if (result == NULL) return NULL;

	//ヘルプ表示
	if (get_option_flag("help", option) || get_option_flag("h", option)) {
		set_result_state(HELPMODE, result);
		return result;
	}

	//バージョン表示
	if (get_option_flag("version", option) || get_option_flag("v", option)) {
		set_result_state(VERSIONMODE, result);
		return result;
	}

	//DBファイル指定なし
	if (get_option_param("f", option) == NULL) {
		set_result_state(NOTPARAMF, result);
		return result;
	}

	//カラーレベル設定
	collv = _get_option_colorlevel(option);

	/*
	 * 本題の類似画像判定処理
	 */

	//ファイル一覧処理
	fp1 = fopen(get_option_param("f", option), "r");
	if (fp1 == NULL) {
		set_result_state(DBFILENOTFOUND, result);
		return result;
	}

	while(fscanf(fp1, "%s", buff) != EOF)
	{
		fp2 = fopen(buff, "r");
		if (fp2 == NULL) {
			continue;
		}

		push_result_filename(buff, result);
		fclose(fp2);
	}
	fclose(fp1);

	// TODO: 非核処理
	length = get_result_length(result);
	for(i=0; i<length-1; i++)
	{
		fp1 = fopen(get_result_filename(i, result), "r");
		img1 = read_image(fp1);
		fclose(fp1);
		//TODO: 減色処理 とりあえず版
		posterize_c3img(img1, collv);
		hist1 = _c3img2hist(img1);
		delete_c3img(img1);

		for(j=i+1; j<length; j++)
		{
			// FIXME: 非対応ファイル読み込み時の動作は？
			fp2 = fopen(get_result_filename(j, result), "r");
			img2 = read_image(fp2);
			fclose(fp2);
			//TODO: 減色処理 とりあえず版
			posterize_c3img(img2, collv);
			hist2 = _c3img2hist(img2);
			delete_c3img(img2);
			tmpsimil = similaly(hist1, hist2);
			free_histgram(hist2);
			/* :TODO:このへんでメモリが壊れている */
			if ( get_result_similaly(i, result) < tmpsimil ) { // 比較対象1の最類似データ更新が比較対象2に更新？
				set_result_similfileno(i, j, result);
				set_result_similaly(i, tmpsimil, result);
			}
			if ( get_result_similaly(j, result) < tmpsimil ) { // 比較対象2の最類似データ更新が比較対象1に更新？
				set_result_similfileno(j, i, result); //FIXME: ここでメモリが壊れている臭い
				set_result_similaly(j, tmpsimil, result);//FIXME: ここでメモリが壊れている臭い
			}
		}
		free_histgram(hist1);

	}

	return result;
}

//--------------------------------------------------------------------------------
// 関数名       ：get_result_filename
// 概要         ：計算結果データから対象計算ファイル名を得る。
// 戻り値       ：char *
// 引数         ：int n
//                ファイル番号
// 引数         ：RESULT *result
//                RESULTデータ
//----------------------------------------------------------------------------
char *get_result_filename(int n, RESULT *result)
{
	char buff[64];

	snprintf(buff, 60, "%d", n);

	return get_value_str(buff, result->files);
}

//--------------------------------------------------------------------------------
// 関数名       ：push_result_filename
// 概要         ：計算結果データへ計算対象ファイルを追加する
// 戻り値       ：int
// 引数         ：char *fname
//                ファイル名
// 引数         ：RESULT *result
//                RESULTデータ
//----------------------------------------------------------------------------
int push_result_filename(char *fname, RESULT *result)
{
	char buff[64];

	snprintf(buff, 60, "%d", result->length);
	set_key_str(buff, fname, result->files);
	result->length += 1;

	return 0;
}

//--------------------------------------------------------------------------------
// 関数名       ：get_result_similfileno
// 概要         ：計算結果データから画像番号に対応した類似画像の番号を返す
// 戻り値       ：int
// 引数         ：int n
//                ファイル番号
// 引数         ：RESULT *result
//                RESULTデータ
//----------------------------------------------------------------------------
int get_result_similfileno(int n, RESULT *result)
{
	char buff[64];

	snprintf(buff, 60, "%d", n);

	return get_value_int(buff, result->similfileno);

}

//--------------------------------------------------------------------------------
// 関数名       ：set_result_similfileno
// 概要         ：計算結果データへ設定する画像ファイル番号を類似画像番号をアカヅケする
// 戻り値       ：int
// 引数         ：int n
//                ファイル番号
// 引数         ：int no
//                類似画像ファイル番号
// 引数         ：RESULT *result
//                RESULTデータ
//----------------------------------------------------------------------------
int set_result_similfileno(int n, int no, RESULT *result)
{
	char buff[64];

	snprintf(buff, 60, "%d", n);
	set_key_int(buff, no, result->similfileno);

	return 0;
}


//--------------------------------------------------------------------------------
// 関数名       ：get_result_similaly
// 概要         ：計算結果データからファイル番号の類似度を取得する
// 戻り値       ：double
// 引数         ：int n
//                ファイル番号
// 引数         ：RESULT *result
//                RESULTデータ
//----------------------------------------------------------------------------
double get_result_similaly(int n, RESULT *result)
{
	char buff[64];

	snprintf(buff, 60, "%d", n);

	return get_value_double(buff, result->similars);
}


//--------------------------------------------------------------------------------
// 関数名       ：set_result_similaly
// 概要         ：計算結果データへファイル番号の類似度を設定する
// 戻り値       ：int
// 引数         ：int n
//                ファイル番号
// 引数         ：double simil
//                類似度
// 引数         ：RESULT *result
//                RESULTデータ
//----------------------------------------------------------------------------
int set_result_similaly(int n, double simil, RESULT *result)
{
	char buff[64];

	snprintf(buff, 60, "%d", n);
	set_key_double(buff, simil, result->similars);

	return 0;
}

//--------------------------------------------------------------------------------
// 関数名       ：get_result_length
// 概要         ：計算ファイル数を返す
// 戻り値       ：int
// 引数         ：RESULT *result
//                RESULTデータ
//----------------------------------------------------------------------------
int get_result_length(RESULT *result)
{
	return result->length;
}


//--------------------------------------------------------------------------------
// 関数名       ：get_result_state
// 概要         ：計算結果の状態を返す
// 戻り値       ：int
// 引数         ：RESULT *result
//                RESULTデータ
//----------------------------------------------------------------------------
int get_result_state(RESULT *result)
{
	return result->state;
}


//--------------------------------------------------------------------------------
// 関数名       ：set_result_state
// 概要         ：計算結果の状態を設定する
// 戻り値       ：int
// 引数         ：int state
//                状態
// 引数         ：RESULT *result
//                RESULTデータ
//----------------------------------------------------------------------------
int set_result_state(int state, RESULT *result)
{
	result->state = state;
	return 0;
}


//--------------------------------------------------------------------------------
// 関数名       ：_c3img2hist
// 概要         ：C3IMG抽象化レイヤー画像データをヒストグラム化する
// 戻り値       ：HIST *
// 引数         ：C3IMG *img
//                C3IMG抽象化レイヤー画像データ
//----------------------------------------------------------------------------
static HIST *_c3img2hist(C3IMG *img)
{
	HIST *hist;
	int *ar;
	int i;
	int vect;
	int bit;

	switch(img->level){
	case C3IMGCOLOR3BIT:
		bit = 3;
		break;

	case C3IMGCOLOR6BIT:
		bit = 6;
		break;

	case C3IMGCOLOR12BIT:
		bit = 12;
		break;

	case C3IMGCOLOR24BIT:
		bit = 24;
		break;

	case C3IMGCOLORGRAY1BIT:
		bit = 1;
		break;

	case C3IMGCOLORGRAY2BIT:
		bit = 2;
		break;

	case C3IMGCOLORGRAY4BIT:
		bit = 4;
		break;

	case C3IMGCOLORGRAY8BIT:
		bit = 8;
		break;

	}

	switch(img->type){
	case C3IMGCOLOR:
		ar = (int *)calloc(sizeof(int), (int)pow(2, bit));
		if (ar == NULL) return NULL;

		for(i=0; i<get_c3img_width(img)*get_c3img_height(img); i++)
		{
			/*
			 * FIXME: -c 1でバグ発生
			 */
			vect = (int)pow(2, (bit/3)*2) * img->r[i]/(bit/3) + (int)pow(2, bit/3) * img->g[i]/(bit/3) + img->b[i]/(bit/3);
			ar[vect]++;
		}

		hist = create_histgram_i((int)pow(2, bit), ar);

		break;

	case C3IMGGRAY:
		ar = (int *)calloc(sizeof(int), (int)pow(2, bit));
		if (ar == NULL) return NULL;

		for(i=0; i<get_c3img_width(img)*get_c3img_height(img); i++)
		{
			vect = ((img->r[i] + img->g[i] + img->b[i]) / 3)/(bit);
			ar[vect]++;
		}

		hist = create_histgram_i((int)pow(2, bit), ar);
		break;
	}

	free(ar);


	if(hist == NULL) return NULL;

	return hist;
}

//--------------------------------------------------------------------------------
// 関数名       ：__get_option_colorlevel
// 概要         ：コマンドライン引数から入力したカラーレベルを取得する
// 戻り値       ：int *
// 引数         ：OPTION *
//                OPTIONデータ
//----------------------------------------------------------------------------
static int _get_option_colorlevel(OPTION *option)
{
	char *sptr;
	int collv;

	sptr = get_option_param("colorlevel", option);
	if (sptr) {
		collv = atoi(sptr);
		switch(collv) {
		case 1:
			return C3IMGCOLOR3BIT;
			break;

		case 2:
			return C3IMGCOLOR6BIT;
			break;

		case 3:
			return C3IMGCOLOR12BIT;
			break;

		case 4:
			return C3IMGCOLOR24BIT;
			break;

		case 5:
			return C3IMGCOLORGRAY1BIT;
			break;

		case 6:
			return C3IMGCOLORGRAY2BIT;
			break;

		case 7:
			return C3IMGCOLORGRAY4BIT;
			break;

		case 8:
			return C3IMGCOLORGRAY8BIT;
			break;
		}
	}

	sptr = get_option_param("c", option);
	if (sptr) {
		collv = atoi(sptr);
		switch(collv) {
		case 1:
			return C3IMGCOLOR3BIT;
			break;

		case 2:
			return C3IMGCOLOR6BIT;
			break;

		case 3:
			return C3IMGCOLOR12BIT;
			break;

		case 4:
			return C3IMGCOLOR24BIT;
			break;

		case 5:
			return C3IMGCOLORGRAY1BIT;
			break;

		case 6:
			return C3IMGCOLORGRAY2BIT;
			break;

		case 7:
			return C3IMGCOLORGRAY4BIT;
			break;

		case 8:
			return C3IMGCOLORGRAY8BIT;
			break;
		}
	}

	return C3IMGCOLOR24BIT;
}
