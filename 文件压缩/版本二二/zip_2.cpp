#include "../HuffmanTree.h"
#define NAMESIZE 500

void zip(const char*s)
{
	//s	ΪҪѹ���ĵ�ַ���ļ����� 
	FILE*pr,*pw;	
	int a,i=0 ,w[256],j,n=256,m=2*n-1;
	HuffmanCode HC,hc;HuffmanTree HT;
	char zipname[NAMESIZE]="zip",c=' ';//ѹ�����ѹ���ļ���Ϊzip+ԭ�ļ���+.txt. 
	while(s[i]!='.'){zipname[3+i]=s[i];i++;}
	zipname[3+i]='.';zipname[4+i]='t';zipname[5+i]='x';zipname[6+i]='t';zipname[7+i]='\0';


	pr=fopen(s,"rb");

	if(!pr){
		printf("�ļ���ʧ�ܣ�");exit(1);
	}
	for(int i=0;i<256;i++)w[i]=0;
	a=fgetc(pr);
	while(!feof(pr))
	{
		w[a]++;a=fgetc(pr);
	}
	fclose(pr);

	HuffmanCoding(HT,HC,w,256);//����������飬���潫�ļ�����չ���ͻ�����������ѹ���ļ��У�
	 
	pw=fopen(zipname,"wb");
	


	//��д����չ�� ����չ��ռ 4 ���ֽڡ� ����4���ÿո�' '����; 
	j=0;
	while(s[++i]){
		fwrite(s+i,1,1,pw);j++;
	}
	while(j++<4)fwrite(&c,1,1,pw);
	//������д����������� 
	for(int i=n+1;i<=m;i++) {
		fwrite(&((HT+i)->lchild),2,1,pw);
		fwrite(&((HT+i)->rchild),2,1,pw);
	}
	//��ʱ���������Ѿ�����ѹ���ļ��С� 
	
	pr=fopen(s,"rb");//���´��ļ������ļ���ѹ���� 
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
	printf("ѹ���ɹ���\n");
	
	fclose(pr);fclose(pw);
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
	const char*s="mystring.txt";
	const char*str2="zipmystring.txt";
	zip(s);
	rezip(str2);
	return 0; 
}