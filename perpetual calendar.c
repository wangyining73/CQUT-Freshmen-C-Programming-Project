#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<math.h>
/*蔡勒公式计算星期、功能模块化、节日用结构体管理
与当前系统时间结合*/ 
typedef struct{//节日结构体 
	int month;
	int day;
	char name[20];
}Festival;
Festival festivals[]={//结构数据库(阳历节日)
	{1,1,"元旦节"},
	{2,14,"情人节"},
	{3,8,"妇女节"},
	{3,12,"植树节"},
	{4,1,"愚人节"},
	{5,1,"劳动节"},
	{5,4,"青年节"},
	{6,1,"儿童节"},
	{7,1,"建党节"},
	{8,1,"建军节"},
	{9,10,"教师节"},
	{10,1,"国庆节"},
	{12,24,"平安夜"},
	{12,25,"圣诞节"}, 
};
#define FESTIVAL_COUNT (sizeof(festivals)/sizeof(festivals[0]))
int isleapyear(int year)  //闰真，不是为假
{
    return (year%4==0&&year%100!=0)||(year%400==0)?1:0;
}
int monthday(int year,int month) //某年的月份天数
{
    switch(month)
    {
       case 1:case 3:case 5:case 7: case 8: case 10: case 12:
       return 31; 
       case 4:case 6:case 9:case 11:
       return 30;
       case 2:  //2月分天数判断 
       return isleapyear(year)?29:28;
    }
    return 0;
}
int daysbetween(int y1,int m1,int d1,int y2,int m2,int d2)//反馈两个天数的差 
{
	int days1=0,days2=0;
	int year,month;
	for(year=1;year<y1;year++)//计算第一个日期的总天数 
	{
		days1+=isleapyear(year)?366:365;
	}
	for(month=1;month<m1;month++)
	{
		days1+=monthday(y1,month);
	}
	days1+=d1-1;
	for(year=1;year<y2;year++)//第二个日期的总天数 
	{
		days2+=isleapyear(year)?366:365;
	}
	for(month=1;month<m2;month++)
	{
		days2+=monthday(y2,month);
	}
	days2+=d2-1;
	return days1-days2;//返回两天之差 
}
char* getfestival(int month,int day)//基础查询函数，遍历节日数组，查找匹配的月份和日期 
{
	int i=0;
	for(i=0;i<FESTIVAL_COUNT;i++)
	{
		if(festivals[i].month==month&&festivals[i].day==day)
		return festivals[i].name;
	}
	return NULL;
}
int dayofweek(int year,int month,int day)//蔡勒公式算某天是星期几(0为周天) 
{
	int k=0,j=0,h=0;
	if(month<3)
	{
		month+=12;
		year--;	
	}
	k=year%100;
	j=year/100;
	h=(day+13*(month+1)/5+k+k/4+j/4+5*j)%7;	
	return (h+6)%7;	
} 
int first(int year,int month)//获取每月第一天是周几 
{
    return dayofweek(year,month,1); 
}
void calendar(int year,int month)  //日历显示函数 
{
    int i,j;
    int firstday=first(year,month);  //运用函数判第一天
    int allday=monthday(year,month);  //运用函数判当月一共的天数
    printf("-----------------------\n");
    printf("%10d年%d月\n",year,month);
    printf("-----------------------\n");
    printf("日 一 二 三 四 五 六\n");
    for(i=0;i<firstday;i++)  //第一行以1开头前面的空格
    {
        printf("   ");
    }
    for(j=1;j<=allday;j++)
    {
        printf("%2d",j);
        if((firstday+j)%7==0)
        {
            printf("\n");  //过六后换行
        }
        else
        {
            printf(" ");  //数字间的空格
        }
    }
    printf("\n");
}
int isvalid(int year,int month,int day)//exe3中子函数判断日期是否合规 
{
	if(year<1582||year>9999)
	return 0;
	if(month<1||month>12)
	return 0;
	if(day<1||day>monthday(year,month))
	return 0;
	return 1;//有效返回1 
}
int exe1()
{
	int year,i;
	printf("请输入人一个年份<例如2001>:");
	scanf("%d",&year); 
	if(year<1582||year>9999)  //框定年月的范围
    {
        printf("请输入 1582-9999 年之间的年份（公历适用范围）！\n");
        return 0;
    }
    for(i=1;i<=12;i++)
    {
    	calendar(year,i);//各月日历显示 
    	printf("\n");
	}
	return 0;
}
int exe2()  //月历主函数 
{
    int year,month;
    printf("请输入一个年份<例如:2001>:");
    scanf("%d",&year);
    if(year<1582||year>9999)  //框定年月的范围
    {
        printf("请输入1582-9999年之间的年份(公历适用范围)！\n");
        return 0;
    }
    printf("请输入一个月份<1-12的数字>:");
    scanf("%d",&month);
    if(month<1||month>12)
    {
        printf("请输入正确月份！");
        return 0;
    }
    calendar(year,month);
	return 0;  
}
int exe3()
{
	int year,month,day;
	printf("------------------------\n");
	printf("       日期查询\n");
	printf("------------------------\n");
	printf("请输入查询年份<1582-9999>:");
	scanf("%d",&year); 
	printf("请输入查询月份<1-12>:");
	scanf("%d",&month);
	printf("请输入查询日期<1-31>:"); 
	scanf("%d",&day);
	if(!isvalid(year,month,day))//判断输入日期是否合规 
	{
		printf("输入日期无效！\n");
		printf("%d年%d月最多有%d天\n",year,month,monthday(year,month));
		return 0;
	}
	time_t t=time(NULL);//获取当前系统时间公式 
	struct tm *current_time=localtime(&t);
	int current_year=current_time->tm_year+1900;
	int current_month=current_time->tm_mon+1;
	int current_day=current_time->tm_mday;
	printf("------------------------\n");
	printf("        查询结果\n");
	printf("------------------------\n");
	printf("查询日期：%d年%d月%d日\n",year,month,day);
	int weekday=dayofweek(year,month,day);//该天为周几操作 
	printf("这天是:"); 
	switch(weekday)
	{
		case 1:printf("星期一\n");break;
		case 2:printf("星期二\n");break;
		case 3:printf("星期三\n");break;
		case 4:printf("星期四\n");break;
		case 5:printf("星期五\n");break;
		case 6:printf("星期六\n");break;
		case 0:printf("星期日\n");break;
	}
	//距今信息 
	int days_diff=daysbetween(year,month,day,current_year,current_month,current_day);
	printf("距今信息：");
	if(days_diff==0)
	{
		printf("就是今天！\n");
	}
	else if(days_diff>0)
	{
		printf("距今还有%d天\n",days_diff);	
	}
	else
	{
		printf("距今已过去%d天\n",abs(days_diff));	
	}
	//节日信息 
	char* festival=getfestival(month,day);
	printf("节日信息：");
	if(festival!=NULL)
	{
		printf("%s\n",festival);
	}
	else
	{
		printf("非公历节日\n");
	}
}
int main() 
{
	int select=1;
	while(select!=0)
	{
		system("cls"); 
		printf("------------------------\n");
		printf("         万年历\n");
		printf("------------------------\n");
		printf("1:输入年份，输出年历\n");
		printf("2:输入年、月，输出月历\n");
		printf("3:当日日期查询\n"); 
		printf("0:退出\n");
		printf("------------------------\n");
		printf("请选择:");
		scanf("%d",&select);
		switch(select)
		{
			case 1:exe1();break;
			case 2:exe2();break;
			case 3:exe3();break;
			case 0:printf("期待您的下次使用！\n");break;
			default:printf("请输入正确题号！");break;
		} 
		system("pause"); 
	}
	return 0;
}
