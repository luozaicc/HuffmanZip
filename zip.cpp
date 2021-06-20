//�����ļ�ѹ��
#include "zip.h"
#include "HuffmanTree.h"
#include <string>
#define NAMESIZE 500

void zip(const char* s)
{
	//s	ΪҪѹ���ĵ�ַ���ļ�����
	FILE *pr = NULL, *pw = NULL;
	int a, nameIndex = 0, w[256], i, j, n = 256, m = 2 * n - 1;
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
	// ���潫�ļ�����չ���ͻ�����������ѹ���ļ���
	fopen_s(&pw, zipname, "wb");
	// ��д����չ�� ����չ��ռ 4 ���ֽڡ� ����4���ÿո�' '����;
	j = 0;
	while (s[++nameIndex]) {
		fwrite(s + nameIndex, 1, 1, pw);
		j++;
	}
	while (j++ < 4) fwrite(&c, 1, 1, pw);
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
	printf("ѹ���ɹ���\n");
	fclose(pr);
	fclose(pw);
}
