#include "HuffmanTree.h"
#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>

/**
 * 从森林中找出两个根结点权重最小的树。
 * 将找到的两棵树的根结点的下标用l和r带回。
 * 入参   T：森林结点数组
 *        n：森林结点数组长度
 */
void Select(HuffmanTree T, int n, int& l, int& r)
{
	//从T的一到n个节点中找出没有结合（即parent=0）的节点中权重weight最小的两个节点的下标；用l，r带回；
	HuffmanTree p = T + 1; 
	bool bLeft = false, bRight = false;
	for (int i = 1; i <= n; ++i, ++p) {
		if (!p->parent) {//找出双亲节点为空的节点；
			if (!bLeft) {
				l = i;
				bLeft = true;
			}
			else if (!bRight) {
				r = i;
				bRight = true;
			}
			else if (p->weight < (T + l)->weight || p->weight < (T + r)->weight) {
				if ((T + l)->weight <= (T + r)->weight) {
					r = i;
				}
				else {
					l = i;
				}
			}
		}
	}
}

void StrCopy(char* str, const char* c)
{
	char* s = str;
	while (*c != '\0') *s++ = *c++;
	*s = '\0';
}

void HuffmanCoding(HuffmanTree& HT, HuffmanCode& HC, int* w, int n)
{
	int f, c, start, m1, m2, i, m = 2 * n - 1;
	HuffmanTree p;
	// 申请霍夫曼树内存空间
	HT = (HuffmanTree)malloc(sizeof(HTNode) * (m + 1));
	if (!HT) {
		printf("内存不足，操作失败！");
		exit(OVERFLOW);
	}
	// 初始化霍夫曼树节点
	for (p = HT + 1, i = 1; i <= n; ++i, ++w, ++p) {
		p->weight = *w;
		p->parent = 0;
		p->lchild = 0;
		p->rchild = 0;
	}
	for (i = n + 1; i <= m; ++i, ++p) {
		p->weight = 0;
		p->parent = 0;
		p->lchild = 0;
		p->rchild = 0;
	}
	// 构造霍夫曼树
	for (i = n + 1, p = HT + i; i <= m; ++i, ++p) {
		Select(HT, i - 1, m1, m2);
		p->weight = (HT + m1)->weight + (HT + m2)->weight;
		p->lchild = m1;
		p->rchild = m2;
		(HT + m1)->parent = i;
		(HT + m2)->parent = i;
	}

	//for(i=1;i<=m;i++)
	//printf("%d  %d  %d  %d  ",HT[i].weight,HT[i].parent,HT[i].lchild,HT[i].rchild);
	// 求对应的霍夫曼编码
	HC = (HuffmanCode)malloc(sizeof(char*) * (n + 1));
	if (!HC) {
		printf("内存不足，操作失败！");
		exit(OVERFLOW);
	}
	char* szCodeTmp = (char*)malloc(sizeof(char) * n);
	if (!szCodeTmp) {
		printf("内存不足，操作失败！");
		exit(OVERFLOW);
	}
	szCodeTmp[n - 1] = '\0';
	for (i = 1; i <= n; ++i) {
		start = n - 1;
		//叶到根逆向求编码
		for (f = HT[i].parent, c = i; f != 0; c = f, f = HT[f].parent) {
			if (HT[f].lchild == c) szCodeTmp[--start] = '0';
			else szCodeTmp[--start] = '1';
		}
		HC[i] = (char*)malloc(sizeof(char) * (n - start));
		StrCopy(HC[i], szCodeTmp + start);
	}
	free(szCodeTmp);
}

/**
 * 输出霍夫曼编码
 */
void PrintHuffmanCode(HuffmanCode C, int* s, int n)
{
	char* p;
	for (int i = 1; i <= n; ++i) {
		printf("权重：%3d，编码：", s[i - 1]);
		p = C[i];
		while (*p != '\0')
			printf("%c", *p++);
		printf("\n");
	}
}

/**
 * 根据霍夫曼树，将霍夫曼编码后的字符进行解码。
 */
Status Rewin(HuffmanTree& T, char* s, int* x, int n, int& m)
{
	//根据huffman树，将s所指向的字符串解码,要求字符串以#结束。用 x 带回解码在数组中的下标, x 从 1 开始。
	HuffmanTree p = T + 2 * n - 1; m = 0;
	while (*s != '#') {
		if (*s == '0' && p->lchild) p = T + p->lchild;
		else if (*s == '1' && p->rchild) p = T + p->rchild;

		if (!p->lchild && !p->rchild) {
			if (*s == '0') *x = (T + p->parent)->lchild;
			else *x = (T + p->parent)->rchild;
			x++;
			p = T + 2 * n - 1;
			m++;
		}
		s++;
	}
	return OK;
}
/*
int main()
{
	int s[20]={7,3,5,4,9},x[20],n=5,m;
	char*c="001110111010#";
	HuffmanCode HC;HuffmanTree HT;
	HuffmanCoding(HT,HC,s,n);PrintHuffmanCode(HC,n);
	if(Rewin(HT,c,x,n,m))//x为每个字符在s中的下标+1；
	for(int i=0;i<m;i++)printf("%d",s[x[i]-1]);
}*/
