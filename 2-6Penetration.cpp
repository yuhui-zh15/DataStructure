/*************************************************************************
    > File Name: Penetration.cpp
    > Author: Zhang Yuhui
    > Mail: yuhui-zh15@mails.tsinghua.edu.cn 
    > Created Time: 三 10/26 15:10:26 2016
 ************************************************************************/

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
using namespace std;
const int maxn = 100010;
const int maxw = 100000000;
const int minw = 1;

//树叶节点
struct Node {
	int child[2];//左右孩子节点
	int size;//元素数目
	long long sum;//元素总和
	Node() { child[0] = child[1] = size = sum = 0; }
};

//可持久化线段树
struct SegTree {
	Node T[maxn * 64];//树数据
	int num;//树内存位置，对应于T[]的位置，分配内存用
	int root[maxn];//N个树树根节点
	
	//构造函数
	SegTree() {
		num = 1;
	}
	//修改，pre为上一棵树节点，now为本棵树节点，l，r表示[l, r]区间，value为待插入删除值，state表示状态，1为删除，0为插入
	void modify(int pre, int now, int l, int r, const int& value, const bool& state) {
		T[now] = T[pre];
		if (state) {
			T[now].size--;
			T[now].sum -= value;
		}
		else {
			T[now].size++;
			T[now].sum += value;
		}
		if (l == r) return;
		int mid = (l + r) >> 1;
		if (value <= mid) {
			T[now].child[0] = ++num;
			modify(T[pre].child[0], T[now].child[0], l, mid, value, state);
		}
		else {
			T[now].child[1] = ++num;
			modify(T[pre].child[1], T[now].child[1], mid + 1, r, value, state);
		}
	}

	//查询，now为当前节点，l，r表示[l, r]区间，k为查询个数[1, k]
	long long query(int now, int l, int r, int k) {
		if (k >= T[now].size) return T[now].sum;
		if (l == r) return (long long)k * l;
		int lsize = T[T[now].child[0]].size;
		int mid = (l + r) >> 1;
		if (lsize >= k) return query(T[now].child[0], l, mid, k);
		else return T[T[now].child[0]].sum + query(T[now].child[1], mid + 1, r, k - lsize);
	}
};

//处理后的指令
struct CMD { 
	int p, w;//操作靶子位置、靶子抗性
	bool d;//加入或删除,0加入1删除
};

//qsort比较函数
int cmp(const void* a, const void* b) {
	return ((CMD*)a)->p - ((CMD*)b)->p;
}

void init() {
	//创建
	int m, n;//靶子数，询问数
	CMD *cmd = new CMD[maxn << 1];//处理后的指令
	SegTree *t = new SegTree;//可持久化线段树
	scanf("%d%d", &m, &n);
	//读入靶子信息，并处理
	int l, r, w;
	for (int i = 0; i < m; i++) {
		scanf("%d%d%d", &l, &r, &w);
		cmd[i << 1].p = l;
		cmd[i << 1].w = w;
		cmd[i << 1].d = false;
		cmd[i << 1 | 1].p = r + 1;
		cmd[i << 1 | 1].w = w;
		cmd[i << 1 | 1].d = true;
	}
	//排序指令
	qsort(cmd, m << 1, sizeof(cmd[0]), cmp);
	//建树
	int now, pre;
	pre = ++t->num;
	t->modify(1, pre, minw, maxw, cmd[0].w, cmd[0].d);
	for (int i = 1; i < (m << 1); i++) {
		if (cmd[i].p != cmd[i - 1].p) {
			//更新根节点信息
			for (int j = cmd[i - 1].p; j != cmd[i].p; j++)
				t->root[j] = pre;
		}
		now = ++t->num;
		t->modify(pre, now, minw, maxw, cmd[i].w, cmd[i].d);
		pre = now;
	}
	//询问
	int x, a, b, c, k;
	long long Pre = 1;//上次射击数据
	for (int i = 0; i < n; i++) {
		scanf("%d%d%d%d", &x, &a, &b, &c);
		k = ((long long)a * Pre + (long long)b) % (long long)c + 1;
		Pre = t->query(t->root[x], minw, maxw, k);
		printf("%lld\n", Pre);
	}
	//删除
	delete []cmd; delete t;
}

int main() {
	setvbuf(stdin, new char[1 << 24], _IOFBF, 1 << 24);
	setvbuf(stdout, new char[1 << 24], _IOFBF, 1 << 24);
	init();
	return 0;
}
