#include "struct.h"

//��p->key[1..p->keynum]��k 
int Search(BTree p,int k) 
{
	int i = 1;
	while(i <= p->keynum && k > p->key[i]) i++;
	return i; 
}

result Search_BTree(BTree T,KeyType k) 
{
	/**
	 * ��m��B��T�ϲ��ҹؼ���k����r����(pt,i,tag)
	 * �����ҳɹ�������tag=1��ָ��pt��ָ����е�i���ؼ��ֵ���k
	 * ����tag=0����Ҫ����ؼ���Ϊk�ļ�¼��Ӧλ��pt����е�i-1���͵�i���ؼ���֮�� 
	 */
	result r;
	int i = 0,found = NOTFOUND;
	//��ʼ��pָ����ڵ㣻p������ָ������㣬qָ����˫�� 
	BTree p = T,q = NULL;
	while(p && found == NOTFOUND) {
		//��p->key[1..p->keynum]�в���p->key[i-1]<k<=p->key[i]		
		i = Search(p,k);
		//�ҵ�����ؼ���
		if(i <= p->keynum && p->key[i] == k) found = 1;
		else {
			//û�ҵ���ָ������
			q = p;
			p = p->ptr[i-1]; 
		} 
	}
	if(found == FOUND) {
		//���ҳɹ�������k��λ��p��i 
		r.pt = p;
		r.i = i;
		r.tag = FOUND;
	} else {
		//���Ҳ��ɹ�������k�Ĳ���λ��q��i 
		r.pt = q;
		r.i = i;
		r.tag = NOTFOUND;
	}
	return r;
} 

//�����µĸ��ڵ�
void New_Root(BTree &T, BTree p, int k, BTree ap, Record *rec) {
	T = (BTNode*)malloc(sizeof(BTNode));
	T->keynum = 1;
	T->ptr[0] = p;
	T->ptr[1] = ap;
	T->key[1] = k;
	T->recptr[1] = rec;
	if (p != NULL) p->parent = T;
	if (ap != NULL) ap->parent = T;
	T->parent = NULL;				//�¸���˫���ǿ�ָ��
}

//���ѽ�� 
void Split_Node(BTree &q,int s,BTree &ap) {
	//��q�����ѳ�������㣬ǰһ�뱣����ԭ��㣬��һ������ap��ָ�½��
	int i,j,n = q->keynum;
	//�����½�� 
	ap = (BTNode *)malloc(sizeof(BTNode));
	ap->ptr[0] = q->ptr[s];
	//��һ������ap��� 
	for(i = s+1,j= 1;i <= n;i++,j++) {
		ap->key[j] = q->key[i];
		ap->ptr[j] = q->ptr[i];
		ap->recptr[i-s] = q->recptr[i];
	} 
	ap->keynum = n-s;
	ap->parent = q->parent;
	//�޸��½����ӽڵ��parent�� 
	for(i = 0;i <= n-s;i++) {
		//��ap���к��ӽ��ָ��ap
		if(ap->ptr[i])	
			ap->ptr[i]->parent = ap;
		//q����ǰһ�뱣�����޸�keynum 
		q->keynum = s-1;
	}
}

/**
 * k��ap�ֱ�嵽q->key[i+1]��q->ptr[i+1]
 * ������ؼ���Ϊk�ļ�¼recprt
 */
void Insert(BTree &q, int i, int k, BTree &ap, Record *rec) {
	int j = q->keynum;
    for (j; j >= i; j--) {
        q->key[j + 1] = q->key[j];	//�ؼ���ָ�������һλ 
        q->ptr[j + 1] = q->ptr[j];	//���ӽ��ָ�������һλ 
        q->recptr[j+1] = q->recptr[j];
    }
    q->key[i] = k;	//��ֵ 
    q->ptr[i] = ap;
    q->recptr[i] = rec;
    if (ap != NULL)
        ap->parent = q;
    q->keynum++;	//����ؼ��ֺ�ý��Ĺؼ�������+1 
}

/**
 * ��m��B��T�Ͻ��*q��key[i]��key[i+1]֮�����ؼ���K�ͼ�¼rec��
 * ���������������˫�������б�Ҫ�Ľ����ѵ�����ʹT����m��B����
 */
Status Insert_BTree(BTree &T, KeyType k, BTree q, int i, Record *rec)
{
	BTree ap = NULL;
	int finished = FALSE;
	if (!q) New_Root(T, NULL, k, NULL, rec);		// T�ǿ��������ɽ����ؼ���K�ĸ����*T
	else {
		while (!finished)
		{
			Insert(q, i, k, ap, rec);			// ��k��ap�ֱ���뵽q->key[i+1]��q->ptr[i+1]
			if (q->keynum < m) finished = TRUE; // �������
			else {
				Split_Node(q, (m + 1) / 2, ap);		// ���ѽ��Q
				k = q->key[(m + 1) / 2];
				rec = q->recptr[(m + 1) / 2];
				if (q->parent)
				{								// ��˫�׽��*q�в���k�Ĳ���λ��
					q = q->parent;
					i = Search(q, k);
				}
				else finished = OVERFLOW;		// ���ڵ��ѷ���Ϊ*q��*ap�������
			}
		}
		if (finished == OVERFLOW)				// ������ѷ���Ϊ���*q��*ap
			New_Root(T, q, k, ap, rec);			// �������¸����*T,q��apΪ����ָ��
	}
	return OK;
}

void Successor(BTree &q, int i) {
	//*q���ĵ�i���ؼ���Ϊk����q�ĺ�̹ؼ��ִ���q������qָ�������ڽ��
	BTree p = q;
	q = q->ptr[i];
	while (q->ptr[0]) q = q->ptr[0];	//����p�ĺ��
	p->key[i] = q->key[1];			//��¼����
	p->recptr[i] = q->recptr[1];
	i = 1;							//�����Ӧ��ɾ��q��ָ���ĵ�1���ؼ���
}

void Remove(BTree &q, int i) {
	//ɾ��q��ָ����i���ؼ��ּ����¼
	for (; i < q->keynum; i++) {	//�ؼ��ֺͼ�¼ָ��ǰ��
		q->key[i] = q->key[i + 1];
		q->recptr[i] = q->recptr[i + 1];
	}
	q->keynum--;					//�ؼ�����Ŀ��1
}

/**
 * ��q���ֵܽ��ؼ��ִ���(m-1)/2,����ֵܽ��������С������󣩵Ĺؼ��ֵ�˫�׽�㣬
 * ����˫�׽����С��(����ڣ��ҽ����ùؼ��ֵĹؼ���������q��,������OK�����򷵻�EREOR��
 */
Status Borrow(BTree &q)
{
	int	i;
	BTree p = q->parent, b = NULL;				// pָ��q��˫�׽��
	for (i = 0; p->ptr[i] != q; i++);			// ����q��˫��p������λ��
	if (i >= 0 && i + 1 <= p->keynum && p->ptr[i + 1]->keynum > (m - 1) / 2)
	{											// ��q�����ֵܹؼ��ָ�������(m-1)/2
		b = p->ptr[i + 1];						// bָ�����ֵܽ��
		q->ptr[1] = b->ptr[0];					// ����ָ��ҲҪͬ���ƶ�
		q->key[1] = p->key[i + 1];				// �Ӹ��ڵ���i+1���ؼ���
		q->recptr[1] = p->recptr[i + 1];
		p->key[i + 1] = b->key[1];				// b��һ���ؼ������Ƶ����ڵ�
		p->recptr[i + 1] = b->recptr[1];
		for (i = 1; i <= b->keynum; i++)		// b��һ���ؼ������ƣ����ʣ���¼ǰ��һλ
		{
			b->key[i] = b->key[i + 1];
			b->recptr[i] = b->recptr[i + 1];
			b->ptr[i - 1] = b->ptr[i];
		}
	}
	else if (i > 0 && p->ptr[i - 1]->keynum > (m - 1) / 2)
	{											// ��q�����ֵܹؼ��ָ�����Լ(m-1)/2
		b = p->ptr[i - 1];						// bָ�����ֵܽ��
		q->ptr[1] = q->ptr[0];
		q->ptr[0] = b->ptr[b->keynum];
		q->key[1] = p->key[i];					// �Ӹ��ڵ���i���ؼ���
		q->recptr[1] = p->recptr[i];
		p->key[i] = b->key[b->keynum];			// ��b���һ���ؼ������Ƶ����ڵ�
		p->recptr[i] = b->recptr[b->keynum];
	}
	else return ERROR;							// �޹ؼ��ִ���(m-1)/2���ֵ�
	q->keynum++;
	b->keynum--;
	for (i = 0; i <= q->keynum; i++)
		if (q->ptr[i]) q->ptr[i]->parent = q;	// ˢ��q���ӽ���˫��ָ��
	return OK;
}

void Merge(BTree &q) {
	int i, j;
	BTree p = q->parent, b = NULL;				//pָ��q�ĸ���
	for (i = 0; p->ptr[i] != q; i++);			//����q�ڸ���p�е�����λ��
	if (i == 0) {								//��Ϊ0������ϲ�Ϊ�ֵܵĵ�һ���ؼ���
		b = p->ptr[i + 1];
		for (j = b->keynum; j >= 0; j--) {		//��b�Ĺؼ��ֺͼ�¼����һλ
			b->key[j + 1] = b->key[j];
			b->recptr[j + 1] = b->recptr[j];
			b->ptr[j + 1] = b->ptr[j];
		}
		b->ptr[0] = q->ptr[0];					//�ϲ�
		b->key[1] = p->key[1];
		b->recptr[1] = p->recptr[1];
	}
	else if (i > 0) {//��q�ڸ��׵�����λ�ô���0����ϲ�Ϊ�ֵ�b�����һ���ؼ���
		b = p->ptr[i - 1];
		b->key[b->keynum + 1] = p->key[i];		//�ϲ�
		b->recptr[b->keynum + 1] = p->recptr[i];
		b->ptr[b->keynum + 1] = q->ptr[0];
	}
	if (i == 0 || i == 1)//��iΪ0��1���轫�����p�ؼ���ǰ��һλ
		for (i; i < p->keynum; i++) {
			p->key[i] = p->key[i + 1];
			p->ptr[i] = p->ptr[i + 1];
			p->recptr[i] = p->recptr[i + 1];
		}
	p->keynum--;
	b->keynum++;
	free(q);
	q = b;										//qָ���޸ĵ��ֵܽ��
	for (i = 0; i <= b->keynum; i++)
		if (b->ptr[i]) b->ptr[i]->parent = b;	//ˢ��b���ӽ���˫��ָ��
}

/**
 * ��m��B��T��ɾ���ؼ���k�����Ӧ��¼��������OK��
 * ��T�ϲ����ڹؼ���k���򷵻�ERROR��
 */
Status Delete_BTree(BTree &T, KeyType k)
{
	KeyType	x = k;
	BTree q, b = NULL;
	int finished = FALSE, i = 1;
	result res = Search_BTree(T, k);				// ��T�в��ҹؼ���k
	if (res.tag == 0) return ERROR;				// δ������
	else
	{
		q = res.pt;								// qָ���ɾ���
		i = res.i;
		if (q->ptr[0]) Successor(q, i);			// ��q���������գ�(�ǵײ���)
												// �������̴�֮������qָ�������ڽ��
		Remove(q, i);							// ɾ��q��ָ�����е�i���ؼ��ּ���¼
		if (q->keynum >= (m - 1) / 2 || !q->parent)		// ��ɾ����ؼ��ָ�����С��(m-1)/2��q�Ǹ��ڵ�
		{
			finished = TRUE;					// ɾ�����
			if (q->keynum == 0) T = NULL;		// ��q�Ĺؼ��ָ���Ϊ0 ����Ϊ����
		}
		while (!finished)
		{
			if (Borrow(q))	finished = TRUE;	// ��q�������ֵܽ��ؼ��ִ���(m-1)/2,��Ӹ�
												// �ֵܽ������һ����󣨻���С���ؼ��ֵ�
												// ���ڵ㣬�Ӹ��ڵ��һ�ؼ��ֵ�q
			else {								// ��q�����ֵܹؼ��ָ��������ک�m /2��-1
				Merge(q);						// ��q�е�ʣ�ಿ�ֺ�˫���е���عؼ��ֺϲ���q��һ���ֵ���
				q = q->parent;					// ���˫��
				if (q == T && T->keynum == 0)	// ����ɾ���ĸ��ڵ��Ǹ�T��T�Ĺؼ��ָ���Ϊ0
				{
					T = T->ptr[0];				// �¸�	
					T->parent = NULL;
					free(q);					// ɾ��ԭ˫�׽��
					finished = TRUE;
				}
				else if (q->keynum >= m / 2) finished = TRUE;
			}									// �ϲ���˫�׹ؼ��ָ���������(m-1)/2�����
		}
	}
	return OK;
}

//����B��T����ÿ��������visit����
void BTree_Traverse(BTree T, void(*visit)(BTree)) {
	if (!T) return;
	//��ӡT����ڶ�Ӧ�����йؼ�����Ϣ 
	visit(T);
	for (int i = 0; i <= T->keynum; i++) {
		if (T->ptr[i]) BTree_Traverse(T->ptr[i], visit);
	}
}

// �ݹ��԰������ʽ��ʾB��T,ÿ���������Ϊtab
void Print_BTree(BTree T,int tab)
{
	if (!T)	return;
	int i;
	for (i = 0; i <= tab; i++) putchar(' ');			// ����x
	for (i = 1; i <= T->keynum; i++)
	{
		printf("%d  ", T->key[i]);
	}
	printf("\n");
	for (i = 0; i <= T->keynum; i++)				// �ݹ���ʾ�������ؼ���
		Print_BTree(T->ptr[i], tab + 3);
}

void Print(BTree T)
{
	printf("========================================\n");
	printf("==          �������ʽ��ʾB��         ==\n");
	Print_BTree(T,4);
	printf("========================================\n");
}

/**
 * ���L�Ѵ��ڣ�res����B�������L�е�λ�û�Ӧ�ò����λ��
 * ���������Ѵ���B�飬��ֻ��B��Ŀ�������ӣ��������B�鵽���L��
 */
void Insert_Book(BTree &L, BookType B, result res) {
	BTree pt;
	BookType book;
	//����в����ڸ��飬�����
	if (res.tag == NOTFOUND) {
		if(OK == Insert_BTree(L, B->ID, res.pt, res.i, B)) printf("�ɱ�ɹ���\n");
		else printf("�ɱ�ʧ�ܣ�\n");	
	}
	else {
		book = (res.pt)->recptr[res.i];
		book->current += B->total;		//�ִ�������
		book->total += B->total;		//�ܿ������
		printf("�ɱ�ɹ���\n");
	}
}

//�������д���B�飬��������ɾ��B�����Ϣ��������OK�����򷵻�ERROR
Status Delete_Book(BTree &L, BookType B) {
	if (Delete_BTree(L, B->ID)) return OK;	//ɾ���ɹ�
	else return ERROR;							//ɾ��ʧ��
}

//�������д���B�飬��B���ִ�������0����ɳ��裬����TRUE�����򷵻�FALSE
Status Can_Borrow(BTree L, BookType B, ReaderType R) {
	if (B->current > 0) return TRUE;			//�ִ���������
	else return FALSE;							//������������������
}

/**
 * ���L���ڣ�B��������е��鲢�ҿɱ�����R���ģ���ͨ��AvaiBorrow()�жϣ�
 * ���һ��B�飬�Ǽǽ�����R����Ϣ���ı��ִ�����
 */
void Borrow_Book(BTree L, BookType B, ReaderType R)
{
	ReaderType	r;
	GetDate(R->borDate);						//  ��ȡϵͳ��ǰʱ��Ϊ����ʱ������
	AddDate(R->retDate, R->borDate, KEEP_DAYS);	//��ǰʱ�����90��Ϊ��������
	if (!B->reader) B->reader = R;				// �����������ߣ���ֱ�ӵǼ�
	else {
		for (r = B->reader; r->rnext; r = r->rnext);
		r->rnext = R;							// ���򵽽���������β���Ǽ�
	}
	B->current--;								// �ִ�����1
}

/**
 * booknumΪ����ͼ���ţ�IDnum�ǽ����ߵ�ID��
 * ������в�����ͼ����Ϊbooknum���飬��������������-1
 * ���н��ļ�¼����ע���ü�¼������B��R����ͼ����Ϣ�ͽ�������Ϣ������1
 * ��û��r���ĵļ�¼������B����ͼ����Ϣ��������0
 */
Status Return_Book(BTree L, int ID, int readerID, BookType &B, ReaderType &R) {
	result res = Search_BTree(L, ID);		//��B���ϰ�ͼ��������
	if (res.tag == NOTFOUND) return OVERFLOW;
	B = (res.pt)->recptr[res.i];
	ReaderType reader = (res.pt)->recptr[res.i]->reader;
	ReaderType pre = reader;
	while (reader) {
		if (reader->ID == readerID) {
			R = reader;
			B->current++;						//�ִ�����һ
			reader->retDate = reader->borDate;			//�ѹ黹����Ľ����ߵĻ���ʱ����Ϊ����ʱ�䣬����֮����Ľ�������Ϣʱ�ж� 
			return OK;
		}
		pre = reader;
		reader = reader->rnext;
	}
	return ERROR;

}

//���ͷ����ʽ
void Print_Head() {
	printf("\n");
	printf("|---------------------------------ͼ�������Ϣ----------------------------------|\n");
	printf("|  ���  |       ����         |      ����     | �ִ� | �ܿ�� | ������� | ���� |\n");
}

//��ʾB�������Ϣ
void Print_Body(BookType B) { 
	printf("|--------|--------------------|---------------|------|--------|----------|------|\n");
	printf("|  %-4d  |%-20s|%-15s|", B->ID, B->bookname, B->author);
	printf(" %-5d|   %-4d | %-6d   |%-6.1f|\n", B->current, B->total, B->publishedYear, B->price);
}

//���ײ���ʽ
void Print_Tail() {
	printf("|--------|--------------------|---------------|------|--------|----------|------|\n");
}

//�Ա����ʽ��ʾһ����Ļ�����Ϣ
void Print_Book(BookType B) {	
	Print_Head();
	Print_Body(B);
	Print_Tail();
	printf("\n");
}

//�Ա����ʽ��ʾһ����Ľ�������Ϣ
void Print_Borrower(BookType B) {	
	ReaderType p;
	ReaderType r = B->reader;
	char rpos[3][7] = { "����","����","�ѹ黹" };
	char * status;
	printf("|-------------------------ͼ�������Ϣ------------------------|\n");
	printf("\n");
	printf("| ID��|  ����  |    ��������    |    �黹����    | ״̬ |\n");
	printf("|-----|--------|----------------|----------------|------|\n");
	for (p = B->reader; p; p = p->rnext)
	{
		status = rpos[Earlier(p->borDate, p->retDate)];
		printf("|%-5d|%-8s| %-4d��%2d��%2d�� | %-4d��%2d��%2d�� |%-6s|\n",
			p->ID, p->name, p->borDate.tm_year, p->borDate.tm_mon, p->borDate.tm_mday,
			p->retDate.tm_year, p->retDate.tm_mon, p->retDate.tm_mday, status);
	}
}

//��ʾB��ÿ�����ļ�¼��Ϣ
void Print_BookNode(BTree p) {
	int i;
	for (i = 1; i <= p->keynum; i++) {
		Print_Body(p->recptr[i]);
	}
}

//��ʾ���L����ͼ����Ϣ
void Print_All(BTree L) {		
	Print_Head();
	BTree_Traverse(L, Print_BookNode);
	Print_Tail();
}

/**
 * �����L�д���ͼ����ΪID���飬�����ڸ����������Ϣ������OK
 * ���L�������򷵻�ERROR
 */
Status Show_Book_Info(BTree L, int ID) {
	BookType B;
	result res = Search_BTree(L, ID);
	if (res.tag == NOTFOUND) {
		printf("����в����ڴ��飬����ʧ�ܡ�\n");
		return ERROR;
	}
	B = (res.pt)->recptr[res.i];
	Print_Book(B);
	return OK;
}

int menu() 
{	//�˵�
	int choice;
	printf("------------------------------------------------------\n");
	printf("--                                                  --\n");
	printf("--                  ͼ��ݹ���ϵͳ                  --\n");
	printf("--                                                  --\n");
	printf("--                  1.�ɱ����                      --\n");
	printf("--                  2.������                      --\n");
	printf("--                  3.����ͼ��                      --\n");
	printf("--                  4.�黹ͼ��                      --\n");
	printf("--                  5.�鿴ĳͼ����Ϣ                --\n");
	printf("--                  6.�鿴ĳ���������Ϣ            --\n");
	printf("--                  7.�鿴ͼ���ȫ��ͼ��            --\n");
	printf("--                  0.�˳�                          --\n");
	printf("--                                                  --\n");
	printf("------------------------------------------------------\n");

	do {
		printf("��ѡ��ָ���������ܣ�����0-7����һ�����֣�:");
		scanf("%d", &choice);
	} while (choice < 0 || choice > 7);			//����Ƿ�����
	return choice;
}

//��½����
Status login() {	
	int password, i;
	printf("---------------------------------\n"); 
	printf("--                             --\n"); 
	printf("--         ͼ�����ϵͳ        --\n"); 
	printf("--  Welcome to Library System  --\n");
	printf("--                             --\n"); 
	printf("--                             --\n"); 
	printf("--         ���ݽṹ����        --\n"); 
	printf("--          3219004950         --\n"); 
	printf("--           ��Ӿʫ            --\n"); 
	printf("--                             --\n"); 
	printf("---------------------------------\n");
	printf("������ͼ�����Ա����:");	//����Ϊ4950 
	scanf("%d", &password);
	fflush(stdin);

	Sleep(100);
	system("cls");				//����

	if (password == Manager_Code)//�ж��ǲ��ǹ���Ա
		return OK;
	else
		printf("����������������룡\n");
		login();
}

// ��ȡϵͳʱ�䣬����tm�ṹ�����tim
void GetDate(tm &tim)		
{
	time_t curtime = time(0);
	tim = *localtime(&curtime);
	tim.tm_year += 1900;		              // tm �����1900��
	tim.tm_mon++;				         // tm month ��0-11���ʼ�1
}

// ��date1�����ڼ�day��󸳸�date2
void AddDate(tm &date2, tm date1, int day)
{
	date2.tm_year = date1.tm_year + (day / 30 + date1.tm_mon) / 12;
	date2.tm_mon = (date1.tm_mon + (day / 30)) % 12;
	date2.tm_mday = (date1.tm_mday + day) % 30;
}

/**
 * ���ȱȽ�borDate��retDate�Ƿ�һ�£���Ϊ����ʱ���������ʱ����Ϊһ����Ϊ�ѹ黹�ı�־��
 * ���һ���򷵻�RETURNED��ʾ�ѹ黹
 * �����һ�����ȡϵͳ��ǰʱ��today����retDate�Ƚ�
 * ���today����/����retDate�򷵻�NORMAL�����򷵻�OVERDUE 
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
    printf("--      ���ݽṹ����      --\n"); 
	printf("--      ͼ�����ϵͳ      --\n"); 
	printf("--       3219004950       --\n"); 
	printf("--         ��Ӿʫ         --\n"); 
	printf("----------------------------\n");
	
	if (login()) {
		while (1) {
			switch (menu())
			{
			//�ɱ�ͼ��	
			case 1:
				{
					B = (BookType)malloc(sizeof(BookNode));
					B->reader = NULL;
					printf("������Ҫ����ͼ���ţ�\n");
					fflush(stdin);
					int ID;
					scanf("%d", &ID);
					B->ID = ID;
					res = Search_BTree(L, B->ID);
					if (res.tag == NOTFOUND) {
						fflush(stdin);
						printf("���������������\n");
						fflush(stdin);
						scanf("%s",B->bookname);
						printf("������������ߣ�\n");
						fflush(stdin);
						scanf("%s",B->author);
						printf("�������������\n");
						fflush(stdin);
						scanf("%d", &B->current);
						printf("�����������ݣ�\n");
						fflush(stdin);
						scanf("%d", &B->publishedYear);
						printf("�������鱾�۸�\n");
						fflush(stdin);
						scanf("%f", &B->price);
						B->total = B->current;
						Insert_Book(L, B, res);
						//�����������������������ӡ���� 
						Print(L);
						break;
					}
					else {	//ͼ���Ѵ��ڣ�ֻ���ӿ��
						printf("������и����¼�����������ӵĲ�����\n");
						fflush(stdin);
						scanf("%d", &B->total);
						Insert_Book(L, B, res);
						B = (res.pt)->recptr[res.i];
						//����Ǿ������ӿ����ֻ��ӡ����������Ϣ 
						Print_Book(B);
						system("pause");
						break; 
					}	
				}

			//��տ��
			case 2:
				{
					printf("������Ҫɾ����ͼ��ͼ���ţ�\n");
					fflush(stdin);
					scanf("%d", &booknum);
					res = Search_BTree(L, booknum);
					if (res.tag) {
						B = (res.pt)->recptr[res.i];
						Print_Book(B);
						printf("\n��ȷ��ɾ�������ͼ��<Yȷ�ϣ����ఴ���������˵�>?");
						fflush(stdin);
						flag = getchar();
						if (flag == 'Y' || flag == 'y') {
							Delete_Book(L, B);
							printf("\n�����ϡ�\n");
							Print(L);
						}
					}
					else {
						printf("\n��ⲻ���ڴ��飬ɾ��ʧ�ܡ�\n");
					}
					printf("\t");
					system("pause");
					break;	
				}
				
			//����ͼ��	
			case 3:
				{
					ReaderType reader = (ReaderType)malloc(sizeof(ReaderNode));
					reader->rnext = NULL;
					printf("���������ͼ��ͼ���ţ�\n");
					fflush(stdin);
					scanf("%d", &booknum);
					getchar();
					res = Search_BTree(L, booknum);
					if (res.tag) {
						B = (res.pt)->recptr[res.i];
						printf("����������������\n");
						fflush(stdin);
						scanf("%s",reader->name);
						printf("����������ID�ţ�\n");
						fflush(stdin);
						scanf("%d", &reader->ID);
						if (Can_Borrow(L, B, reader)) {
							Borrow_Book(L, B, reader);
							printf("\n�������,��ע������ʱ��Ϊ90��.");
						}
						else {
							printf("\n��治�㣬����ʧ�ܡ�");
							free(R);
						}
					}
					else {
						printf("\n����޴��飬����ʧ�ܡ�");
						free(R);
					}
					printf("\t");
					system("pause");
					break;
				}
				
			//�黹ͼ��	
			case 4:
				int temp;
				printf("\n������黹ͼ���ţ�\n");
				scanf("%d", &booknum);
				printf("����������ID�ţ�\n");
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
						printf("\n��������Ѿ�����%d�죬��۳����ڷ���%0.1fԪ!", temp, price);
					}

					printf("\n������%d��%s������ɡ�\n",R->ID,R->name);
					R = NULL;
				}
				else if (k == FALSE) {
					printf("\n����ʧ�ܣ�ϵͳû����������¼��");
				}
				else {
					printf("\n����в����ڴ���!");
				}
				printf("\t");
				system("pause");
				break;
				
			//����ĳ��ͼ��
			case 5:
				{
					int ID;
					printf("������Ҫ���ҵ�ͼ��ı�ţ�\n");
					scanf("%d", &ID);
					Show_Book_Info(L, ID);
					break;
				}
				
			//���ĳͼ��Ľ�������Ϣ	
			case 6:
				printf("������Ҫ���ҵ�ͼ��ı�ţ�\n");
				scanf("%d", &book_num);
				res = Search_BTree(L, book_num);
				if (res.tag) {
					B = (res.pt)->recptr[res.i];
					printf("��������Ϣ���£�\n");
					Print_Borrower(B);
				}
				else {
					printf("\n��ⲻ���ڴ��飬����ʧ�ܡ�\n");
				}
				printf("\t");
				system("pause");
				break;
				
			//����ܿ��	
			case 7:
				printf("ͼ����ܿ����Ϣ���£�\n");
				Print_All(L);
				break;
			}
		}
	}
	else {
		printf("����������������µ�¼\n");
		system("pause");
	}
	return 0;
}
