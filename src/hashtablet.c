//================================================================================
//      FILE
//--------------------------------------------------------------------------------
//      ファイル名      :       hashtablet.c
//      概要            :       ハッシュテーブルを扱うライブラリhashtableのテストツール
//--------------------------------------------------------------------------------
#include <stdio.h>
#include "hashtable.h"

int main(void){
	HASHTABLE *hash_t1;
	HASHTABLE *hash_t2;


	hash_t1 = new_hashtable(1024);
	set_key_str("key1", "疑似", hash_t1);
	puts(get_value_str("key1", hash_t1));
	set_key_str("key2", "ロボット", hash_t1);
	puts(get_value_str("key2", hash_t1));
	set_key_str("key1", "高性能", hash_t1);
	puts(get_value_str("key1", hash_t1));
	set_key_str("key1", "識別不可能", hash_t1);
	puts(get_value_str("key1", hash_t1));
	set_key_str("key3", "レーダーマン", hash_t1);
	puts(get_value_str("key3", hash_t1));
	
	set_key_str("key1", "疑似a", hash_t1);
	puts(get_value_str("key1", hash_t1));
	set_key_str("key2", "ロボットa", hash_t1);
	puts(get_value_str("key2", hash_t1));
	set_key_str("key1", "高性能a", hash_t1);
	puts(get_value_str("key1", hash_t1));
	set_key_str("key1", "識別不可能a", hash_t1);
	puts(get_value_str("key1", hash_t1));
	set_key_str("key3", "レーダーマンa", hash_t1);
	puts(get_value_str("key3", hash_t1));
	del_hashtable(hash_t1);


	puts("-----");

	/* コリジョンテスト */
	hash_t2 = new_hashtable(1);
	set_key_str("key1", "疑似", hash_t2);
	puts(get_value_str("key1", hash_t2));
	set_key_str("key2", "ロボット", hash_t2);
	puts(get_value_str("key2", hash_t2));
	set_key_str("key1", "高性能", hash_t2);
	puts(get_value_str("key1", hash_t2));
	set_key_str("key1", "識別不可能", hash_t2);
	puts(get_value_str("key1", hash_t2));
	set_key_str("key3", "レーダーマン2", hash_t2);
	puts(get_value_str("key3", hash_t2));


	set_key_str("key1", "疑似", hash_t2);
	puts(get_value_str("key1", hash_t2));
	set_key_str("key2", "ロボット", hash_t2);
	puts(get_value_str("key2", hash_t2));
	set_key_str("key1", "高性能", hash_t2);
	puts(get_value_str("key1", hash_t2));
	set_key_str("key1", "aaaa", hash_t2);
	puts(get_value_str("key1", hash_t2));
	set_key_str("key3", "レーダーマン22", hash_t2);
	puts(get_value_str("key3", hash_t2));
	del_hashtable(hash_t2);

	puts("-----");

	/* 整数型テスト */
	hash_t2 = new_hashtable(1);
	set_key_int("key1", 12345, hash_t2);
	printf("%d\n",get_value_int("key1", hash_t2));
	set_key_int("key1", 67890, hash_t2);
	printf("%d\n",get_value_int("key1", hash_t2));
	del_hashtable(hash_t2);


	return 0;
}
