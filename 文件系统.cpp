#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<windows.h>

typedef struct file{
 	
	char filename[100];				//文件名 
	int filesize;					//文件大小 
	int filetype;					//文件类型  0代表文件，1代表文件夹 
	struct file *child,*next,*pre,*father;	//文件的同级文件或者文件夹下的文件 
	//采用多叉树结构，child表示下层文件的指针，next，pre表示当前文件同级目录下的文件，分别表示当前文件的前一个和后一个文件
	//father表示该文件的父级目录，这样的写法可以简化切换目录时的操作。 
}File; 

HANDLE hd=GetStdHandle(STD_OUTPUT_HANDLE);//获取handle 用于改变命令行窗口的颜色，高亮显示操作 

char path[100]="#";//当前目录 
char allcommand[9][10] = {"help", "ls", "create", "md", "dir", "cd",  "delete","exit","clean"};
char helpmenu[9][40]={"获取帮助","显示目录下所有文件及目录","创建文件","创建目录","显示目录下所有文件及目录","改变当前目录","删除文件或目录","退出","清空屏幕"};
//设置功能菜单 
File *root;//设置根目录，最顶层的目录 
File *current;//设置当前目录的路径 

void welcome()//输出欢迎界面 
{
	
	char str[5][48]={"    *******************************************",
	"    *                                         *",
	"    *        Welcome to My File System        *",
	"    *                                         *",
	"    *******************************************"};
	printf("\n");
    int i,j;
    for(i=0;i<5;i++)
    {
    	for(j=0;j<48;j++)
    	{
        	SetConsoleTextAttribute(hd,(i*44+j)%10+2);//用于改变字体颜色 
        	printf("%c",str[i][j]);
    	}
    	printf("\n");
	}
	SetConsoleTextAttribute(hd,2);//改变颜色 
    printf("\n"); 
}

void print_first()//输出 当前目录及用户名，由于只有单个用户，所以直接输出root 
{
	SetConsoleTextAttribute(hd,7);
	printf("[root@localhost]%s:",path);
	SetConsoleTextAttribute(hd,2);
}

void print_help()//输出功能菜单 
{
	SetConsoleTextAttribute(hd,3);//改变颜色 
	int i;
	for(i=0;i<9;i++)
		printf("%s		%s\n",allcommand[i],helpmenu[i]);
	SetConsoleTextAttribute(hd,2);
}

void initsystem()//初始化文件系统 
{
	File* Boss=(File*)malloc(sizeof(File));//Boss文件，是root的父目录，所以目录都是Boss的子孙节点 
	root=(File*)malloc(sizeof(File));//root目录节点 
	current=(File*)malloc(sizeof(File));//当前目录节点 
	File* childfile=(File*)malloc(sizeof(File));//子目录 
	strcpy(childfile->filename,"蔺虎虎的文件");//初始化一个文件，存放在root目录下 
	childfile->next=NULL;// 由于只有一个文件，所以其前后均为NULL，所有目录下的第一个文件节点的pre指针都为空 ，最后一个节点的next为空，方便鉴别 
	childfile->pre=NULL;
	childfile->child=NULL;// 文件没有子文件或子文件夹 
	childfile->father=root;//设置其父节点 
	childfile->filetype=0;//定义文件类型为文件 
	childfile->filesize=200;//定义文件大小 
	strcpy(root->filename,"root");			//创建根目录 
	root->filetype=1;				 		//规定为文件夹
	root->filesize=0;						//规定文件夹大小为0 
	root->next=NULL;						//root无前后节点，其父节点为Boss，子节点为其目录下的文件 
	root->pre=NULL; 						
	root->child=childfile; 
	root->father=Boss;
	Boss->child=root;						//定义Boss文件 
	Boss->father=NULL;
	strcpy(Boss->filename,"all");
	current=Boss;							//current指针指向Boss节点 ，定义current指针主要是为了方便目录的切换，文件的创建删除等操作 
}

int checkcommand(char command[20])//检查命令，判断是那种操作 
{
	int i;
	for(i=0;i<9;i++)
	{
		if(strcmp(command, allcommand[i]) == 0)
			return i;			//返回该操作的序号 
	}
	return -1;
}

void print_file()				//ls，dir操作，显示当前目录下的所有文件或文件夹 
{
	SetConsoleTextAttribute(hd,4);
	File *p;					//定义指针p，指向当前目录下的第一个节点 
	p=current->child;	
	while(p!=NULL)				//从第一个节点开始往后遍历，如果非空就输出 
	{	
		if(p->filetype==0)
			printf("%s		%d\n",p->filename,p->filesize);
		else
			printf("%s\n",p->filename);
		p=p->next;
	}
	SetConsoleTextAttribute(hd,2);
}

void creat_file()				//create 创建文件操作 
{	
	SetConsoleTextAttribute(hd,5);
	char file_name[100];
	int file_size;
	scanf("%s",&file_name);		//输入文件名 
	scanf("%d",&file_size);		//输入文件大小 
	File *new_file=(File*)malloc(sizeof(File));
	strcpy(new_file->filename,file_name);
	new_file->filesize=file_size;		//设置文件的大小 
	new_file->filetype=0;				//设置文件类型为文件 
	new_file->child=NULL;				//将文件的子节点和后续节点设置为空，该文件节点插入至当前目录的最后一个节点的后续，成为新的最后一个节点 
	new_file->next=NULL;
	new_file->father=current;			//定义其父节点 
	File *p;
	p=current->child;
	int flag=1;
	while(p!=NULL)						//开始寻找最后一个节点 
	{
		if(strcmp(p->filename,file_name)==0&&p->filetype==0)//在寻找的过程中顺便判断是否存在相同的文件，可以存在相同名字的文件和文件夹
															//删除时删除第一个文件（或文件夹） ，即删除先创建的 
		{
			printf("file already exists!\n");
			flag=0;
			break;
		}
		else
			p=p->next;
	}
	if(flag==1)
	{
		p=current->child;					//判断是否是当前路径下的第一个文件，然后分别设定其pre指针 
		if(p==NULL)
		{
			new_file->pre=NULL;
			current->child=new_file;
			printf("creat success!\n");
		}
		else
		{
			while(p->next!=NULL)
				p=p->next;
			new_file->pre=p;
			p->next=new_file;
			printf("creat success!\n");
		}
	}
	SetConsoleTextAttribute(hd,2);
}

void creat_folder()				//md 创建文件夹操作 
{
	char folder_name[100];
	scanf("%s",&folder_name);		//输入文件夹名称 
	File *new_folder=(File*)malloc(sizeof(File));
	strcpy(new_folder->filename,folder_name);
	new_folder->filesize=0;			//设置文件夹属性，大小，类型，前后节点，父子节点 
	new_folder->filetype=1;
	new_folder->child=NULL;
	new_folder->next=NULL;
	new_folder->father=current;
	File *p;
	p=current->child;
	int flag=1;						//设置一个变量，标识是否存在相同文件夹名的文件夹 
	SetConsoleTextAttribute(hd,5);
	while(p!=NULL)					//从第一个节点开始遍历查找是否有相同文件夹名的文件夹，顺便将指针指向该层目录的最后一个节点 
	{
		if(strcmp(p->filename,folder_name)==0&&p->filetype==1)
		{
			printf("folder already exists!\n");
			flag=0;
			break;
		}
		else
			p=p->next;
	}
	if(flag==1)						//判断是否存在同名 
	{
		p=current->child;
		if(p==NULL)					//判断是否为该路径下的第一个文件 
		{
			new_folder->pre=NULL;
			current->child=new_folder;
			printf("creat success!\n");
		}
		else
		{
			while(p->next!=NULL)
				p=p->next;
			new_folder->pre=p;
			p->next=new_folder;
			printf("creat success!\n");
		}
	}
	SetConsoleTextAttribute(hd,2);
}

void cut_path(char name[100])	//修改路径名 
{
	int len,len_path;
	len=strlen(name);			//获取路径名的长度和当前path的长度，然后从后往前都设置为'\0' 
	len_path=strlen(path)-1;
	while(len>=0)
	{
		path[len_path]='\0';
		len--;
		len_path--;
	}
}

void change_dir()		//cd 修改当前路径操作 
{
	char temp_name[100];
	char folder_name[100];
	scanf("%s",&folder_name);
	int flag=1;
	if(strcmp(folder_name,"..")==0)		//判断是否为返回上层目录操作 
	{
		if(current->father!=NULL)
		{
			strcpy(temp_name,current->filename);//获取当前路径的最后一个文件夹名 
			cut_path(temp_name);				//从路径中减去当前的文件夹名 
			current=current->father;			//将current指针指向上一层目录 
		}
			
		return ; 
	}
	File *p;
	p=current->child;
	while(p!=NULL)
	{
		if(strcmp(p->filename,folder_name)==0&&p->filetype==1)
		{
			strcat(path,"/");
			strcat(path,folder_name);
			current=p;
			flag=0;
			break;
		}
		else
			p=p->next;
	}
	SetConsoleTextAttribute(hd,5);
	if(flag==1)
		printf("folder does not exist!\n");
	SetConsoleTextAttribute(hd,2);
}

void delete_file(File *p) //删除文件操作 
{
	File *t,*t_next,*t_pre;
	t=p;
	t_next=t->next;		//记录前后节点，用于判断存在一个节点还是多个节点的情况，也用于区分是第一个节点还是最后一个还是中间节点。 
	t_pre=t->pre;
	if(t_pre==NULL&&t_next==NULL)			//只有一个节点 
	{
		t->father->child=NULL;
		free(p);
	}
	else if(t_pre!=NULL&&t_next==NULL)		//最后一个节点 
	{
		t_pre->next=NULL;
		free(p);
	}
	else if(t_pre==NULL&&t_next!=NULL)		//第一个节点 
	{
		t_next->pre=NULL;
		t->father->child=t_next;
		free(p);
	}
	else									//中间节点 
	{
		t_pre->next=t_next;
		t_next->pre=t_pre;
		free(p);
	}
}

void delete_folder(File *p)	//删除文件夹操作，通过递归的调用将其子孙节点都删除掉 
{
	if(p->child!=NULL)		//如果有子节点调用删除文件夹操作继续删除 ，递归至最深一层的节点，从深往浅的一层一层删除 
		delete_folder(p->child);
	if(p->next!=NULL)		//删除同级目录下的下一个节点，也应用删除文件夹的操作，递归至最后一个节点，从后往前删除 
		delete_folder(p->next);
	File *t;
	t=p->pre;
	if(t!=NULL)
		t->next=NULL;
	free(p);
}

void delete_f()			//delete 删除文件/文件夹命令 
{
	SetConsoleTextAttribute(hd,5);
	char file_folder_name[100];
	scanf("%s",&file_folder_name);	//输入文件名 
	File *p;
	p=current->child;				//将指针指向当前目录的第一个文件 
	int flag=1;						//设置标识变量，标识是否找到 
	while(p!=NULL)					//从第一个文件开始向下寻找，看有没有要删除的文件或文件夹，如果有重名的文件或文件夹就删除排在前面的，即先创建的 
	{
		if(strcmp(p->filename,file_folder_name)==0)
		{
			if(p->child!=NULL)		//判断是文件还是文件夹，用子节点判断，比用文件类型判断方便 
				delete_folder(p->child);	//删除文件夹操作 
			delete_file(p);					//删除文件操作，将文件夹的子节点都删除掉，文件夹就和文件是一样的了，可以用相同的操作来删除 
			flag=0;
			break;
		}
		else
			p=p->next;
	}
	if(flag==1)
		printf("file/folder does not exist!\n");
	else
		printf("delete success!\n");
	SetConsoleTextAttribute(hd,2);
}

void test()							//死循环，等待输入命令，然后判断命令的类别，对相应的类别做相应的操作，调用其对应的函数 
{
	char command[20];
	while(1)
	{
		print_first();
		scanf("%s",&command);		//输入命令 
		int t=checkcommand(command);	//判断命令类别 
		if(t==-1)
			printf("Input error!\n");
		else
		{
			switch(t){
				case 0:				//显示帮助菜单 
					print_help();
					break;
				case 1:				//显示目录下所有文件及目录
					print_file();
					break;
				case 2:				//创建文件 
					creat_file();
					break;			 
				case 3:				//创建目录 
					creat_folder();
					break;
				case 4:				//显示目录下所有文件及目录
					print_file();
					break;
				case 5:				//改变当前目录 
					change_dir();
					break;
				case 6:				//删除文件或目录 
					delete_f();
					break;
				case 8:				//清理屏幕 
					system("cls");
					break;
				case 7:				//退出 
					SetConsoleTextAttribute(hd,7);
					return ;	 
			}
		}
	}
}
int main()		//主函数，调用initsystem，welcome，test三个函数，初始化系统，输出欢迎界面，然后进入死循环等待输入命令 
{
	initsystem();
	welcome();
	test();
	return 0;
} 
