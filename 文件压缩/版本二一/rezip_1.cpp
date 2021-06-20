//文件解压
#include "../HuffmanTree.h"
#define NAMESIZE 500

void NextByte(int a,char*b)
{
	for(int i=0;i<8;i++)
		if(a){b[7-i]=48+a%2;a=a/2;}
		else b[7-i]='0';
	b[8]='#';
}
void NextByte_2(int a,char*c)
{
	char b[8];int i=0;c[0]='0';
	for( i=0;i<8;i++)
		if(a){b[7-i]=48+a%2;a=a/2;}else break;
	for(int k=0;k<i;k++)
		c[k]=b[8-i+k];

	if(!i)i++;//目的是排除a=0的情况；
	c[i]='#';
}
Status ReWrite(HuffmanTree &T,FILE*pr,int n,char*rezipname)
{
	//
	char c[9],*s;HuffmanTree p=T+2*n-1;int a,pre_a,w=0;//w为写入文件的每个字节的十进制数
	a=fgetc(pr);pre_a=a;
	if(!feof(pr))NextByte(a,c);
	else NextByte_2(a,c);
	s=c;a=fgetc(pr);
	FILE*pw=fopen(rezipname,"wb");

	 while(!feof(pr)){
 		if(*s=='0'&&p->lchild)p=T+p->lchild;
		 else if(*s=='1'&&p->rchild)p=T+p->rchild;
		 if(!p->lchild&&!p->rchild){
 			if(*s=='0'){w=(T+p->parent)->lchild-1;fwrite(&w,1,1,pw);}
 			else {w=(T+p->parent)->rchild-1;fwrite(&w,1,1,pw);}
 			p=T+2*n-1;//将一个字节写入文件 也就是解密了一个编码后，要将p指向根节点。也就是数组 T 的最后一个元素
 		}
 		s++;
	 	if(*s=='#'){//如果*s为 '#'说明从压缩文件读出个这个字节已经解码完成。需要再读入一个字节 ；
		 		NextByte(a,c);s=c;pre_a=a;a=fgetc(pr);
		 }
 	}
	 a=pre_a;NextByte_2(a,c);s=c;
 	while(*s!='#')
 	{
	 	if(*s=='0'&&p->lchild)p=T+p->lchild;
		 else if(*s=='1'&&p->rchild)p=T+p->rchild;
		 if(!p->lchild&&!p->rchild){
 			if(*s=='0'){w=(T+p->parent)->lchild-1;fwrite(&w,1,1,pw);}
 			else {w=(T+p->parent)->rchild-1;fwrite(&w,1,1,pw);}
 			p=T+2*n-1;//将一个字节写入文件 也就是解密了一个编码后，要将p指向根节点。也就是数组 T 的最后一个元素
 		}
 		s++;
	 }
 	fclose(pw);
 	return OK;
}
void rezip(const char*s)
{
	//s	为压缩文件的地址和文件名。
	printf("进行文件解压\n");
	FILE*pr,*pw;
	int a,i=0,j,n=256,m=2*n-1;UInt32 w[256];
	HuffmanCode HC,hc;HuffmanTree HT;
	char rezipname[NAMESIZE]="re";
	i=0;
	while(s[i]!='.'){rezipname[2+i]=s[i];i++;}
	rezipname[2+i]='.';i++;

	pr=fopen(s,"rb");
	if(!pr){
		printf("无法打开解压文件\n");exit(1);
	}
	j=0;a=fgetc(pr);
	while(j<4){
		rezipname[2+i]=a;if(j<3)a=fgetc(pr);j++;i++;
	}
	HT=(HuffmanTree)malloc(sizeof(HTNode)*(m+1));
	HT[0].weight=0;HT[0].parent=0;HT[0].lchild=0;HT[0].rchild=0;
	for(i=1;i<=n;i++){
		HT[i].weight=0;//此时不需要权重，所以可以随便赋值。
		HT[i].lchild=0;
		HT[i].rchild=0;
	}

	for(i=n+1;i<=m;i++)
	{
		HT[i].weight=0;//此时不需要权重，所以可以随便赋值。
		fread(&a,2,1,pr);HT[i].lchild=a;HT[a].parent=i;
		fread(&a,2,1,pr);HT[i].rchild=a;HT[a].parent=i;
	}
	HT[m].parent=0;
	//此时HT为霍夫曼树，

	ReWrite(HT,pr,n,rezipname);//将压缩文件解压
	fclose(pr);
	printf("解压成功！\n");
}
int main()
{
	const char*str2="zip123.txt";
	rezip(str2);
	return 0;
}
