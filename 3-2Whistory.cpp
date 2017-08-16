/*************************************************************************
    > File Name: Cycle.cpp
    > Author: Zhang Yuhui
    > Mail: yuhui-zh15@mails.tsinghua.edu.cn 
    > Created Time: 三  8/17 10:43:14 2016
 ************************************************************************/

#include <iostream>
#include <cstdio>
#include <cstring>
using namespace std;

const int maxn = 10010;//最大字符串个数
const int maxs = 1010;//最大字符串长度

//KMP类，用于判断两个字符串循环移位相等
struct KMP {
	//next表
	int next[maxn];
	//构造next表
	void buildNext(const char* P) {
		int m = strlen(P), j = 0;
		int t = next[0] = -1;
		while (j < m - 1) {
			if (t < 0 || P[j] == P[t]) {
				j++; t++; next[j] = (P[j] == P[t])? next[t]: t;
			}
			else {
				t = next[t];
			}
		}
	}
	//与目标串进行匹配
	int match(const char* P, const char* T) {
		buildNext(P);
		int n = strlen(T), i = 0;
		int m = strlen(P), j = 0;
		while (i < n && j < m) {
			if (j < 0 || T[i] == P[j]) {
				i++; j++;
			}
			else {
				j = next[j];
			}
		}
		return i - j;
	}
};

//HashTable类，对字符串散列
struct HashTable {

	int hash_table_size;//散列大小
	int hash_cnt;//字符串个数
	int* hash_table;//散列表
	char hash_buffer[maxn];//缓冲区，用于KMP比较
	char hash_str[maxn][maxs];//储存字符串
	KMP kmp;//KMP比较器
	//构造
	HashTable(const int& size = 1000007) {
		hash_cnt = 0;
		hash_table_size = size;
		hash_table = new int[hash_table_size];
		for (register int i = 0; i < hash_table_size; i++) hash_table[i] = -1; 
	}
	//析构
	~HashTable() {
		delete[] hash_table;
	}
	//Hash函数，计算字符串key所有循环移位的最小hash值
	unsigned int Hash(const char* key) {
		//计算key的哈希值
		unsigned int len = strlen(key);
		unsigned int seed = 131;
		unsigned int hash = key[0];
		unsigned int pow = 1;//记录了seed ^ ( n - 1 )，用于计算最小hash值
		for (int i = 1; i < len; i++) {
			pow *= seed;
			hash = hash * seed + key[i];
		}
		//计算key所有循环移位的最小hash值，保证循环移位相同的字符串具有相同hash值
		unsigned int min_hash = hash;
		for (int i = 0; i < len; i++) {
			hash = (hash - pow * key[i]) * seed + key[i];
			if (hash < min_hash) min_hash = hash;
		}
		return min_hash % hash_table_size;
	}
	//判等器，先将目标串连接2次，再用待匹配串通过KMP算法匹配，若P="abc", T="bca"，则实际匹配的是"abc"和"bcabca"
	bool Equal(const char* P, const char* T) {
		strcpy(hash_buffer, T);
		strcat(hash_buffer, T);
		return kmp.match(P, hash_buffer) <= strlen(T);
	}
	//Hash查询储存，其中储存、查询采用了若有则向后移动的方式
	int Search(const char *key) {
		int addr = Hash(key);
		while (1) {
			//如果没有，记录至hash表
			if (hash_table[addr] == -1) {
				hash_table[addr] = ++hash_cnt;
				strcpy(hash_str[hash_cnt], key);
				return 0;
			}
			//若有，返回hash值
			if (Equal(key, hash_str[hash_table[addr]])) {
				++hash_cnt;
				return hash_table[addr];
			}
			addr = (addr + 1) % hash_table_size;
		}
	}
};

void init() {
	HashTable *hash = new HashTable;
	int n, m;
	scanf("%d%d", &n, &m);
	char s[maxn];
	for (int i = 0; i < m; i++) {
		scanf("%s", s);
		printf("%d\n", hash->Search(s));
	}
	delete hash;
}

int main() {
	setvbuf(stdin, new char[1 << 24], _IOFBF, 1 << 24);
	setvbuf(stdout, new char[1 << 24], _IOFBF, 1 << 24);
	init();
	return 0;
}
