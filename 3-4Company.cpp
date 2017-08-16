/*************************************************************************
    > File Name: company.cpp
    > Author: Zhang Yuhui
    > Mail: yuhui-zh15@mails.tsinghua.edu.cn
    > Created Time: 四 12/ 1 19:14:09 2016
 ************************************************************************/

#include <cstdio>
#include <iostream>
#include <cstring>
using namespace std;
const int maxn = 500010;

//HashTable散列
struct HashTable {
	int table_size;//散列大小
	int cnt;//散列元素个数
	int size;//loop校验环大小
	int* table;//散列表，-1代表未插入，-2代表已删除
	int* loop;//校验环表
	int* array_value;//value值
	long long* array_key;//key值
	//构造
	HashTable(const int& _table_size = 10000007) {
		size = cnt = 0;
		table_size = _table_size;
		table = new int[table_size];
		loop = new int[maxn];
		array_value = new int[maxn];
		array_key = new long long[maxn];
		for (register int k = 0; k != table_size; k++) table[k] = -1;
	}
	//析构
	~HashTable() {
		delete []table;
		delete []array_value;
		delete []array_key;
		delete []loop;
	}
	//散列函数，取模法
	unsigned int Hash(const long long& key) {
		return key % table_size;
	}
	//插入key，如果有重复则向后插入
	void Insert(const long long& key, const int& value) {
		int addr = Hash(key);
		while (1) {
			//如果该位置未被插入或已被删除，新建元素
			if (table[addr] == -1) {
				table[addr] = size;
				array_key[size] = key;
				array_value[size] = value;
				loop[size] = addr;//插入校验环
				size++;
				cnt++;
				return;
			}
			//如果已有元素，更新value值
			if (table[addr] != -2 && array_key[table[addr]] == key) {
				array_value[table[addr]] = value;
				return;
			};
			//向后查询
			addr = (addr + 1) % table_size;
		}
	}
	//删除key
	void Delete(const long long& key) {
		int addr = Hash(key);
		while (1) {
			//如果该位置未被插入，说明没有该元素，返回
			if (table[addr] == -1) {
				return;
			}
			//如果该位置未被删除并且key相同，删除该元素
			if (table[addr] != -2 && array_key[table[addr]] == key) {
				cnt--;
				table[addr] = -2;//标记为已删除
				return;
			};
			//向后查询
			addr = (addr + 1) % table_size;
		}
	}
	//查询key
	int Query(const long long& key) {
		int addr = Hash(key);
		while (1) {
			//如果该位置未被插入，说明没有该元素，返回
			if (table[addr] == -1) {
				return -1;
			}
			//如果该位置未被删除并且key相同，返回该元素value
			if (table[addr] != -2 && array_key[table[addr]] == key) {
				return array_value[table[addr]];
			};
			//向后查询
			addr = (addr + 1) % table_size;
		}
	}
	//删除全部散列表，由于loop向量的存在，直接将对应位置置-1，再将cnt置0即可
	void Clear() {
		for (int i = 0; i < size; i++) { table[loop[i]] = -1; }
		size = cnt = 0;
	}
	//返回散列表中元素个数
	int Sum() {
		return cnt;
	}
};

//数组转整数，区间[l, r)
long long atoi(char* s, const int& l, const int& r) {
	long long ret = 0;
	for (int i = l; i < r; i++) {
		ret = ret * 10 + s[i] - '0';
	}
	return ret;
}

void init() {
	//初始化
	HashTable* hash = new HashTable;
	long long n, m;
	char s[1000];
	int k;
	gets(s);
	for (k = 0; s[k] != ' '; k++);
	n = atoi(s, 0, k);
	m = atoi(s, k + 1, strlen(s));
	//处理指令
	long long key, value;
	long long ans = 0;
	for (int i = 0; i < m; i++) {
		gets(s);
		switch(s[0]) {
		case 'I':
			for (k = 2; s[k] != ' '; k++);
			key = atoi(s, 2, k);
			value = atoi(s, k + 1, strlen(s));
			hash->Insert(key, value);
			break;
		case 'O':
			key = atoi(s, 2, strlen(s));
			hash->Delete(key);
			break;
		case 'C':
			hash->Clear();
			break;
		case 'N':
			ans += hash->Sum();
			break;
		case 'Q':
			key = atoi(s, 2, strlen(s));
			ans += hash->Query(key);
			break;
		}
	}
	printf("%lld\n", ans);
	delete hash;
}

int main() {
	setvbuf(stdin, new char[1 << 24], _IOFBF, 1 << 24);
	setvbuf(stdout, new char[1 << 20], _IOFBF, 1 << 20);
	init();
	return 0;
}
