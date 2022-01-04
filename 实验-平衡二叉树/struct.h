#ifndef STRUCT_H_

#include <stdio.h>
#include <stdlib.h>

#define LH +1 //左子树比右子树高，简称左高
#define EH 0 //左右子树等高，简称等高 
#define RH -1 //右子树比左子树高，简称右高 
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define SUCCESS 1

typedef int Status;
typedef int RcdType;

//平衡二叉树结构体定义 
typedef struct BBSTNode
{
	RcdType data;
	int bf;					   //平衡因子
	BBSTNode *lchild, *rchild; //左右孩子结点 
} BBSTNode, *BBSTree;

//平衡二叉树相关接口定义

//左旋处理
void L_Rotate(BBSTree &p);

//右旋处理
void R_Rotate(BBSTree &p); 

//左平衡处理
void Left_Balance(BBSTree &T);

//右平衡处理
void Right_Balance(BBSTree &T);

//插入结点
Status Insert_AVL(BBSTree &T, RcdType e, Status &taller);

//根据输入的数据创建平衡二叉树 
BBSTree Create_BBSTree();

//求平衡二叉树深度 
int Get_Depth(BBSTree T);

//判断平衡二叉树是否不为空 
Status Is_Not_Null(BBSTree T);

//搜索结点并返回以其为根的子树 
BBSTree Serach_Tree(BBSTree T, RcdType e);

//删除结点
Status Delete_Tree(BBSTree &T, RcdType e, Status &shorter);

//递归先序遍历
void PreOrder_RecTraverse(BBSTree T);

//递归中序遍历
void InOrder_RecTraverse(BBSTree T);

//递归后序遍历
void LastOrder_RecTraverse(BBSTree T);

//凹入表形式打印平衡二叉树 
void Print(BBSTree T);

//合并平衡二叉树
void Merge_AVL(BBSTree &T1, BBSTree &T2);

//分裂平衡二叉树（T1结点均小于/等于e，T2均大于e） 
void Spilt_AVL(BBSTree T, RcdType e, BBSTree &T1, BBSTree &T2);

#endif
