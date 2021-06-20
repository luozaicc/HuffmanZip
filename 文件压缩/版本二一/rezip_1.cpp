//�ļ���ѹ
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

	if(!i)i++;//Ŀ�����ų�a=0�������
	c[i]='#';
}
Status ReWrite(HuffmanTree &T,FILE*pr,int n,char*rezipname)
{
	//
	char c[9],*s;HuffmanTree p=T+2*n-1;int a,pre_a,w=0;//wΪд���ļ���ÿ���ֽڵ�ʮ������
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
 			p=T+2*n-1;//��һ���ֽ�д���ļ� Ҳ���ǽ�����һ�������Ҫ��pָ����ڵ㡣Ҳ�������� T �����һ��Ԫ��
 		}
 		s++;
	 	if(*s=='#'){//���*sΪ '#'˵����ѹ���ļ�����������ֽ��Ѿ�������ɡ���Ҫ�ٶ���һ���ֽ� ��
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
 			p=T+2*n-1;//��һ���ֽ�д���ļ� Ҳ���ǽ�����һ�������Ҫ��pָ����ڵ㡣Ҳ�������� T �����һ��Ԫ��
 		}
 		s++;
	 }
 	fclose(pw);
 	return OK;
}
void rezip(const char*s)
{
	//s	Ϊѹ���ļ��ĵ�ַ���ļ�����
	printf("�����ļ���ѹ\n");
	FILE*pr,*pw;
	int a,i=0,j,n=256,m=2*n-1;UInt32 w[256];
	HuffmanCode HC,hc;HuffmanTree HT;
	char rezipname[NAMESIZE]="re";
	i=0;
	while(s[i]!='.'){rezipname[2+i]=s[i];i++;}
	rezipname[2+i]='.';i++;

	pr=fopen(s,"rb");
	if(!pr){
		printf("�޷��򿪽�ѹ�ļ�\n");exit(1);
	}
	j=0;a=fgetc(pr);
	while(j<4){
		rezipname[2+i]=a;if(j<3)a=fgetc(pr);j++;i++;
	}
	HT=(HuffmanTree)malloc(sizeof(HTNode)*(m+1));
	HT[0].weight=0;HT[0].parent=0;HT[0].lchild=0;HT[0].rchild=0;
	for(i=1;i<=n;i++){
		HT[i].weight=0;//��ʱ����ҪȨ�أ����Կ�����㸳ֵ��
		HT[i].lchild=0;
		HT[i].rchild=0;
	}

	for(i=n+1;i<=m;i++)
	{
		HT[i].weight=0;//��ʱ����ҪȨ�أ����Կ�����㸳ֵ��
		fread(&a,2,1,pr);HT[i].lchild=a;HT[a].parent=i;
		fread(&a,2,1,pr);HT[i].rchild=a;HT[a].parent=i;
	}
	HT[m].parent=0;
	//��ʱHTΪ����������

	ReWrite(HT,pr,n,rezipname);//��ѹ���ļ���ѹ
	fclose(pr);
	printf("��ѹ�ɹ���\n");
}
int main()
{
	const char*str2="zip123.txt";
	rezip(str2);
	return 0;
}
