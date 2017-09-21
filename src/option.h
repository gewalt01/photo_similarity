//--------------------------------------------------------------------------------
//      ファイル名      :       option.h
//      概要            :       コマンドライン引数を使いやすい形に扱う
//--------------------------------------------------------------------------------
#ifndef INCLUDE_GUARD_7495D813_A96A_4384_910B_4E74307EFB9F
#define INCLUDE_GUARD_7495D813_A96A_4384_910B_4E74307EFB9F

#include "hashtable.h"

#define PARSEOPTMAXBUFF 0x7EFF
#define PARSEOPTERRORHYPHEN -1
#define PARSEOPTERRORDOUBLEHYPHEN -2
#define PARSEOPTWORSEVALUE -10

typedef struct {
	int state;
	HASHTABLE *param;
	HASHTABLE *flag;
} OPTION;

OPTION *new_option();

int free_option(OPTION *);

int get_option_flag(char *, OPTION *);

int set_option_flag(char *, OPTION *);

int del_option_flag(char *, OPTION *);

char *get_option_param(char *, OPTION *);

int set_option_param(char *, char *, OPTION *);

int del_option_param(char *, OPTION *);

int get_option_state(OPTION *);

int set_option_state(int, OPTION *);

OPTION *parse_option(int , char**);

#endif
