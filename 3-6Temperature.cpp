/*************************************************************************
 > File Name: Temperature.cpp
 > Author: Zhang Yuhui
 > Mail: yuhui-zh15@mails.tsinghua.edu.cn
 > Created Time: 五 12/ 2 21:50:21 2016
 ************************************************************************/

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "temperature.h"
using namespace std;
const int maxn = 50010;
const int MAX = 2147483647;
const int MIN = -2147483648;

//树叶节点
struct Node {
    int child[2];//左右孩子节点
    int size;//元素数目
    long long sum;//元素总和
    Node() { child[0] = child[1] = size = sum = 0; }
};

//可持久化线段树
struct SegTree {
    Node T[maxn * 35];//树数据
    int num;//树内存位置，对应于T[]的位置，分配内存用
    int root[maxn];//N个树树根节点
    int root_x[maxn];//N个树树根节点对应x值
    
    //构造函数
    SegTree() {
        T[1].child[0] = T[1].child[1] = T[1].size = T[1].sum = 0;
        num = 1;
    }
    //二分查找LowerBound 寻找最后一个小于m者
    int LowerBound(int *S, const int& m, int lo, int hi) {
        while (lo < hi) {
            int ran = ((long long)hi + (long long)lo) >> 1;
            if (m <= S[ran]) hi = ran;
            else lo = ran + 1;
        }
        return --lo;
    }
    //二分查找UpperBound 寻找最后一个小于等于m者
    int UpperBound(int *S, const int& m, int lo, int hi) {
        while (lo < hi) {
            int ran = ((long long)hi + (long long)lo) >> 1;
            if (m < S[ran]) hi = ran;
            else lo = ran + 1;
        }
        return --lo;
    }
    //插入，pre为上一棵树节点，now为本棵树节点，l，r表示[l, r]区间，key为键值，value为元素值
    void Insert(int pre, int now, int l, int r, const int& key, const int& value) {
        T[now] = T[pre];
        T[now].size++;
        T[now].sum += value;
        if (l == r) return;
        int mid = ((long long)l + (long long)r) >> 1;
        if (key <= mid) {
            T[now].child[0] = ++num;
            Insert(T[pre].child[0], T[now].child[0], l, mid, key, value);
        }
        else {
            T[now].child[1] = ++num;
            Insert(T[pre].child[1], T[now].child[1], mid + 1, r, key, value);
        }
    }
    //查询，now为当前节点，l，r表示[l, r]区间，L，R表示查询的[L, R]区间，cnt返回区间中元素个数
    long long Query(int now, int l, int r, const int& L, const int& R, int &cnt) {
        if (L <= l && r <= R) {
            cnt += T[now].size;
            return T[now].sum;
        }
        int m = ((long long)l + (long long)r) >> 1;
        long long ret = 0;
        if (L <= m) ret += Query(T[now].child[0], l, m, L, R, cnt);
        if (m < R) ret += Query(T[now].child[1], m + 1, r, L, R, cnt);
        return ret;
    }
};

//观测站信息
struct Station {
    int x, y, temp;
} station[maxn];

//qsort比较函数
int cmp(const void* a, const void* b) {
    return ((Station*)b)->x < ((Station*)a)->x;
}

void build_tree(SegTree* t, Station* s, const int& n, int& root_cnt) {
    int now, pre;
    pre = ++t->num;
    //第一个特殊处理
    t->Insert(1, pre, MIN, MAX, s[0].y, s[0].temp);
    //处理中间数据
    for (int i = 1; i < n; i++) {
        if (s[i].x != s[i - 1].x) {
            //如果x坐标不同，更新根节点信息
            t->root[root_cnt] = pre;
            t->root_x[root_cnt] = s[i - 1].x;
            root_cnt++;
        }
        now = ++t->num;
        t->Insert(pre, now, MIN, MAX, s[i].y, s[i].temp);
        pre = now;
    }
    //最后一个特殊处理
    t->root[root_cnt] = pre;
    t->root_x[root_cnt] = s[n - 1].x;
    root_cnt++;
}

void get_query(SegTree* t, const int& root_cnt) {
    int x1, y1, x2, y2, p1, p2, cnt1, cnt2;
    long long q1, q2, ans;
    while (GetQuery(&x1, &y1, &x2, &y2)) {
        //二分查找
        p1 = t->LowerBound(t->root_x, x1, 0, root_cnt);
        p2 = t->UpperBound(t->root_x, x2, 0, root_cnt);
        cnt1 = cnt2 = 0;
        //可持久化线段树搜索
        q1 = (p1 == -1)? 0: t->Query(t->root[p1], MIN, MAX, y1, y2, cnt1);
        q2 = (p2 == -1)? 0: t->Query(t->root[p2], MIN, MAX, y1, y2, cnt2);
        ans = q2 - q1;
        if (cnt2 - cnt1) ans /= (cnt2 - cnt1);
        //输出
        Response(ans);
    }
}

void init() {
    //初始化+排序
    SegTree *t = new SegTree;//可持久化线段树
    int n = GetNumOfStation();
    for (int i = 0; i < n; i++) {
        GetStationInfo(i, &station[i].x, &station[i].y, &station[i].temp);
    }
    qsort(station, n, sizeof(station[0]), cmp);
    //建树+处理数据
    int root_cnt = 0;
    build_tree(t, station, n, root_cnt);
    get_query(t, root_cnt);
    //删除
    delete t;
}

int main() {
    setvbuf(stdin, new char[1 << 24], _IOFBF, 1 << 24);
    setvbuf(stdout, new char[1 << 24], _IOFBF, 1 << 24);
    init();
    return 0;
}

