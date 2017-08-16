/*************************************************************************
    > File Name: Stock.cpp
    > Author: Zhang Yuhui
    > Mail: yuhui-zh15@mails.tsinghua.edu.cn 
    > Created Time: 三 10/12 18:31:18 2016
 ************************************************************************/

#include <iostream>
#include <cstdio>
#include <cstring>
using namespace std;

const int maxn = 1000010;

//节点类
struct Node {
	int num, no, succ, pred;//可购买股票数量、节点序号、前驱、后继
	void init() { num = no = pred = succ = 0; }
};

//队列类
struct Queue {
    Node Q[maxn];//数组模拟队列
	int size;//队列大小，分配内存
    int header;//头哨兵
    int trailer;//尾哨兵
	int cnt_in;//加入队列序号
	int cnt_out;//弹出队列序号
    long long sum;//总股票数
	Queue() {
        sum = size = cnt_in = cnt_out = 0;
        //1号为header，2号为trailer
        header = ++size;
        trailer = ++size;
        //初始化
        Q[header].init();
        Q[header].succ = trailer;
        Q[header].num = 2147483647;//头哨兵赋为最大值避免被删
        Q[trailer].init();
        Q[trailer].pred = header;
	}
	void Enqueue(int D, int X) {
		//求和
		sum += (long long)(Q[Q[header].succ].num) * D;
		//插入队列
		size++;
		cnt_in++;
        //赋值，建立连接
        Q[size].init();
        Q[size].no = cnt_in;
        Q[size].num = X;
        Q[size].pred = Q[trailer].pred;
        Q[size].succ = trailer;
        Q[Q[size].succ].pred = size;
        Q[Q[size].pred].succ = size;
		//删除不如新插入元素优的前驱
		while (Q[size].num >= Q[Q[size].pred].num) {
			Q[size].pred = Q[Q[size].pred].pred;
            Q[Q[size].pred].succ = size;
		}
	}
	void Dequeue(long long D) {
		//求和
		sum += (long long)(Q[Q[header].succ].num) * D;
		//弹出，如果头结点序号等于当前删除序号时
		cnt_out++;
		if (Q[Q[header].succ].no == cnt_out) {
            Q[header].succ = Q[Q[header].succ].succ;
            Q[Q[header].succ].pred = header;
		}
	}
};

//字符串转整数
inline int atoi(char* s) {
	int len = strlen(s);
	int ret = 0;
	for (register int i = 0; i < len; i++) {
		ret = ret * 10 + s[i] - '0';
	}
	return ret;
}

inline void init() {
    //初始化
	Queue *queue = new Queue;
    long long N, D, X;
    //读入N
	char s[100];
	gets(s);
	N = atoi(s);
	//读入指令
	for (int i = 0; i < (N << 1); i++) {
		//处理输入序列，判断加入队列或删除队列
		gets(s);
		int len = strlen(s);
		int j = 0;
		while (j < len && s[j] != ' ') j++;
		if (j != len) {
			X = atoi(s + j + 1);
			s[j] = '\0';
			D = atoi(s);
			queue->Enqueue(D, X);
		}
		else {
			D = atoi(s);
			queue->Dequeue(D);
		}
	}
	printf("%lld\n", queue->sum);
	delete queue;
}

int main() {
	setvbuf(stdin, new char[1 << 24], _IOFBF, 1 << 24);
    setvbuf(stdout, new char[1 << 20], _IOFBF, 1 << 20);
	init();
	return 0;
}
