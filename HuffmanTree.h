#ifndef __HUFFMANTREE_H__
#define __HUFFMANTREE_H__

#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>

//函数结果状态代码
#define  TRUE                 1
#define  FALSE                0
#define   OK                  1
#define  ERROR                0
#define  INFEASIBLE           -1
#define  OVERFLOW             -2
typedef  int Status;
typedef unsigned long UInt32;
typedef struct {
	UInt32 weight;		// 节点权重
	int parent;			// 双亲和左右孩子节点下标
	int lchild;
	int rchild;
}HTNode, * HuffmanTree, * HTree;

typedef char** HuffmanCode;      //霍夫曼编码类型

/**
 * 从森林中找出两个根结点权重最小的树。
 * 将找到的两棵树的根结点的下标用l和r带回。
 * 入参   T：森林结点数组
 *        n：森林结点数组长度
 */
void Select(HuffmanTree T, int n, int& l, int& r);

void StrCopy(char* str, const char* c);

/**
 * 根据权重数组，构造出霍夫曼树
 * HuffmanTree& HT：[OUT], 构造出的霍夫曼树
 * HuffmanCode& HC：[OUT]，对应的霍夫曼编码
 * int* w：权重数组
 * int n：权重数组长度
 */
void HuffmanCoding(HuffmanTree& HT, HuffmanCode& HC, int* w, int n);

/**
 * 输出霍夫曼编码
 */
void PrintHuffmanCode(HuffmanCode C, int* s, int n);

/**
 * 根据霍夫曼树，将霍夫曼编码后的字符进行解码。
 */
Status Rewin(HuffmanTree& T, char* s, int* x, int n, int& m);

#endif
