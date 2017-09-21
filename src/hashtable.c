//================================================================================
//      FILE
//--------------------------------------------------------------------------------
//      ファイル名      :       hashtable.c
//      概要            :       ハッシュテーブルを扱うライブラリ
//                              チェイン法を用いた連想配列の実装
//--------------------------------------------------------------------------------
#include "hashtable.h"


//----------------------------------------------------------------------------
// 関数名       ：new_hashtable
// 概要         ：ハッシュテーブルの作成
// 戻り値       ：HASHTABLE *
// 引数         ：int size
//                ハッシュテーブルのサイズ
//----------------------------------------------------------------------------
HASHTABLE *new_hashtable(int size)
{
        int i;
        HASHTABLE *hash_t;

        hash_t = (HASHTABLE *)malloc(sizeof(HASHTABLE));

        if (hash_t == NULL) return NULL;

        /*
         * FIXME: sizeが素数のほうがシノニムが発生しにくい。
         */
        hash_t->length = size;
        hash_t->table = (HASHNODE **)malloc(sizeof(HASHNODE *) * hash_t->length);

        if (hash_t->table == NULL) {
                free(hash_t);
                return NULL;
        }

        for(i=0; i < hash_t->length; i++)
        {
                hash_t->table[i] = NULL;
        }

        return hash_t;
}

//----------------------------------------------------------------------------
// 関数名       ：del_hashtable
// 概要         ：ハッシュテーブルの領域開放
// 戻り値       ：int
// 引数         ：HASHTABLE *hash_t
//                ハッシュテーブルデータ
//----------------------------------------------------------------------------
int del_hashtable(HASHTABLE * hash_t){
        /* 未実装 */
        int i;

        for(i = 0; i < hash_t->length; i++)
        {
                del_node_r(hash_t->table[i]);
        }

        free(hash_t);
        hash_t = NULL;

        return 0;
}

//----------------------------------------------------------------------------
// 関数名       ：new_node
// 概要         ：ハッシュテーブルノード作成
// 戻り値       ：HASHNODE *
// 引数         ：void
//----------------------------------------------------------------------------
HASHNODE *new_node()
{
        HASHNODE *node;
        node = (HASHNODE *)malloc(sizeof(HASHNODE));

        if (node == NULL) return NULL;

        node->child = NULL;
        node->key = NULL;
        node->parent = NULL;
        node->val = NULL;

        return node;
}

//----------------------------------------------------------------------------
// 関数名       ：del_node
// 概要         ：ハッシュテーブルノードの領域開放
// 戻り値       ：int
// 引数         ：HASHNODE *node
//                ハッシュテーブルノードデータ
//----------------------------------------------------------------------------
int del_node(HASHNODE *node)
{
	if (node == NULL) return -1;

	free(node->key);
	free(node->val);
	free(node);

	node = NULL;

	return 0;
}

//----------------------------------------------------------------------------
// 関数名       ：del_node_r
// 概要         ：再帰的に子孫ノードと自身の領域開放
// 戻り値       ：int
// 引数         ：HASHNODE *node
//                ハッシュテーブルノードデータ
//----------------------------------------------------------------------------
int del_node_r(HASHNODE *node)
{
	if (node == NULL) return -1;

	if(node->child != NULL) del_node_r(node->child);
	del_node(node);

	return 0;
}


//----------------------------------------------------------------------------
// 関数名       ：get_node
// 概要         ：ノードの検索
// 戻り値       ：HASHNODE *
// 引数         ：char *key
//                キー
// 引数         ：HASHTABLE *hash_t
//                ハッシュテーブルデータ
//----------------------------------------------------------------------------
HASHNODE *get_node(char *key, HASHTABLE *hash_t)
{
        /*
         * XXX:　ノードが循環していると無限ループの危険あり…かもしれない///
         */

        int tmp_key;
        HASHNODE *tmp_node;

        tmp_key = hash(key, hash_t);
        tmp_node = hash_t->table[tmp_key];

        while(tmp_node != NULL)
        {
                if ( strcmp(key, tmp_node->key) == 0 ) {
                        return tmp_node;
                } else {
                        tmp_node = tmp_node->child;
                }
        }

        return NULL;
}

//----------------------------------------------------------------------------
// 関数名       ：get_value
// 概要         ：値の取得
// 戻り値       ：void *
// 引数         ：char *key
//                キー
// 引数         ：HASHTABLE *hash_t
//                ハッシュテーブルデータ
//----------------------------------------------------------------------------
void *get_value(char *key, HASHTABLE *hash_t)
{
        HASHNODE *tmp_node;
        tmp_node = get_node(key, hash_t);

        if(tmp_node != NULL) return tmp_node->val;

        return NULL;
}

//----------------------------------------------------------------------------
// 関数名       ：del_value
// 概要         ：キーの削除
// 戻り値       ：int
// 引数         ：char *key
//                キー
// 引数         ：HASHTABLE *hash_t
//                ハッシュテーブルデータ
//----------------------------------------------------------------------------
int del_key(char *key, HASHTABLE *hash_t)
{
        HASHNODE *tmp_node;
        tmp_node = get_node(key, hash_t);

        if(tmp_node != NULL) {
                tmp_node->parent->child = tmp_node->child;
                if(tmp_node->child != NULL) tmp_node->child->parent = tmp_node->parent;
                del_node(tmp_node);
        }

        return 0;
}

//----------------------------------------------------------------------------
// 関数名       ：set_key
// 概要         ：キーの値のセット
// 戻り値       ：int
// 引数         ：char *key
//                キー
// 引数         ：void *val
//                値
// 引数         ：HASHTABLE *hash_t
//                ハッシュテーブルデータ
//----------------------------------------------------------------------------
int set_key(char *key, void *val, HASHTABLE *hash_t)
{
        int tmp_key;
        HASHNODE *tnode;
        HASHNODE *nnode;

        //キーが存在すれば上書き
        tnode = get_node(key, hash_t);
        if(tnode != NULL) {
        	free(tnode->val);
        	tnode->val = val;
        	return 0;
        }

        //キーが存在しないので新規作成
        nnode = new_node();
        nnode->key = (char *)malloc(strlen(key) + 1);
        if ( nnode->key == NULL ) return -1;
        strcpy(nnode->key, key);
        nnode->val = val;

        tmp_key = hash(key, hash_t);
        tnode = hash_t->table[tmp_key];

        if(tnode == NULL) {

        	hash_t->table[tmp_key] = nnode;

        	return 0;
        }

        //リスト探索
        while(tnode->child != NULL)
        {
        	tnode = tnode->child;
        }

        tnode->child = nnode;
        nnode->parent = tnode;


       return 0;
}

//----------------------------------------------------------------------------
// 関数名       ：hash
// 概要         ：ハッシュ関数
// 戻り値       ：void *
// 引数         ：char *key
//                キー
// 引数         ：HASHTABLE *hash_t
//                ハッシュテーブルデータ
//----------------------------------------------------------------------------
int hash(char *key, HASHTABLE *hash_t)
{
        /* XXX:　ハッシュ関数仮実装 */
        int tmp_key;
        tmp_key = (key[0] * 5 * 11 * 21 * 23) % hash_t->length;

        return tmp_key;
}

//----------------------------------------------------------------------------
// 関数名       ：get_value_str
// 概要         ：get_valueの文字列用
// 戻り値       ：char *
// 引数         ：char *key
//                キー
// 引数         ：HASHTABLE *hash_t
//                ハッシュテーブルデータ
//----------------------------------------------------------------------------
char *get_value_str(char *key, HASHTABLE *hash_t)
{
	char *tmpstr;
	tmpstr = (char *) get_value(key, hash_t);
	return tmpstr;
}

//----------------------------------------------------------------------------
// 関数名       ：set_key_str
// 概要         ：set_keyの文字列用
// 戻り値       ：int
// 引数         ：char *key
//                キー
// 引数         ：double val
//                値
// 引数         ：HASHTABLE *hash_t
//                ハッシュテーブルデータ
//----------------------------------------------------------------------------
int set_key_str(char *key, char *val, HASHTABLE *hash_t)
{
	int len;
	char *str;

	len = strlen(val);
	str = (char *)malloc(sizeof(char) * len + 1);

	if (str == NULL) return -1;
	strcpy(str, val);

	return set_key(key, str, hash_t);
}

//----------------------------------------------------------------------------
// 関数名       ：get_value_int
// 概要         ：get_valueのint型用
// 戻り値       ：int
// 引数         ：char *key
//                キー
// 引数         ：HASHTABLE *hash_t
//                ハッシュテーブルデータ
//----------------------------------------------------------------------------
int get_value_int(char *key, HASHTABLE *hash_t)
{
	int *intptr;
	intptr = (int *) get_value(key, hash_t);
	return (intptr == NULL) ? 0: *intptr;
}

//----------------------------------------------------------------------------
// 関数名       ：set_key_int
// 概要         ：set_keyのint型用
// 戻り値       ：int
// 引数         ：char *key
//                キー
// 引数         ：int val
//                値
// 引数         ：HASHTABLE *hash_t
//                ハッシュテーブルデータ
//----------------------------------------------------------------------------
int set_key_int(char *key, int val, HASHTABLE *hash_t)
{
	int *n;

	n = (int *)malloc(sizeof(int));

	if (n == NULL) return -1;

	*n = val;

	return set_key(key, n, hash_t);
}

//----------------------------------------------------------------------------
// 関数名       ：get_value_double
// 概要         ：get_valueのdouble型用
// 戻り値       ：double
// 引数         ：char *key
//                キー
// 引数         ：HASHTABLE *hash_t
//                ハッシュテーブルデータ
//----------------------------------------------------------------------------
double get_value_double(char *key, HASHTABLE *hash_t)
{
	double *doubleptr;
	doubleptr = (double *) get_value(key, hash_t);
	return (doubleptr == NULL) ? 0: *doubleptr;
}

//----------------------------------------------------------------------------
// 関数名       ：set_key_double
// 概要         ：set_keyのdouble型用
// 戻り値       ：int
// 引数         ：char *key
//                キー
// 引数         ：double val
//                値
// 引数         ：HASHTABLE *hash_t
//                ハッシュテーブルデータ
//----------------------------------------------------------------------------
int set_key_double(char *key, double val, HASHTABLE *hash_t)
{
	double *n;

	n = (double *)malloc(sizeof(double));

	if (n == NULL) return -1;

	*n = val;

	return set_key(key, n, hash_t);
}

