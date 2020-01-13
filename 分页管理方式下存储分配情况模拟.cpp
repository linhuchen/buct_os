#include<stdio.h>
#include<string.h>
#include<iostream>
using namespace std;

//定义作业的结构，包含作业名，作业大小，在主存中的存储位置 
struct Job{
	char Name[20];		//作业名 
	int Size;			//作业大小 
	int Address[64];	//作业中各个页地址 ，页表 
};
Job job[64];			//总共64个页，所以最多能由64个作业 
int job_num=0;			//作业总数 
int Disk[8][8]={0};		// 主存位示图 
int Empty=64;			//空闲块数 

void print_disk()		//输出主存状态 
{
	int i,j;
	printf("Disk occupation status:\n");		//输出主存位示图 
	for(i=0;i<8;i++)
	{
		for(j=0;j<8;j++)
			printf("%d	",Disk[i][j]);
		printf("\n");
	}
	printf("Disk Free Blocks is %d\n",Empty); 	//输出 主存空闲块数 
}

int find_address()		//寻找空闲块 
{
	int i,j;
	for(i=0;i<8;i++)
		for(j=0;j<8;j++)
			if(Disk[i][j]==0)
				return i*8+j;
}

void Distribution()		//给作业分配空间 
{
	if(job_num==64)		//作业数达到最多，分配失败 
	{
		printf("Allocation failed!\n");
		return ;
	}
	char name[20];
	int size;
	int i,loc,loc_i,loc_j;
	printf("Please enter job name:");		//输入作业名 
	scanf("%s",&name);	
	printf("Please enter job size:");		//输入作业大小 
	scanf("%d",&size);
	for(i=0;i<job_num;i++)					//检查是否存在同名作业 
		if(strcmp(name,job[i].Name)==0)
		{
			printf("Job already exists!!!\nAllocation failed!\n");
			return ;
		}
	if(size>Empty)							//检查存储空间是否足够 
		printf("Allocation failed!\n");
	else
	{
		strcpy(job[job_num].Name,name);		//写作业信息相当于JCB 
		job[job_num].Size=size;
		for(i=0;i<size;i++)					//将作业分块调入内存 
		{
			loc=find_address();				//寻找首块空闲地址 编号 
			loc_i=loc/8;					//计算地址位置 
			loc_j=loc%8;
			job[job_num].Address[i]=loc;
			Disk[loc_i][loc_j]=1;
			Empty=Empty-1;
		}
		job_num++;
		printf("Allocation succeed!\n");
		printf("Page table is \n");			//输出页表信息 
		for(i=0;i<size;i++)
			printf("%d		%d\n",i,job[job_num-1].Address[i]);
		print_disk();
	}
}

void change_order(int t)				//将回收的作业的作业信息覆盖掉 
{
	for(int i=t;i<job_num;i++)
		job[i]=job[i+1];
}

void Recycle()			//回收操作 
{
	if(job_num==0)		//如果没有作业，回收失败 
	{
		printf("Recycle failed!\n");
		return ;
	}
	char name[20];
	int i,t,size,loc,loc_i,loc_j;
	t=-1;
	printf("Please enter job name:");	//输入作业名，按作业名查找作业 
	scanf("%s",&name);
	for(i=0;i<job_num;i++)
		if(strcmp(job[i].Name,name)==0)
		{
			t=i;
			break;
		}
	if(t==-1)			//检查作业是否存在 
	{
		printf("Job does not exist!!!\nRecycle failed!\n");
		return ;
	}
	size=job[t].Size;		//读取作业大小，删除作业在内存中所占据的空间（修改位示图） 
	for(i=0;i<size;i++)
	{
		loc=job[t].Address[i];
		loc_i=loc/8;
		loc_j=loc%8;
		Disk[loc_i][loc_j]=0;
		Empty++;
	}
	change_order(t);		//覆盖作业信息 
	job_num--;
	printf("Recycle succeed!\n");
	print_disk();
}

void test()
{
	int op;
	printf("分页管理方式下存储分配情况模拟\n");
	printf("0.Distribution\n");
	printf("1.Recycle\n");
	printf("2.Show Disk State\n");
	printf("3.Exit\n");
	printf("Please enter the function you want to use:");
	scanf("%d",&op);
	while(op!=3)
	{
		if(op==0)
			Distribution();
		else if(op==1)
			Recycle();
		else
			print_disk();
		printf("Please enter the function you want to use:");
		scanf("%d",&op);
	}
}

int main()
{
	test();
	return 0;
 } 
