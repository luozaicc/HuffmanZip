//�����ļ�ѹ��
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
	//������д����������� ֻ��Ҫ��¼���������ĵ�257�� 256*2Ԫ�ص����Һ��ӾͿ����ˡ�
	for(int i=n+1;i<=m;i++) {
		fwrite(&((HT+i)->lchild),2,1,pw);
		fwrite(&((HT+i)->rchild),2,1,pw);
	}
	/*
	//��ʱ���������Ѿ�����ѹ���ļ��С�
	˼·�����ļ���ֻҪ�ļ�������󣬾�ѭ���� ���ݶ�����ֵ ascII�� �ҵ���Ӧ��HUffamanCode �ճɰ�λΪһ���ֽ�
	 д���ļ��С� */
	pr=fopen(s,"rb");//���´��ļ������ļ�����ѹ����
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

int main()
{
	const char*s="123.doc";
	zip(s);
	return 0;
}
