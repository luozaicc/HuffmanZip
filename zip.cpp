//进行文件压缩
#include "zip.h"
#include "HuffmanTree.h"
#include <string>
#define NAMESIZE 500

void zip(const char* s)
{
	//s	为要压缩的地址和文件名。
	FILE *pr = NULL, *pw = NULL;
	int a, nameIndex = 0, w[256], i, j, n = 256, m = 2 * n - 1;
	HuffmanCode HC, hc;
	HuffmanTree HT;
	char zipname[NAMESIZE] = "zip", c = ' ';//压缩后的压缩文件名为zip+原文件名+.txt.
	const char* suffix = ".txt";
	while (s[nameIndex] != '.') {
		zipname[3 + nameIndex] = s[nameIndex];
		nameIndex++;
	}
	strncpy_s(zipname + 3 + nameIndex, 100, suffix, strlen(suffix));

	if (fopen_s(&pr, s, "rb")) {
		printf("文件打开失败！");
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

	HuffmanCoding(HT, HC, w, 256);//求出霍夫曼树
	// 下面将文件的扩展名和霍夫曼树存入压缩文件中
	fopen_s(&pw, zipname, "wb");
	// 先写入扩展名 ，扩展名占 4 个字节。 不足4个用空格' '补齐;
	j = 0;
	while (s[++nameIndex]) {
		fwrite(s + nameIndex, 1, 1, pw);
		j++;
	}
	while (j++ < 4) fwrite(&c, 1, 1, pw);
	// 下面是写入霍夫曼树。只需要记录霍夫曼树的第257到 256*2元素的左右孩子就可以了。
	for (i = n + 1; i <= m; i++) {
		fwrite(&((HT + i)->lchild), 2, 1, pw);
		fwrite(&((HT + i)->rchild), 2, 1, pw);
	}
	/*
	 * 思路，打开文件，只要文件不到最后，就循环。 
	 * 根据读出的值 ascII码 找到对应的HUffamanCode 凑成八位为一个字节写入文件中。
	 */
	fopen_s(&pr, s, "rb");//重新打开文件遍历文件进行压缩。
	
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
	printf("压缩成功！\n");
	fclose(pr);
	fclose(pw);
}
