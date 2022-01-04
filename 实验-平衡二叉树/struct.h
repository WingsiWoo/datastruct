#ifndef STRUCT_H_

#include <stdio.h>
#include <stdlib.h>

#define LH +1 //���������������ߣ�������
#define EH 0 //���������ȸߣ���Ƶȸ� 
#define RH -1 //���������������ߣ�����Ҹ� 
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define SUCCESS 1

typedef int Status;
typedef int RcdType;

//ƽ��������ṹ�嶨�� 
typedef struct BBSTNode
{
	RcdType data;
	int bf;					   //ƽ������
	BBSTNode *lchild, *rchild; //���Һ��ӽ�� 
} BBSTNode, *BBSTree;

//ƽ���������ؽӿڶ���

//��������
void L_Rotate(BBSTree &p);

//��������
void R_Rotate(BBSTree &p); 

//��ƽ�⴦��
void Left_Balance(BBSTree &T);

//��ƽ�⴦��
void Right_Balance(BBSTree &T);

//������
Status Insert_AVL(BBSTree &T, RcdType e, Status &taller);

//������������ݴ���ƽ������� 
BBSTree Create_BBSTree();

//��ƽ���������� 
int Get_Depth(BBSTree T);

//�ж�ƽ��������Ƿ�Ϊ�� 
Status Is_Not_Null(BBSTree T);

//������㲢��������Ϊ�������� 
BBSTree Serach_Tree(BBSTree T, RcdType e);

//ɾ�����
Status Delete_Tree(BBSTree &T, RcdType e, Status &shorter);

//�ݹ��������
void PreOrder_RecTraverse(BBSTree T);

//�ݹ��������
void InOrder_RecTraverse(BBSTree T);

//�ݹ�������
void LastOrder_RecTraverse(BBSTree T);

//�������ʽ��ӡƽ������� 
void Print(BBSTree T);

//�ϲ�ƽ�������
void Merge_AVL(BBSTree &T1, BBSTree &T2);

//����ƽ���������T1����С��/����e��T2������e�� 
void Spilt_AVL(BBSTree T, RcdType e, BBSTree &T1, BBSTree &T2);

#endif
