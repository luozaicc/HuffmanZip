#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>

//�������״̬����
#define  TRUE                 1
#define  FALSE                0
#define   OK                  1
#define  ERROR                0
#define  INFEASIBLE           -1
#define  OVERFLOW             -2
typedef  int Status;
typedef unsigned long UInt32;
typedef struct {
	 UInt32 weight;             // �ڵ�Ȩ��
	 int parent,lchild,rchild;  // ˫�׺����Һ��ӽڵ��±�
}HTNode,*HuffmanTree,*HTree;

typedef char**HuffmanCode;      //��������������

/**
 * ��ɭ�����ҳ����������Ȩ����С������
 * ���ҵ����������ĸ������±���l��r���ء�
 * ���   T��ɭ�ֽ������
 *        n��ɭ�ֽ�����鳤��
 */
void Select(HuffmanTree T,int n,int&l,int&r)
{
	//��T��һ��n���ڵ����ҳ�û�н�ϣ���parent=0���Ľڵ���Ȩ��weight��С�������ڵ���±ꣻ��l��r���أ�
	HuffmanTree p=T+1;int a=0,b=0;
	for(int i=1;i<=n;++i,++p){
		if(!p->parent){//�ҳ�˫�׽ڵ�Ϊ�յĽڵ㣻
			if(!a){l=i;a=1;}
			else if(!b){r=i;b=1;}
			else if(p->weight<(T+l)->weight||p->weight<(T+r)->weight){
				if((T+l)->weight<=(T+r)->weight)r=i;
				else l=i;
			}
		}
	}
}

void StrCopy(char*str,const char*c)
{
	char *s=str;
	while(*c!='\0')*s++=*c++;
	*s='\0';
}

/**
 *
 */
void HuffmanCoding(HuffmanTree &HT,HuffmanCode &HC,int *w,int n)
{
	int f,c,start,m1,m2,i,m=2*n-1;HuffmanTree p;
	HT=(HuffmanTree)malloc(sizeof(HTNode)*(m+1));
	if(!HT){printf("�ڴ治�㣬����ʧ�ܣ�");exit(OVERFLOW);}
	for(p=HT+1,i=1;i<=n;++i,++w,++p){
		p->weight=*w;p->parent=0;p->lchild=0;p->rchild=0;
	}
	for(i=n+1;i<=m;++i,++p){
		p->weight=0;p->parent=0;p->lchild=0;p->rchild=0;
	}
	for(i=n+1,p=HT+i;i<=m;++i,++p){
		Select(HT,i-1,m1,m2);
		p->weight=(HT+m1)->weight+(HT+m2)->weight;
		p->lchild=m1;p->rchild=m2;
		(HT+m1)->parent=i;(HT+m2)->parent=i;
	}

	//for(i=1;i<=m;i++)
	//printf("%d  %d  %d  %d  ",HT[i].weight,HT[i].parent,HT[i].lchild,HT[i].rchild);

	HC=(HuffmanCode)malloc(sizeof(char*)*(n+1));
	if(!HC){
		printf("�ڴ治�㣬����ʧ�ܣ�");exit(OVERFLOW);
	}
	char *cd=(char*)malloc(sizeof(char)*n);cd[n-1]='\0';
	for(i=1;i<=n;++i){
		start=n-1;
  		for(f=HT[i].parent,c=i; f!=0;c=f,f=HT[f].parent)//Ҷ�������������
      		if(HT[f].lchild==c)cd[--start]='0';  else cd[--start]='1';
		HC[i]=(char*)malloc(sizeof(char)*(n-start));
		StrCopy(HC[i],cd+start);
	}
	free(cd);
}

/**
 * �������������
 */
void PrintHuffmanCode(HuffmanCode C,int* s,int n)
{
	char *p;
	for(int i=1;i<=n;++i){
	    printf("Ȩ�أ�%3d�����룺", s[i-1]);
		p=C[i];
		while(*p!='\0')
		printf("%c",*p++);
		printf("\n");
	}
}

/**
 * ���ݻ������������������������ַ����н��롣
 */
Status Rewin(HuffmanTree &T,char*s,int*x,int n,int &m)
{
	//����huffman������s��ָ����ַ�������,Ҫ���ַ�����#�������� x ���ؽ����������е��±�, x �� 1 ��ʼ��
	HuffmanTree p=T+2*n-1;m=0;
	 while(*s!='#'){
 		 if(*s=='0'&&p->lchild)p=T+p->lchild;
		 else if(*s=='1'&&p->rchild)p=T+p->rchild;
//		 else return FALSE;
		 if(!p->lchild&&!p->rchild){
 			if(*s=='0')*x=(T+p->parent)->lchild;
 			else *x=(T+p->parent)->rchild;
 			x++;p=T+2*n-1;m++;
 		}
 		s++;
 	}
 	return OK;
}
/*
int main()
{
	int s[20]={7,3,5,4,9},x[20],n=5,m;
	char*c="001110111010#";
	HuffmanCode HC;HuffmanTree HT;
	HuffmanCoding(HT,HC,s,n);PrintHuffmanCode(HC,n);
	if(Rewin(HT,c,x,n,m))//xΪÿ���ַ���s�е��±�+1��
	for(int i=0;i<m;i++)printf("%d",s[x[i]-1]);
}*/
