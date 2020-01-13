#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//定义设备种类 
struct machinetype{
	char type[10];		//设备名 
	int sum;			//设备总数量 
	int available;		//可用设备数量 
	int address;		//设备地址 
};
 
//定义设备 
struct machine{
	int number_absolute;	//设备绝对地址 
	int number_relative;	//设备相对地址 
	int status;				//设备状态 好坏 0代表坏，1代表好 
	int available;			//设备是否可用 1代表可用，0代表不可用 
	char jobname[10];		//作业名 
};

machinetype Machinetype[4];	//定义四种设备 设备0为输入设备，1为打印设备，2代表磁盘，3代表 
machine Machine[10];		//定义十台设备 

void init_machinetype()			//初始化设备类型与数量 
{
	Machinetype[0].address=0;
	Machinetype[0].available=2;
	Machinetype[0].sum=2;
	strcpy(Machinetype[0].type,"input");
	Machinetype[1].address=2;
	Machinetype[1].available=3;
	Machinetype[1].sum=3;
	strcpy(Machinetype[1].type,"printer");
	Machinetype[2].address=5;
	Machinetype[2].available=4;
	Machinetype[2].sum=4;
	strcpy(Machinetype[2].type,"disk");
	Machinetype[3].address=9;
	Machinetype[3].available=1;
	Machinetype[3].sum=1;
	strcpy(Machinetype[3].type,"tape");
}

void init_machine()//初始化十台设备 
{
	int i;
	for(i=0;i<10;i++)
	{
		Machine[i].number_absolute=i+1;		//设备绝对地址 
		Machine[i].available=1;				//设备可用 
		Machine[i].status=1;				//设备状态为好 
	}
		
}

void allocate()			//分配作业 
{
	//作业名，设备类型，设备相对号
	char name[10],type[10];
	int number;
	int i=0,loc; 
	printf("请输入作业名，作业所需设备类型，设备相对号:\n");
	scanf("%s %s %d",&name,&type,&number); 
	while(i<4&&strcmp(Machinetype[i].type,type))
		i++;	
	if(i==4)
	{
		printf("不存在该类设备!\n");
		return ;
	}	
	if(Machinetype[i].available<1)
	{
		printf("设备不足，分配失败!\n");
		return ;
	}
	loc=Machinetype[i].address;
	while(loc<10&&(Machine[loc].available!=1||Machine[loc].status!=1))
		loc++;
	Machinetype[i].available--;
	Machine[loc].available=0;
	strcpy(Machine[loc].jobname,name);
	Machine[loc].number_relative=number;
	
}

void reclaim()				//回收作业 
{
	char name[10],type[10];
	int i=0,loc,num,maxloc,flag=0;
	printf("请输入作业名，作业归还设备类型:\n");
	scanf("%s %s",&name,&type); 
	while(i<4&&strcmp(Machinetype[i].type,type))
		i++;	
	if(i==4)
	{
		printf("不存在该类设备!\n");
		return ;
	}
	loc=Machinetype[i].address;
	num=Machinetype[i].sum;
	maxloc=loc+num;
	for(;loc<maxloc;loc++)
		if(Machine[loc].available==0&&strcmp(Machine[loc].jobname,name)==0)
		{
			flag++;
			Machine[loc].available=1;
			strcpy(Machine[loc].jobname,"");
			Machine[loc].number_relative=0;
		}
	Machinetype[i].available=Machinetype[i].available+flag;
	if(flag==0)
		printf("该作业没有使用该类设备\n");
	else
		printf("归还成功！\n");
}

void print_all()			//输出设备分配表 
{
	int i;
	printf("设备类表:\n");
	printf("设备类型	设备总量	空闲好设备\n");
	for(i=0;i<4;i++)
		printf("%s		%d			%d\n",Machinetype[i].type,Machinetype[i].sum,Machinetype[i].available);
	printf("设备表:\n");
	printf("绝对号	好/坏	是否可用	占用作业名	相对号\n");
	for(i=0;i<10;i++)
		printf("%d	%d	%d	%s	%d\n",Machine[i].number_absolute,Machine[i].status,Machine[i].available,Machine[i].jobname,Machine[i].number_relative);
}

void test()
{
	int op;
	printf("0－退出，1－分配，2－回收，3－显示\n");
	while(1)
	{
		printf("选择功能项（0~3）:");
		scanf("%d",&op);
		switch(op)
		{
			case 0:
				return ;
			case 1:
				allocate();
				break;
			case 2:
				reclaim();
				break;
			case 3:
				print_all();
				break;
		}
	}
}

int main()
{
	init_machinetype();
	init_machine(); 
	test();
	return 0;
}
