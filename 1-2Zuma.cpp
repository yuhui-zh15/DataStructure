/*************************************************************************
    > File Name: Zuma.cpp
    > Author: Zhang Yuhui
    > Mail: yuhui-zh15@mails.tsinghua.edu.cn 
    > Created Time: 六  7/23 17:33:47 2016
 ************************************************************************/

#include <cstdio>
#include <iostream>
#include <cstring>

using namespace std;
const int maxn = 1000010;//最大输入大小

//Node类
struct Node
{
    Node() { data = child[0] = child[1] = parent = size = 0; }
	int child[2], parent;//左右孩子，父亲
	int size;//子树大小
	char data;//数据
};

//Splay类
struct Splay {
    Node *T;//树的数据区
    int _size;//树规模
    int _root;//树根
    //构造函数
    Splay() {
        T = new Node[maxn << 1];
        _size = 0;
    }
    //析构函数
    ~Splay() {
        delete []T;
    }
    //建树函数
    void build_tree(char* s, int len) {
        //左哨兵
        _root = 1;
        _size = 1;
        //右哨兵
        T[_root].child[1] = ++_size;
        T[_size].size = 1;
        T[_size].parent = _root;
        update(_root);
        //初始化树
        for (int i = 0; i < len; i++) {
            splay(find(i + 1), 0);
            splay(find(T[_root].size), _root);
            int _root_r = T[_root].child[1];
            T[_root_r].child[0] = ++_size;
            T[_size].data = s[i];
            T[_size].size = 1;
            T[_size].parent = _root_r;
            update(_root_r);
            update(_root);
            splay(_size, 0);
        }
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
    //更新规模
    void update(int node) {
        T[node].size = T[T[node].child[0]].size + T[T[node].child[1]].size + 1;
    }
    //寻秩访问，原理是基于维护的各节点树规模大小
    int find(int r) {
        int node = _root;
        while (1) {
            int lsize = T[T[node].child[0]].size;
            if (lsize + 1 == r) return node;
            if (lsize >= r) node = T[node].child[0];
            else { node = T[node].child[1]; r -= lsize + 1; }
        }
    }
    //节点旋转压缩版，node为旋转结点，ori为旋转取向
    void rotate(int node, int ori) {
        int p = T[node].parent, counter = ori ^ 1;
        T[p].child[ori] = T[node].child[counter];
        if (T[p].child[ori]) T[T[p].child[ori]].parent = p;
        T[node].child[counter] = p; T[node].parent = T[p].parent; T[p].parent = node;
        if (T[T[node].parent].child[0] == p) T[T[node].parent].child[0] = node;
        if (T[T[node].parent].child[1] == p) T[T[node].parent].child[1] = node;
        update(p); update(node);
    }
    //伸展，ord表示伸展到哪个点，比如ord＝root表示转到root以下就停，ord＝0表示转到root
    void splay(int node, int ord) {
        while (T[node].parent != ord) {
            int p = T[node].parent, g = T[p].parent;
            int p_ori = T[p].child[0] == node? 0: 1;
            if (g == ord) { rotate(node, p_ori); continue; }
            int g_ori = T[g].child[0] == p? 0: 1;
            if (p_ori == g_ori) { rotate(p, g_ori); rotate(node, p_ori); }
            else { rotate(node, p_ori); rotate(node, g_ori); }
        }
        if (!ord) _root = node;
    }
    //插入数据，插入在根节点右孩子的左子树
    void insert(char data) {
        int _root_r = T[_root].child[1];
        T[_root_r].child[0] = ++_size;
        T[_size].data = data;
        T[_size].size = 1;
        T[_size].parent = _root_r;
        update(_root_r);
        update(_root);
        splay(_size, 0);
    }
    //删除数据，删除根节点右孩子的左子树
    void remove() {
        int _root_r = T[_root].child[1];
        T[_root_r].child[0] = 0;
        update(_root_r);
        update(_root);
    }
    //非递归打印整棵树
    void output() {
        int node = find(2);
        while (node) {
            if (T[node].data != 0) printf("%c", T[node].data);
            node = succ(node);
        }
        printf("\n");
    }
    //执行指令
    void run(int r, char c) {
        //插入
        splay(find(r + 1), 0);
        splay(find(r + 2), _root);
        insert(c);

        //循环消除
        while (1) {
            char c = T[_root].data;
            if (c == 0) break;
            int rr = T[T[_root].child[0]].size + 1, ll = T[T[_root].child[0]].size + 1;
            while (T[find(rr)].data == c) rr++;
            while (T[find(ll)].data == c) ll--;

            if (rr - ll > 3) {
                splay(find(ll), 0);
                splay(find(rr), _root);
                remove();
                continue;
            }
            break;
        }
    }
};

//array转int
int atoi(char* s, int l) {
    int ret = 0;
    for (int i = 0; i < l; i++) {
        ret = ret * 10 + s[i] - '0';
    }
    return ret;
}

void init() {
    //初始化数组
    Splay* t = new Splay();//Splay建树
    char* instr = new char[20000000];//读入字符串
    int inposi = 0;//读入指针
    char* cmd = new char[100];//指令字符串
    fread(instr, 1, 20000000, stdin);//一次读入
    //读第一行
    while (instr[inposi] != '\n') inposi++;
    //建树
    t->build_tree(instr, inposi);
    //解析m
    inposi++;
    int k = 0;
    while (instr[inposi] != '\n') {
        cmd[k++] = instr[inposi++];
    }
    cmd[k] = 0;
    int m = atoi(cmd, k);
    while (m--)
    {
        //写入cmd字符串
        inposi++;
        int k = 0;
        while (instr[inposi] != '\n') {
            cmd[k++] = instr[inposi++];
        }
        cmd[k] = 0;
        //解析指令
        int i = 0;
        while (i < k && cmd[i] != ' ') i++;
        int r = atoi(cmd, i);
        char c = cmd[i + 1];
        //执行指令
        t->run(r, c);
    }
    //打印
    t->output();
    //删除数组
    delete t;
    delete []instr;
    delete []cmd;
}

int main()
{
	setvbuf(stdout, new char[1 << 24], _IOFBF, 1 << 24);
    init();
	return 0;
}
