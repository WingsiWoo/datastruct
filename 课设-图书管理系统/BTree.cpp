#include "struct.h"

//在p->key[1..p->keynum]找k 
int Search(BTree p,int k) 
{
	int i = 1;
	while(i <= p->keynum && k > p->key[i]) i++;
	return i; 
}

result Search_BTree(BTree T,KeyType k) 
{
	/**
	 * 在m阶B树T上查找关键字k，用r返回(pt,i,tag)
	 * 若查找成功，则标记tag=1，指针pt所指结点中第i个关键字等于k
	 * 否则tag=0，若要插入关键字为k的记录，应位于pt结点中第i-1个和第i个关键字之间 
	 */
	result r;
	int i = 0,found = NOTFOUND;
	//初始，p指向根节点；p将用于指向待查结点，q指向其双亲 
	BTree p = T,q = NULL;
	while(p && found == NOTFOUND) {
		//在p->key[1..p->keynum]中查找p->key[i-1]<k<=p->key[i]		
		i = Search(p,k);
		//找到待查关键字
		if(i <= p->keynum && p->key[i] == k) found = 1;
		else {
			//没找到，指针下移
			q = p;
			p = p->ptr[i-1]; 
		} 
	}
	if(found == FOUND) {
		//查找成功，返回k的位置p及i 
		r.pt = p;
		r.i = i;
		r.tag = FOUND;
	} else {
		//查找不成功，返回k的插入位置q及i 
		r.pt = q;
		r.i = i;
		r.tag = NOTFOUND;
	}
	return r;
} 

//生成新的根节点
void New_Root(BTree &T, BTree p, int k, BTree ap, Record *rec) {
	T = (BTNode*)malloc(sizeof(BTNode));
	T->keynum = 1;
	T->ptr[0] = p;
	T->ptr[1] = ap;
	T->key[1] = k;
	T->recptr[1] = rec;
	if (p != NULL) p->parent = T;
	if (ap != NULL) ap->parent = T;
	T->parent = NULL;				//新根的双亲是空指针
}

//分裂结点 
void Split_Node(BTree &q,int s,BTree &ap) {
	//将q结点分裂成两个结点，前一半保留在原结点，后一般移入ap所指新结点
	int i,j,n = q->keynum;
	//生成新结点 
	ap = (BTNode *)malloc(sizeof(BTNode));
	ap->ptr[0] = q->ptr[s];
	//后一半移入ap结点 
	for(i = s+1,j= 1;i <= n;i++,j++) {
		ap->key[j] = q->key[i];
		ap->ptr[j] = q->ptr[i];
		ap->recptr[i-s] = q->recptr[i];
	} 
	ap->keynum = n-s;
	ap->parent = q->parent;
	//修改新结点的子节点的parent域 
	for(i = 0;i <= n-s;i++) {
		//将ap所有孩子结点指向ap
		if(ap->ptr[i])	
			ap->ptr[i]->parent = ap;
		//q结点的前一半保留，修改keynum 
		q->keynum = s-1;
	}
}

/**
 * k和ap分别插到q->key[i+1]和q->ptr[i+1]
 * 并插入关键字为k的记录recprt
 */
void Insert(BTree &q, int i, int k, BTree &ap, Record *rec) {
	int j = q->keynum;
    for (j; j >= i; j--) {
        q->key[j + 1] = q->key[j];	//关键字指针向后移一位 
        q->ptr[j + 1] = q->ptr[j];	//孩子结点指针向后移一位 
        q->recptr[j+1] = q->recptr[j];
    }
    q->key[i] = k;	//赋值 
    q->ptr[i] = ap;
    q->recptr[i] = rec;
    if (ap != NULL)
        ap->parent = q;
    q->keynum++;	//插入关键字后该结点的关键字总数+1 
}

/**
 * 在m阶B树T上结点*q的key[i]与key[i+1]之间插入关键字K和记录rec。
 * 若引起结点过大，则沿双亲链进行必要的结点分裂调整，使T仍是m阶B树。
 */
Status Insert_BTree(BTree &T, KeyType k, BTree q, int i, Record *rec)
{
	BTree ap = NULL;
	int finished = FALSE;
	if (!q) New_Root(T, NULL, k, NULL, rec);		// T是空树，生成仅含关键字K的根结点*T
	else {
		while (!finished)
		{
			Insert(q, i, k, ap, rec);			// 将k和ap分别插入到q->key[i+1]和q->ptr[i+1]
			if (q->keynum < m) finished = TRUE; // 插入完成
			else {
				Split_Node(q, (m + 1) / 2, ap);		// 分裂结点Q
				k = q->key[(m + 1) / 2];
				rec = q->recptr[(m + 1) / 2];
				if (q->parent)
				{								// 在双亲结点*q中查找k的插入位置
					q = q->parent;
					i = Search(q, k);
				}
				else finished = OVERFLOW;		// 根节点已分裂为*q和*ap两个结点
			}
		}
		if (finished == OVERFLOW)				// 根结点已分裂为结点*q和*ap
			New_Root(T, q, k, ap, rec);			// 需生成新根结点*T,q和ap为子树指针
	}
	return OK;
}

void Successor(BTree &q, int i) {
	//*q结点的第i个关键字为k，用q的后继关键字代替q，且令q指向后继所在结点
	BTree p = q;
	q = q->ptr[i];
	while (q->ptr[0]) q = q->ptr[0];	//查找p的后继
	p->key[i] = q->key[1];			//记录代替
	p->recptr[i] = q->recptr[1];
	i = 1;							//代替后应该删除q所指结点的第1个关键字
}

void Remove(BTree &q, int i) {
	//删除q所指结点第i个关键字及其记录
	for (; i < q->keynum; i++) {	//关键字和记录指针前移
		q->key[i] = q->key[i + 1];
		q->recptr[i] = q->recptr[i + 1];
	}
	q->keynum--;					//关键字数目减1
}

/**
 * 若q的兄弟结点关键字大于(m-1)/2,则从兄弟结点上移最小（或最大）的关键字到双亲结点，
 * 而将双亲结点中小于(或大于）且紧靠该关键字的关键字下移至q中,并返回OK，否则返回EREOR。
 */
Status Borrow(BTree &q)
{
	int	i;
	BTree p = q->parent, b = NULL;				// p指向q的双亲结点
	for (i = 0; p->ptr[i] != q; i++);			// 查找q在双亲p的子树位置
	if (i >= 0 && i + 1 <= p->keynum && p->ptr[i + 1]->keynum > (m - 1) / 2)
	{											// 若q的右兄弟关键字个数大于(m-1)/2
		b = p->ptr[i + 1];						// b指向右兄弟结点
		q->ptr[1] = b->ptr[0];					// 子树指针也要同步移动
		q->key[1] = p->key[i + 1];				// 从父节点借第i+1个关键字
		q->recptr[1] = p->recptr[i + 1];
		p->key[i + 1] = b->key[1];				// b第一个关键字上移到父节点
		p->recptr[i + 1] = b->recptr[1];
		for (i = 1; i <= b->keynum; i++)		// b第一个关键字上移，需把剩余记录前移一位
		{
			b->key[i] = b->key[i + 1];
			b->recptr[i] = b->recptr[i + 1];
			b->ptr[i - 1] = b->ptr[i];
		}
	}
	else if (i > 0 && p->ptr[i - 1]->keynum > (m - 1) / 2)
	{											// 若q的左兄弟关键字个数大约(m-1)/2
		b = p->ptr[i - 1];						// b指向左兄弟结点
		q->ptr[1] = q->ptr[0];
		q->ptr[0] = b->ptr[b->keynum];
		q->key[1] = p->key[i];					// 从父节点借第i个关键字
		q->recptr[1] = p->recptr[i];
		p->key[i] = b->key[b->keynum];			// 将b最后一个关键字上移到父节点
		p->recptr[i] = b->recptr[b->keynum];
	}
	else return ERROR;							// 无关键字大于(m-1)/2的兄弟
	q->keynum++;
	b->keynum--;
	for (i = 0; i <= q->keynum; i++)
		if (q->ptr[i]) q->ptr[i]->parent = q;	// 刷新q的子结点的双亲指针
	return OK;
}

void Merge(BTree &q) {
	int i, j;
	BTree p = q->parent, b = NULL;				//p指向q的父亲
	for (i = 0; p->ptr[i] != q; i++);			//查找q在父亲p中的子树位置
	if (i == 0) {								//若为0，则需合并为兄弟的第一个关键字
		b = p->ptr[i + 1];
		for (j = b->keynum; j >= 0; j--) {		//将b的关键字和记录后移一位
			b->key[j + 1] = b->key[j];
			b->recptr[j + 1] = b->recptr[j];
			b->ptr[j + 1] = b->ptr[j];
		}
		b->ptr[0] = q->ptr[0];					//合并
		b->key[1] = p->key[1];
		b->recptr[1] = p->recptr[1];
	}
	else if (i > 0) {//若q在父亲的子树位置大于0，需合并为兄弟b的最后一个关键字
		b = p->ptr[i - 1];
		b->key[b->keynum + 1] = p->key[i];		//合并
		b->recptr[b->keynum + 1] = p->recptr[i];
		b->ptr[b->keynum + 1] = q->ptr[0];
	}
	if (i == 0 || i == 1)//若i为0或1，需将父结点p关键字前移一位
		for (i; i < p->keynum; i++) {
			p->key[i] = p->key[i + 1];
			p->ptr[i] = p->ptr[i + 1];
			p->recptr[i] = p->recptr[i + 1];
		}
	p->keynum--;
	b->keynum++;
	free(q);
	q = b;										//q指向修改的兄弟结点
	for (i = 0; i <= b->keynum; i++)
		if (b->ptr[i]) b->ptr[i]->parent = b;	//刷新b的子结点的双亲指针
}

/**
 * 在m阶B树T上删除关键字k及其对应记录，并返回OK。
 * 如T上不存在关键字k，则返回ERROR。
 */
Status Delete_BTree(BTree &T, KeyType k)
{
	KeyType	x = k;
	BTree q, b = NULL;
	int finished = FALSE, i = 1;
	result res = Search_BTree(T, k);				// 在T中查找关键字k
	if (res.tag == 0) return ERROR;				// 未搜索到
	else
	{
		q = res.pt;								// q指向待删结点
		i = res.i;
		if (q->ptr[0]) Successor(q, i);			// 若q的子树不空，(非底层结点)
												// 则以其后继代之，且令q指向后继所在结点
		Remove(q, i);							// 删除q所指向结点中第i个关键字及记录
		if (q->keynum >= (m - 1) / 2 || !q->parent)		// 若删除后关键字个数不小于(m-1)/2或q是根节点
		{
			finished = TRUE;					// 删除完成
			if (q->keynum == 0) T = NULL;		// 若q的关键字个数为0 ，则为空树
		}
		while (!finished)
		{
			if (Borrow(q))	finished = TRUE;	// 若q的相邻兄弟结点关键字大于(m-1)/2,则从该
												// 兄弟结点上移一个最大（或最小）关键字到
												// 父节点，从父节点借一关键字到q
			else {								// 若q相邻兄弟关键字个数均等于┌m /2┑-1
				Merge(q);						// 将q中的剩余部分和双亲中的相关关键字合并至q的一个兄弟中
				q = q->parent;					// 检查双亲
				if (q == T && T->keynum == 0)	// 若被删结点的父节点是根T且T的关键字个数为0
				{
					T = T->ptr[0];				// 新根	
					T->parent = NULL;
					free(q);					// 删除原双亲结点
					finished = TRUE;
				}
				else if (q->keynum >= m / 2) finished = TRUE;
			}									// 合并后双亲关键字个数不少于(m-1)/2，完成
		}
	}
	return OK;
}

//遍历B树T，对每个结点调用visit函数
void BTree_Traverse(BTree T, void(*visit)(BTree)) {
	if (!T) return;
	//打印T结点内对应的所有关键字信息 
	visit(T);
	for (int i = 0; i <= T->keynum; i++) {
		if (T->ptr[i]) BTree_Traverse(T->ptr[i], visit);
	}
}

// 递归以凹入表形式显示B树T,每层的缩进量为tab
void Print_BTree(BTree T,int tab)
{
	if (!T)	return;
	int i;
	for (i = 0; i <= tab; i++) putchar(' ');			// 缩进x
	for (i = 1; i <= T->keynum; i++)
	{
		printf("%d  ", T->key[i]);
	}
	printf("\n");
	for (i = 0; i <= T->keynum; i++)				// 递归显示子树结点关键字
		Print_BTree(T->ptr[i], tab + 3);
}

void Print(BTree T)
{
	printf("========================================\n");
	printf("==          凹入表形式显示B树         ==\n");
	Print_BTree(T,4);
	printf("========================================\n");
}

/**
 * 书库L已存在，res包含B书在书库L中的位置或应该插入的位置
 * 如果书库中已存在B书，则只将B书的库存量增加，否则插入B书到书库L中
 */
void Insert_Book(BTree &L, BookType B, result res) {
	BTree pt;
	BookType book;
	//书库中不存在该书，则插入
	if (res.tag == NOTFOUND) {
		if(OK == Insert_BTree(L, B->ID, res.pt, res.i, B)) printf("采编成功！\n");
		else printf("采编失败！\n");	
	}
	else {
		book = (res.pt)->recptr[res.i];
		book->current += B->total;		//现存量增加
		book->total += B->total;		//总库存增加
		printf("采编成功！\n");
	}
}

//如果书库中存在B书，则从书库中删除B书的信息，并返回OK，否则返回ERROR
Status Delete_Book(BTree &L, BookType B) {
	if (Delete_BTree(L, B->ID)) return OK;	//删除成功
	else return ERROR;							//删除失败
}

//如果书库中存在B书，若B书现存量大于0，则可出借，返回TRUE，否则返回FALSE
Status Can_Borrow(BTree L, BookType B, ReaderType R) {
	if (B->current > 0) return TRUE;			//现存量大于零
	else return FALSE;							//其他情况均不允许出借
}

/**
 * 书库L存在，B书是书库中的书并且可被读者R借阅（已通过AvaiBorrow()判断）
 * 借出一本B书，登记借阅者R的信息，改变现存量。
 */
void Borrow_Book(BTree L, BookType B, ReaderType R)
{
	ReaderType	r;
	GetDate(R->borDate);						//  获取系统当前时间为借书时间日期
	AddDate(R->retDate, R->borDate, KEEP_DAYS);	//当前时间加上90天为还书日期
	if (!B->reader) B->reader = R;				// 无其他借阅者，则直接登记
	else {
		for (r = B->reader; r->rnext; r = r->rnext);
		r->rnext = R;							// 否则到借阅者链表尾，登记
	}
	B->current--;								// 现存量减1
}

/**
 * booknum为还书图书编号，IDnum是借阅者的ID号
 * 若书库中不存在图书编号为booknum的书，则搜索出错，返回-1
 * 若有借阅记录，则注销该记录，并用B和R返回图书信息和借阅者信息并返回1
 * 若没有r借阅的记录，则用B返回图书信息，并返回0
 */
Status Return_Book(BTree L, int ID, int readerID, BookType &B, ReaderType &R) {
	result res = Search_BTree(L, ID);		//在B树上按图书编号搜索
	if (res.tag == NOTFOUND) return OVERFLOW;
	B = (res.pt)->recptr[res.i];
	ReaderType reader = (res.pt)->recptr[res.i]->reader;
	ReaderType pre = reader;
	while (reader) {
		if (reader->ID == readerID) {
			R = reader;
			B->current++;						//现存量加一
			reader->retDate = reader->borDate;			//把归还了书的借书者的还书时间设为借书时间，便于之后查阅借阅者信息时判断 
			return OK;
		}
		pre = reader;
		reader = reader->rnext;
	}
	return ERROR;

}

//表格头部格式
void Print_Head() {
	printf("\n");
	printf("|---------------------------------图书基本信息----------------------------------|\n");
	printf("|  编号  |       书名         |      作者     | 现存 | 总库存 | 出版年份 | 定价 |\n");
}

//显示B书基本信息
void Print_Body(BookType B) { 
	printf("|--------|--------------------|---------------|------|--------|----------|------|\n");
	printf("|  %-4d  |%-20s|%-15s|", B->ID, B->bookname, B->author);
	printf(" %-5d|   %-4d | %-6d   |%-6.1f|\n", B->current, B->total, B->publishedYear, B->price);
}

//表格底部格式
void Print_Tail() {
	printf("|--------|--------------------|---------------|------|--------|----------|------|\n");
}

//以表格形式显示一本书的基本信息
void Print_Book(BookType B) {	
	Print_Head();
	Print_Body(B);
	Print_Tail();
	printf("\n");
}

//以表格形式显示一本书的借阅者信息
void Print_Borrower(BookType B) {	
	ReaderType p;
	ReaderType r = B->reader;
	char rpos[3][7] = { "超期","正常","已归还" };
	char * status;
	printf("|-------------------------图书借阅信息------------------------|\n");
	printf("\n");
	printf("| ID号|  姓名  |    出借日期    |    归还日期    | 状态 |\n");
	printf("|-----|--------|----------------|----------------|------|\n");
	for (p = B->reader; p; p = p->rnext)
	{
		status = rpos[Earlier(p->borDate, p->retDate)];
		printf("|%-5d|%-8s| %-4d年%2d月%2d日 | %-4d年%2d月%2d日 |%-6s|\n",
			p->ID, p->name, p->borDate.tm_year, p->borDate.tm_mon, p->borDate.tm_mday,
			p->retDate.tm_year, p->retDate.tm_mon, p->retDate.tm_mday, status);
	}
}

//显示B树每个结点的记录信息
void Print_BookNode(BTree p) {
	int i;
	for (i = 1; i <= p->keynum; i++) {
		Print_Body(p->recptr[i]);
	}
}

//显示书库L所有图书信息
void Print_All(BTree L) {		
	Print_Head();
	BTree_Traverse(L, Print_BookNode);
	Print_Tail();
}

/**
 * 若书库L中存在图书编号为ID的书，则现在该书的所有信息并返回OK
 * 书库L不存在则返回ERROR
 */
Status Show_Book_Info(BTree L, int ID) {
	BookType B;
	result res = Search_BTree(L, ID);
	if (res.tag == NOTFOUND) {
		printf("书库中不存在此书，查找失败。\n");
		return ERROR;
	}
	B = (res.pt)->recptr[res.i];
	Print_Book(B);
	return OK;
}

int menu() 
{	//菜单
	int choice;
	printf("------------------------------------------------------\n");
	printf("--                                                  --\n");
	printf("--                  图书馆管理系统                  --\n");
	printf("--                                                  --\n");
	printf("--                  1.采编入库                      --\n");
	printf("--                  2.清除库存                      --\n");
	printf("--                  3.借阅图书                      --\n");
	printf("--                  4.归还图书                      --\n");
	printf("--                  5.查看某图书信息                --\n");
	printf("--                  6.查看某书借阅者信息            --\n");
	printf("--                  7.查看图书馆全部图书            --\n");
	printf("--                  0.退出                          --\n");
	printf("--                                                  --\n");
	printf("------------------------------------------------------\n");

	do {
		printf("请选择指定操作功能（输入0-7任意一个数字）:");
		scanf("%d", &choice);
	} while (choice < 0 || choice > 7);			//避免非法输入
	return choice;
}

//登陆界面
Status login() {	
	int password, i;
	printf("---------------------------------\n"); 
	printf("--                             --\n"); 
	printf("--         图书管理系统        --\n"); 
	printf("--  Welcome to Library System  --\n");
	printf("--                             --\n"); 
	printf("--                             --\n"); 
	printf("--         数据结构课设        --\n"); 
	printf("--          3219004950         --\n"); 
	printf("--           吴泳诗            --\n"); 
	printf("--                             --\n"); 
	printf("---------------------------------\n");
	printf("请输入图书管理员密码:");	//密码为4950 
	scanf("%d", &password);
	fflush(stdin);

	Sleep(100);
	system("cls");				//清屏

	if (password == Manager_Code)//判断是不是管理员
		return OK;
	else
		printf("密码错误，请重新输入！\n");
		login();
}

// 获取系统时间，赋予tm结构体变量tim
void GetDate(tm &tim)		
{
	time_t curtime = time(0);
	tim = *localtime(&curtime);
	tim.tm_year += 1900;		              // tm 年份少1900年
	tim.tm_mon++;				         // tm month 从0-11，故加1
}

// 把date1的日期加day天后赋给date2
void AddDate(tm &date2, tm date1, int day)
{
	date2.tm_year = date1.tm_year + (day / 30 + date1.tm_mon) / 12;
	date2.tm_mon = (date1.tm_mon + (day / 30)) % 12;
	date2.tm_mday = (date1.tm_mday + day) % 30;
}

/**
 * 首先比较borDate和retDate是否一致（因为还书时会把这两个时间设为一致作为已归还的标志）
 * 如果一致则返回RETURNED表示已归还
 * 如果不一致则获取系统当前时间today并与retDate比较
 * 如果today早于/等于retDate则返回NORMAL，否则返回OVERDUE 
 */
int	Earlier(tm borDate, tm retDate)
{
	if((borDate.tm_year == retDate.tm_year) && (borDate.tm_mon == retDate.tm_mon) && (borDate.tm_mday == retDate.tm_mday))
	return RETURNED;
	else {
		tm today;
		GetDate(today);
		if (today.tm_year < retDate.tm_year)	return NORMAL;
		if (today.tm_year > retDate.tm_year)	return OVERDUE;
		if (today.tm_mon < retDate.tm_mon)		return NORMAL;
		if (today.tm_mon > retDate.tm_mon)		return OVERDUE;
		if (today.tm_mday < retDate.tm_mday)	return NORMAL;
		return OVERDUE;
	}
}

int main() {
	BTree L = NULL;
	BookType B;
	result res;
	ReaderType R;
	int booknum, IDnum, book_num;
	Status k;
	char flag;
	
	printf("----------------------------\n"); 
    printf("--      数据结构课设      --\n"); 
	printf("--      图书管理系统      --\n"); 
	printf("--       3219004950       --\n"); 
	printf("--         吴泳诗         --\n"); 
	printf("----------------------------\n");
	
	if (login()) {
		while (1) {
			switch (menu())
			{
			//采编图书	
			case 1:
				{
					B = (BookType)malloc(sizeof(BookNode));
					B->reader = NULL;
					printf("请输入要入库的图书编号：\n");
					fflush(stdin);
					int ID;
					scanf("%d", &ID);
					B->ID = ID;
					res = Search_BTree(L, B->ID);
					if (res.tag == NOTFOUND) {
						fflush(stdin);
						printf("请输入该书书名：\n");
						fflush(stdin);
						scanf("%s",B->bookname);
						printf("请输入该书作者：\n");
						fflush(stdin);
						scanf("%s",B->author);
						printf("请输入入库数：\n");
						fflush(stdin);
						scanf("%d", &B->current);
						printf("请输入出版年份：\n");
						fflush(stdin);
						scanf("%d", &B->publishedYear);
						printf("请输入书本价格：\n");
						fflush(stdin);
						scanf("%f", &B->price);
						B->total = B->current;
						Insert_Book(L, B, res);
						//如果是新书入库则把整棵树打印出来 
						Print(L);
						break;
					}
					else {	//图书已存在，只增加库存
						printf("书库已有该书记录，请输入增加的册数：\n");
						fflush(stdin);
						scanf("%d", &B->total);
						Insert_Book(L, B, res);
						B = (res.pt)->recptr[res.i];
						//如果是旧书增加库存则只打印该树最新信息 
						Print_Book(B);
						system("pause");
						break; 
					}	
				}

			//清空库存
			case 2:
				{
					printf("请输入要删除的图书图书编号：\n");
					fflush(stdin);
					scanf("%d", &booknum);
					res = Search_BTree(L, booknum);
					if (res.tag) {
						B = (res.pt)->recptr[res.i];
						Print_Book(B);
						printf("\n您确认删除上面的图书<Y确认，其余按键返回主菜单>?");
						fflush(stdin);
						flag = getchar();
						if (flag == 'Y' || flag == 'y') {
							Delete_Book(L, B);
							printf("\n清除完毕。\n");
							Print(L);
						}
					}
					else {
						printf("\n书库不存在此书，删除失败。\n");
					}
					printf("\t");
					system("pause");
					break;	
				}
				
			//借阅图书	
			case 3:
				{
					ReaderType reader = (ReaderType)malloc(sizeof(ReaderNode));
					reader->rnext = NULL;
					printf("请输入借阅图书图书编号：\n");
					fflush(stdin);
					scanf("%d", &booknum);
					getchar();
					res = Search_BTree(L, booknum);
					if (res.tag) {
						B = (res.pt)->recptr[res.i];
						printf("请输入您的姓名：\n");
						fflush(stdin);
						scanf("%s",reader->name);
						printf("请输入您的ID号：\n");
						fflush(stdin);
						scanf("%d", &reader->ID);
						if (Can_Borrow(L, B, reader)) {
							Borrow_Book(L, B, reader);
							printf("\n借书完成,请注意借阅最长时间为90天.");
						}
						else {
							printf("\n库存不足，借阅失败。");
							free(R);
						}
					}
					else {
						printf("\n库存无此书，借阅失败。");
						free(R);
					}
					printf("\t");
					system("pause");
					break;
				}
				
			//归还图书	
			case 4:
				int temp;
				printf("\n请输入归还图书编号：\n");
				scanf("%d", &booknum);
				printf("请输入您的ID号：\n");
				scanf("%d", &IDnum);
				k = Return_Book(L, booknum, IDnum, B, R);
				if (k == TRUE) {
					tm today;
					GetDate(today);
					if (!Earlier(today, R->retDate))
					{
						for (temp = -1; Earlier(R->retDate, today); temp++)
							AddDate(R->retDate, R->retDate, 1);
						double price = (float)temp * OUT_DATE_PRICE;
						printf("\n您借的书已经逾期%d天，需扣除逾期费用%0.1f元!", temp, price);
					}

					printf("\n借阅者%d号%s还书完成。\n",R->ID,R->name);
					R = NULL;
				}
				else if (k == FALSE) {
					printf("\n还书失败，系统没有您借该书记录。");
				}
				else {
					printf("\n书库中不存在此书!");
				}
				printf("\t");
				system("pause");
				break;
				
			//查找某种图书
			case 5:
				{
					int ID;
					printf("请输入要查找的图书的编号：\n");
					scanf("%d", &ID);
					Show_Book_Info(L, ID);
					break;
				}
				
			//输出某图书的借阅者信息	
			case 6:
				printf("请输入要查找的图书的编号：\n");
				scanf("%d", &book_num);
				res = Search_BTree(L, book_num);
				if (res.tag) {
					B = (res.pt)->recptr[res.i];
					printf("借阅者信息如下：\n");
					Print_Borrower(B);
				}
				else {
					printf("\n书库不存在此书，查找失败。\n");
				}
				printf("\t");
				system("pause");
				break;
				
			//输出总库存	
			case 7:
				printf("图书馆总库存信息如下：\n");
				Print_All(L);
				break;
			}
		}
	}
	else {
		printf("密码输入错误。请重新登录\n");
		system("pause");
	}
	return 0;
}
