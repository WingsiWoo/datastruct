#ifndef STRUCT_H_

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define m 3		//B树的阶，设为3 
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define SUCCESS 1
#define FOUND 1
#define NOTFOUND 0
#define OVERFLOW -1

#define Manager_Code 4950  //管理员密码
#define NAME_LEN 20		//姓名长度
#define BKNAME_LEN 100	//书名长度
#define KEEP_DAYS 90	//借阅天数
#define OUT_DATE_PRICE 0.1	//超期每天一毛钱

#define RETURNED 2	//已归还 
#define NORMAL 1	//正常 
#define OVERDUE 0	//逾期 

typedef int KeyType;
typedef int Status;

/*****************************书和借阅者结点存储定义*************************/
typedef struct ReaderNode {
	int ID;                      //ID
	char name[NAME_LEN];		//姓名
	struct {
		tm borDate;				// 借书日期
		tm retDate;				// 还书日期
	};
	ReaderNode *rnext;              //指向下一个借阅者的指针
}ReaderNode, *ReaderType;	//借阅者的指针及结点类型 

typedef struct BookNode {
	int ID;   						//图书编号
	char bookname[BKNAME_LEN];		//书名
	char author[NAME_LEN];			//作者名字
	int current;					//现存量
	int total;						//总库存
	int publishedYear;				//出版年份
	float price;					//价格
	ReaderNode * reader;			//借阅者链表指针
}BookNode, *BookType;				//图书类型

typedef BookNode Record;			//图书馆（B树）的记录为书本 

typedef struct BTNode{
	int keynum;		//结点当前的关键字个数 
	KeyType key[m+1];	//关键字数组，key[0]未用 
	struct BTNode * parent;		//双亲结点指针 
	struct BTNode * ptr[m+1];	//孩子结点指针数组，0号单元未用 
	Record * recptr[m+1];	//记录指针向量，0号单元未用 
} BTNode, *BTree;	//B树的结点及指针类型 

typedef struct {
	struct BTNode * pt;	//指向找到的结点
	int i;		//1≤i≤m，在结点中的关键字位序
	int tag;	//1：查找成功，0：查找失败 
} result, *resultType;	//B树的查找结果类型

/*******************************B树相关操作接口定义***************************/

//在p->key[1..p->keynum]找k 
int Search(BTree p,int k);

/**
 * B树的查找接口 
 * 在m阶B树T上查找关键字k，用r返回(pt,i,tag)
 * 若查找成功，则标记tag=1，指针pt所指结点中第i个关键字等于k
 * 否则tag=0，若要插入关键字为k的记录，应位于pt结点中第i-1个和第i个关键字之间 
 */
result Search_BTree(BTree T,KeyType k);

//生成新的根节点
void New_Root(BTree &T, BTree p, int k, BTree ap, Record *rec);

//分裂结点 
void Split_Node(BTree &q,int s,BTree &ap);

//关键字x和新结点指针ap分别插入到q->key[i]和q->ptr[i]
void Insert(BTree &q, int i, int k, BTree &ap, Record *rec);

/**
 * B树的插入接口 
 * 在B树T中q结点的key[i-1]和key[i]之间插入关键字k
 * 若插入后结点关键字个数等于B树的阶，则沿双亲指针链进行结点分裂，使T仍是m阶B树 
 */
Status Insert_BTree(BTree &T, KeyType k, BTree q, int i, Record *rec);

//*q结点的第i个关键字为k，用q的后继关键字代替q，且令q指向后继所在结点
void Successor(BTree &q, int i);

//删除q所指结点第i个关键字及其记录
void Remove(BTree &q, int i);

/**
 * 若q的兄弟结点关键字大于(m-1)/2,则从兄弟结点上移最小（或最大）的关键字到双亲结点，
 * 而将双亲结点中小于(或大于）且紧靠该关键字的关键字下移至q中,并返回OK，否则返回EREOR。
 */
Status Borrow(BTree &q);

//合并结点 
void Merge(BTree &q);

/**
 * 在m阶B树T上删除关键字k及其对应记录，并返回OK。
 * 如T上不存在关键字k，则返回ERROR。
 */
Status Delete_BTree(BTree &T, KeyType k);

// 递归以凹入表形式显示B树T,每层的缩进量为tab
void Print_BTree(BTree T,int tab);

//凹入表形式打印B树
void Print(BTree T);

//B树遍历 
void BTree_Traverse(BTree T, void(*visit)(BTree));

/*******************************图书馆接口定义***************************/

//新增书籍 
void Insert_Book(BTree &L, BookType B, result res);

//删除书籍 
Status Delete_Book(BTree &L, BookType B);

//判断书籍是否可以出借 
Status Can_Borrow(BTree L, BookType B, ReaderType R);

//借书 
void BorrowBook(BTree L, BookType B, ReaderType R);

//还书 
Status ReturnBook(BTree L, int ID, int readerID, BookType &B, ReaderType &R);

//打印表格头 
void Print_Head();

//打印表格内容（指定书籍信息） 
void Print_Body(BookType B);

//打印表格尾 
void Print_Tail();

//以表格形式输出一本书的信息
void Print_Book(BookType B);

//显示B树每个结点的记录信息
void Print_BookNode(BTree p);

//显示书库L所有图书信息
void Print_All(BTree L);

//以表格形式显示一本书的借阅者信息
void Print_Borrower(BookType B);

//输出指定某本书的信息
Status Show_Book_info(BTree L, int ID);

//输出选择菜单
int menu();

/******************************系统时间接口定义**************************/

//获取系统时间，赋予tm结构体变量tim
void GetDate(tm &tim);

//把date1的日期加day天后赋给date2
void AddDate(tm &date2, tm date1, int day);

/**
 * 首先比较borDate和retDate是否一致（因为还书时会把这两个时间设为一致作为已归还的标志）
 * 如果一致则返回RETURNED表示已归还
 * 如果不一致则获取系统当前时间today并与retDate比较
 * 如果today早于/等于retDate则返回NORMAL，否则返回OVERDUE 
 */
int	Earlier(tm borDate, tm retDate);


#endif
