//进行文件压缩
#include "../HuffmanTree.h"
#define NAMESIZE 500

void zip(const char*s)
{
	//s	为要压缩的地址和文件名。
	FILE*pr,*pw;
	int a,i=0 ,w[256],j,n=256,m=2*n-1;
	HuffmanCode HC,hc;HuffmanTree HT;
	char zipname[NAMESIZE]="zip",c=' ';//压缩后的压缩文件名为zip+原文件名+.txt.
	while(s[i]!='.'){zipname[3+i]=s[i];i++;}
	zipname[3+i]='.';zipname[4+i]='t';zipname[5+i]='x';zipname[6+i]='t';zipname[7+i]='\0';


	pr=fopen(s,"rb");

	if(!pr){
		printf("文件打开失败！");exit(1);
	}
	for(int i=0;i<256;i++)w[i]=0;
	a=fgetc(pr);
	while(!feof(pr))
	{
		w[a]++;a=fgetc(pr);
	}
	fclose(pr);

	HuffmanCoding(HT,HC,w,256);//求出霍夫曼书，下面将文件的扩展名和霍夫曼树存入压缩文件中；

	pw=fopen(zipname,"wb");



	//先写入扩展名 ，扩展名占 4 个字节。 不足4个用空格' '补齐;
	j=0;
	while(s[++i]){
		fwrite(s+i,1,1,pw);j++;
	}
	while(j++<4)fwrite(&c,1,1,pw);
	//下面是写入霍夫曼树。 只需要记录霍夫曼树的第257到 256*2元素的左右孩子就可以了。
	for(int i=n+1;i<=m;i++) {
		fwrite(&((HT+i)->lchild),2,1,pw);
		fwrite(&((HT+i)->rchild),2,1,pw);
	}
	/*
	//此时霍夫曼树已经存入压缩文件中。
	思路，打开文件，只要文件不到最后，就循环。 根据读出的值 ascII码 找到对应的HUffamanCode 凑成八位为一个字节
	 写入文件中。 */
	pr=fopen(s,"rb");//重新打开文件遍历文件进行压缩。
	a=fgetc(pr);
	i=0;int b=0;hc=HC;char*p=*(hc+a+1);
	while(!feof(pr)||*p!='\0')
	{
		for(i=0;i<8;i++){
			if(*p=='\0'){
				a=fgetc(pr);
				if(a>=0)p=hc[a+1];
				else {
						int h=1;
						for(int j=0;j<8-i;j++)h=h*2;
						b=b*h;break;
					}
			}
			int h=1;
			for(int j=0;j<7-i;j++)h=h*2;
			b=b+(*p-48)*h;
			++p;
		}
		fwrite(&b,1,1,pw);b=0;
	}
	printf("压缩成功！\n");

	fclose(pr);fclose(pw);
}

int main()
{
	const char*s="123.doc";
	zip(s);
	return 0;
}
