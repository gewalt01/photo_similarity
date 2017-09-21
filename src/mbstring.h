//--------------------------------------------------------------------------------
//      ファイル名      :       mbstring.h
//      概要            :       マルチバイト文字を扱うライブラリ
//--------------------------------------------------------------------------------


#ifndef INCLUDE_GUARD_CACD6414_DF7F_11E3_9144_FBB3C8EE1ADA
#define INCLUDE_GUARD_CACD6414_DF7F_11E3_9144_FBB3C8EE1ADA

#include <stdio.h>
#include <string.h>


#define ENCODINGUTF8 1
#define ENCODINGEUCJP 2
#define ENCODINGSJIS 4


#define UTF8_2B1OCT 1
#define UTF8_3B1OCT 2
#define UTF8_4B1OCT 4
#define EUCJP_KANJI1OCT 8
#define EUCJP_KANAHOJO1OCT 16
#define SJIS_1OCT 64

#define UTF8_2B2OCT 128
#define UTF8_3B2OCT 256
#define UTF8_4B2OCT 512
#define EUCJP_KANJI2OCT 1024
#define EUCJP_KANA2OCT 2048
#define EUCJP_HOJO2OCT 4096
#define SJIS_2OCT 8192

#define UTF8_3B3OCT 16384
#define UTF8_4B3OCT 32768
#define EUCJP_HOJO3OCT 65536

#define UTF8_4B4OCT 131072

#define ASCII 262144
#define UTF8_ASCII 524288
#define EUCJP_ASCII 1048576
#define SJIS_ASCII 2097152
#define SJIS_KANA 4194304

int count_chars_utf8(unsigned char *);

int count_chars_eucjp(unsigned char *);

int count_chars_sjis(unsigned char *);

int count_halfwidth_chars_utf8(unsigned char *);

int count_halfwidth_chars_eucjp(unsigned char *);

int count_halfwidth_chars_sjis(unsigned char *);

int encoding(unsigned char *);

#endif
