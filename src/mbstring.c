//================================================================================
//      FILE
//--------------------------------------------------------------------------------
//      ファイル名      :       mbstring.c
//      概要            :       マルチバイト文字を扱うライブラリ
//--------------------------------------------------------------------------------

#include "mbstring.h"


static int push_value_unique(int *, int *, int);


//----------------------------------------------------------------------------
// 関数名       ：count_chars_utf8
// 概要         ：UTF-8の文字数を返す
// 戻り値       ：int
// 引数         ：unsigned char *str
//              UTF-8文字列
//----------------------------------------------------------------------------
int count_chars_utf8(unsigned char *str)
{
	/* FIXME:サロゲートペア非対応。そのうち修正。<-やるやるって言っていつやるんですか？ */
	int i;
	int j;
	int len;
	int state;
	int count;
	unsigned int unichar;

	len = strlen(str);
	state = 0;
	count = 0;

	for(i=0; i<len; i++)
	{
		switch(state){
		case 0:
			if ( 0xC0 <= str[i] && str[i] <= 0xDF ) state = 1; // UTF8_2B1OCT
			if ( 0xE0 <= str[i] && str[i] <= 0xEF ) state = 2; // UTF8_3B1OCT
			if ( 0xF0 <= str[i] && str[i] <= 0xFF ) state = 4; // UTF8_4B1OCT
			if ( str[i] <= 0x7E ) state = 524288; // UTF8_ASCII

			break;

		case UTF8_2B1OCT:
			if ( 0x80 <= str[i] && str[i] <= 0xBF ) state = 128; // UTF8_2B2OCT

			break;

		case UTF8_3B1OCT:
			if ( 0x80 <= str[i] && str[i] <= 0xBF ) state = 256; // UTF8_3B2OCT

			break;

		case UTF8_4B1OCT:
			if ( 0x80 <= str[i] && str[i] <= 0xBF ) state = 512; // UTF8_4B2OCT

			break;

		case UTF8_2B2OCT:
			if ( 0xC0 <= str[i] && str[i] <= 0xDF ) state = 1; // UTF8_2B1OCT
			if ( 0xE0 <= str[i] && str[i] <= 0xEF ) state = 2; // UTF8_3B1OCT
			if ( 0xF0 <= str[i] && str[i] <= 0xFF ) state = 4; // UTF8_4B1OCT
			if ( str[i] <= 0x7E ) state = 524288; // UTF8_ASCII

			break;

		case UTF8_3B2OCT:
			if ( 0x80 <= str[i] && str[i] <= 0xBF ) state = 16384; // UTF8_3B3OCT

			break;

		case UTF8_4B2OCT:
			if ( 0x80 <= str[i] && str[i] <= 0xBF ) state = 32768; // UTF8_4B3OCT

			break;

		case UTF8_3B3OCT:
			if ( 0xC0 <= str[i] && str[i] <= 0xDF ) state = 1; // UTF8_2B1OCT
			if ( 0xE0 <= str[i] && str[i] <= 0xEF ) state = 2; // UTF8_3B1OCT
			if ( 0xF0 <= str[i] && str[i] <= 0xFF ) state = 4; // UTF8_4B1OCT
			if ( str[i] <= 0x7E ) state = 524288; // UTF8_ASCII

			break;

		case UTF8_4B3OCT:
			if ( 0x80 <= str[i] && str[i] <= 0xBF ) state = 131072; // UTF8_4B4OCT

			break;

		case UTF8_4B4OCT:
			if ( 0xC0 <= str[i] && str[i] <= 0xDF ) state = 1; // UTF8_2B1OCT
			if ( 0xE0 <= str[i] && str[i] <= 0xEF ) state = 2; // UTF8_3B1OCT
			if ( 0xF0 <= str[i] && str[i] <= 0xFF ) state = 4; // UTF8_4B1OCT
			if ( str[i] <= 0x7E ) state = 524288; // UTF8_ASCII

			break;

		case UTF8_ASCII:
			if ( 0xC0 <= str[i] && str[i] <= 0xDF ) state = 1; // UTF8_2B1OCT
			if ( 0xE0 <= str[i] && str[i] <= 0xEF ) state = 2; // UTF8_3B1OCT
			if ( 0xF0 <= str[i] && str[i] <= 0xFF ) state = 4; // UTF8_4B1OCT
			if ( str[i] <= 0x7E ) state = 524288; // UTF8_ASCII

			break;
		}

		unichar = 0;

		switch(state) {
		case UTF8_2B2OCT:
			count++;
			break;

		case UTF8_3B3OCT:
			count++;
			break;

		case UTF8_4B4OCT:
			count++;
			break;

		case UTF8_ASCII:
			count++;
			break;

		}
	}

	return count;

}

//----------------------------------------------------------------------------
// 関数名       ：count_chars_eucjp
// 概要         ：EUC-JPの文字数を返す
// 戻り値       ：int
// 引数         ：unsigned char *str
//               EUC-JP文字列
//----------------------------------------------------------------------------
int count_chars_eucjp(unsigned char *str)
{
	int i;
	int j;
	int len;
	int state;
	int count;

	len = strlen(str);
	state = 0;
	count = 0;

	for(i=0; i<len; i++)
	{
		switch(state){
		case 0:
			if ( 0xA1 <= str[i] && str[i] <= 0xFE ) state = 8; // EUCJP_KANJI1OCT
			if ( str[i] == 0x8E ) state = 16; // EUCJP_KANAHOJO1OCT
			if ( str[i] <= 0x7E ) state = 1048576; // EUCJP_ASCII

			break;

		case EUCJP_KANJI1OCT:
			if ( 0xA1 <= str[i] && str[i] <= 0xFE ) state = 1024; // EUCJP_KANJI2OCT

			break;

		case EUCJP_KANAHOJO1OCT:
			if( 0xE0 <= str[i] && str[i] <= 0xFE ) {
				state = 4096; // EUCJP_HOJO2OCT
				break;
			}

			for(j=i; j<len; j++)
			{
				if ( str[j] <= 0x7E || str[j] == 0x8E ) break;
			}

			if ( (j-i) % 2 == 1) {
				state = 2048; // EUCJP_KANA2OCT
			} else {
				state = 4096; // EUCJP_HOJO2OCT
			}

			break;

		case EUCJP_KANJI2OCT:
			if ( 0xA1 <= str[i] && str[i] <= 0xFE ) state = 8; // EUCJP_KANJI1OCT
			if ( str[i] == 0x8E ) state = 16; // EUCJP_KANAHOJO1OCT
			if ( str[i] <= 0x7E ) state = 1048576; // EUCJP_ASCII

			break;

		case EUCJP_KANA2OCT:
			if ( 0xA1 <= str[i] && str[i] <= 0xFE ) state = 8; // EUCJP_KANJI1OCT
			if ( str[i] == 0x8E ) state = 16; // EUCJP_KANAHOJO1OCT
			if ( str[i] <= 0x7E ) state = 1048576; // EUCJP_ASCII

			break;

		case EUCJP_HOJO2OCT:
			if ( 0xA1 <= str[i] && str[i] <= 0xFE ) state = 65536; // EUCJP_HOJO3OCT

			break;

		case EUCJP_HOJO3OCT:
			if ( 0xA1 <= str[i] && str[i] <= 0xFE ) state = 8; // EUCJP_KANJI1OCT
			if ( str[i] == 0x8E ) state = 16; // EUCJP_KANAHOJO1OCT
			if ( str[i] <= 0x7E ) state = 1048576; // EUCJP_ASCII

			break;

		case EUCJP_ASCII:
			if ( 0xA1 <= str[i] && str[i] <= 0xFE ) state = 8; // EUCJP_KANJI1OCT
			if ( str[i] == 0x8E ) state = 16; // EUCJP_KANAHOJO1OCT
			if ( str[i] <= 0x7E ) state = 1048576; // EUCJP_ASCII

			break;
		}

		switch(state) {
		case EUCJP_KANJI2OCT:
			count++;
			break;

		case EUCJP_KANA2OCT:
			count++;
			break;

		case EUCJP_HOJO3OCT:
			count++;
			break;

		case EUCJP_ASCII:
			count++;
			break;
		}
	}

	return count;
}

//----------------------------------------------------------------------------
// 関数名       ：count_chars_sjis
// 概要         ：Shift_JISの文字数を返す
// 戻り値       ：int
// 引数         ：unsigned char *str
//               Shift_JIS文字列
//----------------------------------------------------------------------------
int count_chars_sjis(unsigned char *str)
{
	int i;
	int len;
	int state;
	int count;

	len = strlen(str);
	state = 0;
	count = 0;

	for(i=0; i<len; i++)
	{
		switch(state){
		case 0:
			if ( (0x81 <= str[i] && str[i] <= 0x9F) || (0xE0 <= str[i] && str[i] <= 0xEF) ) state = 64; // SJIS_1OCT
			if ( 0xA1 <= str[i] && str[i] <= 0xDF ) state = 4194304; // SJIS_KANA
			if ( str[i] <= 0x7E ) state = 2097152; // SJIS_ASCII

			break;

		case SJIS_1OCT:
			if ( (0x40 <= str[i] && str[i] <= 0x7E) || (0x80 <= str[i] && str[i] <= 0xFC) ) state = 8192; // SJIS_2OCT

			break;

		case SJIS_2OCT:
			if ( (0x81 <= str[i] && str[i] <= 0x9F) || (0xE0 <= str[i] && str[i] <= 0xEF) ) state = 64; // SJIS_1OCT
			if ( 0xA1 <= str[i] && str[i] <= 0xDF ) state = 4194304; // SJIS_KANA
			if ( str[i] <= 0x7E ) state = 2097152; // SJIS_ASCII

			break;

		case SJIS_KANA:
			if ( (0x81 <= str[i] && str[i] <= 0x9F) || (0xE0 <= str[i] && str[i] <= 0xEF) ) state = 64; // SJIS_1OCT
			if ( 0xA1 <= str[i] && str[i] <= 0xDF ) state = 4194304; // SJIS_KANA
			if ( str[i] <= 0x7E ) state = 2097152; // SJIS_ASCII

			break;

		case SJIS_ASCII:
			if ( (0x81 <= str[i] && str[i] <= 0x9F) || (0xE0 <= str[i] && str[i] <= 0xEF) ) state = 64; // SJIS_1OCT
			if ( 0xA1 <= str[i] && str[i] <= 0xDF ) state = 4194304; // SJIS_KANA
			if ( str[i] <= 0x7E ) state = 2097152; // SJIS_ASCII

			break;
		}

		switch(state) {
		case SJIS_KANA:
			count++;
			break;

		case SJIS_2OCT:
			count++;
			break;

		case SJIS_ASCII:
			count++;
			break;
		}
	}

	return count;
}


//----------------------------------------------------------------------------
// 関数名       ：count_halfwidth_chars_utf8
// 概要         ：UTF-8の半角文字数を返す
// 戻り値       ：int
// 引数         ：unsigned char *str
//               UTF-8文字列
//----------------------------------------------------------------------------
int count_halfwidth_chars_utf8(unsigned char *str)
{
	/* FIXME:サロゲートペア非対応。そのうち修正。<-やるやるって言っていつやるんですか？ */
	int i;
	int j;
	int len;
	int state;
	int count;
	unsigned int unichar;

	len = strlen(str);
	state = 0;
	count = 0;

	for(i=0; i<len; i++)
	{
		switch(state){
		case 0:
			if ( 0xC0 <= str[i] && str[i] <= 0xDF ) state = 1; // UTF8_2B1OCT
			if ( 0xE0 <= str[i] && str[i] <= 0xEF ) state = 2; // UTF8_3B1OCT
			if ( 0xF0 <= str[i] && str[i] <= 0xFF ) state = 4; // UTF8_4B1OCT
			if ( str[i] <= 0x7E ) state = 524288; // UTF8_ASCII

			break;

		case UTF8_2B1OCT:
			if ( 0x80 <= str[i] && str[i] <= 0xBF ) state = 128; // UTF8_2B2OCT

			break;

		case UTF8_3B1OCT:
			if ( 0x80 <= str[i] && str[i] <= 0xBF ) state = 256; // UTF8_3B2OCT

			break;

		case UTF8_4B1OCT:
			if ( 0x80 <= str[i] && str[i] <= 0xBF ) state = 512; // UTF8_4B2OCT

			break;

		case UTF8_2B2OCT:
			if ( 0xC0 <= str[i] && str[i] <= 0xDF ) state = 1; // UTF8_2B1OCT
			if ( 0xE0 <= str[i] && str[i] <= 0xEF ) state = 2; // UTF8_3B1OCT
			if ( 0xF0 <= str[i] && str[i] <= 0xFF ) state = 4; // UTF8_4B1OCT
			if ( str[i] <= 0x7E ) state = 524288; // UTF8_ASCII

			break;

		case UTF8_3B2OCT:
			if ( 0x80 <= str[i] && str[i] <= 0xBF ) state = 16384; // UTF8_3B3OCT

			break;

		case UTF8_4B2OCT:
			if ( 0x80 <= str[i] && str[i] <= 0xBF ) state = 32768; // UTF8_4B3OCT

			break;

		case UTF8_3B3OCT:
			if ( 0xC0 <= str[i] && str[i] <= 0xDF ) state = 1; // UTF8_2B1OCT
			if ( 0xE0 <= str[i] && str[i] <= 0xEF ) state = 2; // UTF8_3B1OCT
			if ( 0xF0 <= str[i] && str[i] <= 0xFF ) state = 4; // UTF8_4B1OCT
			if ( str[i] <= 0x7E ) state = 524288; // UTF8_ASCII

			break;

		case UTF8_4B3OCT:
			if ( 0x80 <= str[i] && str[i] <= 0xBF ) state = 131072; // UTF8_4B4OCT

			break;

		case UTF8_4B4OCT:
			if ( 0xC0 <= str[i] && str[i] <= 0xDF ) state = 1; // UTF8_2B1OCT
			if ( 0xE0 <= str[i] && str[i] <= 0xEF ) state = 2; // UTF8_3B1OCT
			if ( 0xF0 <= str[i] && str[i] <= 0xFF ) state = 4; // UTF8_4B1OCT
			if ( str[i] <= 0x7E ) state = 524288; // UTF8_ASCII

			break;

		case UTF8_ASCII:
			if ( 0xC0 <= str[i] && str[i] <= 0xDF ) state = 1; // UTF8_2B1OCT
			if ( 0xE0 <= str[i] && str[i] <= 0xEF ) state = 2; // UTF8_3B1OCT
			if ( 0xF0 <= str[i] && str[i] <= 0xFF ) state = 4; // UTF8_4B1OCT
			if ( str[i] <= 0x7E ) state = 524288; // UTF8_ASCII

			break;
		}

		unichar = 0;

		switch(state) {
		case UTF8_2B2OCT:
			unichar = (str[i-1] & 0x1F)<<6 | (str[i] & 0x3F);
			if ( unichar <= 0x7E || ( 0xFF61 <= unichar && unichar <= 0xFFDC) || ( 0xFFE8 <= unichar && unichar <= 0xFFEE) ) count++;
			break;

		case UTF8_3B3OCT:
			unichar = (str[i-2] & 0xF)<<12 | (str[i-1] & 0x3F)<<6 | (str[i] & 0x3F);
			if ( unichar <= 0x7E || ( 0xFF61 <= unichar && unichar <= 0xFFDC) || ( 0xFFE8 <= unichar && unichar <= 0xFFEE) ) count++;
			break;

		case UTF8_4B4OCT:
			unichar = (str[i-3] & 0x7)<<18 | (str[i-2] & 0x3F)<<12 | (str[i-1] & 0xBF)<<6 | (str[i] & 0x3F);
			if ( unichar <= 0x7E || ( 0xFF61 <= unichar && unichar <= 0xFFDC) || ( 0xFFE8 <= unichar && unichar <= 0xFFEE) ) count++;
			break;

		case UTF8_ASCII:
			unichar = str[i];
			if ( unichar <= 0x7E ) count++;
			break;

		}
	}

	return count;

}

//----------------------------------------------------------------------------
// 関数名       ：count_halfwidth_chars_eucjp
// 概要         ：EUC-JPの半角文字数を返す
// 戻り値       ：int
// 引数         ：unsigned char *str
//               EUC-JP文字列
//----------------------------------------------------------------------------
int count_halfwidth_chars_eucjp(unsigned char *str)
{
	int i;
	int j;
	int len;
	int state;
	int count;

	len = strlen(str);
	state = 0;
	count = 0;

	for(i=0; i<len; i++)
	{
		switch(state){
		case 0:
			if ( 0xA1 <= str[i] && str[i] <= 0xFE ) state = 8; // EUCJP_KANJI1OCT
			if ( str[i] == 0x8E ) state = 16; // EUCJP_KANAHOJO1OCT
			if ( str[i] <= 0x7E ) state = 1048576; // EUCJP_ASCII

			break;

		case EUCJP_KANJI1OCT:
			if ( 0xA1 <= str[i] && str[i] <= 0xFE ) state = 1024; // EUCJP_KANJI2OCT

			break;

		case EUCJP_KANAHOJO1OCT:
			if( 0xE0 <= str[i] && str[i] <= 0xFE ) {
				state = 4096; // EUCJP_HOJO2OCT
				break;
			}

			for(j=i; j<len; j++)
			{
				if ( str[j] <= 0x7E || str[j] == 0x8E ) break;
			}

			if ( (j-i) % 2 == 1) {
				state = 2048; // EUCJP_KANA2OCT
			} else {
				state = 4096; // EUCJP_HOJO2OCT
			}

			break;

		case EUCJP_KANJI2OCT:
			if ( 0xA1 <= str[i] && str[i] <= 0xFE ) state = 8; // EUCJP_KANJI1OCT
			if ( str[i] == 0x8E ) state = 16; // EUCJP_KANAHOJO1OCT
			if ( str[i] <= 0x7E ) state = 1048576; // EUCJP_ASCII

			break;

		case EUCJP_KANA2OCT:
			if ( 0xA1 <= str[i] && str[i] <= 0xFE ) state = 8; // EUCJP_KANJI1OCT
			if ( str[i] == 0x8E ) state = 16; // EUCJP_KANAHOJO1OCT
			if ( str[i] <= 0x7E ) state = 1048576; // EUCJP_ASCII

			break;

		case EUCJP_HOJO2OCT:
			if ( 0xA1 <= str[i] && str[i] <= 0xFE ) state = 65536; // EUCJP_HOJO3OCT

			break;

		case EUCJP_HOJO3OCT:
			if ( 0xA1 <= str[i] && str[i] <= 0xFE ) state = 8; // EUCJP_KANJI1OCT
			if ( str[i] == 0x8E ) state = 16; // EUCJP_KANAHOJO1OCT
			if ( str[i] <= 0x7E ) state = 1048576; // EUCJP_ASCII

			break;

		case EUCJP_ASCII:
			if ( 0xA1 <= str[i] && str[i] <= 0xFE ) state = 8; // EUCJP_KANJI1OCT
			if ( str[i] == 0x8E ) state = 16; // EUCJP_KANAHOJO1OCT
			if ( str[i] <= 0x7E ) state = 1048576; // EUCJP_ASCII

			break;
		}

		switch(state) {
		case EUCJP_KANA2OCT:
			count++;
			break;

		case EUCJP_ASCII:
			count++;
			break;
		}
	}

	return count;
}

//----------------------------------------------------------------------------
// 関数名       ：count_halfwidth_chars_sjis
// 概要         ：Shitf_JISの半角文字数を返す
// 戻り値       ：int
// 引数         ：unsigned char *str
//               Shift_JIS文字列
//----------------------------------------------------------------------------
int count_halfwidth_chars_sjis(unsigned char *str)
{
	int i;
	int len;
	int state;
	int count;

	len = strlen(str);
	state = 0;
	count = 0;

	for(i=0; i<len; i++)
	{
		switch(state){
		case 0:
			if ( (0x81 <= str[i] && str[i] <= 0x9F) || (0xE0 <= str[i] && str[i] <= 0xEF) ) state = 64; // SJIS_1OCT
			if ( 0xA1 <= str[i] && str[i] <= 0xDF ) state = 4194304; // SJIS_KANA
			if ( str[i] <= 0x7E ) state = 2097152; // SJIS_ASCII

			break;

		case SJIS_1OCT:
			if ( (0x40 <= str[i] && str[i] <= 0x7E) || (0x80 <= str[i] && str[i] <= 0xFC) ) state = 8192; // SJIS_2OCT

			break;

		case SJIS_2OCT:
			if ( (0x81 <= str[i] && str[i] <= 0x9F) || (0xE0 <= str[i] && str[i] <= 0xEF) ) state = 64; // SJIS_1OCT
			if ( 0xA1 <= str[i] && str[i] <= 0xDF ) state = 4194304; // SJIS_KANA
			if ( str[i] <= 0x7E ) state = 2097152; // SJIS_ASCII

			break;

		case SJIS_KANA:
			if ( (0x81 <= str[i] && str[i] <= 0x9F) || (0xE0 <= str[i] && str[i] <= 0xEF) ) state = 64; // SJIS_1OCT
			if ( 0xA1 <= str[i] && str[i] <= 0xDF ) state = 4194304; // SJIS_KANA
			if ( str[i] <= 0x7E ) state = 2097152; // SJIS_ASCII

			break;

		case SJIS_ASCII:
			if ( (0x81 <= str[i] && str[i] <= 0x9F) || (0xE0 <= str[i] && str[i] <= 0xEF) ) state = 64; // SJIS_1OCT
			if ( 0xA1 <= str[i] && str[i] <= 0xDF ) state = 4194304; // SJIS_KANA
			if ( str[i] <= 0x7E ) state = 2097152; // SJIS_ASCII

			break;
		}

		switch(state) {
		case SJIS_KANA:
			count++;
			break;

		case SJIS_ASCII:
			count++;
			break;
		}
	}

	return count;
}



//----------------------------------------------------------------------------
// 関数名       ：encoding
// 概要         ：NFA(のようなもの)を用いた文字コードの判定
//                判定結果を定数で返す。
// 戻り値       ：int
// 引数         ：unsigned char *str
//               入力文字列
//----------------------------------------------------------------------------
int encoding(unsigned char *str)
{
		// XXX: Shift_JISとEUC-JPの判定には無理がある(特にShift_JIS半角カタカナとEUC-JPの漢字の判別)
        int count = 0;
        int tmpcount;
        int set[256];
        int tmpset[256];
        int state;
        int i;

        unsigned char *tmpsptr;
        tmpsptr = str;

        count = 1;
        set[0] = 0;

        do{

                tmpcount = 0;

                for(i = 0; i < count; i++)
                {
                        switch(set[i]){
                        case 0:
                                if ( 0xC0 <= *tmpsptr && *tmpsptr <= 0xDF ) push_value_unique(tmpset, &tmpcount, 1); // UTF8_2B1OCT
                                if ( 0xE0 <= *tmpsptr && *tmpsptr <= 0xEF ) push_value_unique(tmpset, &tmpcount, 2); // UTF8_3B1OCT
                                if ( 0xF0 <= *tmpsptr && *tmpsptr <= 0xFF ) push_value_unique(tmpset, &tmpcount, 4); // UTF8_4B1OCT
                                if ( 0xA1 <= *tmpsptr && *tmpsptr <= 0xFE ) push_value_unique(tmpset, &tmpcount, 8); // EUCJP_KANJI1OCT
                                if ( *tmpsptr == 0x8E ) push_value_unique(tmpset, &tmpcount, 16); // EUCJP_KANAHOJO1OCT
                                if ( (0x81 <= *tmpsptr && *tmpsptr <= 0x9F) || (0xE0 <= *tmpsptr && *tmpsptr <= 0xEF) ) push_value_unique(tmpset, &tmpcount, 64); // SJIS_1OCT
                                if ( 0xA1 <= *tmpsptr && *tmpsptr <= 0xDF ) push_value_unique(tmpset, &tmpcount, 4194304); // SJIS_KANA

                                if ( (0x00 <= *tmpsptr && *tmpsptr <= 0x7F) ) push_value_unique(tmpset, &tmpcount, 262144); // ASCII
                                break;

                        case ASCII:
                                if ( 0xC0 <= *tmpsptr && *tmpsptr <= 0xDF ) push_value_unique(tmpset, &tmpcount, 1); // UTF8_2B1OCT
                                if ( 0xE0 <= *tmpsptr && *tmpsptr <= 0xEF ) push_value_unique(tmpset, &tmpcount, 2); // UTF8_3B1OCT
                                if ( 0xF0 <= *tmpsptr && *tmpsptr <= 0xFF ) push_value_unique(tmpset, &tmpcount, 4); // UTF8_4B1OCT
                                if ( 0xA1 <= *tmpsptr && *tmpsptr <= 0xFE ) push_value_unique(tmpset, &tmpcount, 8); // EUCJP_KANJI1OCT
                                if ( *tmpsptr == 0x8E ) push_value_unique(tmpset, &tmpcount, 16); // EUCJP_KANA1OCT
                                if ( *tmpsptr == 0x8E ) push_value_unique(tmpset, &tmpcount, 32); // EUCJP_HOJO1OCT
                                if ( (0x81 <= *tmpsptr && *tmpsptr <= 0x9F) || (0xE0 <= *tmpsptr && *tmpsptr <= 0xEF) ) push_value_unique(tmpset, &tmpcount, 64); // SJIS_1OCT
                                if ( 0xA1 <= *tmpsptr && *tmpsptr <= 0xDF ) push_value_unique(tmpset, &tmpcount, 4194304); // SJIS_KANA

                                if ( (0x00 <= *tmpsptr && *tmpsptr <= 0x7F) ) push_value_unique(tmpset, &tmpcount, 262144); // ASCII
                                break;

                        case UTF8_ASCII:
                                if ( 0xC0 <= *tmpsptr && *tmpsptr <= 0xDF ) push_value_unique(tmpset, &tmpcount, 1); // UTF8_2B1OCT
                                if ( 0xE0 <= *tmpsptr && *tmpsptr <= 0xEF ) push_value_unique(tmpset, &tmpcount, 2); // UTF8_3B1OCT
                                if ( 0xF0 <= *tmpsptr && *tmpsptr <= 0xFF ) push_value_unique(tmpset, &tmpcount, 4); // UTF8_4B1OCT

                                if ( (0x00 <= *tmpsptr && *tmpsptr <= 0x7F) ) push_value_unique(tmpset, &tmpcount, 524288); // UTF8_ASCII
                                break;

                        case EUCJP_ASCII:
                                if ( 0xA1 <= *tmpsptr && *tmpsptr <= 0xFE ) push_value_unique(tmpset, &tmpcount, 8); // EUCJP_KANJI1OCT
                                if ( *tmpsptr == 0x8E ) push_value_unique(tmpset, &tmpcount, 16); // EUCJP_KANA1OCT
                                if ( *tmpsptr == 0x8E ) push_value_unique(tmpset, &tmpcount, 32); // EUCJP_HOJO1OCT


                                if ( (0x00 <= *tmpsptr && *tmpsptr <= 0x7F) ) push_value_unique(tmpset, &tmpcount, 1048576); // EUCJP_ASCII
                                break;

                        case SJIS_ASCII:
                                if ( (0x81 <= *tmpsptr && 0x9F <= *tmpsptr) || (0xE0 <= *tmpsptr && *tmpsptr <= 0xEF) ) push_value_unique(tmpset, &tmpcount, 64); // SJIS_1OCT
                                if ( 0xA1 <= *tmpsptr && *tmpsptr <= 0xDF ) push_value_unique(tmpset, &tmpcount, 4194304); // SJIS_KANA

                                if ( (0x00 <= *tmpsptr && *tmpsptr <= 0x7F) ) push_value_unique(tmpset, &tmpcount, 2097152); // SJIS_ASCII
                                break;

                        case SJIS_KANA:
                                if ( (0x81 <= *tmpsptr && 0x9F <= *tmpsptr) || (0xE0 <= *tmpsptr && *tmpsptr <= 0xEF) ) push_value_unique(tmpset, &tmpcount, 64); // SJIS_1OCT
                                if ( 0xA1 <= *tmpsptr && *tmpsptr <= 0xDF ) push_value_unique(tmpset, &tmpcount, 4194304); // SJIS_KANA

                                if ( (0x00 <= *tmpsptr && *tmpsptr <= 0x7F) ) push_value_unique(tmpset, &tmpcount, 2097152); // SJIS_ASCII
                                break;

                        case UTF8_2B1OCT:
                                if ( 0x80 <= *tmpsptr && *tmpsptr <= 0xBF ) push_value_unique(tmpset, &tmpcount, 128); // UTF8_2B2OCT

                                break;

                        case UTF8_3B1OCT:
                                if ( 0x80 <= *tmpsptr && *tmpsptr <= 0xBF ) push_value_unique(tmpset, &tmpcount, 256); // UTF8_3B2OCT

                                break;

                        case UTF8_4B1OCT:
                                if ( 0x80 <= *tmpsptr && *tmpsptr <= 0xBF ) push_value_unique(tmpset, &tmpcount, 512); // UTF8_4B2OCT

                                break;

                        case EUCJP_KANJI1OCT:
                                if ( 0xA1 <= *tmpsptr && *tmpsptr <= 0xFE ) push_value_unique(tmpset, &tmpcount, 1024); // EUCJP_KANJI2OCT

                                break;

                        case EUCJP_KANAHOJO1OCT:
                                if ( 0xA1 <= *tmpsptr && *tmpsptr <= 0xDF ) push_value_unique(tmpset, &tmpcount, 2048); // EUCJP_KANA2OCT
                                if ( 0xA1 <= *tmpsptr && *tmpsptr <= 0xFE ) push_value_unique(tmpset, &tmpcount, 4096); // EUCJP_HOJO2OCT

                                break;

                        case SJIS_1OCT:
                                if ( (0x40 <= *tmpsptr && 0x7E <= *tmpsptr) || (0x80 <= *tmpsptr && *tmpsptr <= 0xFC) ) push_value_unique(tmpset, &tmpcount, 8192); // SJIS_2OCT

                                break;


                        case UTF8_2B2OCT:
                                if ( 0xC0 <= *tmpsptr && *tmpsptr <= 0xDF ) push_value_unique(tmpset, &tmpcount, 1); // UTF8_2B1OCT
                                if ( 0xE0 <= *tmpsptr && *tmpsptr <= 0xEF ) push_value_unique(tmpset, &tmpcount, 2); // UTF8_3B1OCT
                                if ( 0xF0 <= *tmpsptr && *tmpsptr <= 0xFF ) push_value_unique(tmpset, &tmpcount, 4); // UTF8_4B1OCT

                                if ( (0x00 <= *tmpsptr && *tmpsptr <= 0x7F) ) push_value_unique(tmpset, &tmpcount, 524288); // UTF8_ASCII

                                break;

                        case UTF8_3B2OCT:
                                if ( 0x80 <= *tmpsptr && *tmpsptr <= 0xBF ) push_value_unique(tmpset, &tmpcount, 16384); // UTF8_3B3OCT
                                break;

                        case UTF8_4B2OCT:
                                if ( 0x80 <= *tmpsptr && *tmpsptr <= 0xBF ) push_value_unique(tmpset, &tmpcount, 32768); // UTF8_4B3OCT
                                break;

                        case EUCJP_KANJI2OCT:
                                if ( 0xA1 <= *tmpsptr && *tmpsptr <= 0xFE ) push_value_unique(tmpset, &tmpcount, 8); // EUCJP_KANJI1OCT
                                if ( *tmpsptr == 0x8E ) push_value_unique(tmpset, &tmpcount, 16); // EUCJP_KANA1OCT
                                if ( *tmpsptr == 0x8E ) push_value_unique(tmpset, &tmpcount, 32); // EUCJP_HOJO1OCT


                                if ( (0x00 <= *tmpsptr && *tmpsptr <= 0x7F) ) push_value_unique(tmpset, &tmpcount, 1048576); // EUCJP_ASCII
                                break;

                        case EUCJP_KANA2OCT:
                                if ( 0xA1 <= *tmpsptr && *tmpsptr <= 0xFE ) push_value_unique(tmpset, &tmpcount, 8); // EUCJP_KANJI1OCT
                                if ( *tmpsptr == 0x8E ) push_value_unique(tmpset, &tmpcount, 16); // EUCJP_KANA1OCT
                                if ( *tmpsptr == 0x8E ) push_value_unique(tmpset, &tmpcount, 32); // EUCJP_HOJO1OCT

                                if ( (0x00 <= *tmpsptr && *tmpsptr <= 0x7F) ) push_value_unique(tmpset, &tmpcount, 1048576); // EUCJP_ASCII
                                break;

                        case EUCJP_HOJO2OCT:
                                if ( *tmpsptr == 0x8E ) push_value_unique(tmpset, &tmpcount, 65536); // EUCJP_HOJO3OCT
                                break;

                        case SJIS_2OCT:
                                if ( (0x81 <= *tmpsptr && 0x9F <= *tmpsptr) || (0xE0 <= *tmpsptr && *tmpsptr <= 0xEF) ) push_value_unique(tmpset, &tmpcount, 64); // SJIS_1OCT
                                if ( 0xA1 <= *tmpsptr && *tmpsptr <= 0xDF ) push_value_unique(tmpset, &tmpcount, 4194304); // SJIS_KANA

                                if ( (0x00 <= *tmpsptr && *tmpsptr <= 0x7F) ) push_value_unique(tmpset, &tmpcount, 2097152); // SJIS_ASCII
                                break;


                        case UTF8_3B3OCT:
                                if ( 0xC0 <= *tmpsptr && *tmpsptr <= 0xDF ) push_value_unique(tmpset, &tmpcount, 1); // UTF8_2B1OCT
                                if ( 0xE0 <= *tmpsptr && *tmpsptr <= 0xEF ) push_value_unique(tmpset, &tmpcount, 2); // UTF8_3B1OCT
                                if ( 0xF0 <= *tmpsptr && *tmpsptr <= 0xFF ) push_value_unique(tmpset, &tmpcount, 4); // UTF8_4B1OCT

                                if ( (0x00 <= *tmpsptr && *tmpsptr <= 0x7F) ) push_value_unique(tmpset, &tmpcount, 524288); // UTF8_ASCII
                                break;

                        case UTF8_4B3OCT:
                                if ( 0x80 <= *tmpsptr && *tmpsptr <= 0xBF ) push_value_unique(tmpset, &tmpcount, 131072); // UTF8_4B4OCT
                                break;

                        case EUCJP_HOJO3OCT:
                                if ( 0xA1 <= *tmpsptr && *tmpsptr <= 0xFE ) push_value_unique(tmpset, &tmpcount, 8); // EUCJP_KANJI1OCT
                                if ( *tmpsptr == 0x8E ) push_value_unique(tmpset, &tmpcount, 16); // EUCJP_KANA1OCT
                                if ( *tmpsptr == 0x8E ) push_value_unique(tmpset, &tmpcount, 32); // EUCJP_HOJO1OCT

                                if ( (0x00 <= *tmpsptr && *tmpsptr <= 0x7F) ) push_value_unique(tmpset, &tmpcount, 1048576); // EUCJP_ASCII
                                break;


                        case UTF8_4B4OCT:
                                if ( 0xC0 <= *tmpsptr && *tmpsptr <= 0xDF ) push_value_unique(tmpset, &tmpcount, 1); // UTF8_2B1OCT
                                if ( 0xE0 <= *tmpsptr && *tmpsptr <= 0xEF ) push_value_unique(tmpset, &tmpcount, 2); // UTF8_3B1OCT
                                if ( 0xF0 <= *tmpsptr && *tmpsptr <= 0xFF ) push_value_unique(tmpset, &tmpcount, 4); // UTF8_4B1OCT

                                if ( (0x00 <= *tmpsptr && *tmpsptr <= 0x7F) ) push_value_unique(tmpset, &tmpcount, 524288); // UTF8_ASCII
                                break;

                        }
                }



				count = tmpcount;
				memcpy(set, tmpset, sizeof(int) * tmpcount);

				if (count == 1 && set[0] != ASCII) return set[0];


        } while(*tmpsptr++);

        return 0;
}


//----------------------------------------------------------------------------
// 関数名       ：push_value_unique
// 概要         ：NFA(のようなもの)の状態集合に状態(っぽい何か)を追加する
// 戻り値       ：int
// 引数         ：int *set
//               状態集合
// 引数         ：int *count
//               状態集合の要素数
// 引数         ：int value
//               追加する状態
//----------------------------------------------------------------------------
static int push_value_unique(int *set, int *count, int value)
{
        int i;

        for(i = 0; i < (*count); i++)
        {
                if (set[i] == value) return 0;
        }

        set[i] = value;
        (*count)++;

        return 0;
}
