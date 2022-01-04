#include "struct.h"

//左旋处理
void L_Rotate(BBSTree &p)
{
	BBSTree rc = p->rchild;
	p->rchild = rc->lchild;
	rc->lchild = p;
	p = rc;
}

//右旋处理
void R_Rotate(BBSTree &p)
{
	BBSTree lc = p->lchild;
	p->lchild = lc->rchild;
	lc->rchild = p;
	p = lc;
}

//左平衡处理
void Left_Balance(BBSTree &T)
{
	BBSTree lc, rd;
	lc = T->lchild;
	switch (lc->bf)
	{
	//LL型，右旋调整	
	case LH:
		T->bf = lc->bf = EH;
		R_Rotate(T);
		break;
	//LR型 ，双旋处理 
	case RH:
		rd = lc->rchild;
		//修改T及其左孩子的平衡因子 
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
		//对T的左子树作左旋调整 
		L_Rotate(T->lchild);
		//对T作右旋调整 
		R_Rotate(T);
		break;
	}
}

//右平衡处理
void Right_Balance(BBSTree &T)
{
	BBSTree rc, ld;
	rc = T->rchild;
	switch (rc->bf)
	{
	//RR型，左旋处理	
	case RH:
		T->bf = rc->bf = EH;
		L_Rotate(T);
		break;
	//RL型，双旋处理	
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
		//对T的右孩子作右旋调整 
		R_Rotate(T->rchild);
		//对T作左旋调整 
		L_Rotate(T);
	}
}

//插入结点
Status Insert_AVL(BBSTree &T, RcdType e, Status &taller)
{
	//实现对e插入到二叉树的操作
    if(!T) {
        T = (BBSTree)malloc(sizeof(BBSTNode));
        T->data = e;
        T->bf = EH;
        T->lchild = NULL;
        T->rchild = NULL;
        taller = TRUE;
    } else if(e == T->data) { //树中已存在和e相等的结点 
        taller = FALSE;
        return FALSE; //未插入 
    } else if(e < T->data) { //插入到左子树 
        if(FALSE == Insert_AVL(T->lchild, e, taller)) return FALSE; //未插入 
        if(TRUE == taller) {
            switch(T->bf) { //检查T的平衡因子 
            	//原左高，左平衡处理
                case LH: 
					Left_Balance(T); 
					taller = FALSE; 
					break;
				//原等高，左变高 	
                case EH: 
					T->bf = LH; 
					taller = TRUE; 
					break; 
				//原右高，变等高 
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
                	//原本左高，变等高
                    case LH: 
                        T->bf = EH; 
						taller = FALSE; 
						break;
                    //原本等高，变右高 
                    case EH: 
                        T->bf = RH; 
						taller = TRUE; 
						break;
                     //原本右高，右平衡处理     
                    case RH:
                        Right_Balance(T); 
						taller = FALSE; 
						break; 
                }
            }
    } 
    return TRUE;
}

//根据输入的数据创建平衡二叉树 
BBSTree Create_BBSTree()
{
	BBSTree T = NULL;
    Status taller = TRUE;  
    RcdType *a;
    int n;
    
    printf("请输入插入元素的个数：");
    scanf("%d", &n);
    a = (RcdType*)malloc(sizeof(int)*n);
       
    printf("请输入要插入的元素："); 
    for(int i = 0; i<n; i++){
        scanf("%d", &a[i]);
    }
    
    for(int j = 0; j<n; j++){
        Insert_AVL(T, a[j], taller);
    }
    return T;
}

//求平衡二叉树深度 
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

//判断平衡二叉树是否不为空 
Status Is_Not_Null(BBSTree T)
{
	if (!T)
		return FALSE;
	return TRUE;
}

//搜索结点并返回以其为根的子树 
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

//删除结点
Status Delete_Tree(BBSTree &T, RcdType e, Status &shorter)
{
 static int tag = 0;
    if(!T) {
        return FALSE;
    } else if(e == T->data) {
        BBSTNode *q = NULL;
        //如果该结点只有一个孩子，则将自子树取代该结点
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
        //如果被删结点有两个孩子，则找到结点的前驱结点的前驱结点，并将前驱结点的值赋给该结点，然后删除前驱结点 
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
        //删除完后，调整结点的平衡因子
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
                //如果本来右子树较高，删除后不平衡，进行右平衡操作 
                case RH:
                    Right_Balance(T); //右平衡处理     
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
        //删除完后，调整平衡因子
        if(shorter && (tag == 0)) {
            switch(T->bf) {
                case LH:
                    Left_Balance(T); //左平衡处理
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

//递归先序遍历
void PreOrder_RecTraverse(BBSTree T)
{
	if (!T) return;
	printf("%d ", T->data);
	PreOrder_RecTraverse(T->lchild);
	PreOrder_RecTraverse(T->rchild);
}

//递归中序遍历
void InOrder_RecTraverse(BBSTree T)
{
	if (!T) return;
	if (T->lchild)
		InOrder_RecTraverse(T->lchild);
	printf("%d ", T->data);
	if (T->rchild)
		InOrder_RecTraverse(T->rchild);
}

//递归后序遍历
void LastOrder_RecTraverse(BBSTree T)
{
	if (!T) return;
	if (T->lchild)
		LastOrder_RecTraverse(T->lchild);
	if (T->rchild)
		LastOrder_RecTraverse(T->rchild);
	printf("%d ", T->data);
}

//显示平衡二叉树，i表示当前层次 
void Print_AVL(BBSTree &T,int i) 
{
	//以凹入表形式打印（先序遍历）
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

//真正的合并方法，T为合并后的树 
void Real_Merge(BBSTree &T, BBSTree T2) {
	Status taller = FALSE;
    if (T2) {
        Real_Merge(T, T2->lchild);
        Insert_AVL(T, T2->data, taller);
        Real_Merge(T, T2->rchild);
    }
}

//合并平衡二叉树（此方法只是判断了应该合并到哪一棵树，真正的合并方法是Real_Merge） 
void Merge_AVL(BBSTree &T1, BBSTree &T2) {
	//把深度较小的树合并到深度较大的树
	if(Get_Depth(T1) >= Get_Depth(T2)) {
		//合并到T1 
		Real_Merge(T1,T2);
		Print(T1);
	} else {
		//合并到T2 
		Real_Merge(T2,T1);
		Print(T2);
	}
} 

//分裂平衡二叉树（T1结点均小于/等于e，T2均大于e） 
void Spilt_AVL(BBSTree T, RcdType e, BBSTree &T1, BBSTree &T2) {
    Status taller = FALSE;
    if (T) {
    	//递归访问左子树
        Spilt_AVL(T->lchild, e, T1, T2); 
        if(T->data <= e) {
            Insert_AVL(T1, T->data, taller);
        } else if (T->data > e) {
            Insert_AVL(T2, T->data, taller);
        }
        //递归访问右子树 
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
    printf("--      数据结构实验      --\n"); 
	printf("--       平衡二叉树       --\n"); 
	printf("--       3219004950       --\n"); 
	printf("--         吴泳诗         --\n"); 
	printf("----------------------------\n\n");
	
	printf("------------------------------\n"); 
	printf("--         二叉树展示       --\n"); 
	printf("-- 注：如果左右孩子之一为空 --\n"); 
	printf("--    则以x显式地打印出来   --\n"); 
	printf("-- 但叶子结点不会打印两个x  --\n"); 
	printf("------------------------------\n"); 
    
    while(num != 11){ 
        printf("\n");
        printf("1：创建一棵平衡二叉树\n");
        printf("2：先序遍历\n");
        printf("3：中序遍历\n");
        printf("4：后序遍历\n");
        printf("5：插入一个结点\n");
        printf("6：删除一个结点\n");
        printf("7：求平衡二叉树的深度\n"); 
        printf("8: 合并两棵平衡二叉树\n");
        printf("9: 分裂二叉树\n");
        printf("10: 查找结点\n");
        printf("11：退出系统\n");
        printf("\n");
        printf("请输入对应功能的序号：");
        scanf("%d", &num);        
        switch(num){
            case 1:
                T = Create_BBSTree();
                if(NULL != T) printf("创建成功！\n"); 
                Print(T);
                break;
            case 2:
            	if(Is_Not_Null(T) == FALSE) {
            		printf("请先创建一颗平衡二叉树！\n");
            		break;	
				} else {
					printf("先序遍历结果如下：\n");
	                PreOrder_RecTraverse(T); 
	                printf("\n");
	                break;
				}
            case 3:
            	if(Is_Not_Null(T) == FALSE) {
            		printf("请先创建一颗平衡二叉树！\n");
            		break;
				} else {
					printf("中序遍历结果如下：\n");
                	InOrder_RecTraverse(T); 
                	printf("\n");
                	break;	
				} 
            case 4:
            	if(Is_Not_Null(T) == FALSE) {
            		printf("请先创建一颗平衡二叉树！\n");
            		break;
            	} else {
					printf("后序遍历结果如下：\n");
	                LastOrder_RecTraverse(T); 
	                printf("\n");
	                break;
				}
            case 5:
            	if(Is_Not_Null(T) == FALSE) {
            		printf("请先创建一颗平衡二叉树！\n");
            		break;
            	} else {
            		printf("请输入要插入的元素：");
	                scanf("%d", &e);
	                if(TRUE == Insert_AVL(T, e, taller)){
	                	printf("插入成功！\n");	
	                	Print(T);
					} else {
						printf("插入失败！\n"); 
					}
	                break;
				}
            case 6:
            	if(Is_Not_Null(T) == FALSE) {
            		printf("请先创建一颗平衡二叉树！\n");
            		break;
            	} else {
            		printf("请输入要删除的结点：");
	                scanf("%d", &e);
	                if(TRUE == Delete_Tree(T, e, shorter)){
	                	printf("删除成功！\n"); 
	                	Print(T);
					} else {
						printf("删除失败，结点不存在！\n");
					}
	                break; 
				}
            case 7:
            	if(Is_Not_Null(T) == FALSE) {
            		printf("请先创建一颗平衡二叉树！\n");
            		break;
            	} else {
            		printf("平衡二叉树的深度为：");
					int dep = Get_Depth(T);
					printf("%d", dep);
					printf("\n");
					break; 
				}
			case 8:
				{
					printf("请创建两棵平衡二叉树\n");
	        		printf("第一棵树\n");
	        		BBSTree T1 = Create_BBSTree();
	        		if(NULL != T1) printf("创建成功！\n"); 
	        		Print(T1);
	        		printf("第二棵树\n");
	        		BBSTree T2 = Create_BBSTree();
	                if(NULL != T2) printf("创建成功！\n"); 
	                Print(T2);
	                printf("合并后的树\n");
	                Merge_AVL(T1,T2);
	                break;	
				}
			case 9:
				if(Is_Not_Null(T) == FALSE) {
            		printf("请先创建一颗平衡二叉树！\n");
            		break;
            	} else {
            		printf("请输入分裂的依据\n");
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
            		printf("请先创建一颗平衡二叉树！\n");
            		break;
            	} else {
            		printf("请输入想要查找的结点\n");
            		int y;
            		scanf("%d",&y);
            		BBSTree T1 = Serach_Tree(T,y);
            		if(T1) {
            			printf("查找成功！\n");
            			Print(T1); 
					} else {
						printf("查找失败!\n");
					}
					break;
				}
        }  
    }
}
