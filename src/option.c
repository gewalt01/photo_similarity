//================================================================================
//      FILE
//--------------------------------------------------------------------------------
//      ファイル名      :       option.c
//      概要            :       コマンドライン引数を使いやすい形に扱う
//--------------------------------------------------------------------------------
#include "option.h"

/*
 * コマンドライン引数のパーサ
 * user%./app -f -ab -p value　 --flag --PARAM=VALUE
 * のようにコマンドライン引数をとれるようにする.
 *　"f", "a", "b" "flag" : フラグ名
 *　"p", "PARAM" : パラメータ名
 *
 * 禁止入力 "-", "--"
 * フラグ名に"-"は使えない
 *
 * user%./app -f -p value worseval --flag --PARAM=VALUE
 * user%./app worseval -f -p value --flag --PARAM=VALUE
 * のような入力ではworsevalは無視する.
 *
 * 保守したくないコードだ…
 */

//文字の最初から見つかった場所までの距離を探索
static int _strchrlen(char *, int);

//----------------------------------------------------------------------------
// 関数名       ：new_option
// 概要         ：OPTION型作成
// 戻り値       ：OPTION *
// 引数         ：void
//----------------------------------------------------------------------------
OPTION *new_option()
{
	OPTION *option;

	option = (OPTION *)malloc(sizeof(OPTION));
	if (option == NULL) return NULL;

	option->state = 0;
	option->flag = new_hashtable(127);
	option->param = new_hashtable(127);
	if (option->flag == NULL || option->param == NULL) {
		free_option(option);
		return NULL;
	}

	return option;
}

//----------------------------------------------------------------------------
// 関数名       ：free_option
// 概要         ：OPTION型の領域開放
// 戻り値       ：int
// 引数         ：OPTION *option
//              OPTION型データ
//----------------------------------------------------------------------------
int free_option(OPTION *option)
{
	del_hashtable(option->flag);
	del_hashtable(option->param);
	free(option);

	return 0;
}


//----------------------------------------------------------------------------
// 関数名       ：get_option_flag
// 概要         ：フラグのゲッタ。
// 戻り値       ：int
// 引数         ：char *key
//               フラグ名
// 引数         ：OPTION *option
//               OPTION型データ
//----------------------------------------------------------------------------
int get_option_flag(char *key, OPTION *option)
{
	return get_value_int(key, option->flag);
}

//----------------------------------------------------------------------------
// 関数名       ：set_option_flag
// 概要         ：フラグのセッタ。
// 戻り値       ：int
// 引数         ：char *key
//               フラグ名
// 引数         ：OPTION *option
//               OPTION型データ
//----------------------------------------------------------------------------
int set_option_flag(char *key, OPTION *option)
{
	set_key_int(key, 1, option->flag);

	return 0;
}

//----------------------------------------------------------------------------
// 関数名       ：del_option_flag
// 概要         ：フラグ削除
// 戻り値       ：int
// 引数         ：char *key
//               フラグ名
// 引数         ：OPTION *option
//               OPTION型データ
//----------------------------------------------------------------------------
int del_option_flag(char *key, OPTION *option)
{
	del_key(key, option->flag);

	return 0;
}

//----------------------------------------------------------------------------
// 関数名       ：get_option_param
// 概要         ：パラメータのゲッタ。
// 戻り値       ：char *
// 引数         ：char *key
//               パラメータ名
// 引数         ：OPTION *option
//               OPTION型データ
//----------------------------------------------------------------------------
char *get_option_param(char *key, OPTION *option)
{
	return get_value_str(key, option->param);
}

//----------------------------------------------------------------------------
// 関数名       ：set_option_param
// 概要         ：パラメータのセッタ。
// 戻り値       ：int
// 引数         ：char *key
//               パラメータ名
// 引数         ：char *val
//               パラメータの値
// 引数         ：OPTION *option
//               OPTION型データ
//----------------------------------------------------------------------------
int set_option_param(char *key, char *val, OPTION *option)
{
	set_key_str(key, val, option->param);

	return 0;
}

//----------------------------------------------------------------------------
// 関数名       ：del_option_param
// 概要         ：パラメータ削除
// 戻り値       ：int
// 引数         ：char *key
//               パラメータ名
// 引数         ：OPTION *option
//               OPTION型データ
//----------------------------------------------------------------------------
int del_option_param(char *key, OPTION *option)
{
	del_key(key, option->param);

	return 0;
}

//----------------------------------------------------------------------------
// 関数名       ：get_option_state
// 概要         ：OPTION型状態のゲッタ
// 戻り値       ：int
// 引数         ：OPTION *option
//               OPTION型データ
//----------------------------------------------------------------------------
int get_option_state(OPTION *option)
{
	return option->state;
}

//----------------------------------------------------------------------------
// 関数名       ：set_option_state
// 概要         ：OPTION型状態のセッタ
// 戻り値       ：int
// 引数         ：int state
//               OPTION型状態
// 引数         ：OPTION *option
//               OPTION型データ
//----------------------------------------------------------------------------
int set_option_state(int state, OPTION *option)
{
	option->state = state;

	return 0;
}

//----------------------------------------------------------------------------
// 関数名       ：parse_option
// 概要         ：コマンドライン引数パーサ
// 戻り値       ：OPTION *
// 引数         ：int argc
//               自明
// 引数         ：char **argv
//               自明
//----------------------------------------------------------------------------
OPTION *parse_option(int argc, char **argv)
{
	int i;
	int j;
	int k;
	int c;
	int eq_len;
	char buff1[PARSEOPTMAXBUFF];
	char buff2[PARSEOPTMAXBUFF];
	OPTION *option;

	option = new_option();
	if (option == NULL) return NULL;

	for(i=1; i<argc; i++)
	{
		c = strlen(argv[i]);

		// "-", "--"の除外,
		if ( c == 1 ) {
			if (argv[i][0] == '-') {
				set_option_state(PARSEOPTERRORHYPHEN, option);
				return option;
			}
		}

		if ( c == 2 ) {
			if (argv[i][0] == '-' && argv[i][1] == '-') {
				set_option_state(PARSEOPTERRORDOUBLEHYPHEN, option);
				return option;
			}
		}

		if( argv[i][0] != '-' ) continue;

		/* parse */

		if (argv[i][0] == '-' && argv[i][1] == '-') { // --flag or --PARAM=FLAGの探索
			eq_len = _strchrlen(argv[i]+2, '=');
			if( eq_len != -1 ) { //--PARAM=VALUE

				strcpy(buff1, argv[i]+2);
				buff1[eq_len] = '\0';

				strcpy(buff2, argv[i]+2+eq_len+1);

				set_option_param(buff1, buff2, option);

			} else { // --flag

				strcpy(buff1, argv[i]+2);

				set_option_flag(buff1, option);

			}

		} else { //"-f" or "-p val" pr "-flg"

			for(j=1; j<c; j++)
			{
				if (argv[i][j] == '-') {
					set_option_state(PARSEOPTERRORHYPHEN, option);
					return option;
				}
			}

			if (c > 2) {
				for(j=1; j<c; j++)
				{
					buff1[0] = argv[i][j];
					buff1[1] = '\0';

					set_option_flag(buff1, option);
				}
			} else {
				if (i+1 < argc)
				{
					if (argv[i+1][0] == '-') {
						buff1[0] = argv[i][1];
						buff1[1] = '\0';

						set_option_flag(buff1, option);
					} else {
						buff1[0] = argv[i][1];
						buff1[1] = '\0';

						set_option_param(buff1, argv[i+1], option);
					}
				} else {
					buff1[0] = argv[i][1];
					buff1[1] = '\0';

					set_option_flag(buff1, option);
				}
			}

		}

	}

	return option;
}

//----------------------------------------------------------------------------
// 関数名       ：_strchrlen
// 概要         ：文字が検索文字列中で最初に出現する位置を返す。
//               文字列中にとエラーとして-1を返す
// 戻り値       ：OPTION *
// 引数         ：char *str
//               検索対象文字列
// 引数         ：int c
//               検索文字
//----------------------------------------------------------------------------
static int _strchrlen(char *str, int c)
{
	int i;
	int len;

	len = strlen(str);

	for(i=0; i<len; i++)
	{
		if (str[i] == c) return i;
	}

	return -1;
}
