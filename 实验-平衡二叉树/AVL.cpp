#include "struct.h"

//��������
void L_Rotate(BBSTree &p)
{
	BBSTree rc = p->rchild;
	p->rchild = rc->lchild;
	rc->lchild = p;
	p = rc;
}

//��������
void R_Rotate(BBSTree &p)
{
	BBSTree lc = p->lchild;
	p->lchild = lc->rchild;
	lc->rchild = p;
	p = lc;
}

//��ƽ�⴦��
void Left_Balance(BBSTree &T)
{
	BBSTree lc, rd;
	lc = T->lchild;
	switch (lc->bf)
	{
	//LL�ͣ���������	
	case LH:
		T->bf = lc->bf = EH;
		R_Rotate(T);
		break;
	//LR�� ��˫������ 
	case RH:
		rd = lc->rchild;
		//�޸�T�������ӵ�ƽ������ 
		switch (rd->bf)
		{
		case LH:
			T->bf = RH;
			lc->bf = EH;
			break;
		case EH:
			T->bf = lc->bf = EH;
			break;
		case RH:
			T->bf = EH;
			lc->bf = LH;
			break;
		}
		rd->bf = EH;
		//��T������������������ 
		L_Rotate(T->lchild);
		//��T���������� 
		R_Rotate(T);
		break;
	}
}

//��ƽ�⴦��
void Right_Balance(BBSTree &T)
{
	BBSTree rc, ld;
	rc = T->rchild;
	switch (rc->bf)
	{
	//RR�ͣ���������	
	case RH:
		T->bf = rc->bf = EH;
		L_Rotate(T);
		break;
	//RL�ͣ�˫������	
	case LH:
		ld = rc->lchild;
		switch (ld->bf)
		{
		case RH:
			T->bf = LH;
			rc->bf = EH;
			break;
		case EH:
			T->bf = rc->bf = EH;
			break;
		case LH:
			T->bf = EH;
			rc->bf = RH;
			break;
		}
		ld->bf = EH;
		//��T���Һ������������� 
		R_Rotate(T->rchild);
		//��T���������� 
		L_Rotate(T);
	}
}

//������
Status Insert_AVL(BBSTree &T, RcdType e, Status &taller)
{
	//ʵ�ֶ�e���뵽�������Ĳ���
    if(!T) {
        T = (BBSTree)malloc(sizeof(BBSTNode));
        T->data = e;
        T->bf = EH;
        T->lchild = NULL;
        T->rchild = NULL;
        taller = TRUE;
    } else if(e == T->data) { //�����Ѵ��ں�e��ȵĽ�� 
        taller = FALSE;
        return FALSE; //δ���� 
    } else if(e < T->data) { //���뵽������ 
        if(FALSE == Insert_AVL(T->lchild, e, taller)) return FALSE; //δ���� 
        if(TRUE == taller) {
            switch(T->bf) { //���T��ƽ������ 
            	//ԭ��ߣ���ƽ�⴦��
                case LH: 
					Left_Balance(T); 
					taller = FALSE; 
					break;
				//ԭ�ȸߣ����� 	
                case EH: 
					T->bf = LH; 
					taller = TRUE; 
					break; 
				//ԭ�Ҹߣ���ȸ� 
                case RH: 
					T->bf = EH; 
					taller = FALSE; 
					break; 
            }
        }
    } else {
        if(FALSE == Insert_AVL(T->rchild, e, taller)) return FALSE;
            if(TRUE == taller) {
                switch(T->bf) {
                	//ԭ����ߣ���ȸ�
                    case LH: 
                        T->bf = EH; 
						taller = FALSE; 
						break;
                    //ԭ���ȸߣ����Ҹ� 
                    case EH: 
                        T->bf = RH; 
						taller = TRUE; 
						break;
                     //ԭ���Ҹߣ���ƽ�⴦��     
                    case RH:
                        Right_Balance(T); 
						taller = FALSE; 
						break; 
                }
            }
    } 
    return TRUE;
}

//������������ݴ���ƽ������� 
BBSTree Create_BBSTree()
{
	BBSTree T = NULL;
    Status taller = TRUE;  
    RcdType *a;
    int n;
    
    printf("���������Ԫ�صĸ�����");
    scanf("%d", &n);
    a = (RcdType*)malloc(sizeof(int)*n);
       
    printf("������Ҫ�����Ԫ�أ�"); 
    for(int i = 0; i<n; i++){
        scanf("%d", &a[i]);
    }
    
    for(int j = 0; j<n; j++){
        Insert_AVL(T, a[j], taller);
    }
    return T;
}

//��ƽ���������� 
int Get_Depth(BBSTree T)
{
	int depthLeft, depthRight;
	if (!T)
		return 0;
	else
	{
		depthLeft = Get_Depth(T->lchild);
		depthRight = Get_Depth(T->rchild);
		return 1 + (depthLeft > depthRight ? depthLeft : depthRight);
	}
}

//�ж�ƽ��������Ƿ�Ϊ�� 
Status Is_Not_Null(BBSTree T)
{
	if (!T)
		return FALSE;
	return TRUE;
}

//������㲢��������Ϊ�������� 
BBSTree Serach_Tree(BBSTree T, RcdType e)
{
	if (!T)
		return NULL;
	else if (e == T->data)
		return T;
	else if (e < T->data)
		return Serach_Tree(T->lchild, e);
	else
		return Serach_Tree(T->rchild, e);
}

//ɾ�����
Status Delete_Tree(BBSTree &T, RcdType e, Status &shorter)
{
 static int tag = 0;
    if(!T) {
        return FALSE;
    } else if(e == T->data) {
        BBSTNode *q = NULL;
        //����ý��ֻ��һ�����ӣ���������ȡ���ý��
        if(!T->lchild) {
            q = T;
            T = T->rchild;
            free(q);
            shorter = TRUE;
        } else if(!T->rchild) {
            q = T;
            T = T->lchild;
            free(q);
            shorter = TRUE;
        } 
        //�����ɾ������������ӣ����ҵ�����ǰ������ǰ����㣬����ǰ������ֵ�����ý�㣬Ȼ��ɾ��ǰ����� 
        else{
            q = T->lchild;
            while(q->rchild) {
                q = q->rchild;
            }
            T->data = q->data;
            if(T->lchild->data == q->data) {
                tag = 1;
            }
            Delete_Tree(T->lchild, q->data, shorter);
            if(tag == 1) {
                BBSTree r = T->rchild;
                if(NULL == r) T->bf = 0;
                else {
                    switch(r->bf) {
                        case EH:
                            T->bf = RH; break;
                        default: Right_Balance(T); break;
                    }
                }
            }
        }
    } else if(e < T->data) {
        if(!Delete_Tree(T->lchild, e, shorter)) {
            return FALSE;
        }
        //ɾ����󣬵�������ƽ������
        if(shorter && (tag == 0)) {
            switch(T->bf) {
                case LH:
                    T->bf = EH;
                    shorter = TRUE;
                    break;
                case EH:
                    T->bf = RH;
                    shorter = FALSE;
                    break;
                //��������������ϸߣ�ɾ����ƽ�⣬������ƽ����� 
                case RH:
                    Right_Balance(T); //��ƽ�⴦��     
                    if(T->rchild->bf == EH)
                        shorter = FALSE;
                    else
                        shorter = TRUE;
                    break;
            }
        } 
    } else if(e > T->data) {
        if(!Delete_Tree(T->rchild, e, shorter)) {
            return FALSE;
        }
        //ɾ����󣬵���ƽ������
        if(shorter && (tag == 0)) {
            switch(T->bf) {
                case LH:
                    Left_Balance(T); //��ƽ�⴦��
                    if(T->lchild->bf == EH)
                        shorter = FALSE;
                    else 
                        shorter = TRUE;
                    break;
                case EH:
                    T->bf = LH;
                    shorter = TRUE;
                    break; 
            }
        }
        if(tag == 1) {
            int depthLeft = Get_Depth(T->lchild);
            int depthRight = Get_Depth(T->rchild);
            T->bf = depthLeft - depthRight;
        } 
    } 
    return OK;
}

//�ݹ��������
void PreOrder_RecTraverse(BBSTree T)
{
	if (!T) return;
	printf("%d ", T->data);
	PreOrder_RecTraverse(T->lchild);
	PreOrder_RecTraverse(T->rchild);
}

//�ݹ��������
void InOrder_RecTraverse(BBSTree T)
{
	if (!T) return;
	if (T->lchild)
		InOrder_RecTraverse(T->lchild);
	printf("%d ", T->data);
	if (T->rchild)
		InOrder_RecTraverse(T->rchild);
}

//�ݹ�������
void LastOrder_RecTraverse(BBSTree T)
{
	if (!T) return;
	if (T->lchild)
		LastOrder_RecTraverse(T->lchild);
	if (T->rchild)
		LastOrder_RecTraverse(T->rchild);
	printf("%d ", T->data);
}

//��ʾƽ���������i��ʾ��ǰ��� 
void Print_AVL(BBSTree &T,int i) 
{
	//�԰������ʽ��ӡ�����������
	for(int x = 0;x <= i;x++) {
 		printf("   ");
 	}
	 if (!T) {
	 	printf("x \n");	
	 } else {
		printf("%d \n", T->data);
		if(T->lchild || T->rchild) {
			Print_AVL(T->lchild,i+1);
			Print_AVL(T->rchild,i+1);	
		}
	}
} 

void Print(BBSTree T) 
{
	printf("==============================\n");
	Print_AVL(T,1); 
	printf("==============================\n");
}

//�����ĺϲ�������TΪ�ϲ������ 
void Real_Merge(BBSTree &T, BBSTree T2) {
	Status taller = FALSE;
    if (T2) {
        Real_Merge(T, T2->lchild);
        Insert_AVL(T, T2->data, taller);
        Real_Merge(T, T2->rchild);
    }
}

//�ϲ�ƽ����������˷���ֻ���ж���Ӧ�úϲ�����һ�����������ĺϲ�������Real_Merge�� 
void Merge_AVL(BBSTree &T1, BBSTree &T2) {
	//����Ƚ�С�����ϲ�����Ƚϴ����
	if(Get_Depth(T1) >= Get_Depth(T2)) {
		//�ϲ���T1 
		Real_Merge(T1,T2);
		Print(T1);
	} else {
		//�ϲ���T2 
		Real_Merge(T2,T1);
		Print(T2);
	}
} 

//����ƽ���������T1����С��/����e��T2������e�� 
void Spilt_AVL(BBSTree T, RcdType e, BBSTree &T1, BBSTree &T2) {
    Status taller = FALSE;
    if (T) {
    	//�ݹ����������
        Spilt_AVL(T->lchild, e, T1, T2); 
        if(T->data <= e) {
            Insert_AVL(T1, T->data, taller);
        } else if (T->data > e) {
            Insert_AVL(T2, T->data, taller);
        }
        //�ݹ���������� 
        Spilt_AVL(T->rchild, e, T1, T2);
    }
}

int main()
{
  	BBSTree T = NULL;
    int num = 0;
    RcdType e;
    Status taller = TRUE;
    Status shorter = TRUE;
    
    printf("----------------------------\n"); 
    printf("--      ���ݽṹʵ��      --\n"); 
	printf("--       ƽ�������       --\n"); 
	printf("--       3219004950       --\n"); 
	printf("--         ��Ӿʫ         --\n"); 
	printf("----------------------------\n\n");
	
	printf("------------------------------\n"); 
	printf("--         ������չʾ       --\n"); 
	printf("-- ע��������Һ���֮һΪ�� --\n"); 
	printf("--    ����x��ʽ�ش�ӡ����   --\n"); 
	printf("-- ��Ҷ�ӽ�㲻���ӡ����x  --\n"); 
	printf("------------------------------\n"); 
    
    while(num != 11){ 
        printf("\n");
        printf("1������һ��ƽ�������\n");
        printf("2���������\n");
        printf("3���������\n");
        printf("4���������\n");
        printf("5������һ�����\n");
        printf("6��ɾ��һ�����\n");
        printf("7����ƽ������������\n"); 
        printf("8: �ϲ�����ƽ�������\n");
        printf("9: ���Ѷ�����\n");
        printf("10: ���ҽ��\n");
        printf("11���˳�ϵͳ\n");
        printf("\n");
        printf("�������Ӧ���ܵ���ţ�");
        scanf("%d", &num);        
        switch(num){
            case 1:
                T = Create_BBSTree();
                if(NULL != T) printf("�����ɹ���\n"); 
                Print(T);
                break;
            case 2:
            	if(Is_Not_Null(T) == FALSE) {
            		printf("���ȴ���һ��ƽ���������\n");
            		break;	
				} else {
					printf("�������������£�\n");
	                PreOrder_RecTraverse(T); 
	                printf("\n");
	                break;
				}
            case 3:
            	if(Is_Not_Null(T) == FALSE) {
            		printf("���ȴ���һ��ƽ���������\n");
            		break;
				} else {
					printf("�������������£�\n");
                	InOrder_RecTraverse(T); 
                	printf("\n");
                	break;	
				} 
            case 4:
            	if(Is_Not_Null(T) == FALSE) {
            		printf("���ȴ���һ��ƽ���������\n");
            		break;
            	} else {
					printf("�������������£�\n");
	                LastOrder_RecTraverse(T); 
	                printf("\n");
	                break;
				}
            case 5:
            	if(Is_Not_Null(T) == FALSE) {
            		printf("���ȴ���һ��ƽ���������\n");
            		break;
            	} else {
            		printf("������Ҫ�����Ԫ�أ�");
	                scanf("%d", &e);
	                if(TRUE == Insert_AVL(T, e, taller)){
	                	printf("����ɹ���\n");	
	                	Print(T);
					} else {
						printf("����ʧ�ܣ�\n"); 
					}
	                break;
				}
            case 6:
            	if(Is_Not_Null(T) == FALSE) {
            		printf("���ȴ���һ��ƽ���������\n");
            		break;
            	} else {
            		printf("������Ҫɾ���Ľ�㣺");
	                scanf("%d", &e);
	                if(TRUE == Delete_Tree(T, e, shorter)){
	                	printf("ɾ���ɹ���\n"); 
	                	Print(T);
					} else {
						printf("ɾ��ʧ�ܣ���㲻���ڣ�\n");
					}
	                break; 
				}
            case 7:
            	if(Is_Not_Null(T) == FALSE) {
            		printf("���ȴ���һ��ƽ���������\n");
            		break;
            	} else {
            		printf("ƽ������������Ϊ��");
					int dep = Get_Depth(T);
					printf("%d", dep);
					printf("\n");
					break; 
				}
			case 8:
				{
					printf("�봴������ƽ�������\n");
	        		printf("��һ����\n");
	        		BBSTree T1 = Create_BBSTree();
	        		if(NULL != T1) printf("�����ɹ���\n"); 
	        		Print(T1);
	        		printf("�ڶ�����\n");
	        		BBSTree T2 = Create_BBSTree();
	                if(NULL != T2) printf("�����ɹ���\n"); 
	                Print(T2);
	                printf("�ϲ������\n");
	                Merge_AVL(T1,T2);
	                break;	
				}
			case 9:
				if(Is_Not_Null(T) == FALSE) {
            		printf("���ȴ���һ��ƽ���������\n");
            		break;
            	} else {
            		printf("��������ѵ�����\n");
            		int x;
            		scanf("%d",&x);
            		BBSTree T1 = NULL;
            		BBSTree T2 = NULL;
            		Spilt_AVL(T,x,T1,T2);
            		Print(T1);
            		Print(T2);
            		break;
            	}
            case 10:
				if(Is_Not_Null(T) == FALSE) {
            		printf("���ȴ���һ��ƽ���������\n");
            		break;
            	} else {
            		printf("��������Ҫ���ҵĽ��\n");
            		int y;
            		scanf("%d",&y);
            		BBSTree T1 = Serach_Tree(T,y);
            		if(T1) {
            			printf("���ҳɹ���\n");
            			Print(T1); 
					} else {
						printf("����ʧ��!\n");
					}
					break;
				}
        }  
    }
}
