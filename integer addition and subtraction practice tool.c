#include<stdio.h>
#include<stdlib.h>
#include<time.h>
/*随机生成+多条件校验+状态去重*/ 
int used[101][101][101][2][2]={0}; 
int exe1()
{
	int total,correct=0,wrong=0,score=0;
	int a,b,c,op1,op2,answer,user_answer,i,valid;
	//int used[101][101][101][2][2]={0};//设置随机数种子，确保每次运行题目不同
	float rate;
	time_t start,end;
	double duration;
	srand((unsigned int)time(NULL));
	printf("请输入题目数量(1-100)：");
	scanf("%d",&total);
	while(1)
	{
		if(total>=1&&total<=100)
		break; 
		printf("输入无效，请重新输入！");
	}
	printf("计时开始！\n");
	start=time(NULL);
	for(i=0;i<total;i++)//一道道生 
	{
		do
		{
			valid=0;
			a=rand()%100+1;
			b=rand()%100+1;
			c=rand()%100+1;
			op1=rand()%2;//0做加法，1做减法
			op2=rand()%2;
			if(op1==0)
			{
				if(a+b>100)
				{
					valid=1;	
				}
				if(op2==0)
				{
					if(a+b+c>100)
					{
						valid=1;
					}
				}
				else
				{
					if(a+b-c<0)
					{
						valid=1;
					}
				}
			}
			else//op1是－ 
			{
				if(a-b<0)
				{
					valid=1;
				}
				if(op2==0)
				{
					if(a-b+c>100)
					{
						valid=1;
					}
				}
				else
				{
					if(a-b-c<0)
					{
						valid=1;
					}
				}
			}
		}
		while(used[a][b][c][op1][op2]==1||valid);//题目重复，不合要求的重新生成 
		used[a][b][c][op1][op2]=1;
		if(op1==0&&op2==0)
		{
			answer=a+b+c;
			printf("第%d题：%d+%d+%d=",i+1,a,b,c);
		}
		else if(op1==0&&op2==1)
		{
			answer=a+b-c;
			printf("第%d题：%d+%d-%d=",i+1,a,b,c);
		}
		else if(op1==1&&op2==1)
		{
			answer=a-b-c;
			printf("第%d题：%d-%d-%d=",i+1,a,b,c);
		}
		else if(op1==1&&op2==0)
		{
			answer=a-b+c;
			printf("第%d题：%d-%d+%d=",i+1,a,b,c);
		}
		scanf("%d",&user_answer);
		if(user_answer==answer)
		{
			correct++;
			score+=10;
			printf("√+10分\n\n");
		}
		else
		{
			wrong++;
			printf("×正确答案是%d\n\n",answer);
		}
	}
	end=time(NULL);
	duration=difftime(end,start);
	printf("练习结束！来看看结果吧\n");
	system("pause"); 
	rate=(float)score/(total*10)*100;
	printf("*-------------------*\n");
	printf("      练习结果\n");
	printf("*-------------------*\n"); 
	printf("总题数：%d\n",total);
	printf("正确题数：%d\n",correct);
	printf("错误题数：%d\n",wrong);
	printf("用时：%f秒\n",duration);
	printf("得分：%d/%d(满分)\n",score,total*10);
	printf("得分率：%.2f%%\n",rate);
	printf("评语：");
	if(rate>=90)
	{
		printf("优秀！！！你对加减法的掌握程度已经达到熟练的程度了，继续保持呀！\n"); 
	}
	else if(rate>=70&&rate<90)
	{
		printf("良好！！你已经大部分掌握加减法了，勤加练习，相信你离满分不远了！\n"); 
	}
	else if(rate>=60&&rate<70)
	{
		printf("及格！多多练习，注意掌握方法，注意细节，相信下一个加减法大神就是你！\n"); 
	}
	else if(rate>=0&&rate<60)
	{
		printf("啊欧，这次没有及格呀，建议先了解加减法本质后，从简单练起，再来挑战哦，等着你下一次凯旋！\n");
	}
	return 0; 
}
void exe2()
{
	printf("\033[38;5;202m欢迎使用整数加减法练习器！\033[0m\n");
	printf("\n");
	printf("本练习器会随机生成100以内的三个整数和加减运算符\n");
	printf("每次练习前由您来指定题目数(<=100),题目不重复\n");
	printf("\n"); 
	printf("练习完毕后，我们将对您的答案进行判断，并计分\n");
	printf("最后，统计并输出回答正确和错误的题目数、最后得分，并给予您评语\n");
	printf("\033[5;92m现在，开启你的练习之旅吧！\033[0m\n");	
} 
int main()
{
	int select=1;
	while(select!=0)
	{
		system("cls");
		printf("      整数加减法练习器\n");
		printf("*--------------------------*\n");
		printf("1:开始练习\n");
		printf("2:规则说明(建议先查看)\n");
		printf("0:退出练习器\n");
		printf("*--------------------------*\n");
		printf("请选择(1/2/0):");
		scanf("%d",&select);
		switch(select)
		{
			case 1:exe1();break; 
			case 2:exe2();break;
			case 0:printf("\033[93m记得常来练习呀！\033[0m\n");break;
			default:printf("请输入正确题号！\n");break;	
		}	
		system("pause");
	}
	return 0;
}
