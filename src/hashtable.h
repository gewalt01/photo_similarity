//================================================================================
//      FILE
//--------------------------------------------------------------------------------
//      ファイル名      :       hashtable.h
//      概要            :       ハッシュテーブルを扱うライブラリ
//                              チェイン法を用いた連想配列の実装
//--------------------------------------------------------------------------------
#ifndef INCLUDE_GUARD_6014849A_DF7F_11E3_8FDB_F7683AEE5C32
#define INCLUDE_GUARD_6014849A_DF7F_11E3_8FDB_F7683AEE5C32


#include <stdlib.h>
#include <string.h>

#define HASHSIZE 1024

typedef struct h_node{
	char *key;
	void *val;
	struct h_node *parent;
	struct h_node *child;
} HASHNODE;

typedef struct{
	int length;
	HASHNODE **table;
} HASHTABLE;

HASHTABLE *new_hashtable(int);

int del_hashtable(HASHTABLE *);

HASHNODE *new_node();

int del_node(HASHNODE *node);

int del_node_r(HASHNODE *node);

HASHNODE *get_node(char *, HASHTABLE *);

void *get_value(char *, HASHTABLE *);

int del_key(char *, HASHTABLE *);

int set_key(char *, void *, HASHTABLE *);

int hash(char *, HASHTABLE *);


char *get_value_str(char *, HASHTABLE *);

int set_key_str(char *, char *, HASHTABLE *);

int get_value_int(char *, HASHTABLE *);

int set_key_int(char *, int, HASHTABLE *);

double get_value_double(char *, HASHTABLE *);

int set_key_double(char *, double, HASHTABLE *);


#endif
