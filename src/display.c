//================================================================================
//      FILE
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//      ファイル名      :       display.c
//      概要            :       データの出力を司る。MVCのV?
//--------------------------------------------------------------------------------
#include "display.h"

static void _help();

static void _version();

//----------------------------------------------------------------------------
// 関数名       ：display
// 概要         ：データの出力
// 戻り値       ：int
// 引数         ：RESULT *result
//               processの実行結果
// 引数         ：OPTION *option
//               コマンドライン引数データ
//----------------------------------------------------------------------------
int display(RESULT *result, OPTION *option)
{
	//int i;
	//int r_length;

	// HELP表示
	if ( get_result_state(result) == HELPMODE) {
		_help();
		return 0;
	}

	// Version表示
	if ( get_result_state(result) == VERSIONMODE) {
		_version();
		return 0;
	}

	// -f, --fileパラメータなし
	if ( get_result_state(result) == NOTPARAMF) {
		puts("Error:");
		puts("  Need parameter \"-f\" or \"--file\"");
		puts("");
		puts("--- Help ---");
		_help();
		return 0;
	}

	// DBファイルがない
	if ( get_result_state(result) == DBFILENOTFOUND) {
		printf("Error:\n");
		printf("  File \"%s\" is not exist.\n", get_option_param("f", option));
		return 0;
	}

	// Version表示
	if ( get_result_state(result) == VERSIONMODE) {
		_version();
		return 0;
	}


	if (get_option_flag("o", option) || get_option_flag("output", option)) {
		display_html(result, option);
	} else {
		display_console(result, option);
	}


	return 0;
}

//----------------------------------------------------------------------------
// 関数名       ：_help
// 概要         ：自明
// 戻り値       ：void
// 引数         ：void
//----------------------------------------------------------------------------
static void _help()
{
	/* TODO:help */
	puts("Options:");
	puts("  -f listfile, --file=listfile               ");
	puts("  -c colorlevel, --colorlevel=colorlevel     ");
	puts("                                       reduce color   ");
	puts("  -0, --output                         output HTML file");
	puts("  -v, --version                        show version   ");
	puts("  -h, --help                           show help      ");

	puts("");

	puts("Color level:");
	puts("  1: 3bit color");
	puts("  2: 6bit color");
	puts("  3: 12bit color");
	puts("  4: 24bit color [default]");
	puts("  5: 1bit gray scale");
	puts("  6: 2bit gray scale");
	puts("  7: 4bit gray scale");
	puts("  8: 8bit gray scale");

	puts("");

	puts("For Example:");
	puts("  #main -f \"files.lst\"");
	puts("  #main --file=\"files.lst\" -c 1");
	puts("  #main --help");

}

//----------------------------------------------------------------------------
// 関数名       ：_version
// 概要         ：自明
// 戻り値       ：void
// 引数         ：void
//----------------------------------------------------------------------------
static void _version()
{
	puts("                                 |\\    /|       ");
	puts("                              ___| \\,,/_/       ");
	puts("                           ---__/ \\/    \\       ");
	puts("                          __--/     (D)  \\      ");
	puts("                          _ -/    (_      \\     ");
	puts("                         // /       \\_ / ==\\    ");
	puts("   __-------_____--___--/           / \\_ O o)   ");
	puts("  /            C 3                  /   \\==/`   ");
	puts(" /          Version 0.0.0          /            ");
	puts("||          )                   \\_/\\            ");
	puts("||         /              _      /  |           ");
	puts("| |      /--______      ___\\    /\\  :           ");
	puts("| /   __-  - _/   ------    |  |   \\ \\          ");
	puts(" |   -  -   /                | |     \\ )        ");
	puts(" |  |   -  |                 | )     | |        ");
	puts("  | |    | |                 | |    | |         ");
	puts("  | |    < |                 | |   |_/          ");
	puts("  < |    /__\\                <  \\               ");
	puts("  /__\\                       /___\\              ");
}

