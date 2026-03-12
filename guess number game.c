#include<stdio.h>
#include<stdlib.h>
#include<time.h>
/*二分，ANSI颜色，人机身份互换*/
int exe1()
{
	srand(time(NULL));//播下随机数种子
	int result=rand()%100+1;
	int num=0,count=0;//存放输入的数字
	time_t start,end;
	double duration;
	start=time(NULL);
	printf("系统已随机想好一个1-100的整数数字，来猜猜看吧！\n");
	system("pause");
	printf("\033[41m计时开始！\033[0m\n");
	do
	{
		printf("请输入：");
		scanf("%d",&num);
		if(num>result)
		{
			printf("猜大了,再试试吧\n");
			count++; 
		}
		else if(num<result)
		{
			printf("猜小了，再试试吧\n");
			count++;
		}
		else if(num==result)
		{
			end=time(NULL);
			duration=difftime(end,start);
			count++; 
			printf("\033[93m猜对啦，真厉害！\033[0m\n");
			printf("用时：%0f秒,猜了%d次\n",duration,count);	
			break; 
		}	
	} 
	while(num!=result);
	printf("\033[93mresult=%d\033[0m\n",result);
	printf("\n");
	return 0;
}
int exe2()//二分 
{
	int low=1,high=100,count=0,guess,feedback;
	double duration;
	time_t start,end;
	printf("请玩家在心里想一个1-100间的整数，我来猜！\n");
	printf("我的猜测结果你可以用数字告诉我\n");
	printf("\033[93m1:我猜小了，2:我猜大了，3:猜对啦\033[0m\n");
	printf("准备好了吗^_^\n");
	system("pause"); 
	start=time(NULL);
	while(1)
	{
		guess=(low+high)/2;
		count++;
		printf("我猜是：%d\n",guess);
		printf("请反馈(1:我猜小了，2:我猜大了，3:猜对啦):");
		scanf("%d",&feedback);
		if(feedback==1)
		{
			low=guess+1;
		}
		else if(feedback==2)
		{
			high=guess-1;
		}
		else if(feedback==3)
		{
			end=time(NULL);
			duration=difftime(end,start);
			printf("太棒了，我用%d次就猜对了，用时%.2f秒\n",count,duration);
			break;
		}
		else
		{
			printf("输入错误请用1/2/3告诉我答案哦\n"); 
		}
		if(low>high)
		{
			end=time(NULL);
			duration=difftime(end,start);
			printf("你好像作弊了，数字不在1-100之间呜呜~\n");
			printf("耗时：%.2f秒\n",duration);
			break; 
		}
	}
	printf("\n");
	return 0;
}
int exe3()
{
	printf("*---------------------------------------------------------------------------------------------------*\n");
	printf("\033[38;5;202m玩家你好^_^\033[0m\n");
	printf("游戏规则：\n"); 
	printf("游戏规则：两个玩家，一方为电脑，一方为人。\n"); 
	printf("一方出数字，一方猜。出数字的一方首先想好一个1-100的正整数，不告诉另一方，然后告知另一方开始猜测\n");
	printf("假设正确数字为98：如猜测为56，则提示猜测方“猜小了”；如猜测为109，则提示猜测方“猜大了”；\n");
	printf("持续猜测，直到猜中为止，最后会为玩家显示游戏总时长。\n");
	printf("\033[5;92m现在，开启你的猜数字游戏之旅吧！\033[0m\n"); 
	printf("*---------------------------------------------------------------------------------------------------*\n");
	printf("\n");
	return 0;
}
int main()
{
	int select=1;
	while(select!=0)
	{
		system("cls");
		printf("*--------------------------*\n");
		printf("|\033[5;91m^_^欢迎体验猜数字小游戏^_^\033[0m|\n");
		printf("*--------------------------*\n");
		printf("|\033[94m          菜 单           \033[0m|\n");
		printf("|\033[97m1:开始游戏(猜数字)        \033[0m|\n");
		printf("|\033[97m2:开始游戏(出数字)        \033[0m|\n");
		printf("|\033[97m3:游戏规则(建议先阅读)    \033[0m|\n");
		printf("|\033[97m0:退出游戏                \033[0m|\n");
		printf("*--------------------------*\n");
		printf("请选择：");
		scanf("%d",&select);
		switch(select)
		{
			case 1:exe1();break;
			case 2:exe2();break;
			case 3:exe3();break;
			case 0:printf("\033[93m期待您的下一次游玩，再见！\033[0m\n");break; 
			default:printf("选择错误请重新选择！\n");printf("\n");break;
		}
		system("pause");
	}
	return 0;
}
