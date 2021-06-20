#ifndef __HUFFMANTREE_H__
#define __HUFFMANTREE_H__

#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>

//�������״̬����
#define  TRUE                 1
#define  FALSE                0
#define   OK                  1
#define  ERROR                0
#define  INFEASIBLE           -1
#define  OVERFLOW             -2
typedef  int Status;
typedef unsigned long UInt32;
typedef struct {
	UInt32 weight;		// �ڵ�Ȩ��
	int parent;			// ˫�׺����Һ��ӽڵ��±�
	int lchild;
	int rchild;
}HTNode, * HuffmanTree, * HTree;

typedef char** HuffmanCode;      //��������������

/**
 * ��ɭ�����ҳ����������Ȩ����С������
 * ���ҵ����������ĸ������±���l��r���ء�
 * ���   T��ɭ�ֽ������
 *        n��ɭ�ֽ�����鳤��
 */
void Select(HuffmanTree T, int n, int& l, int& r);

void StrCopy(char* str, const char* c);

/**
 * ����Ȩ�����飬�������������
 * HuffmanTree& HT��[OUT], ������Ļ�������
 * HuffmanCode& HC��[OUT]����Ӧ�Ļ���������
 * int* w��Ȩ������
 * int n��Ȩ�����鳤��
 */
void HuffmanCoding(HuffmanTree& HT, HuffmanCode& HC, int* w, int n);

/**
 * �������������
 */
void PrintHuffmanCode(HuffmanCode C, int* s, int n);

/**
 * ���ݻ������������������������ַ����н��롣
 */
Status Rewin(HuffmanTree& T, char* s, int* x, int n, int& m);

#endif
