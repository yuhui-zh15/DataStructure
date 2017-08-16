#include <cstdio>
#include <iostream>
#include <cstring>
using namespace std;

const int maxn = 500010;//集合中的数字个数

struct Trie {
    int ch[32 * maxn][2];//节点的边信息
    int value[32 * maxn];//节点边权，记录边重复次数
    int size;//树中当前节点个数
    //树清空，初始时只有一个根节点
    Trie() {
        size = 1;
        ch[0][0] = ch[0][1] = 0;
        memset(value, 0, sizeof(value));
    }
    //在字典树中插入X,和一般字典树的操作相同,将X的二进制插入到字典树中 
    void insert(int x) {   
        int cur = 0;
        for (int i = 31; i >= 0; --i) {
            int idx = (x >> i) & 1;
            if (!ch[cur][idx]) {//节点不存在
                ch[size][0] = ch[size][1] = 0;//分配内存
                ch[cur][idx] = size++;//新建节点
            }
            value[ch[cur][idx]]++;//中间节点附加信息为边重复次数
            cur = ch[cur][idx];//往下走
        }
    }
    //在字典树中查找和X异或的最大值的元素Y,返回Y的值
    int query(int x) {
		int res = 0;
        int cur = 0;
        for (int i = 31; i >= 0; --i) {
            int idx = (x >> i) & 1;
            if (ch[cur][idx ^ 1] && value[ch[cur][idx ^ 1]]) {
				cur = ch[cur][idx ^ 1];//value判断是否被删除
				res = res | ((idx ^ 1) << i);
			}
            else {
				cur = ch[cur][idx];
				res = res | (idx << i);
			}
		}
        return res;
    }
    //在字典树中删除X，通过递减value实现
    void remove(int x) {
        int cur = 0;
        for (int i = 31; i >= 0; --i) {
            int idx = (x >> i) & 1;
            value[ch[cur][idx]]--;
            cur = ch[cur][idx];
        }
    }
};

void init() {
    //初始化
    const long long MOD = 2147483648;
    int* A = new int[maxn];
    int* B = new int[maxn];
    Trie* T = new Trie;
    //读入
    int N, K;
    scanf("%d%d", &N, &K);
    A[0] = B[0] = 0;
    for (register int i = 1; i <= N; i++) { 
        scanf("%d", &A[i]);
        B[i] = B[i - 1] ^ A[i];
    }
    //累加
    long long sum = 0;
    for (int i = 1; i <= K; i++) T->insert(B[i]);
    sum = (sum + T->query(B[0])) % MOD;
    for (int i = 2; i <= N; i++) {
        T->remove(B[i - 1]);
        if (i + K - 1 <= N) T->insert(B[i + K - 1]);
        int q = T->query(B[i - 1]) ^ B[i - 1];
        sum = (sum + q) % MOD;
    }
    //输出
    printf("%lld\n", sum);
    delete []A; delete []B; delete T;
}

int main() {
    setvbuf(stdin, new char[1 << 24], _IOFBF, 1 << 24);
    setvbuf(stdout, new char[1 << 20], _IOFBF, 1 << 20);
    init();
    return 0;
}




