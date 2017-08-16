/*************************************************************************
    > File Name: Zauberberg.cpp
    > Author: Zhang Yuhui
    > Mail: yuhui-zh15@mails.tsinghua.edu.cn 
    > Created Time: 六  9/17 19:17:51 2016
 ************************************************************************/

#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
using namespace std;

const int maxn = 100000 + 10;

struct Insect {
	int h;
	char c;
};

//qsort比较函数
int cmp(const void *a, const void *b) {
	return ((Insect*)b)->h - ((Insect*)a)->h;
}

//处理数据，为二分查找奠定基础
void process(int &N, int &H, int &M, int &cur, int *Height, int *NHit, int *NFalse, double *PHit, double *PFalse, Insect *insects) {
	memset(NHit, 0, sizeof(*NHit));
	memset(NFalse, 0, sizeof(*NFalse));
	qsort(insects, N, sizeof(insects[0]), cmp);
	
	int SumHit = 0;//染病生物总数
	int SumFalse = 0;//健康生物总数
	//第一个数据特殊情况处理
	Height[cur] = insects[0].h;
	if (insects[0].c == '+') {
		SumHit++;
		NHit[cur] = (NHit[cur] == 0)? 1: NHit[cur] + 1;
		NFalse[cur] = (NFalse[cur] == 0)? 0: NFalse[cur];
	}
	else {
		SumFalse++;
		NFalse[cur] = (NFalse[cur] == 0)? 1: NFalse[cur] + 1;
		NHit[cur] = (NHit[cur] == 0)? 0: NHit[cur];
	}
	//循环统计杀中数和误杀数
	for (register int i = 1; i < N; i++) {
		if (insects[i].h != Height[cur]) cur++;
		Height[cur] = insects[i].h;
		if (insects[i].c == '+') {
			SumHit++;
			NHit[cur] = (NHit[cur] == 0)? NHit[cur - 1] + 1: NHit[cur] + 1;
			NFalse[cur] = (NFalse[cur] == 0)? NFalse[cur - 1]: NFalse[cur];
		}
		else if (insects[i].c == '-')
		{
			SumFalse++;
			NFalse[cur] = (NFalse[cur] == 0)? NFalse[cur - 1] + 1: NFalse[cur] + 1;
			NHit[cur] = (NHit[cur] == 0)? NHit[cur - 1]: NHit[cur];
		}
	}
	//处理为为double用于进行二分查找
	cur++;
	for (register int i = 0; i < cur; i++) {
		PHit[i] = (double) 1.0 * NHit[i] / SumHit;
		PFalse[i] = (double) 1.0 * NFalse[i] / SumFalse;
	}
	//开头插 H 0 0
	if (Height[0] != H) {
		for (register int i = cur; i > 0; i--) {
			Height[i] = Height[i - 1];
			PHit[i] = PHit[i - 1];
			PFalse[i] = PFalse[i - 1];
		}
		Height[0] = H;
		PHit[0] = 0.0;
		PFalse[0] = 0.0;
		cur++;
	}
}

//二分查找LowerBound 寻找最后一个小于m者
inline int LowerBound(double *S, double m, int lo, int hi) {
	while (lo < hi) {
		int ran = (hi + lo) >> 1;
		if (m - S[ran] <= 0) hi = ran;
		else lo = ran + 1;
	}
	return --lo;
}

//二分查找UpperBound 寻找最后一个小于等于m者
inline int UpperBound(double *S, double m, int lo, int hi) {
	while (lo < hi) {
		int ran = (hi + lo) >> 1;
		if (m - S[ran] < 0) hi = ran;
		else lo = ran + 1;
	}
	return --lo;
}

//计算结果
inline void calc(int &cur, double &phit, double &pfalse, double *PHit, double *PFalse, int *Height) {
	int lo = LowerBound(PHit, phit, 0, cur);
	int hi = UpperBound(PFalse, pfalse, 0, cur);
	if (lo >= hi) printf("-1\n");
	else {
		if (hi == cur - 1) printf("0 %d\n", Height[lo + 1]);
		else printf("%d %d\n", Height[hi + 1] + 1, Height[lo + 1]);
	}
}

//初始化
inline void init() {
	//建立
	int N, H, M;//生物数、高度、询问数
	int cur = 0;//对高度离散化
	int *Height = new int[maxn], *NHit = new int[maxn], *NFalse = new int[maxn];//杀中数、误杀数
	double *PHit = new double[maxn], *PFalse = new double[maxn];//杀中率、误杀率
	Insect *insects = new Insect[maxn];
	//读入
	scanf("%d%d", &N, &H);
	for (register int i = 0; i < N; i++) {
		scanf("%d %c", &insects[i].h, &insects[i].c);
	}
	//预处理
	process(N, H, M, cur, Height, NHit, NFalse, PHit, PFalse, insects);
	//查询
	scanf("%d", &M);
	for (int i = 0; i < M; i++) {
		double phit, pfalse;
		scanf("%lf%lf", &phit, &pfalse);
		calc(cur, phit, pfalse, PHit, PFalse, Height);
	}
	//删除
	delete []Height; delete []NHit; delete []NFalse;
	delete []PHit; delete []PFalse; delete []insects;
}

int main() {
	setvbuf(stdin, new char[1 << 24], _IOFBF, 1 << 24);
	setvbuf(stdout, new char[1 << 24], _IOFBF, 1 << 24);
	init();
	return 0;
}
