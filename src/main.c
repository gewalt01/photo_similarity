//================================================================================
//      FILE
//--------------------------------------------------------------------------------
//      ファイル名      :       main.c
//      概要            :      エントリーポイント。
//--------------------------------------------------------------------------------
#include "main.h"

int main(int argc, char **argv)
{
	OPTION *option;
	RESULT *result;

	option = parse_option(argc, argv);
	if (option == NULL) exit(-1);

	result = process(option);
	if (result == NULL) exit(-1);

	display(result, option);

	return 0;
}
