//�����ļ���ѹ�� 			�������� 
#include "../HuffmanTree.h"
#define NAMESIZE 500

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
		printf("�ļ���ʧ�ܣ�");exit(1);
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
	
	
	int gm=0;
	char aa[80000],b[8];int x[10000],d;a=fgetc(pr);
	/*aa��b�ȿ���һ�����ڴ�ռ䡣�Ƚ��ļ�д���ڴ��� Ȼ����д���ļ�� ����ֻ�ܽ�ѹ�Ƚ�С���ļ��� */
	while(!feof(pr))
	{
			for(int i=0;i<8;i++){
				if(a){b[7-i]=48+a%2;a=a/2;}
				else b[7-i]='0';
			}
			for(int i=0;i<8;i++)
			aa[8*j+i]=b[i];
			j++;d=a;
			a=fgetc(pr);
	}
	for(i=0;i<8;i++)if(a){b[7-i]=48+a%2;a=a/2;}else break;
	for(int k=0;k<i;k++)
		aa[8*j-8+k]=b[8-i+k];
//	for(;i<7;i++)aa[8*j-8+i]='0';
	aa[8*j-8+i]='0';aa[8*j-8+i+1]='#';
	//���潫aa����ı������ 
	Rewin(HT,aa,x,n,gm);
	FILE *fw;
	
	
	fw=fopen(rezipname,"wb");
	for(int i=0;i<gm;i++)
	{x[i]=x[i]-1;fwrite(x+i,1,1,fw);}
	fclose(fw);fclose(pr);
	printf("��ѹ�ɹ���\n");
}
int main()
{
	const char*str2="zip123.txt";
	rezip(str2);
	return 0; 
}