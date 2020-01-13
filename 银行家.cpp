#include<stdio.h>

//银行家算法，检查是否存在安全队列 
int check(int Have[50][50],int Need[50][50],int Available[50],int n_kinds,int n_task)
{
	int result[50],flag[50];
	int i,j,k,t,num,flag_task;
	for(i=0;i<n_task;i++)
		flag[i]=1;
	num=0;				//对已分配完成的程序计数，用于终止循环以及判断是否存在安全队列 
	flag_task=1;		//用于标志在一次遍历后是否进行了分配，未进行就退出 
	while((num<n_task)&&(flag_task==1))
	{
		flag_task=0;
		for(j=0;j<n_task;j++)			//对所有进程进行遍历，寻找可以分配的进程分配资源 
		{
			if(flag[j]==1)
			{
				t=1;					//标志是否资源向量大于等于需求向量 
				for(k=0;k<n_kinds;k++)
				{
					if(Need[j][k]>Available[k])
						t=0;
				}
				if(t==1)			//分配资源 
				{
					flag[j]=0;
					for(k=0;k<n_kinds;k++)
						Available[k]=Available[k]+Have[j][k];
					flag_task=1;
					result[num]=j;
					num++;
				}
			}
		}
	}
	if(num==n_task)
	{
		printf("存在安全序列\n");
		printf("某一安全序列为:");
		for(i=0;i<n_task;i++)
			printf("%d ",result[i]);
		printf("\n");
		return 1;
	}
	else
	{
		printf("不存在安全序列！\n");
		return 0;
	}
}

//由于检查算法会更改数组，所以对数组进行copy然后用copy出来的数组进行检测 
int copy_test(int Have[50][50],int Need[50][50],int Available[50],int n_kinds,int n_task)
{
	int i,j;
	int have_c[50][50],need_c[50][50],ava_c[50];		//拷贝数组 
	for(i=0;i<n_task;i++)
		for(j=0;j<n_kinds;j++)
		{
			have_c[i][j]=Have[i][j];
			need_c[i][j]=Need[i][j];
		}
	for(j=0;j<n_kinds;j++)
		ava_c[j]=Available[j];
	return check(have_c,need_c,ava_c,n_kinds,n_task);		//检查是否存在安全队列 
}

//输出所有信息，包括最大需求矩阵，已分配矩阵，需求矩阵，可用资源向量 
void print_table(int Have[50][50],int Need[50][50],int Available[50],int n_kinds,int n_task,int Max[50][50])
{
	int i,j;
	printf("最大需求矩阵：\n");
	for(i=0;i<n_task;i++)
	{
		for(j=0;j<n_kinds;j++)
			printf("%d	",Max[i][j]);
		printf("\n");
	}

	printf("已分配矩阵：\n");
	for(i=0;i<n_task;i++)
	{
		for(j=0;j<n_kinds;j++)
			printf("%d	",Have[i][j]);
		printf("\n");
	}
		
	printf("需求矩阵：\n");
	for(i=0;i<n_task;i++)
	{
		for(j=0;j<n_kinds;j++)
			printf("%d	",Need[i][j]);
		printf("\n");
	}
		
	printf("可用资源向量:\n");
	for(i=0;i<n_kinds;i++)
		printf("%d	",Available[i]);
	printf("\n");
}

//用户输入申请分配信息合法性检查 
int check_distribution(int Have[50][50],int Need[50][50],int Available[50],int n_kinds,int n_task,int want[50],int n,int Max[50][50])
{
	int i;
	if(n>=n_task)				//检查进程是否存在 
		return 0;
	for(i=0;i<n_kinds;i++)			//检查是否申请多余可用资源向量，以及是否超出最大需求矩阵 
	{
		if(Available[i]<want[i])
			return 0;
		if(Have[n][i]+want[i]>Max[n][i])
			return 0;
	}		
}

//输入分配信息，尝试分配，如果成功就改变数组，未成功回退 
int distribution(int Have[50][50],int Need[50][50],int Available[50],int n_kinds,int n_task,int Max[50][50])
{
	int n,i,flag;
	int want[50];
	printf("请输入进程的序号:");
	scanf("%d",&n);
	printf("请输入各类资源的数量:");
	for(i=0;i<n_kinds;i++)
		scanf("%d",&want[i]);
	if(check_distribution(Have,Need,Available,n_kinds,n_task,want,n,Max)==0)
	{
		printf("输入有误！！！\n");						//检查输入的合法性 
		return 0;		
	}
	for(i=0;i<n_kinds;i++)								//对资源进行分配 
	{
		Have[n][i]=Have[n][i]+want[i];
		Need[n][i]=Need[n][i]-want[i];
		Available[i]=Available[i]-want[i];
	}
	flag=copy_test(Have,Need,Available,n_kinds,n_task);		//检查是否存在安全队列 
	if(flag==1)
		printf("分配成功！\n");								//如果存在，输出队列退出 
	else
	{
		printf("分配失败！\n");								//如果不存在，程序回退，将已分配的回收回去 
		for(i=0;i<n_kinds;i++)
		{
			Have[n][i]=Have[n][i]-want[i];
			Need[n][i]=Need[n][i]+want[i];
			Available[i]=Available[i]+want[i];
		}
	}	
	
}

//输入所有进程信息，提示用户操作 
void test()
{
	int op;
	int Max[50][50],Have[50][50],Need[50][50],Available[50];
	int i,j,k,n_task,n_kinds;
	printf("请输入进程的数量：");						//输入进程的数量 
	scanf("%d",&n_task);
	printf("请输入进程需要资源的种类数：");				//输入资源的种类数 
	scanf("%d",&n_kinds);
	printf("请输入各类资源的数目：");					//输入总的资源数 
	for(i=0;i<n_kinds;i++)
		scanf("%d",&Available[i]);						
	for(i=0;i<n_task;i++)								//输入最大需求矩阵 
	{
		printf("请输入进程%d需要的各种资源数目:\n",i);
		for(j=0;j<n_kinds;j++)
			scanf("%d",&Max[i][j]);
		printf("请输入进程%d已有的各种资源数目:\n",i);		//输入已分配矩阵 
		for(j=0;j<n_kinds;j++)
			scanf("%d",&Have[i][j]);
		
	}
	for(i=0;i<n_task;i++)						//检查输入是否合法 
		for(j=0;j<n_kinds;j++)
		{
		Need[i][j]=Max[i][j]-Have[i][j];		//计算需求矩阵 
		Available[j]=Available[j]-Have[i][j];		//计算资源向量 
		if((Need[i][j]<0)||(Available[j]<0))		//检查合法性 
			{
				printf("输入有误！！！");			//存在输入错误，退出程序 
				return ;
			}
		}
	printf("输入0查看安全队列\n");
	printf("输入1进行资源分配\n");
	printf("输入2查看所有信息\n");
	printf("输入3退出程序\n");
	printf("请输入：");
	scanf("%d",&op);
	while(op!=3)
	{
		if(op==0)
			copy_test(Have,Need,Available,n_kinds,n_task);			//检测是否存在安全队列，如果存在就输出 
		if(op==1)
			distribution(Have,Need,Available,n_kinds,n_task,Max);	//对当前的进程分配资源  
		if(op==2)
			print_table(Have,Need,Available,n_kinds,n_task,Max);	//输出进程的所有信息 
		printf("请输入：");
		scanf("%d",&op);
	} 
 } 

int main()
{
	test();
	return 0;
} 
