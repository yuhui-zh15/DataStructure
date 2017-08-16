/*************************************************************************
    > File Name: StackSort.cpp
    > Author: Zhang Yuhui
    > Mail: yuhui-zh15@mails.tsinghua.edu.cn 
    > Created Time: 二  9/17 11:38:04 2016
 ************************************************************************/

#include <iostream>
#include <cstdio>
#include <cstring>
using namespace std;

const int maxn = 2000000 + 100;

//栈模板类
template <typename T> class Stack {
private:
	int _size;
	T* _elem;
public:
	Stack(int capacity) {
		_elem = new T[capacity];
		_size = 0;
	}
	~Stack() {
		delete []_elem;
	}
	T& at(int index) {
		return _elem[index];
	}
	int size() {
		return _size;
	}
	bool empty() {
		return  !(bool)_size;
	}
	T& top() {
		return _elem[_size - 1];
	}
	T pop() {
		_size--;
		return _elem[_size];
	}
	void push(const T& m) {
		_elem[_size] = m;
		_size++;
	}
};

//运行
void run(int n, Stack<int> *s, Stack<int> *ans, int *a, char *exist) {
	int cur = 0;//当前元素
	int now_n = n;//记录栈最大值
	while (cur < n) {
		while (exist[now_n] == 0) {
			exist[a[cur]] = 1;
			s->push(a[cur++]);
		}
	
		if (exist[now_n] == 1 && now_n == s->top()) {
			ans->push(s->pop());
		}

		while (s->top() >= now_n) {
			ans->push(s->pop());
		}
		now_n--;
	}

	for (register int i = 0; i < ans->size(); i++) {
		printf("%d ", ans->at(i));
	}
	for (register int i = s->size() - 1; i >= 0; i--) {
		printf("%d ", s->at(i));
	}

}

//初始化
void init() {
	//建立
	int n;//序列长度
	Stack<int> *s = new Stack<int>(maxn), *ans = new Stack<int>(maxn);//中转栈和结果栈
	int *a = new int[maxn];//读入序列
	char *exist = new char[maxn];//记录栈中是否存在某个元素
	memset(exist, 0, sizeof(*exist));
	//读入
	scanf("%d", &n);
	for (register int i = 0; i < n; i++) scanf("%d", &a[i]);
	//运行
	run(n, s, ans, a, exist);
	//删除
	delete s;
	delete ans;
	delete []a;
	delete []exist;
}

int main() {
	setvbuf(stdin, new char[1 << 24], _IOFBF, 1 << 24);
	setvbuf(stdout, new char[1 << 24], _IOFBF, 1 << 24);
	init();
	return 0;
}
