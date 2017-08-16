/*************************************************************************
    > File Name: Zerone.cpp
    > Author: Zhang Yuhui
    > Mail: yuhui-zh15@mails.tsinghua.edu.cn
    > Created Time: 六 11/12 16:41:26 2016
 ************************************************************************/

#include <iostream>
#include <cstdio>
#include <cstring>
using namespace std;

typedef unsigned long long ull;
const int maxn = 2000005;

struct Node {
    Node() { child[0] = child[1] = parent = value = win = len = 0; }
    char win;//0为Adam胜，1为Eve胜，char占用空间小
    char len;//节点字符长度，char占用空间小
    int child[2];//左右孩子
    int parent;//父亲
    ull value;//节点的值，由于64位用unsigned long long
};

struct Trie {
    int size;//树中当前节点个数，用于分配空间（模拟new操作）
    Node T[maxn];//Node数组
    //树清空，初始时只有一个根节点，内存地址为1，0为哨兵节点，从第二个开始分配，故size=2
    Trie() { size = 2; }
    //返回胜负
    char Win() { return T[1].win; }
    //获取两个二进制序列公共长度，例如二进制101和100返回2
    int GetCommonLen(const ull& x, const ull& y, const int& len_x, const int& len_y) {
        int minlen = len_x < len_y? len_x: len_y;
        for (int i = 1; i <= minlen; i++) {
            int idx_x = (x >> (len_x - i)) & 1;
            int idx_y = (y >> (len_y - i)) & 1;
            if (idx_x != idx_y)
                return i - 1;
        }
        return minlen;
    }
    //获取二进制第[hi, lo]位数字, lo从0计
    ull GetNum(const ull& x, const int& hi, const int& lo) {
        ull ret = 0;
        for (int i = hi; i >= lo; i--) {
            ret = (ret << 1) + ((x >> i) & 1);
        }
        return ret;
    }
	//从节点p开始更新节点，0为Adam胜，1为Eve胜，储存的为最上虚节点胜负（如10010储存1第一个胜负）
    void Update(int p) {
        while (p != 1) {
            p = T[p].parent;
            if (T[T[p].child[0]].win == 0 && T[T[p].child[1]].win == 0) {
                T[p].win = T[p].len % 2;
            }
            else {
                T[p].win = (T[p].len + 1) % 2;
            }
        }
    }
    //在字典树中插入x，为了节约空间，对节点进行压缩
    void Insert(ull x, int len_x) {
        //从根节点开始下行，1为根节点
        int cur = 1;
        //压缩插入，分四种情况
        while (1) {
            //取出插入元素最左位
            int idx = (x >> (len_x - 1)) & 1;
            //如果没有这条边，新建节点（连接边），赋值为当前的x，长度为当前的len_x，父亲为当前节点，胜负值由len长度决定，从此节点开始向上更新，结束循环
            if (!T[cur].child[idx]) {
                T[cur].child[idx] = size;
                T[size].value = x;
                T[size].len = len_x;
                T[size].parent = cur;
				T[size].win = T[size].len % 2;
                Update(size);
                size++;
                break;
            }
            //如果有这条边
            else {
                //沿边下行，记录该节点数值，长度，和x的公共长度
                cur = T[cur].child[idx];
                ull node = T[cur].value;
                int len_node = T[cur].len;
                int common_len = GetCommonLen(x, node, len_x, len_node);
				//如果公共长度和x长度相同，说明这个值对整棵树没有任何影响，结束循环
                if (common_len == len_x) {
                    break;
                }
                //如果公共长度和node长度相同，说明应该继续下行，更新x长度，更新x，重新进入循环
				else if (common_len == len_node) {
                    len_x -= len_node;
                    x = GetNum(x, len_x - 1, 0);
                    continue;
                }
                //如果公共长度和x、node长度都不同，说明x和node重合了一部分，应该新建一个节点储存公共部分，新建一个节点储存x余下部分，把当前节点改为node余下部分
                else {
                    //记录公共值、x余下部分、node余下部分、x和node的方向（最左位0or1)
                    ull common = GetNum(x, len_x - 1, len_x - common_len);
                    ull remain_x = GetNum(x, len_x - common_len - 1, 0);
                    ull remain_node = GetNum(node, len_node - common_len - 1, 0);
                    int idx_x = (remain_x >> (len_x - common_len - 1)) & 1;
                    int idx_node = (remain_node >> (len_node - common_len - 1)) & 1;
                    //新建一个节点储存公共部分，完成连接
                    T[size].value = common;
                    T[size].len = common_len;
                    T[size].parent = T[cur].parent;
                    T[T[size].parent].child[idx] = size;
                    T[size].child[idx_node] = cur;
                    T[size].child[idx_x] = size + 1;
                    size++;
                    //新建一个节点储存x余下部分，完成连接，更新该点胜负值
                    T[size].value = remain_x;
                    T[size].len = len_x - common_len;
                    T[size].parent = size - 1;
					T[size].win = T[size].len % 2;
                    //把当前节点改为node余下部分，完成连接，更新该点胜负值
                    T[cur].value = remain_node;
                    T[cur].len = len_node - common_len;
                    T[cur].parent = size - 1;
					T[cur].win = (T[cur].win - common_len) % 2;
                    //更新size
                    Update(size);
                    size++;
                    break;
                }
            }
        }
    }
};

//十进制 array to int
int atoi(char* s) {
    int len = strlen(s);
    int ret = 0;
    for (int i = 0; i < len; i++) {
        ret = ret * 10 + s[i] - '0';
    }
    return ret;
}

//二进制01 array to unsigned long long
ull atoull(char* s) {
    int len = strlen(s);
    ull ret = 0;
    for (int i = 0; i < len; i++) {
        ret = (ret << 1) + s[i] - '0';
    }
    return ret;
}

//初始化
void init() {
    //初始化
    Trie *t = new Trie;
    int n;
    ull num;
    char s[100];
    //读入n
    gets(s);
    n = atoi(s);
    //记录上一个位置、上一次结果，用于打印
	int pre = 0, pre_ans;
	//第一个特殊处理
    gets(s);
	num = atoull(s);
	t->Insert(num, strlen(s));
	pre_ans = t->Win();
    //循环插入打印
    for (int i = 1; i < n; i++) {
        gets(s);
        num = atoull(s);
        t->Insert(num, strlen(s));
		if (t->Win() != pre_ans) {
			if (pre_ans == 1) printf("Adam %d %d\n", pre + 1, i);
			else printf("Eve %d %d\n", pre + 1, i);
			pre = i;
		}
		pre_ans = t->Win();
    }
	//最后一个特殊处理
	if (pre_ans == 1) printf("Adam %d %d\n", pre + 1, n);
	else printf("Eve %d %d\n", pre + 1, n);
    delete t;
}

int main() {
    setvbuf(stdin, new char[1 << 20], _IOFBF, 1 << 20);
    setvbuf(stdout, new char[1 << 20], _IOFBF, 1 << 20);
    init();
    return 0;
}
