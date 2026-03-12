#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<conio.h>
#include<windows.h> 
#define BOOK_FILE "books.txt"
#define READER_FILE "readers.txt"
#define BORROW_FILE "borrow.txt"
#define ADMIN_INVITE "CQUT2026" 
typedef struct bk{
	char isbn[20];
	char name[50];
	char author[30];
	char publisher[50]; 
	int stock;
	float price;
	int deleted;//0正常，1下架 
	struct bk *next;
}BOOK;
typedef struct rd{
	char id[12];
	char password[20];
	char username[30];
	int role;//0为管理员，1老师，2学生 
	int max_borrow;
	int borrow_count;//当前已借数量 ，管理员为0 
	float fine_per_day;//超期每天罚款，管理员为0 
	struct rd *next; 
}READER;
typedef struct bw{
	int record_id;//借书流水号(递增) 
	char isbn[20]; 
	char reader_id[20];
	char bookname[50]; 
	char borrow_date[20];
	char due_date[20]; 
	int is_returned;//是否归还0未还1已还 
	struct bw *next;
}BORROW;

BOOK *createbooklist();
void loadbooks(BOOK *head);
BOOK *findbookbyISBN(BOOK *head,char *isbn);
BOOK *findbookbyNAME(BOOK *head,const char *name); 
void addbook(BOOK *head);
void showbook(BOOK *head);
void savebook(BOOK *head); 
int changebookstock(BOOK *head,char *isbn,int delta);

READER *createreaderlist(); 
void loadreaders(READER *head);
void savereader(READER *head);
READER *findreaderbyID(READER *head,char *id);
READER *login(READER *head);
void borrowbook(READER *rhead,READER *user,BOOK *bookhead,BORROW *borhead); 

BORROW *createborrowlist();
void loadborrow(BORROW *head);
void saveborrow(BORROW *head);
int getnextrecordID(BORROW *head);
void getdate(char *date);
void getduedate(char *date,int day);

BORROW *findborrowbyID(BORROW *head,int record_id);
int showmyborrow(READER *user,BORROW *head);
void returnbook(READER *rhead, READER *user,BOOK *bhead, BORROW *borhead); 

time_t strtodate(const char *date);
int get_overdue_days(const char *due_date);
float calc_fine(READER *user,BORROW *record);
void show_my_fines(READER *user,BORROW *borhead);

void admin_show_overdue(READER *rhead,BORROW *borhead);
void remove_book(BOOK *bhead);
void show_not_return(BORROW *borhead);
void admin_show_all_history(READER *rhead,BORROW *borhead);
void show_overdue(BORROW *borhead);
void show_my_history(READER *user,BORROW *borhead);
void show_all_fines(READER *rhead,BORROW *borhead);
void add_teacher(READER *rhead);

void reader_menu(READER *user,READER *rhead,BOOK *bhead,BORROW *borhead);
void admin_menu(READER *user,READER *rhead,BOOK *bhead,BORROW *borhead);
int main_menu();
void set_color(int foreground,int background);

void input_password(char *pwd); 
void encrypt_password(char *pwd);
void decrypt_password(char *pwd);
void register_reader(READER *rhead);
