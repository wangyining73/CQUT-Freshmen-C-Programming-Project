#include<stdio.h>
#include<stdlib.h>
/*辗转相除法计算大约gcd，gcdlcm独立函数封装，考虑真分数合法性
通分约分是否能化为整数或带分数 */
int gcd(int a,int b)//最大公约数 
{
	int temp;
	while(b!=0)
	{
		temp=a%b;
		a=b;
		b=temp;	
	}
	return a;
}
int lcm(int a,int b)//最小公倍数 
{
	return a*b/gcd(a,b);
}
int exe1()
{
	int a,b,c,d;
	int x,y,g;
	printf("加数与被加数都是真分数\n");
	printf("请输入正的加数(分子/分母)："); 
	scanf("%d/%d",&a,&b);
	while(getchar()!='\n');
	printf("请输入正的被加数(分子/分母)：");
	scanf("%d/%d",&c,&d); 
	while(getchar()!='\n');
	if(a<=0||b<=0||c<=0||d<=0)
	{
		printf("输入错误！分子分母必须为正整数\n");
		return 1;
	} 
	if(a>=b||c>=d)
	{
		printf("输入错误！要输入真分数哟\n");
		return 1;
	}
	y=lcm(b,d);//最小公倍数 
	x=a*(y/b)+c*(y/d);
	g=gcd(x,y);//最大公约数 
	x/=g;
	y/=g;
	if(x%y==0)
	{
		printf("这两个正分数之和为%d\n",x/y);
	}
	else if(x>y)
	{
		printf("这两个正分数之和为%d又%d/%d\n",x/y,x%y,y);
	}
	else
	{
		printf("这两个正分数之和为%d/%d\n",x,y);
	}
	return 0;
}
int main()
{
	int select=1;
	while(select!=0)
	{
		system("cls");
		printf("      分数加法计算问题\n");
		printf("*---------------------------*\n");
		printf("1:输入两个正分数作加法\n");
		printf("0：退出程序\n"); 
		printf("*---------------------------*\n");
		printf("请选择(输入1或0)：");
		scanf("%d",&select);
		while(getchar()!='\n');
		switch(select)
		{
			case 1:exe1();break;
			case 0:printf("期待您的下一次体验！\n");break;
			default:printf("请选择正确题号！\n");break; 
		}
		system("pause");	
	}
	return 0;
}
