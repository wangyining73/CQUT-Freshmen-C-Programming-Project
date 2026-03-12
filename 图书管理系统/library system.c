#include "Myhead.h"

int main()
{
	BOOK *bhead=createbooklist();
	loadbooks(bhead);
	READER *rhead=createreaderlist();
	loadreaders(rhead);
	BORROW *borhead=createborrowlist();
	loadborrow(borhead);
	
	int select;
	while(1)
	{
		select=main_menu();
		if(select==0)
		{
			printf("再见！\n");
			break;
		}
		else if(select==1)
		{
			READER *user=login(rhead);
			if(user==NULL)
			{
				system("pause");
				continue;
			}
			if(user->role==0)
				admin_menu(user,rhead,bhead,borhead);
			else
				reader_menu(user,rhead,bhead,borhead);
		}
		else if(select==2)
		{
			register_reader(rhead);
			system("pause");
		}
		else
		{
			set_color(12,0);
			printf("输入无效！\n");
			set_color(7,0);
		}
	}
	return 0;
}

//创建图书链表（带头结点） 
BOOK *createbooklist()
{
	BOOK *head=(BOOK *)malloc(sizeof(BOOK));
	if(head==NULL)
	{
		set_color(12,0);
		printf("内存分配失败！\n");
		set_color(7,0);
		return NULL;
	}
	head->next=NULL;
	return head;
}

//从文件中加载图书尾插
void loadbooks(BOOK *head)
{
	FILE *fp=fopen(BOOK_FILE,"r");
	if(fp==NULL)
	{
		fp=fopen(BOOK_FILE,"w");
		if(fp==NULL)
		{
			set_color(12,0);
			printf("无法创建图书文件！\n");
			set_color(7,0);
			return ;	
		} 
		fclose(fp);
		return;
	}
	BOOK temp;//防止malloc的内存读失败 
	BOOK *tail=head;
	while(fscanf(fp,"%s %s %s %s %d %f %d",temp.isbn,
	temp.name,temp.author,temp.publisher,&temp.stock,
	&temp.price,&temp.deleted)==7)
	{
		BOOK *node=(BOOK *)malloc(sizeof(BOOK));
		*node=temp;
		node->next=NULL;
		tail->next=node;
		tail=node;	
	}
	fclose(fp);	
}

//添加图书
BOOK *findbookbyISBN(BOOK *head,char *isbn)//查ISBN是否存在 
{
	BOOK *p=head->next;
	while(p)
	{
		if(strcmp(p->isbn,isbn)==0)
		return p;
		p=p->next;
	}
	return NULL;
}
BOOK *findbookbyNAME(BOOK *head,const char *name)
{
	BOOK *p=head->next;
	while(p)
	{
		if(strcmp(p->name,name)==0)
			return p;
		p=p->next;
	}
	return NULL;
}
void addbook(BOOK *head)
{
	BOOK newbook;
	printf("请输入ISBN：");
	scanf("%s",newbook.isbn);
	if(findbookbyISBN(head,newbook.isbn))
	{
		printf("该图书已存在！\n");
		return;	
	}
	printf("书名：");
	scanf("%s",newbook.name);
	printf("作者：");
    scanf("%s", newbook.author);
    printf("出版社：");
    scanf("%s", newbook.publisher);
    printf("库存：");
    scanf("%d", &newbook.stock);
    printf("价格：");
    scanf("%f", &newbook.price);
	BOOK *node=(BOOK *)malloc(sizeof(BOOK));
	*node=newbook;
	node->deleted=0;
	node->next=NULL;
	BOOK *p=head;
	while(p->next)
	{
		p=p->next;	
	}
	p->next=node;
	set_color(10,0);
	printf("添加成功！\n");
	set_color(7,0);
	savebook(head);	
} 

//图书浏览
void showbook(BOOK *head)
{
    BOOK *p = head->next;
    int has=0;
    if(p == NULL)
    {
        printf("当前没有图书信息！\n");
        return;
    }
    printf("%-15s %-20s %-15s %-25s %-8s %-8s\n",
    "ISBN", "书名", "作者", "出版社", "库存", "价格");
    while(p)
    {
    	if(p->deleted==0)
    	{
    		printf("%-15s %-20s %-15s %-25s %-8d %-8.2f\n",p->isbn,
        	p->name,p->author,p->publisher,p->stock,p->price);
			has=1;
		}
		p = p->next;
    }
    if(!has)
    	printf("当前没有可借阅的图书\n");
}

//保存函数 
void savebook(BOOK *head)
{
	FILE *fp=fopen(BOOK_FILE,"w");
	if(fp==NULL)
	{
		set_color(12,0);
		printf("book信息无法保存图书文件！\n");
		set_color(7,0);
		return;
	}
	BOOK *p=head->next;
	while(p)
	{
		fprintf(fp,"%s %s %s %s %d %.2f %d\n",p->isbn,p->name,
		p->author,p->publisher,p->stock,p->price,p->deleted);
		p=p->next;
	}
	fclose(fp);
}

//通用库存修改函数 
int changebookstock(BOOK *head,char *isbn,int delta)
{
	BOOK *book=findbookbyISBN(head,isbn);
	if(book==NULL)
	{
		set_color(12,0);
		printf("未找到该ISBN的图书！\n");
		set_color(7,0);
		return 0;
	}
	if(book->stock+delta<0)
	{
		set_color(12,0);
		printf("库存不足，无法借出！\n");
		set_color(7,0);
		return 0;
	}
	book->stock+=delta;
	savebook(head);
	return 1;
}

//读者系统
READER *createreaderlist()
{
	READER *head=(READER *)malloc(sizeof(READER));
	if(head==NULL)
	{
		set_color(12,0);
		printf("内存分配失败！\n");
		set_color(7,0);
		return NULL;
	}
	head->next=NULL;
	return head;
}

void loadreaders(READER *head)
{
	FILE *fp=fopen(READER_FILE,"r");
	if(fp==NULL)
	{
		fp=fopen(READER_FILE,"w");
		if(fp==NULL)
		{
			printf("无法创建读者文件！\n");
			return;
		}
		fclose(fp);
		return;
	}
	READER temp;
	READER *tail=head;
	while(fscanf(fp,"%s %s %s %d %d %d %f",temp.id,temp.password,
	temp.username,&temp.role,&temp.max_borrow,&temp.borrow_count,
	&temp.fine_per_day)==7)
	{
		READER *node=(READER *)malloc(sizeof(READER));
		*node=temp;
		node->next=NULL;
		tail->next=node;
		tail=node;	
	}
	fclose(fp);
} 

//存入readers文件 
void savereader(READER *head)
{
	FILE *fp=fopen(READER_FILE,"w");
	if(fp==NULL)
	{
		set_color(12,0);
		printf("reader信息无法保存文件！\n");
		set_color(7,0);
		return;
	}
	READER *p=head->next;
	while(p)
	{
		fprintf(fp,"%s %s %s ",p->id,p->password,p->username);
		fprintf(fp,"%d %d %d ",p->role,p->max_borrow,p->borrow_count);
		fprintf(fp,"%.2f\n",p->fine_per_day);
		p=p->next;		
	}
	fclose(fp);
}

//按学号查找读者 
READER *findreaderbyID(READER *head,char *id)
{
	READER *p=head->next;
	while(p)
	{
		if(strcmp(p->id,id)==0)
		return p;
		p=p->next;
	}
	return NULL;
}

//登录函数 
READER *login(READER *head)
{
	char id[12],pwd[20];
	
	printf("请输入学号：");
	scanf("%s",id);
	printf("请输入密码：");
	input_password(pwd);
	encrypt_password(pwd);
	
	READER *user=findreaderbyID(head,id);
	if(user==NULL)
	{
		set_color(12,0);
		printf("用户不存在！\n");
		set_color(7,0);
		return NULL;
	}
	if(strcmp(user->password,pwd)!=0)
	{
		set_color(12,0);
		printf("密码错误！\n");
		set_color(7,0);
		return NULL;
	}
	printf("登录成功，欢迎%s！\n",user->username);
	return user;
}

//读者借书
void borrowbook(READER *rhead,READER *user,BOOK *bookhead,BORROW *borhead)//u已登录读者，b图书链表 
{
	int select;
	BOOK *book=NULL;
	printf("1:按ISBN借书(可在【查看所有图书】中获取)\n");
	printf("2:按书名借书\n");
	printf("0:返回上一级\n");
	printf("---------------------\n");
	printf("请选择借书方式:"); 
	scanf("%d",&select);
	printf("---------------------\n");
	if(select==0)
	{
		set_color(10,0); 
		printf("已返回上一级菜单\n");
		set_color(7,0);
		return;
	}
	else if(select==1)
	{
		char isbn[20];
		printf("请输入ISBN：");
		scanf("%s",isbn);
		book=findbookbyISBN(bookhead,isbn); 
	}
	else if(select==2)
	{
		char name[50];
		printf("请输入书名：");
		scanf("%s",name);
		book=findbookbyNAME(bookhead,name);	
	}
	else
	{
		set_color(12,0);
		printf("选择无效！\n");
		set_color(7,0);
		return;	
	}	
	if(book==NULL)
	{
		set_color(12,0);
		printf("未找到该图书！\n");
		set_color(7,0);
		return;
	}
	if(book->stock<=0)
	{
		set_color(12,0);
		printf("该图书库存不足！\n");
		set_color(7,0);
		return;
	}
	if(user->borrow_count>=user->max_borrow)
	{
		set_color(12,0);
		printf("已达到最大借阅数量，无法继续借书！\n");
		set_color(7,0);
		return;
	}
	if(book->deleted)
	{
		printf("该图书已下架，无法借阅！\n");
		return;
	}
	
	BORROW *node=(BORROW *)malloc(sizeof(BORROW));
	if(node==NULL)
	{
		printf("BORROW内存分配失败！\n");
		return;
	}
	node->next=NULL;
	node->is_returned=0;
	node->record_id=getnextrecordID(borhead);
	strcpy(node->isbn,book->isbn);
	strcpy(node->reader_id,user->id);
	strcpy(node->bookname,book->name);
	
	getdate(node->borrow_date);
	if(user->role==1)//老师 
		getduedate(node->due_date,60);
	else
		getduedate(node->due_date,30);
	
	BORROW *p=borhead;
	while(p->next)
	{
		p=p->next;
	}
	p->next=node;	 
	book->stock--;
	user->borrow_count++;
	
	savebook(bookhead);	
	savereader(rhead);
	saveborrow(borhead);
	
	set_color(10,0);
	printf("借书成功！\n");
	set_color(7,0);	
	printf("借书流水号：%d\n",node->record_id);
	printf("借书日期：%s\n",node->borrow_date);
	printf("应还日期：%s\n",node->due_date);	
}

BORROW *createborrowlist()
{
	BORROW *head=(BORROW *)malloc(sizeof(BORROW));
	if(head==NULL)
	{
		set_color(12,0);
		printf("内存分配失败！\n");
		set_color(7,0);
		return NULL;
	}
	head->next=NULL;
	return head;
}

void loadborrow(BORROW *head)
{
	FILE *fp=fopen(BORROW_FILE,"r");
	if(fp==NULL)
	{
		fp=fopen(BORROW_FILE,"w");
		if(fp==NULL)
		{
			printf("无法创建借阅文件！\n");
			return;
		}
		fclose(fp);
		return;//第一次运行没有记录 
	}
	BORROW temp;
	BORROW *tail=head;
	while(fscanf(fp,"%d %s %s %s %s %s %d",&temp.record_id,
	temp.isbn,temp.reader_id,temp.bookname,temp.borrow_date,
	temp.due_date,&temp.is_returned)==7)
	{
		BORROW *node=(BORROW *)malloc(sizeof(BORROW));
		*node=temp;
		node->next=NULL;
		tail->next=node;
		tail=node;
	}
	fclose(fp); 
}

void saveborrow(BORROW *head)
{
	FILE *fp =fopen(BORROW_FILE,"w");
	if(fp==NULL)
	{
		set_color(12,0);
		printf("borrow信息无法保存文件！\n");
		set_color(7,0);
		return;
	}

	BORROW *p =head->next;
	while(p)
	{
		fprintf(fp, "%d %s %s %s ",p->record_id,p->isbn,p->reader_id,p->bookname);
		fprintf(fp,"%s %s %d\n",p->borrow_date,p->due_date,p->is_returned);
		p=p->next;
	}
	fclose(fp);
}

//流水id 
int getnextrecordID(BORROW *head)
{
	int max=0;
	BORROW *p=head->next;
	while(p)
	{
		if(p->record_id>max)
		max=p->record_id;
		p=p->next;
	}
	return max+1;	
}

void getdate(char *date)
{
	time_t t=time(NULL);
	struct tm *tm_info=localtime(&t);
	sprintf(date,"%04d-%02d-%02d",tm_info->tm_year+1900,
	tm_info->tm_mon+1,tm_info->tm_mday);	
} 

void getduedate(char *date,int days)//day是需要多少天还 
{
	time_t t=time(NULL);
	t+=days*24*60*60;
	struct tm *tm_info=localtime(&t);
	sprintf(date,"%04d-%02d-%02d",tm_info->tm_year+1900,
	tm_info->tm_mon+1,tm_info->tm_mday);
}

BORROW *findborrowbyID(BORROW *head,int record_id)
{
	BORROW *p=head->next;
	while(p)
	{
		if(p->record_id==record_id&&p->is_returned==0)
			return p;
		p=p->next;
	}
	return NULL;	
}

//用户自己未还书列表 
int showmyborrow(READER *user,BORROW *head)
{
	BORROW *p=head->next;
    int found=0;
    printf("\n========================= 当前未归还图书 ========================\n");
    printf("%-5s %-15s %-20s %-12s %-12s\n",
    "ID","ISBN","书名","借书日期","应还日期");
    while(p)
    {
        if(strcmp(p->reader_id, user->id)==0&&p->is_returned==0)
        {
            printf("%-5d %-15s %-20s %-12s %-12s\n",
            p->record_id,p->isbn,p->bookname,
            p->borrow_date,p->due_date);
            found = 1;
        }
        p=p->next;
    }
    if(!found)
        printf("暂无未归还图书\n");
    return found;
}

void returnbook(READER *rhead,READER *user,BOOK *bhead,BORROW *borhead)
{
    int rid,has=showmyborrow(user,borhead);
    if(!has)
		return; 
	printf("\n请输入要归还的借书流水号(输入0返回)：");
    scanf("%d",&rid);
    if(rid==0)
	{
		set_color(10,0);
		printf("已取消还书操作\n");
		set_color(7,0);
		return; 
	 } 
    BORROW *record=findborrowbyID(borhead,rid);
    if(record==NULL)
    {
        printf("未找到该借书记录，或已归还！\n");
        return;
    }
    BOOK *book=findbookbyISBN(bhead,record->isbn);
    if(book==NULL)
    {
    	set_color(12,0);
        printf("图书信息异常！\n");
        set_color(7,0);
        return;
    }
    record->is_returned = 1;
    book->stock++;
    if(user->borrow_count>0)
        user->borrow_count--;
    savebook(bhead);
    savereader(rhead);
    saveborrow(borhead);
    set_color(10,0);
    printf("还书成功！\n");
    set_color(7,0);
}

time_t strtodate(const char *date)//字符串形式改能算时间差的 
{
    struct tm tm_date={0};
    sscanf(date,"%d-%d-%d",&tm_date.tm_year,
    &tm_date.tm_mon,&tm_date.tm_mday);
	tm_date.tm_year-=1900;
    tm_date.tm_mon-=1;
    tm_date.tm_isdst=-1;
	return mktime(&tm_date);//返time_t类型时间 
}

int get_overdue_days(const char *due_date)//超期天数 
{
    time_t now=time(NULL);
    time_t due=strtodate(due_date);

    double diff=difftime(now,due);
    if(diff<=0)
        return 0;//没逾 
    return (int)(diff/(24 * 60 * 60));//返回正整数逾多少天 
}

float calc_fine(READER *user,BORROW *record)//根据上面overdue函数算金额 
{
    if(record->is_returned)
        return 0.0f;
    int days=get_overdue_days(record->due_date);
    if(days <= 0)
        return 0.0f;
    return days * user->fine_per_day;
}

void show_my_fines(READER *user,BORROW *borhead)//依赖calc，overdue，读者查看逾期罚款 
{
    BORROW *p=borhead->next;
    float total=0.0f;//浮点型常量，表"0"，明确float非double 
    int found=0;
    printf("\n=============== 我的逾期罚款 ===============\n");
    printf("%-5s %-20s %-12s %-8s\n","ID","书名","应还日期","罚款");
    while(p)
    {
        if(strcmp(p->reader_id,user->id)==0&&p->is_returned==0)
        {
            float fine=calc_fine(user,p);
            if(fine>0)
            {
                printf("%-5d %-20s %-12s %.2f\n",p->record_id,
				p->bookname,p->due_date, fine);
                total+=fine;
                found=1;
            }
        }
        p=p->next;
    }
    if(!found)
        printf("暂无逾期记录\n");
    else
        printf("总罚款：%.2f元\n",total);
}

void admin_show_overdue(READER *rhead,BORROW *borhead)
{
	BORROW *p=borhead->next;
	int found=0;
	printf("\n===============所有逾期未还记录===============\n");
	printf("%-5s %-20s %-12s %-12s %-8s\n","ID","借书人","应还日期","逾期天数");
	while(p)
    {
        if(p->is_returned==0)
        {
            int overdue_days=get_overdue_days(p->due_date);
            if(overdue_days>0)
            {
                printf("%-5d %-20s %-12s %-12s %-8d\n",
                p->record_id,p->bookname,p->reader_id,
                p->due_date,overdue_days);
                found = 1;
            }
        }
        p=p->next;
    }
    if(!found)
        printf("当前没有逾期未还记录\n");
}

void reader_menu(READER *user,READER *rhead,BOOK *bhead,BORROW *borhead)
{
	int select=1;
	while(select!=0)
	{
		system("cls");
		set_color(11, 0);
		printf(" 读者 %s 欢迎您！\n",user->username);
		set_color(7, 0);
		printf("---------------------\n");
		printf("1:查看所有图书\n");
		printf("2:借书\n");
		printf("3:还书\n");
		printf("4:查看我未归还的书\n");
		printf("5:查看我的所有借还记录\n"); 
		printf("6:查看我的逾期罚款\n");
		printf("0:退出\n");
		printf("---------------------\n");
		printf("请选择：");
		scanf("%d",&select);
		switch(select)
		{
			case 1:showbook(bhead);break;
			case 2:borrowbook(rhead,user,bhead,borhead);break;
			case 3:returnbook(rhead,user,bhead,borhead);break;
			case 4:showmyborrow(user,borhead);break;
			case 5:show_my_history(user,borhead);break;
			case 6:show_my_fines(user,borhead);break;
			case 0:printf("退出成功！\n");break;
			default:set_color(12,0);printf("输入无效，请重新输入！\n");set_color(7,0);break;
		}
		system("pause");
	}
}

void admin_menu(READER *user,READER *rhead,BOOK *bhead,BORROW *borhead)
{
	int select=1;
	while(select!=0)
	{
		system("cls");
		set_color(11, 0);
		printf(" 管理员 %s 欢迎您！\n",user->username);
		set_color(7, 0);
		printf("---------------------\n");
		printf("1:添加图书\n");
		printf("2:下架图书\n");
		printf("3:查看所有图书\n");
		printf("4:查看所有未归还借阅\n");
		printf("5:查看所有借还记录\n");
		printf("6:查看所有逾期未还\n");
		printf("7:查看所有读者欠款情况\n"); 
		printf("8:创建新教师用户\n");
		printf("0:退出\n");
		printf("---------------------\n");
		printf("请选择：");
		scanf("%d",&select);
		switch(select)
		{
			case 1:addbook(bhead);break;
			case 2:remove_book(bhead);break;
			case 3:showbook(bhead);break;
			case 4:show_not_return(borhead);break;
			case 5:admin_show_all_history(rhead,borhead);break;
			case 6:show_overdue(borhead);break;
			case 7:show_all_fines(rhead,borhead);break;
			case 8:add_teacher(rhead);break; 
			case 0:printf("退出成功！\n");break;
			default:set_color(12,0);printf("输入无效，请重新输入！\n");set_color(7,0);break;
		}
		system("pause");
	}
}

int main_menu()
{
	int select;
	system("cls");
	set_color(14,0);
	printf("    图书管理系统\n");
	set_color(7,0);
	printf("---------------------\n");
	printf("1:登录\n");
	printf("2:注册\n");
	printf("0:退出\n");
	printf("---------------------\n");
	printf("请选择:");
	scanf("%d",&select);
	return select;
}

void remove_book(BOOK *bhead)
{
	char isbn[20];
    printf("请输入要下架的图书ISBN(输入0返回)：");
    scanf("%s", isbn);
    if(strcmp(isbn,"0")==0)
	{
		printf("已取消下架操作\n");
	} 
    BOOK *book=findbookbyISBN(bhead,isbn);
    if(book==NULL)
    {
    	set_color(12,0);
        printf("未找到该图书！\n");
        set_color(7,0);
        return;
    }
    if(book->deleted==1)
    {
    	printf("该图书已是下架状态！\n");
    	return;
	}
    book->deleted=1;
    savebook(bhead);
    set_color(14, 0);
    printf("图书已下架\n");
    set_color(7, 0);
}
void show_not_return(BORROW *borhead)
{
	BORROW *p=borhead->next;
	int found=0;
	system("cls");
	printf("==================================未归还借阅记录==================================\n");
	printf("%-6s %-15s %-15s %-20s %-12s %-12s\n",
    "ID","ISBN","学号","书名","借书日期","应还日期");
    printf("----------------------------------------------------------------------------------\n");
	while(p)
    {
        if(p->is_returned==0)
        {
            printf("%-6d %-15s %-15s %-20s %-12s %-12s\n",
            p->record_id,p->isbn,p->reader_id,
            p->bookname,p->borrow_date,p->due_date);
            found=1;
        }
        p=p->next;
    }
    if(!found)
    {
        printf("暂无未归还的借阅记录\n");
    }
    printf("==================================================================================\n");
}

void show_overdue(BORROW *borhead)
{
	BORROW *p=borhead->next;
	int found=0;
	system("cls");
	printf("======================================逾期未还借阅记录======================================\n");
	printf("%-6s %-15s %-15s %-20s %-12s %-12s %-8s\n",
    "ID","ISBN","学号","书名","借书日期","应还日期","逾期天数");
    printf("--------------------------------------------------------------------------------------------\n");
	while(p)
    {
        if(p->is_returned==0)
        {
            int overdue_days=get_overdue_days(p->due_date);
            if(overdue_days>0)
            {
                printf("%-6d %-15s %-15s %-20s %-12s %-12s %-6d\n",
                p->record_id,p->isbn,p->reader_id,p->bookname,
                p->borrow_date,p->due_date,overdue_days);
                found=1;
            }
        }
        p=p->next;
    }
    if(!found)
    {
        printf("暂无逾期未还记录\n");
    }
    printf("============================================================================================\n");
}

void show_my_history(READER *user,BORROW *borhead)
{
	BORROW *p=borhead->next;
    int found=0;

    printf("\n=================== 我的借还历史记录 ===================\n");
    printf("%-5s %-20s %-12s %-12s %-8s\n",
    "ID", "书名", "借书日期", "应还日期", "状态");
    while(p)
    {
        if(strcmp(p->reader_id,user->id)==0)
        {
            printf("%-5d %-20s %-12s %-12s %-8s\n",
            p->record_id,p->bookname,p->borrow_date,
            p->due_date,p->is_returned ? "已还" : "未还");
            found = 1;
        }
        p = p->next;
    }
    if(!found)
        printf("暂无任何借书记录\n");

    printf("========================================================\n");
}

void show_all_fines(READER *rhead,BORROW *borhead)
{
	READER *r=rhead->next;
    int has_any = 0;
    printf("\n===================== 所有读者欠款情况 =====================\n");
    printf("%-12s %-10s %-10s\n", "学号", "姓名", "欠款金额");
    while(r)
    {
        if(r->role==0)
        {
            r=r->next;
            continue;
        }
        BORROW *b=borhead->next;
        float total_fine=0.0f;
        while(b)
        {
            if(strcmp(b->reader_id,r->id)==0&&b->is_returned==0)
            {
                total_fine+=calc_fine(r,b);
            }
            b=b->next;
        }
        if(total_fine>0)
        {
            printf("%-12s %-10s %-10.2f\n",
        	r->id,r->username,total_fine);
            has_any=1;
        }
        r=r->next;
    }
    if(!has_any)
        printf("当前没有任何读者存在逾期欠款\n");
	printf("==============================================================\n");	
}

void admin_show_all_history(READER *rhead,BORROW *borhead)
{
	BORROW *b = borhead->next;
    int found = 0;

    printf("\n=================================== 所有借还记录 ==================================\n");
    printf("%-5s %-12s %-10s %-20s %-12s %-12s %-6s\n",
           "ID","学号","姓名","书名","借书日期","应还日期","状态");

    while(b)
    {
        READER *r = findreaderbyID(rhead, b->reader_id);

        printf("%-5d %-12s %-10s %-20s %-12s %-12s %-6s\n",
               b->record_id,
               b->reader_id,
               r ? r->username : "未知",
               b->bookname,
               b->borrow_date,
               b->due_date,
               b->is_returned ? "已还" : "未还");

        found = 1;
        b = b->next;
    }

    if(!found)
        printf("暂无任何借还记录\n");

    printf("===================================================================================\n");
}

void add_teacher(READER *rhead)
{
	READER newr;
    char pwd1[20],pwd2[20];

    printf("请输入教师工号：");
    scanf("%s",newr.id);

    if(findreaderbyID(rhead,newr.id))
    {
    	set_color(12, 0);
        printf("该账号已存在！\n");
        set_color(7, 0);
        return;
    }

    printf("请输入教师姓名：");
    scanf("%s",newr.username);

    printf("请输入密码：");
    input_password(pwd1);
    printf("请再次输入密码：");
    input_password(pwd2);

    if(strcmp(pwd1,pwd2) != 0)
    {
    	set_color(12, 0);
        printf("密码不一致，创建失败！\n");
        set_color(7, 0);
        return;
    }

    encrypt_password(pwd1);
    strcpy(newr.password,pwd1);

    newr.role = 1;          
    newr.max_borrow=20;
    newr.borrow_count=0;
    newr.fine_per_day=0.2;

    READER *node=(READER *)malloc(sizeof(READER));
    *node=newr;
    node->next=NULL;

    READER *p=rhead;
    while(p->next)
    {
    	p=p->next;
	}
    p->next=node;

    savereader(rhead);
    set_color(10, 0);
    printf("教师账号创建成功！\n");
    set_color(7, 0);
}

void input_password(char *pwd)//带*密码 
{
	int i=0;
	char ch;
	while(1)
    {
        ch=getch();
        if(ch=='\r')
        {
            pwd[i]='\0';
            printf("\n");
            break;
        }
        else if(ch=='\b')
        {
            if(i>0)
            {
                i--;
                printf("\b \b");
            }
        }
        else
        {
            pwd[i++]=ch;
            printf("*");
        }
    }
}

void encrypt_password(char *pwd)//简单凯撒 
{
	int i;
    for(i=0;pwd[i];i++)
    {
        pwd[i]=pwd[i]+3;
    }
}
void decrypt_password(char *pwd)
{
	int i;
    for(i=0;pwd[i];i++)
    {
        pwd[i]=pwd[i]-3;
    }
}

void register_reader(READER *rhead)
{
    READER newr;
    char pwd1[20],pwd2[20];
    int is_admin=0;
    char invite[20];
    
    printf("请输入学号：");
    scanf("%s",newr.id);
    if(findreaderbyID(rhead,newr.id))
    {
    	set_color(12, 0);
        printf("该学号已存在！\n");
        set_color(7, 0);
        return;
    }
    printf("请输入用户名：");
    scanf("%s",newr.username);
    printf("请输入密码：");
    input_password(pwd1);
    printf("请再次输入密码：");
    input_password(pwd2);
    if(strcmp(pwd1, pwd2)!=0)
    {
    	set_color(12, 0);
        printf("密码不一致！\n");
        set_color(7, 0);
        return;
    }
    strcpy(newr.password,pwd1);
    encrypt_password(newr.password);
    
    printf("是否注册为管理员？(1是 0否)：");
    scanf("%d", &is_admin);
    if(is_admin)
    {
        printf("请输入管理员邀请码：");
        scanf("%s", invite);
        if(strcmp(invite, ADMIN_INVITE) != 0)
        {
        	set_color(12, 0);
            printf("邀请码错误，无法注册管理员！\n");
            set_color(7, 0);
            return;
        }
        newr.role=0;
        newr.max_borrow=0;
        newr.borrow_count=0;
        newr.fine_per_day=0;
    }
    else
    {
        newr.role=2;          
        newr.max_borrow=10;
        newr.borrow_count=0;
        newr.fine_per_day=0.5;
    }
    
    READER *node=(READER *)malloc(sizeof(READER));
    *node=newr;
    node->next=NULL;
    READER *p=rhead;
	while(p->next)
	{
		p=p->next;
	} 
    p->next=node;

    savereader(rhead);

	set_color(10, 0);
    printf("注册成功！\n");
    set_color(7, 0);
}

void set_color(int foreground,int background)
{
	HANDLE hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, foreground + background * 16);
}
