/*************************************************************************
    > File Name: Pokeface_2.cpp
    > Author: Zhang Yuhui
    > Mail: yuhui-zh15@mails.tsinghua.edu.cn 
    > Created Time: 三 11/30 21:23:57 2016
 ************************************************************************/

#include <cstdio>
#include <iostream>
using namespace std;

const int maxn = 1000010;

//线段树
struct SegTree {
	//sum代表区间个数，add代表翻转状态（懒惰标记）
	int sum[maxn << 2];
	int add[maxn << 2];
	//向上更新sum值
	void PushUp(int v) {
		sum[v] = sum[v << 1] + sum[v << 1 | 1];
	}
	//向下更新sum值，如果有add懒惰标记
	void PushDown(int v, int l, int m, int r) {
		if (add[v]) {
			//add懒惰标记向下传递
			add[v << 1] = (add[v << 1] + add[v]) & 1;
			add[v << 1 | 1] = (add[v << 1 | 1] + add[v]) & 1;
			//sum向下更新
			sum[v << 1] = m - l + 1 - sum[v << 1];
			sum[v << 1 | 1] = r - m - sum[v << 1 | 1];
			add[v] = 0;
		}
	}
	//递归初始化线段树
	void Build(int v, int l, int r) {
		add[v] = 0;
		if (l == r) {
			sum[v] = 1;
			return;
		}
		int m = (l + r) >> 1;
		Build(v << 1, l, m);
		Build(v << 1 | 1, m + 1, r);
		PushUp(v);
	}
	//在[L, R]区间翻转，v是当前节点，[l,r]是当前区间，为递归变量
	void Update(const int& L, const int& R, int v, int l, int r) {
		//包含则区段更新，标记add懒惰标记
		if (L <= l && r <= R) {
			add[v] = (add[v] + 1) % 2;
			sum[v] = r - l + 1 - sum[v];
			return;
		}
		//否则递归向下更新
		int m = (l + r) >> 1;
		PushDown(v, l, m, r);
		if (L <= m) Update(L, R, v << 1, l, m);
		if (m < R) Update(L, R, v << 1 | 1, m + 1, r);
		PushUp(v);
	}
	//在[L, R]区间查询，v是当前节点，[l,r]是当前区间，为递归变量
	int Query(const int& L, const int& R, int v, int l, int r) {
		//包含则直接返回查询值
		if (L <= l && r <= R) {
			return sum[v];
		}
		//否则递归查询
		int m = (l + r) >> 1;
		PushDown(v, l, m, r);
		int ret = 0;
		if (L <= m) ret += Query(L, R, v << 1, l, m);
		if (m < R) ret += Query(L, R, v << 1 | 1, m + 1, r);
		return ret;
	}
};

void init() {
	//初始化线段树
	SegTree *t = new SegTree;
	t->Build(1, 1, maxn);
	//执行指令
	int n, p, q;
	scanf("%d%d%d", &n, &p, &q);
	int type, i, j;
	for (int k = 0; k < p + q; k++) {
		scanf("%d%d%d", &type, &i, &j);
		if (type == 0) {
			t->Update(i, j, 1, 1, maxn);
		}
		else {
			printf("%d\n", t->Query(i, j, 1, 1, maxn));
		}
	}
}

int main() {
	setvbuf(stdin, new char[1 << 24], _IOFBF, 1 << 24);
	setvbuf(stdout, new char[1 << 24], _IOFBF, 1 << 24);
	init();
	return 0;
}
