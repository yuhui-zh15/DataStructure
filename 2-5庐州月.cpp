/*************************************************************************
    > File Name: Lunar.cpp
    > Author: Zhang Yuhui
    > Mail: yuhui-zh15@mails.tsinghua.edu.cn
    > Created Time: 三 10/19 16:24:29 2016
 ************************************************************************/

#include <cstdio>
#include <iostream>
#include <cstring>
#include <cstdlib>
using namespace std;

const int maxn = 500010;

//题目数据结构体
struct Data {
	int p;//价格
	int v;//美化度
	int type;//0代表原料, 1代表石桥
} arr[maxn << 1];

//比较函数
int cmp(const void *a,const void *b)
{
    if ( (*(Data*)a).v > (*(Data*)b).v ) return -1;
    else if ( (*(Data*)a).v == (*(Data*)b).v && (*(Data*)a).type < (*(Data*)b).type) return -1;
    else return 1;
}

//树叶节点
struct Node {
    void init() { data = child[0] = child[1] = parent = key = 0; }
	int child[2], parent, key, data;
};

//Treap平衡树
struct Treap {
	Node *T;//数据
	int size;//记录内存位置
	int root;//记录根节点
    //构造函数
	Treap() {
		T = new Node[maxn << 1];
		//左哨兵，key设为最大保证根不会下溢
		root = 1;
		T[root].data = -2147483647;
        T[root].key = 2147483647;
		size = 1;
		//右哨兵，key设为最小保证右哨兵不会上溢
		T[root].child[1] = ++size;
		T[size].parent = root;
		T[size].data = 2147483647;
        T[size].key = -2147483647;
	}
    //析构函数
	~Treap() {
		delete []T;
	}
    //节点旋转压缩版，node为旋转结点，ori为旋转取向
    void rotate(int node, int ori) {
        int p = T[node].parent, counter = ori ^ 1;
        T[p].child[ori] = T[node].child[counter];
        if (T[p].child[ori]) T[T[p].child[ori]].parent = p;
        T[node].child[counter] = p; T[node].parent = T[p].parent; T[p].parent = node;
        if (T[T[node].parent].child[0] == p) T[T[node].parent].child[0] = node;
        else if (T[T[node].parent].child[1] == p) T[T[node].parent].child[1] = node;
    }
    //node直接后继
    int succ(int node) {
        if (T[node].child[1]) {
            node = T[node].child[1];
            while (T[node].child[0]) node = T[node].child[0];
        }
        else {
            while (T[T[node].parent].child[1] == node) node = T[node].parent;
            node = T[node].parent;
        }
        return node;
    }
    //node直接前驱
    int prev(int node) {
        if (T[node].child[0]) {
            node = T[node].child[0];
            while (T[node].child[1]) node = T[node].child[1];
        }
        else {
            while (T[T[node].parent].child[0] == node) node = T[node].parent;
            node = T[node].parent;
        }
        return node;
    }
    //上溢，比较key大小或已至根节点
    void push_up(int node) {
        while (T[T[node].parent].key < T[node].key && T[node].parent) {
            int ori = T[T[node].parent].child[0] == node? 0: 1;
            rotate(node, ori);
        }
    }
    //下溢，比较key大小或已至叶节点
    void push_down(int node) {
        while (T[node].child[0] || T[node].child[1]) {
            int ori;
            if (!T[node].child[0]) ori = 1;
            else if (!T[node].child[1]) ori = 0;
            else ori = T[T[node].child[0]].key > T[T[node].child[1]].key? 0: 1;    
            rotate(T[node].child[ori], ori);
        }
    }
    //插入，搜索到节点并插入，插入后上溢
	void insert(int data) {
        int dir = -1;
		int node = search(data, dir);
        T[node].child[dir] = ++size;
        T[size].data = data;
        T[size].parent = node;
        T[size].key = rand();
        push_up(size);
	}
    //删除，搜索到节点并下溢，下溢后删除
	int remove(int data) {
		int node = search(data);
        if (T[node].data < data) node = succ(node);
        int ret = T[node].data;
        push_down(node);
        int ori = T[T[node].parent].child[0] == node? 0: 1;
        T[T[node].parent].child[ori] = 0;
		return ret;
	}
    //搜索（用于插入），dir记录了插入方向
	int search(int data, int &dir) {
		int node = root;
		int hot = root;
		while (node != 0) {
            hot = node;
			if (T[node].data <= data) { node = T[node].child[1]; dir = 1; }
			else { node = T[node].child[0]; dir = 0; }
		}
		return hot;
	}
    //搜索（用于搜索），返回不大于data的最靠后者
    int search(int data) {
        int node = root;
        int hot = root;
        while (node != 0) {
            if (T[node].data <= data) { hot = node; node = T[node].child[1];}
            else { node = T[node].child[0];}
        }
        return hot;
    }
};

void init() {
	//初始化
	Treap* t = new Treap();
    srand((unsigned)time(NULL));

	//读入
    int n, m;
    scanf("%d%d", &n, &m);
	int now = 0;
	for (int i = 0; i < n; i++) {
        scanf("%d%d", &arr[now].p, &arr[now].v);
		arr[now].type = 1;
		now++;
	}
	for (int i = 0; i < m; i++) {
        scanf("%d%d", &arr[now].p, &arr[now].v);
		arr[now].type = 0;
		now++;
	}

	//排序 
	qsort(arr, now, sizeof(arr[0]), cmp);

	//查找计算
	long long sum = 0;
	for (int i = 0; i < now; i++) {
		if (arr[i].type) sum += t->remove(arr[i].p);
		else t->insert(arr[i].p);
	}
	printf("%lld\n", sum);
	delete t;
}

int main() {
	setvbuf(stdin, new char[1 << 24], _IOFBF, 1 << 24);
    setvbuf(stdout, new char[1 << 20], _IOFBF, 1 << 20);
	init();
	return 0;
}
