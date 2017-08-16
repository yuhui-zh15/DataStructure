/*************************************************************************
    > File Name: Mooc.cpp
    > Author: Zhang Yuhui
    > Mail: yuhui-zh15@mails.tsinghua.edu.cn
    > Created Time: 四 12/1 16:24:29 2016
 ************************************************************************/

#include <cstdio>
#include <iostream>
#include <cstring>
#include <cstdlib>
using namespace std;

const int maxn = 500010;
const int maxint = 2147483647;

//树叶节点
struct Node {
	Node() { data = child[0] = child[1] = parent = key = 0; }
	int child[2], parent, key, data;
};

//Treap平衡树
struct Treap {
	Node *T;//数据
	int size;//记录内存位置
	int root;//记录根节点
    int dir;//记录搜索方向，用于插入
    int cnt;//记录元素个数
    //构造函数
	Treap() {
		T = new Node[maxn << 1];
		cnt = size = 0; root = 1;
        //左哨兵，key设为最大保证根不会下溢
        ++size;
		T[size].data = -maxint;
        T[size].key = maxint;
		T[size].child[1] = size + 1;
		//右哨兵，key设为最小保证右哨兵不会上溢
        ++size;
		T[size].parent = root;
		T[size].data = maxint;
        T[size].key = -maxint;
	}
    //析构函数
	~Treap() {
		delete []T;
	}
    //节点旋转压缩版，node为旋转结点，ori为旋转取向
    void Rotate(int node, int ori) {
        int p = T[node].parent, counter = ori ^ 1;
        T[p].child[ori] = T[node].child[counter];
        if (T[p].child[ori]) T[T[p].child[ori]].parent = p;
        T[node].child[counter] = p; T[node].parent = T[p].parent; T[p].parent = node;
        if (T[T[node].parent].child[0] == p) T[T[node].parent].child[0] = node;
        else if (T[T[node].parent].child[1] == p) T[T[node].parent].child[1] = node;
    }
    //node直接后继
    int Succ(int node) {
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
    int Prev(int node) {
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
    void PushUp(int node) {
        while (T[T[node].parent].key < T[node].key && T[node].parent) {
            int ori = T[T[node].parent].child[0] == node? 0: 1;
            Rotate(node, ori);
        }
    }
    //下溢，比较key大小或已至叶节点
    void PushDown(int node) {
        while (T[node].child[0] || T[node].child[1]) {
            int ori;
            if (!T[node].child[0]) ori = 1;
            else if (!T[node].child[1]) ori = 0;
            else ori = T[T[node].child[0]].key > T[T[node].child[1]].key? 0: 1;    
            Rotate(T[node].child[ori], ori);
        }
    }
    //插入，搜索到节点并插入，插入后上溢
	void Insert(int data) {
		int node = Search(data);
        T[node].child[dir] = ++size;
        T[size].data = data;
        T[size].parent = node;
        T[size].key = rand();
        PushUp(size);
        cnt++;
	}
    //删除，搜索到节点并下溢，下溢后删除
	int Remove(int data) {
		int node = Search(data);
        int ret = T[node].data;
        PushDown(node);
        int ori = T[T[node].parent].child[0] == node? 0: 1;
        T[T[node].parent].child[ori] = 0;
        cnt--;
		return ret;
	}
    //删除最小值
    int DelMin() {
        if (cnt == 0) return 0;
        int node = Succ(1);//最小值位置在左哨兵后
        int ret = T[node].data;
        PushDown(node);
        int ori = T[T[node].parent].child[0] == node? 0: 1;
        T[T[node].parent].child[ori] = 0;
        cnt--;
        return ret;
    }
    //删除最大值
    int DelMax() {
        if (cnt == 0) return 0;
        int node = Prev(2);//最大值位置在右哨兵前
        int ret = T[node].data;
        PushDown(node);
        int ori = T[T[node].parent].child[0] == node? 0: 1;
        T[T[node].parent].child[ori] = 0;
        cnt--;
        return ret;
    }
    //搜索，dir记录了插入方向
	int Search(int data) {
		int node = root;
		int hot = root;
		while (node != 0) {
            hot = node;
			if (T[node].data <= data) { node = T[node].child[1]; dir = 1; }
			else { node = T[node].child[0]; dir = 0; }
		}
		return hot;
	}
};

void init() {
	//初始化
	Treap* t = new Treap();
    srand((unsigned)time(NULL));
	int N, pts;
    char s[1000];
    //读取指令
    gets(s);
    N = atoi(s);
    for (int i = 0; i < N; i++) {
        gets(s);
        if (s[0] == 'I') {
            pts = atoi(s + 2);
            t->Insert(pts);
        }
        else if (s[0] == 'H') {
            printf("%d\n", t->DelMax());
        }
        else {
            printf("%d\n", t->DelMin());
        }
    }
	delete t;
}

int main() {
	setvbuf(stdin, new char[1 << 24], _IOFBF, 1 << 24);
    setvbuf(stdout, new char[1 << 24], _IOFBF, 1 << 24);
	init();
	return 0;
}
