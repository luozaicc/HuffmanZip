//文件解压
#include "HuffmanTree.h"
#include <stdio.h>
#include "rezip.h"

#define NAMESIZE 500

static void print_zip_huffmancode(HuffmanCode& HC)
{
	int a[255] = { 0 };
	for (int i = 0; i < 255; ++i) {
		a[i] = i;
	}
	PrintHuffmanCode(HC, a, 255);
}

void NextByte(int a, char* b) {
	for (int i = 0; i < 8; i++) {
		if (a) {
			b[7 - i] = 48 + a % 2;
			a = a / 2;
		}
		else b[7 - i] = '0';
	}

	b[8] = '#';
}

void NextByte_2(int a,char*c) {
	char b[8];
	int i = 0;
	c[0] = '0';
	for (i = 0; i < 8; i++) {
		if (a) {
			b[7 - i] = 48 + a % 2;
			a = a / 2;
		}
		else break;
	}
	
	for (int k = 0; k < i; k++)
		c[k] = b[8 - i + k];

	if (!i) i++;//目的是排除a=0的情况；
	c[i] = '#';
}

Status ReWrite(HuffmanTree &T, FILE *pr, int lastByteLen, int n, char *rezipname) {
	char c[9], *s;
	HuffmanTree root = T + 2 * n - 1;
	HuffmanTree p = root;
	int a, pre_a, w = 0;//w为写入文件的每个字节的十进制数
	a = fgetc(pr);
	pre_a = a;
	if (!feof(pr)) NextByte(a, c);
	else NextByte_2(a, c);
	s = c;
	a = fgetc(pr);
	FILE *pw = NULL;
	if (fopen_s(&pw, rezipname, "wb")) {
		printf("无法打开解压缩写入文件\n");
		exit(1);
	}
	while (!feof(pr)){
 		if (*s == '0' && p->lchild) p = T + p->lchild;
		else if(*s == '1' && p->rchild) p = T + p->rchild;
		if (!p->lchild && !p->rchild) {
			w = p - T - 1;
			fwrite(&w, 1, 1, pw);
 			p = root;//将一个字节写入文件 也就是解密了一个编码后，要将p指向根节点。也就是数组 T 的最后一个元素
 		}
 		s++;
	 	if (*s == '#'){//如果*s为 '#'说明从压缩文件读出个这个字节已经解码完成。需要再读入一个字节 ；
		 	NextByte(a, c);
			s = c;
			pre_a = a;
			a = fgetc(pr);
		 }
 	}
	a = pre_a;
	NextByte_2(a, c);
	s = c;
 	while (*s != '#' && lastByteLen-- > 0) {
	 	if (*s == '0' && p->lchild) p = T + p->lchild;
		else if(*s == '1' && p->rchild) p = T + p->rchild;
		if (!p->lchild && !p->rchild){
			w = p - T - 1;
			fwrite(&w, 1, 1, pw);
			p = root;//将一个字节写入文件 也就是解密了一个编码后，要将p指向根节点。也就是数组 T 的最后一个元素
 		}
 		s++;
	 }
 	fclose(pw);
 	return OK;
}

static int GetFileName(char *dstName, const char *srcName, int extNameLen, FILE *pf)
{
	int rezipNameIndex = 0;
	dstName[rezipNameIndex++] = 'r';
	dstName[rezipNameIndex++] = 'e';
	while (*srcName != '.') {
		dstName[rezipNameIndex++] = *srcName;
		srcName++;
	}
	dstName[rezipNameIndex++] = '.';

	// 读取文件前四字节，作为解压后文件的后缀名称
	int readIndex = 0;
	while (readIndex < extNameLen) {
		dstName[rezipNameIndex++] = fgetc(pf);
		readIndex++;
	}
	return rezipNameIndex;
}

/**
 * 从文件中读取HuffmanTree，并用HT带回
 * n 为叶节点数量
 */
void ReadHuffmanTree(HuffmanTree &HT, FILE* pr, int n)
{
	int a = 0, index = 0, m = 2 * n - 1;
	HT = (HuffmanTree)malloc(sizeof(HTNode) * (m + 1));
	if (!HT) {
		printf("内存不足，操作失败！");
		exit(OVERFLOW);
	}
	for (index = 0; index <= n; index++) {
		//此时不需要权重，所以可以随便赋值。
		HT[index] = { 0, 0, 0, 0 };
	}

	// 读取文件 构建霍夫曼树
	for (index = n + 1; index <= m; index++) {
		HT[index].weight = 0;//此时不需要权重，所以可以随便赋值。
		fread(&a, 2, 1, pr);
		HT[index].lchild = a;
		HT[a].parent = index;
		fread(&a, 2, 1, pr);
		HT[index].rchild = a;
		HT[a].parent = index;
	}
	HT[m].parent = 0;
	// 此时HT为霍夫曼树，
}

void rezip(const char*zipName)
{
	//s	为压缩文件的地址和文件名。
	printf("进行文件解压\n");
	FILE *pr;
	int n = 256, lastByteLen;
	HuffmanTree HT;
	char rezipName[NAMESIZE] = { 0 };
	char firstByte;
	if(fopen_s(&pr, zipName, "rb")) {
		printf("无法打开解压文件\n");
		exit(1);
	}
	fread(&firstByte, 1, 1, pr);
	lastByteLen = firstByte >> 4;
	GetFileName(rezipName, zipName, firstByte & 0x0F, pr);

	ReadHuffmanTree(HT, pr, n);

	ReWrite(HT, pr, lastByteLen, n, rezipName);//将压缩文件解压
	fclose(pr);
	printf("解压成功！\n");
}
