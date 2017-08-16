/*************************************************************************
    > File Name: Graphics.cpp
    > Author: Zhang Yuhui
    > Mail: yuhui-zh15@mails.tsinghua.edu.cn 
    > Created Time: 六  9/17 17:50:02 2016
 ************************************************************************/

#include <iostream>
#include <cstdio>
#include <cstdlib>
using namespace std;

const int maxn = 200000 + 10;

//qsort函数指针
int cmp(const void *a, const void *b) {
	return *(int*)a - *(int*)b;
}

//计算叉乘
inline long long calc(int x, int y, int pos, int *a, int *b) {
	return (long long)a[pos] * (long long)(y - b[pos]) + (long long)b[pos] * (long long)x;
}

//二分查询
inline int query(int x, int y, int lo, int hi, int *a, int *b) {
	while (lo < hi) {
		int ran = (hi + lo) >> 1;
		if (calc(x, y, ran, a, b) < 0) hi = ran;
		else lo = ran + 1;
	}
	return lo;
}

//初始化
inline void init() {
	int *a = new int[maxn], *b = new int[maxn];
	int n, m;
	int p, q;
	scanf("%d", &n);
	for (register int i = 0; i < n; i++) scanf("%d", &a[i]);
	for (register int i = 0; i < n; i++) scanf("%d", &b[i]);
	qsort(a, n, sizeof(a[0]), cmp);
	qsort(b, n, sizeof(b[0]), cmp);
	scanf("%d", &m);
	for (int i = 0; i < m; i++) {
		scanf("%d%d", &p, &q);
		printf("%d\n", query(p, q, 0, n, a, b));
	}
	delete []a;
	delete []b;
}


int main() {
	setvbuf(stdin, new char[1 << 24], _IOFBF, 1 << 24);
	setvbuf(stdout, new char[1 << 24], _IOFBF, 1 << 24);
	init();
    return 0;
}
