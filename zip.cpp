//�����ļ�ѹ��
#include "zip.h"
#include "HuffmanTree.h"
#include <string>
#define NAMESIZE 500

static void print_zip_huffmancode(HuffmanCode &HC)
{
	int a[255] = { 0 };
	for (int i = 0; i < 255; ++i) {
		a[i] = i;
	}
	PrintHuffmanCode(HC, a, 255);
}

/**
 * ���sΪҪѹ�����ļ�����
 * ѹ������ļ����ƶ���Ϊ��zip+ԭ�ļ���+.txt
 * �ļ����ݣ�
 * 1byte�����һ�ֽ���Чbit���ȣ�4bit�� + Դ�ļ���׺�����ȣ�4bit��
 * Դ�ļ���׺�����ݳ���
 * 510byte�Ļ�������
 */
void zip(const char* s)
{
	//s	ΪҪѹ���ĵ�ַ���ļ�����
	FILE *pr = NULL, *pw = NULL;
	int a, nameIndex = 0, w[256], i, n = 256, m = 2 * n - 1;
	char firstByte = 0;
	HuffmanCode HC, hc;
	HuffmanTree HT;
	char zipname[NAMESIZE] = "zip", c = ' ';//ѹ�����ѹ���ļ���Ϊzip+ԭ�ļ���+.txt.
	const char* suffix = ".txt";
	while (s[nameIndex] != '.') {
		zipname[3 + nameIndex] = s[nameIndex];
		nameIndex++;
	}
	strncpy_s(zipname + 3 + nameIndex, 100, suffix, strlen(suffix));

	if (fopen_s(&pr, s, "rb")) {
		printf("�ļ���ʧ�ܣ�");
		exit(1);
	}
	for (i = 0; i < 256; ++i) {
		w[i] = 0;
	}
	a = fgetc(pr);
	while (!feof(pr)) {
		w[a]++;
		a = fgetc(pr);
	}
	fclose(pr);

	HuffmanCoding(HT, HC, w, 256);//�����������
	print_zip_huffmancode(HC);
	
	// ��ѹ���ļ�
	if (fopen_s(&pw, zipname, "wb")) {
		printf("�ļ�%s��ʧ�ܣ�", zipname);
		exit(1);
	}

	// ������д���һ�ֽڣ��������һ�ֽ���Чbit����Ŀǰ����֪��������ռλ
	fwrite(&firstByte, 1, 1, pw);

	// ���潫�ļ�����չ���ͻ�����������ѹ���ļ���
	// ��д����չ��
	while (s[++nameIndex]) {
		fwrite(s + nameIndex, 1, 1, pw);
		firstByte++;
	}
	// ������д�����������ֻ��Ҫ��¼���������ĵ�257�� 256*2Ԫ�ص����Һ��ӾͿ����ˡ�
	for (i = n + 1; i <= m; i++) {
		fwrite(&((HT + i)->lchild), 2, 1, pw);
		fwrite(&((HT + i)->rchild), 2, 1, pw);
	}
	/*
	 * ˼·�����ļ���ֻҪ�ļ�������󣬾�ѭ���� 
	 * ���ݶ�����ֵ ascII�� �ҵ���Ӧ��HUffamanCode �ճɰ�λΪһ���ֽ�д���ļ��С�
	 */
	fopen_s(&pr, s, "rb");//���´��ļ������ļ�����ѹ����
	
	hc = HC;
	int byteValue = 0;
	int bitIndex = 7;
	a = fgetc(pr);
	while (!feof(pr)) {
		char* p = *(hc + a + 1);
		while (*p != '\0') {
			int bit = *p - 48;
			byteValue |= (bit << bitIndex);
			++p;
			if (0 == bitIndex) {
				fwrite(&byteValue, 1, 1, pw);
				byteValue = 0;
				bitIndex = 7;
				continue;
			}
			--bitIndex;
		}

		a = fgetc(pr);
	}
	if (7 != bitIndex) {
		fwrite(&byteValue, 1, 1, pw);
	}
	// �����޸����ֽڣ����һ�ֽ���Чbit���ȣ�4bit�� + Դ�ļ���׺�����ȣ�4bit��
	c = 7 - bitIndex;
	if (0 == c) c = 8;
	firstByte |= ((c << 4) & 0xF0);
	fseek(pw, 0, 0);
	fwrite(&firstByte, 1, 1, pw);

	printf("ѹ���ɹ���\n");
	fclose(pr);
	fclose(pw);
}
