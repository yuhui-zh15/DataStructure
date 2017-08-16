/*************************************************************************
    > File Name: BigInt.cpp
    > Author: Zhang Yuhui
    > Mail: yuhui-zh15@mails.tsinghua.edu.cn 
    > Created Time: 五  9/16 20:28:19 2016
 ************************************************************************/

#include <iostream>
#include <cstdio>
#include <cstring>
using namespace std;

const int maxn = 5005;//数组最大位数
const int digit = 8;//高精乘每次8位
const int MOD = 100000000;//取模

//预处理，将char*转换为long long*，同时记录数字位数
inline void preprocess(char* carr, long long* arr, int& larr) {
	int len = strlen(carr);
	larr = (len % digit == 0)? len / digit: len / digit + 1;
	for (int i = len - 1; i >= 0; i -= digit) {
		for (int j = i - digit + 1; j <= i; j++) {
			if (j < 0) continue;
			arr[larr - 1] = arr[larr - 1] * 10 + carr[j] - '0';
		}
		larr--;
	}
	larr = (len % digit == 0)? len / digit: len / digit + 1;
}

//高精度乘法，压8位计算
inline void calc(long long *a, long long *b, long long *ans, int la, int lb) {
	//计算
	for (int i = la - 1; i >= 0; i--) {
		for (int j = lb - 1; j >= 0; j--) {
			ans[i + j + 1] += a[i] * b[j];
		}
	}
	//进位
	for (int i = la + lb - 2; i >= 0; i--) {
		ans[i] += ans[i + 1] / MOD;
		ans[i + 1] %= MOD;
	}
}

//打印
inline void print(long long *ans, int la, int lb) {
	int index = 0;
	while(ans[index] == 0 && index < la + lb) index++;
	printf("%lld", ans[index]);
	for (int i = index + 1; i < la + lb; i++) {
		printf("%08lld", ans[i]);
	}
	printf("\n");
}

//初始化
inline void init() {
	int n;//读入个数
	char *ca = new char[maxn], *cb = new char[maxn];//读入长整数
	long long *a = new long long[maxn], *b = new long long[maxn], *ans = new long long[maxn];//处理后长整数
	int la, lb;//整数位数
	scanf("%d", &n);
	while (n--) {
		scanf("%s %s", ca, cb);
		for (register int i = 0; i < maxn - 1; i++) ans[i] = 0;
		for (register int i = 0; i < maxn - 1; i++) a[i] = 0;
		for (register int i = 0; i < maxn - 1; i++) b[i] = 0;
		preprocess(ca, a, la);
		preprocess(cb, b, lb);
		calc(a, b, ans, la, lb);
		print(ans, la, lb);
	}
	delete []ca; delete []cb;
	delete []a; delete []b; delete []ans;
}

int main() {
	setvbuf(stdin, new char[1 << 24], _IOFBF, 1 << 24);
	setvbuf(stdout, new char[1 << 24], _IOFBF, 1 << 24);
	init();
	return 0;
}
