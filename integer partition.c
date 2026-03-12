#include<stdio.h>
#include<stdlib.h>
//n拆分的整数，start拆分的起始数字
//path储存当前拆分的数字序列，len当前拆分序列的长度
/*递归函数split逐步拆分整数，使用路径数组记录*/ 
void split(int n,int path[],int len)//n还拆的数，pa记录已拆数字序列，l当前序列长 
{
	int i;
	if(n==0&&len>1)
	{
		for(i=0;i<len;i++)
		{
			printf("%d",path[i]);
			if(i<len-1)
			printf("+");
		}
		printf("\n");
	}
	else
	{
		for(i=1;i<=n;i++)
		{
			path[len]=i;
			split(n-i,path,len+1);//递归 
		}
	}
}
int exe1()
{
	int n,path[100];
	printf("请输入您要拆分的整数：");
	scanf("%d",&n);
	if(n<=0)
	{
		printf("请输入正整数哦！\n");
		system("pause");
		return 1; 
	}
	printf("整数%d能拆分为：\n",n);
	split(n,path,0);
	return 0;
}
int main()
{
	int select=1;
	while(select!=0)
	{
		system("cls"); 
		printf("\033[97m          整数拆分\033[0m\n"); 
		printf("*-------------------------*\n");
		printf("1:进行整数拆分\n");
		printf("0:退出\n");
		printf("*-------------------------*\n");
		printf("请选择：");
		scanf("%d",&select);
		switch(select)
		{
			case 1:exe1();break;
			case 0:printf("期待您的下一次使用！\n");break;
			default:printf("输入错误，请重新选择\n");break; 
		} 
		system("pause");
	}
	return 0;
}
