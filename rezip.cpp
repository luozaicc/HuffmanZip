//�ļ���ѹ
#include "HuffmanTree.h"
#include <stdio.h>
#include "rezip.h"

#define NAMESIZE 500

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

	if (!i) i++;//Ŀ�����ų�a=0�������
	c[i] = '#';
}

Status ReWrite(HuffmanTree &T, FILE *pr, int n, char *rezipname) {
	char c[9], *s;
	HuffmanTree p = T + 2 * n - 1;
	int a, pre_a, w = 0;//wΪд���ļ���ÿ���ֽڵ�ʮ������
	a = fgetc(pr);
	pre_a = a;
	if (!feof(pr)) NextByte(a, c);
	else NextByte_2(a, c);
	s = c;
	a = fgetc(pr);
	FILE *pw = NULL;
	if (fopen_s(&pw, rezipname, "wb")) {
		printf("�޷��򿪽�ѹ��д���ļ�\n");
		exit(1);
	}
	while (!feof(pr)){
 		if (*s == '0' && p->lchild) p = T + p->lchild;
		else if(*s == '1' && p->rchild) p = T + p->rchild;
		if (!p->lchild && !p->rchild) {
 			if (*s == '0') {
				w = (T + p->parent)->lchild - 1;
				fwrite(&w, 1, 1, pw);
			}
 			else {
				w = (T + p->parent)->rchild - 1;
				fwrite(&w, 1, 1, pw);
			}
 			p = T + 2 * n - 1;//��һ���ֽ�д���ļ� Ҳ���ǽ�����һ�������Ҫ��pָ����ڵ㡣Ҳ�������� T �����һ��Ԫ��
 		}
 		s++;
	 	if (*s == '#'){//���*sΪ '#'˵����ѹ���ļ�����������ֽ��Ѿ�������ɡ���Ҫ�ٶ���һ���ֽ� ��
		 	NextByte(a, c);
			s = c;
			pre_a = a;
			a = fgetc(pr);
		 }
 	}
	a = pre_a;
	NextByte_2(a, c);
	s = c;
 	while (*s != '#') {
	 	if (*s == '0' && p->lchild) p = T + p->lchild;
		else if(*s == '1' && p->rchild) p = T + p->rchild;
		if (!p->lchild && !p->rchild){
 			if (*s == '0') {
				w = (T + p->parent)->lchild - 1;
				fwrite(&w, 1, 1, pw);
			}
 			else {
				w = (T + p->parent)->rchild - 1;
				fwrite(&w, 1, 1, pw);
			}
 			p = T + 2 * n - 1;//��һ���ֽ�д���ļ� Ҳ���ǽ�����һ�������Ҫ��pָ����ڵ㡣Ҳ�������� T �����һ��Ԫ��
 		}
 		s++;
	 }
 	fclose(pw);
 	return OK;
}

static int GetFileName(char *dstName, const char *srcName, FILE *pf)
{
	int rezipNameIndex = 0;
	dstName[rezipNameIndex++] = 'r';
	dstName[rezipNameIndex++] = 'e';
	while (*srcName != '.') {
		dstName[rezipNameIndex++] = *srcName;
		srcName++;
	}
	dstName[rezipNameIndex++] = '.';

	// ��ȡ�ļ�ǰ���ֽڣ���Ϊ��ѹ���ļ��ĺ�׺����
	int readIndex = 0;
	while (readIndex < 4) {
		dstName[rezipNameIndex++] = fgetc(pf);
		readIndex++;
	}
	return rezipNameIndex;
}

/**
 * ���ļ��ж�ȡHuffmanTree������HT����
 * n ΪҶ�ڵ�����
 */
void ReadHuffmanTree(HuffmanTree &HT, FILE* pr, int n)
{
	int a = 0, index = 0, m = 2 * n - 1;
	HT = (HuffmanTree)malloc(sizeof(HTNode) * (m + 1));
	if (!HT) {
		printf("�ڴ治�㣬����ʧ�ܣ�");
		exit(OVERFLOW);
	}
	for (index = 0; index <= n; index++) {
		//��ʱ����ҪȨ�أ����Կ�����㸳ֵ��
		HT[index] = { 0, 0, 0, 0 };
	}

	// ��ȡ�ļ� ������������
	for (index = n + 1; index <= m; index++) {
		HT[index].weight = 0;//��ʱ����ҪȨ�أ����Կ�����㸳ֵ��
		fread(&a, 2, 1, pr);
		HT[index].lchild = a;
		HT[a].parent = index;
		fread(&a, 2, 1, pr);
		HT[index].rchild = a;
		HT[a].parent = index;
	}
	HT[m].parent = 0;
	// ��ʱHTΪ����������
}

void rezip(const char*zipName)
{
	//s	Ϊѹ���ļ��ĵ�ַ���ļ�����
	printf("�����ļ���ѹ\n");
	FILE *pr;
	int n = 256;
	HuffmanTree HT;
	char rezipName[NAMESIZE] = { 0 };
	if(fopen_s(&pr, zipName, "rb")) {
		printf("�޷��򿪽�ѹ�ļ�\n");
		exit(1);
	}
	GetFileName(rezipName, zipName, pr);

	ReadHuffmanTree(HT, pr, n);

	ReWrite(HT, pr, n, rezipName);//��ѹ���ļ���ѹ
	fclose(pr);
	printf("��ѹ�ɹ���\n");
}
