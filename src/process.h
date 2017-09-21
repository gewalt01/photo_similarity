//================================================================================
//      FILE
//--------------------------------------------------------------------------------
//      ファイル名      :       process.h
//      概要            :       実際の計算処理を司る。MVCのM?
//--------------------------------------------------------------------------------
#ifndef INCLUDE_GUARD_222A9EB2_CB7B_4E06_9882_81DB5150C222
#define INCLUDE_GUARD_222A9EB2_CB7B_4E06_9882_81DB5150C222

#include <stdio.h>
#include <math.h>
#include "hashtable.h"
#include "image.h"
#include "option.h"
#include "hist.h"

#define HELPMODE 1
#define VERSIONMODE 2

#define NOTPARAMF 101
#define DBFILENOTFOUND 102

typedef struct {
	int state;
	int length;
	HASHTABLE *files;
	HASHTABLE *similfileno;
	HASHTABLE *similars;
} RESULT;

RESULT *new_result();

int free_result(RESULT *);

RESULT *process(OPTION *);

char *get_result_filename(int, RESULT *);

int push_result_filename(char *, RESULT *);

double get_result_similaly(int, RESULT *);

int set_result_similaly(int, double, RESULT *);

int get_result_length(RESULT *);

int get_result_state(RESULT *);

int set_result_state(int, RESULT *);

#endif
