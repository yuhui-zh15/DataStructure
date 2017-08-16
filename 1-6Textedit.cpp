/*************************************************************************
    > File Name: 9.cpp
    > Author: Zhang Yuhui
    > Mail: yuhui-zh15@mails.tsinghua.edu.cn 
    > Created Time: 一  9/26 16:31:42 2016
	> Comment: 本程序最精妙的部分在于对Reverse操作的处理，使得翻转的
	时间复杂度控制在了O(1)，全局存一个is_reverse变量代表是否处于Reve
	rse状态，如果处于reverse状态，则移动指针时进行相应的变换操作（如
	左指针左移等价于把左指针左边元素移动到右指针左边，其余操作同理，
	同样的处于Reverse状态下的删除操作也有所不同（如删除左指针元素等价
	于删除右指针左边元素），打印操作也有所不同，通过状态的标记，使得
	除了打印操作O（n）外，所有操作的时间复杂度均为O（1）。
 ************************************************************************/

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
using namespace std;

//Class Node
template <typename T> struct Node {
	//Constructor
	Node(): pred(NULL), succ(NULL) { }
	Node(const T& data_, Node* pred_ = NULL, Node* succ_ = NULL): data(data_), pred(pred_), succ(succ_) { }
	//Data
	T data;
	Node<T>* pred;
	Node<T>* succ;
	//Interface
	Node<T>* insertAsPred(T const& e) {
		Node<T>* x = new Node<T>(e, pred, this);
		pred->succ = x;
		pred = x;
		return x;
	}
	Node<T>* insertAsSucc(T const& e) {
		Node<T>* x= new Node<T>(e, this, succ);
		succ->pred = x;
		succ = x;
		return x;
	}
};

//Class List
template <typename T> class List {
protected:
	int _size;
	Node<T>* header;
	Node<T>* trailer;
	//Left & right cursor
	Node<T>* l_cursor;
	Node<T>* r_cursor;
	//Is reverse state?
	bool is_reverse;
	//Is left cursor at right cursor right?
	bool is_cursor_reverse;
public:
	//Constructor
	List(): _size(0), is_reverse(false), is_cursor_reverse(false) {
		header = new Node<T>;
		trailer = new Node<T>;
		header->succ = trailer;
		header->pred = NULL;
		trailer->pred = header;
		trailer->succ = NULL;
	}
	//Destructor
	~List() {
		//Not do delete to save time
		/*
		while (_size > 0) {
			remove(header->succ);
		}
		delete header;
		delete trailer;
		*/
	}
	//Quick-build List(Reduce the number of allocation of memory)
	void init(char* seq) {
		int len = strlen(seq);
		if (len == 0) return;
		Node<T>* mem = new Node<T>[len + 10];
		mem[0].data = seq[0];
		mem[0].succ = &mem[1];
		mem[0].pred = header;
		header->succ = &mem[0];
		for (int i = 1; i < len; i++) {
			mem[i].data = seq[i];
			mem[i].succ = &mem[i + 1];
			mem[i].pred = &mem[i - 1];
		}
		mem[len - 1].succ = trailer;
		trailer->pred = &mem[len - 1];
		l_cursor = header->succ;
		r_cursor = trailer;
	}
	//Insert at left
	Node<T>* insertL(Node<T>* p, T const& e) {
		_size++;
		return p->insertAsPred(e);
	}
	//Insert as right
	Node<T>* insertR(Node<T>* p, T const& e) {
		_size++;
		return p->insertAsSucc(e);
	}
	//Remove
	T remove(Node<T>* p) {
		T e = p->data;
		p->pred->succ = p->succ;
		p->succ->pred = p->pred;
		//delete p;
		_size--;
		return e;
	}
	//Move cursor left
	bool MOVE_LEFT(char w) {
		if (w == 'L') {
			if (l_cursor == header->succ) { return false; }
			else {
				if (l_cursor == r_cursor) { is_cursor_reverse = false; }
				if (is_reverse) {
					insertL(r_cursor, remove(l_cursor->pred));
				}
				else {
					l_cursor = l_cursor->pred;
				}
				return true;
			}
		}
		else {
			if (r_cursor == header->succ) { return false; }
			else {
				if (l_cursor == r_cursor->pred) { is_cursor_reverse = true; is_reverse = false; }
				if (is_reverse) {
					l_cursor = l_cursor->succ;
					insertL(r_cursor, remove(l_cursor->pred));
					r_cursor = r_cursor->pred;
				}
				else {
					r_cursor = r_cursor->pred; 
				}
				return true;
			}
		}
	}
	//Move cursor right
	bool MOVE_RIGHT(char w) {
		if (w == 'L') {
			if (l_cursor == trailer) { return false; }
			else {
				if (l_cursor->succ == r_cursor) { is_cursor_reverse = true; is_reverse = false; }
				if (is_reverse) {
					insertL(l_cursor, remove(r_cursor->pred));
				}
				else {
					l_cursor = l_cursor->succ; 
				}
				return true;
			}
		}
		else {
			if (r_cursor == trailer) { return false; }
			else { 
				if (l_cursor == r_cursor) { is_cursor_reverse = false; }
				if (is_reverse) {
					r_cursor = r_cursor->succ;
					insertL(l_cursor, remove(r_cursor->pred));
					l_cursor = l_cursor->pred;
				}
				else {
					r_cursor = r_cursor->succ; 
				}
				return true; 
			}
		}
	}
	//Insert
	bool INSERT(char w, char c) {
		if (w == 'L') {
			insertL(l_cursor, c);
			return true;
		}
		else {
			if (is_reverse) {
				insertL(l_cursor, c);
				l_cursor = l_cursor->pred;
			}
			else {
				insertL(r_cursor, c);
			}
			return true;
		}
	}
	//Delete
	bool DELETE(char w) {
		if (w == 'L') {
			if (l_cursor == trailer) { return false; }
			else { 
				if (is_reverse) {
					if (l_cursor == r_cursor->pred) { l_cursor = l_cursor->succ; is_reverse = false; is_cursor_reverse = true; }
					remove(r_cursor->pred);	
				}
				else {
					bool flag = (l_cursor == r_cursor);
					if (flag) { r_cursor = r_cursor->pred; }
					l_cursor = l_cursor->pred;
					remove(l_cursor->succ);
					if (flag) { r_cursor = r_cursor->succ; }
					l_cursor = l_cursor->succ;
					if (l_cursor == r_cursor) { is_cursor_reverse = true; is_reverse = false; }
				}
				return true; 
			}
		}
		else {
			if (r_cursor == trailer) { return false; }
			else { 
				bool flag = (l_cursor == r_cursor);
				if (flag) { l_cursor = l_cursor->pred; }
				r_cursor = r_cursor->pred;
				remove(r_cursor->succ); 
				if (flag) { l_cursor = l_cursor->succ; }
				r_cursor = r_cursor->succ;
				if (l_cursor == r_cursor) { is_cursor_reverse = true; is_reverse = false; }
				return true; 
			}
		}
	}
	//Reverse
	bool REVERSE() {
		if (is_cursor_reverse) return false;
		is_reverse = !is_reverse;
		return true;
	}
	//Print data
	void SHOW() {
		if (is_reverse) {
			Node<T>* cur = header;
			while (cur != l_cursor->pred) {
				cur = cur->succ;
				printf("%c", cur->data);
			}
			cur = r_cursor;
			while (cur != l_cursor) {
				cur = cur->pred;
				printf("%c", cur->data);
			}
			cur = r_cursor->pred;
			while (cur != trailer->pred) {
				cur = cur->succ;
				printf("%c", cur->data);
			}
		}
		else {
			Node<T>* cur = header;
			while (cur != trailer->pred) {
				cur = cur->succ;
				printf("%c", cur->data);
			}
		}
		printf("\n");
	}	
};

const int maxn = 4000000 + 100;
//The raw input
char seq[maxn];
char cmd[10];
int n;
List<char> list;

void init() {
	bool ret;
	gets(seq);
	list.init(seq);
	gets(cmd);
	n = atoi(cmd);
	for (int i = 0; i < n; i++) {
		cmd[0] = '\0';
		gets(cmd);
		if (strlen(cmd) == 0) continue;
		switch(cmd[0]) {
		case '<': 
			ret = list.MOVE_LEFT(cmd[2]);
			if (ret) printf("T\n");
			else printf("F\n");
			break;
		case '>':
			ret = list.MOVE_RIGHT(cmd[2]);
			if (ret) printf("T\n");
			else printf("F\n");
			break;
		case 'I':
			ret = list.INSERT(cmd[2], cmd[4]);
			if (ret) printf("T\n");
			else printf("F\n");
			break;
		case 'D':
			ret = list.DELETE(cmd[2]);
			if (ret) printf("T\n");
			else printf("F\n");
			break;
		case 'R':
			ret = list.REVERSE();
			if (ret) printf("T\n");
			else printf("F\n");
			break;
		case 'S':
			list.SHOW();
			break;
		}	
	}
}


int main() {
	setvbuf(stdin, new char[1 << 26], _IOFBF, 1 << 26);
	setvbuf(stdout, new char[1 << 20], _IOFBF, 1 << 20);
	init();
	return 0;
}
