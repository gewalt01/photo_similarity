//================================================================================
//      FILE
//--------------------------------------------------------------------------------
//      ファイル名      :       display_html.c
//      概要            :       データのHTMLファイルに出力する。display.cの下位モジュール。
//--------------------------------------------------------------------------------
#include "display_html.h"

//----------------------------------------------------------------------------
// 関数名       ：display_html
// 概要         ：HTMLファイルに実行結果を出力
// 戻り値       ：int
// 引数         ：RESULT *result
//                RESILTデータ
// 引数         ：OPTION *option
//                OPTIONデータ
//----------------------------------------------------------------------------
int display_html(RESULT *result, OPTION *option)
{
	/*
	 * XXX:苦肉の策でUTF-8のみ対応
	 */

	FILE *fp;
	char *fname = "out.html";
	int i;
	char buff[10];
	int tmpno;
	int fnamelen;
	double tmpsimil;
	unsigned char *tmpfname;

	//out.htmlに出力

	fp = fopen(fname, "w");
	if (fp == NULL) return -1;

	fprintf(fp, "<!DOCTYPE html>\n");
	fprintf(fp, "<html>\n");
	fprintf(fp, "<head>\n");
	fprintf(fp, "<title>%s</title>\n", fname);
	fprintf(fp, "<meta charset=\"UTF-8\">\n");
	fprintf(fp, "</head>\n");
	fprintf(fp, "<body>\n");
	fprintf(fp, "<table  border=\"1\">\n");
	fprintf(fp, "<tbody>\n");
	fprintf(fp, "<tr>\n");
	fprintf(fp, "<th>original file</th>\n");
	fprintf(fp, "<th>similar file</th>\n");
	fprintf(fp, "<th>similarity</th>\n");
	fprintf(fp, "</tr>\n");

	//実行結果
	for(i=0; i<get_result_length(result);i++)
	{
		fprintf(fp, "<tr>\n");
		tmpfname = get_result_filename(i, result);
		fprintf(fp, "<th><a href=\"%s\">%s<br><img src=\"%s\" style=\"width: auto; max-height: 160px;\"></a></th>\n", tmpfname, tmpfname, tmpfname);
		tmpno = get_result_similfileno(i, result);
		tmpfname = get_result_filename(tmpno, result);
		fprintf(fp, "<th><a href=\"%s\">%s<br><img src=\"%s\" style=\"width: auto; max-height: 160px;\"></a></th>\n", tmpfname, tmpfname, tmpfname);
		tmpsimil = get_result_similaly(i, result);
		fprintf(fp, "<th>%3.1f%%</th>\n", tmpsimil * 100);
		fprintf(fp, "</tr>\n");
	}

	fprintf(fp, "</tbody>\n");
	fprintf(fp, "</table>\n");
	fprintf(fp, "</body>\n");
	fprintf(fp, "</html>\n");

	fclose(fp);

	printf("Outputted to \"%s\"\n", fname);

	return 0;
}
