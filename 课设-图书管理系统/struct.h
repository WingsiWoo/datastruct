#ifndef STRUCT_H_

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define m 3		//B���Ľף���Ϊ3 
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define SUCCESS 1
#define FOUND 1
#define NOTFOUND 0
#define OVERFLOW -1

#define Manager_Code 4950  //����Ա����
#define NAME_LEN 20		//��������
#define BKNAME_LEN 100	//��������
#define KEEP_DAYS 90	//��������
#define OUT_DATE_PRICE 0.1	//����ÿ��һëǮ

#define RETURNED 2	//�ѹ黹 
#define NORMAL 1	//���� 
#define OVERDUE 0	//���� 

typedef int KeyType;
typedef int Status;

/*****************************��ͽ����߽��洢����*************************/
typedef struct ReaderNode {
	int ID;                      //ID
	char name[NAME_LEN];		//����
	struct {
		tm borDate;				// ��������
		tm retDate;				// ��������
	};
	ReaderNode *rnext;              //ָ����һ�������ߵ�ָ��
}ReaderNode, *ReaderType;	//�����ߵ�ָ�뼰������� 

typedef struct BookNode {
	int ID;   						//ͼ����
	char bookname[BKNAME_LEN];		//����
	char author[NAME_LEN];			//��������
	int current;					//�ִ���
	int total;						//�ܿ��
	int publishedYear;				//�������
	float price;					//�۸�
	ReaderNode * reader;			//����������ָ��
}BookNode, *BookType;				//ͼ������

typedef BookNode Record;			//ͼ��ݣ�B�����ļ�¼Ϊ�鱾 

typedef struct BTNode{
	int keynum;		//��㵱ǰ�Ĺؼ��ָ��� 
	KeyType key[m+1];	//�ؼ������飬key[0]δ�� 
	struct BTNode * parent;		//˫�׽��ָ�� 
	struct BTNode * ptr[m+1];	//���ӽ��ָ�����飬0�ŵ�Ԫδ�� 
	Record * recptr[m+1];	//��¼ָ��������0�ŵ�Ԫδ�� 
} BTNode, *BTree;	//B���Ľ�㼰ָ������ 

typedef struct {
	struct BTNode * pt;	//ָ���ҵ��Ľ��
	int i;		//1��i��m���ڽ���еĹؼ���λ��
	int tag;	//1�����ҳɹ���0������ʧ�� 
} result, *resultType;	//B���Ĳ��ҽ������

/*******************************B����ز����ӿڶ���***************************/

//��p->key[1..p->keynum]��k 
int Search(BTree p,int k);

/**
 * B���Ĳ��ҽӿ� 
 * ��m��B��T�ϲ��ҹؼ���k����r����(pt,i,tag)
 * �����ҳɹ�������tag=1��ָ��pt��ָ����е�i���ؼ��ֵ���k
 * ����tag=0����Ҫ����ؼ���Ϊk�ļ�¼��Ӧλ��pt����е�i-1���͵�i���ؼ���֮�� 
 */
result Search_BTree(BTree T,KeyType k);

//�����µĸ��ڵ�
void New_Root(BTree &T, BTree p, int k, BTree ap, Record *rec);

//���ѽ�� 
void Split_Node(BTree &q,int s,BTree &ap);

//�ؼ���x���½��ָ��ap�ֱ���뵽q->key[i]��q->ptr[i]
void Insert(BTree &q, int i, int k, BTree &ap, Record *rec);

/**
 * B���Ĳ���ӿ� 
 * ��B��T��q����key[i-1]��key[i]֮�����ؼ���k
 * ���������ؼ��ָ�������B���Ľף�����˫��ָ�������н����ѣ�ʹT����m��B�� 
 */
Status Insert_BTree(BTree &T, KeyType k, BTree q, int i, Record *rec);

//*q���ĵ�i���ؼ���Ϊk����q�ĺ�̹ؼ��ִ���q������qָ�������ڽ��
void Successor(BTree &q, int i);

//ɾ��q��ָ����i���ؼ��ּ����¼
void Remove(BTree &q, int i);

/**
 * ��q���ֵܽ��ؼ��ִ���(m-1)/2,����ֵܽ��������С������󣩵Ĺؼ��ֵ�˫�׽�㣬
 * ����˫�׽����С��(����ڣ��ҽ����ùؼ��ֵĹؼ���������q��,������OK�����򷵻�EREOR��
 */
Status Borrow(BTree &q);

//�ϲ���� 
void Merge(BTree &q);

/**
 * ��m��B��T��ɾ���ؼ���k�����Ӧ��¼��������OK��
 * ��T�ϲ����ڹؼ���k���򷵻�ERROR��
 */
Status Delete_BTree(BTree &T, KeyType k);

// �ݹ��԰������ʽ��ʾB��T,ÿ���������Ϊtab
void Print_BTree(BTree T,int tab);

//�������ʽ��ӡB��
void Print(BTree T);

//B������ 
void BTree_Traverse(BTree T, void(*visit)(BTree));

/*******************************ͼ��ݽӿڶ���***************************/

//�����鼮 
void Insert_Book(BTree &L, BookType B, result res);

//ɾ���鼮 
Status Delete_Book(BTree &L, BookType B);

//�ж��鼮�Ƿ���Գ��� 
Status Can_Borrow(BTree L, BookType B, ReaderType R);

//���� 
void BorrowBook(BTree L, BookType B, ReaderType R);

//���� 
Status ReturnBook(BTree L, int ID, int readerID, BookType &B, ReaderType &R);

//��ӡ���ͷ 
void Print_Head();

//��ӡ������ݣ�ָ���鼮��Ϣ�� 
void Print_Body(BookType B);

//��ӡ���β 
void Print_Tail();

//�Ա����ʽ���һ�������Ϣ
void Print_Book(BookType B);

//��ʾB��ÿ�����ļ�¼��Ϣ
void Print_BookNode(BTree p);

//��ʾ���L����ͼ����Ϣ
void Print_All(BTree L);

//�Ա����ʽ��ʾһ����Ľ�������Ϣ
void Print_Borrower(BookType B);

//���ָ��ĳ�������Ϣ
Status Show_Book_info(BTree L, int ID);

//���ѡ��˵�
int menu();

/******************************ϵͳʱ��ӿڶ���**************************/

//��ȡϵͳʱ�䣬����tm�ṹ�����tim
void GetDate(tm &tim);

//��date1�����ڼ�day��󸳸�date2
void AddDate(tm &date2, tm date1, int day);

/**
 * ���ȱȽ�borDate��retDate�Ƿ�һ�£���Ϊ����ʱ���������ʱ����Ϊһ����Ϊ�ѹ黹�ı�־��
 * ���һ���򷵻�RETURNED��ʾ�ѹ黹
 * �����һ�����ȡϵͳ��ǰʱ��today����retDate�Ƚ�
 * ���today����/����retDate�򷵻�NORMAL�����򷵻�OVERDUE 
 */
int	Earlier(tm borDate, tm retDate);


#endif
