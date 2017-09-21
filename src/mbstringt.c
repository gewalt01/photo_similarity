//================================================================================
//      FILE
//--------------------------------------------------------------------------------
//      ファイル名      :       mbstringt.c
//      概要            :       マルチバイト文字を扱うライブラリmbstringのテストツール
//--------------------------------------------------------------------------------
#include <stdio.h>
#include "mbstring.h"

int main(void)
{
	FILE *fp1;
	FILE *fp2;
	FILE *fp3;

	char buff[1024];

	fp1 = fopen("utf8.txt", "r");
	fp2 = fopen("eucjp.txt", "r");
	fp3 = fopen("sjis.txt", "r");

	//utf8
	puts("---UTF-8---");
	fgets(buff, 1000, fp1);
	printf("%d\n", encoding(buff));
	printf("%d\n", count_halfwidth_chars_utf8(buff));


	//eucjp
	puts("---EUC_JP---");
	fgets(buff, 1000, fp2);
	printf("%d\n", encoding(buff));
	printf("%d\n", count_halfwidth_chars_eucjp(buff));

	//sjis	
	puts("---Shift_JIS---");
	fgets(buff, 1000, fp3);
	printf("%d\n", encoding(buff));
	printf("%d\n", count_halfwidth_chars_sjis(buff));

	return 0;
}
