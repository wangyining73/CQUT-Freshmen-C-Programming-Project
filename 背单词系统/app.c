#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<ctype.h>
#define NUM 300
#define MAX_SCORE 10
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m" 
#define CYAN "\033[36m"
/*用数组+score，加权概率抽取思想
熟练度为负进入错词本，大小写不敏感输入
ANSI改色，退出程序仍保留记忆*/ 
typedef struct{
	char eng[20];
	char chi[40];
	int score;
}WORD;

int load(WORD *words);//把word.txt全部读进数组，并返数量 
void view(WORD *words,int count);//按编号显示所有单词，不读文件不该数据，不返回值 
void study(WORD *words,int count);//背单词 
void save(WORD *words,int count); 
void resetscore(WORD *words,int count);//重置熟练度 
void view_menu(WORD *words,int count);
void view_by_score(WORD *words,int count); 
void view_wrong_words(WORD *words,int count);

int main()
{
	int select=1,count=0,i;
	WORD words[NUM];
	count=load(words);
	while(select!=0)
	{
		system("cls");
		printf(CYAN"      背单词系统"RESET"\n");
		printf("---------------------\n");
		printf("1:背单词\n");
		printf("2:浏览词汇表\n");
		printf("3:重置熟练度\n");
		printf("4:错词本\n");
		printf("0:退出\n");
		printf("---------------------\n");
		printf("请选择:");
		int ret=scanf("%d",&select);
		if(ret!=1)
		{
			int c;
			while((c=getchar())!='\n'&&c!=EOF);
			{
				select=-1;
			}
		}
		switch(select)
		{
			case 1:study(words,count);break;
			case 2:view_menu(words,count);break;
			case 3:resetscore(words,count);break;
			case 4:view_wrong_words(words,count);break;
			case 0:save(words,count);printf(YELLOW"记得常来练习呀！"RESET"\n");break;
			default:printf("输入无效，请选择1、2、0\n");break;
		} 
		system("pause");
	}
	return 0;
}

int load(WORD *words) 
{
	int i=0;
	int ret;
	FILE *fp;
	fp=fopen("word.txt","r");
	if(fp==NULL)
	{
		printf("无法打开word.txt文件\n");
		exit(1);//直接结束整个程序 
	}
	while(1)
	{
		ret=fscanf(fp,"%19s %39s %d",words[i].eng,words[i].chi,&words[i].score);
		if(ret==3)
		{
			//不做处理，正常情况，继续 
		}
		else if(ret==2)
		{
			words[i].score=0;
		}
		else
		{
			printf("第%d行格式错误\n",i+1); 
			break;
		}
		i++;
		if(i>=NUM)
		break;
	}
	fclose(fp);
	return i;
}

void view_menu(WORD *words,int count)
{
	int select=1;
	while(select!=0)
	{
		system("cls");
		printf("      浏览词汇表\n");
		printf("---------------------\n");
		printf("1:原顺序浏览\n");
		printf("2:按熟练度顺序浏览\n");
		printf("0:返回上一级\n");
		printf("---------------------\n");
		printf("请选择:");
		scanf("%d",&select);
		switch(select)
		{
			case 1:view(words,count);system("pause");break;
			case 2:view_by_score(words,count);system("pause");break;
			case 0:break;
			default:printf("输入无效！\n");system("pause");
		}
	}
}

void view_by_score(WORD *words,int count)
{
	int i,j,temp;
	int idx[NUM];
	for(i=0;i<count;i++)
	{
		idx[i]=i;
	}
	for(i=0;i<count-1;i++)
	{
		for(j=0;j<count-1-i;j++)
		{
			if(words[idx[j]].score>words[idx[j+1]].score)
			{
				temp=idx[j];
				idx[j]=idx[j+1];
				idx[j+1]=temp;
			}
		}
	}
	system("cls");
	printf("按熟练度排序\n");
	printf("---------------------\n");
	for(i=0;i<count;i++)
	{
		printf("%3d %-20s %-20s score:%d\n",i+1,words[idx[i]].eng,words[idx[i]].chi,words[idx[i]].score);
	}	
} 

void resetscore(WORD *words,int count)
{
	int i;
	for(i=0;i<count;i++)
	{
		words[i].score=0;
	}
	save(words,count);
	printf("所有单词熟练度已清零！\n");
}

void view(WORD *words,int count)
{
	int i;
	system("cls");
	printf("词汇表(共%d个单词)\n",count);
	printf("---------------------\n");
	for(i=0;i<count;i++)
	{
		if(words[i].score<0)
		{
		    printf(RED);
		}
		else if(words[i].score==0)
		{
		    printf(YELLOW);
		}
		else
		{
		    printf(GREEN);
		}
		printf("%3d %-20s %-20s score:%d\n",i+1,words[i].eng,words[i].chi,words[i].score);
		printf(RESET);
	}
}

void save(WORD *words,int count)
{
	FILE *fp;
	int i;
	if(count<=0)
	{
		printf("未保存：单词数量为0\n");
		return;
	}
	fp=fopen("word.txt","w");
	if(fp==NULL)
	{
		printf("无法打开文件！\n");
		return;
	}
	for(i=0;i<count;i++)
	{
		fprintf(fp,"%s %s %d\n",words[i].eng,words[i].chi,words[i].score);
	}
	fclose(fp);
}

void study(WORD *words,int count)
{
    int n,i;
    int used[NUM]={0};

    system("cls");
    printf("请输入本次要背的单词数量：");
    scanf("%d",&n);

    if(n<=0 || n>count)//控制选择数 
    {
        printf("输入数量无效！\n");
        return;
    }

    srand((unsigned int)time(NULL));

    for(i=0;i<n;i++)
    {
        int j,k;
        int totalweight=0;
        int weight[NUM];

        for(j=0;j<count;j++)
        {
            if(used[j])
            {
                weight[j]=0;
                continue;
            }
            weight[j]=MAX_SCORE-words[j].score+1; 
            if(weight[j]<1)
                weight[j]=1; 
            totalweight+=weight[j];
        }
        if(totalweight==0)
        {
            printf("没有可抽取的单词了！\n");
            break;
        }

        int r=rand()%totalweight;
        int sum=0;
        int flag=-1;

        for(j=0;j<count;j++)
        {
            if(used[j]) continue;

            sum+=weight[j];
            if(sum>r)
            {
                flag=j;
                break;
            }
        }

        if(flag==-1)
        {
            printf("抽取单词失败！\n");
            continue;
        }

        used[flag]=1;

        char input[20];//开始背单词 
        printf("中文：%s\n",words[flag].chi);
        printf("英文：");
        scanf("%19s", input);
		for(k=0;input[k];k++)
		{
			if(input[k]>='A'&&input[k]<='Z')
				input[k]=input[k]-'A'+'a';
		}
		
		char answer[20];
		strcpy(answer,words[flag].eng);
		for(k=0;answer[k];k++)
		{
			if(answer[k]>='A'&&answer[k]<='Z')
				answer[k]=answer[k]-'A'+'a';
		}
        if(strcmp(input,answer)==0)
        {
            printf(GREEN"√ 正确！"RESET"\n");
            words[flag].score++;
        }
        else
        {
            printf(RED"× 错误！正确答案：%s"RESET"\n",words[flag].eng);
        	words[flag].score--;
		}

        printf("\n");
    }
}
void view_wrong_words(WORD *words,int count)
{
	int i;
	int index=1;
	system("cls");
	printf("       错词本\n"); 
	printf("---------------------\n");
	for(i=0;i<count;i++)
	{
		if(words[i].score<0)
		{
			printf("%3d %-20s %-20s score:%d\n",index,words[i].eng,words[i].chi,words[i].score);
			index++;
		}
	}
	if(index==1)
	{
		printf("当前没有错词！\n");
	}
}
