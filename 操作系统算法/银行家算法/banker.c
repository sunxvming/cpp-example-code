#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define FALSE 0
#define TRUE 1
#define M 10  //最大进程数
#define N 10  //最大资源类数
int progress_num;//进程数
int resource_num;//资源数目
int id; //进程ID号

int Available[N];	//系统资源总数
// 公式:Need[i,j] = Max[i,j] - Allocation[i,j]
int MAX[M][N];		//最大需求矩阵，如果Max[i,j]=k. 表明Pi对类型为Rj资源的最大请求为k.
int Need[M][N];		//需求矩阵     如果Need[i,j] = k， 进程Pi需要至少得到k数量的资源Rj，才能完成任务。
int Allocate[M][N];	//分配矩阵，   如果Allocation[i,j] = k,进程Pi已分配到类型为Rj的资源数量为k
int Request[N];		//请求资源
int Work[N];		//工作向量
int Finish[M];          //进程完成标志
int work_allocation[M][N];
void init_system();
void print_info();
int try_allocate();
void return_back();
int is_safe();
int flag = FALSE;
int safe[M];

void init_system()
{
	printf("请输入进程数目:\n");
	scanf("%d", &progress_num);

	printf("请输入资源种数:\n");
	scanf("%d", &resource_num);

	printf("请输入各类资源总数:\n");
	for (int i = 0; i < resource_num; i++)
		scanf("%d", &Available[i]);

	printf("输入各进程对各资源的最大需求:\n");
	for (int i = 0; i < progress_num; i++)
	{
		printf("进程P%d:\n", i);
		for (int j = 0; j < resource_num; j++)
		{
			scanf("%d", &MAX[i][j]);
		}
	}

	char ch = getchar();
	printf("是否输入分配矩阵？(y/n)");
	while (scanf("%c", &ch))
	{
		if (ch == 'n')
		{
			memset((void*)Allocate, 0, sizeof(Allocate));
			memset((void*)work_allocation,0, sizeof(work_allocation));
			break;
		}
		else if (ch == 'y')
		{
			printf("请输入T时刻资源的分配矩阵:\n");
			for (int i = 0; i < progress_num; i++)
			{
				printf("进程P%d:\n", i);
				for (int j = 0; j < resource_num; j++)
				{
					scanf("%d", &Allocate[i][j]);
				}
			}
			break;
		}
		else
			printf("重新输入!\n");
	}
	memset((void*)Request, 0,sizeof(Request));
	memset((void*)Work, 0, sizeof(Work));
	memset((void*)Finish, 0, sizeof(Finish));

	/*初始时刻 Need*/
	for (int i = 0; i < progress_num; i++)
		for (int j = 0; j < resource_num; j++)
		{
			Need[i][j] = MAX[i][j] - Allocate[i][j];
			Available[j] -= Allocate[i][j];      //总的可用资源 - 初始化分配的资源
			work_allocation[i][j] = Allocate[i][j] + Work[j];
		}
}
int try_allocate()
{
	int ret = TRUE;
	for (int i = 0; i < progress_num; i++)
		for (int j = 0; j < resource_num; j++)
			work_allocation[i][j] = Allocate[i][j] + Work[j];

	printf("请输入请求进程号:\n");
	while (scanf("%d", &id))
	{
		if (id < 0 | id > progress_num)
			printf("进程不存在!\n");
		else break;
	}
	printf("请输入请求资源数:\n");
	for (int i = 0; i < resource_num; i++)
	{
		scanf("%d", &Request[i]);
	}
	printf("开始为进程P%d分配资源:\n", id);
	for (int i = 0; i < resource_num; i++)
	{
		if (Request[i] > Need[id][i])
		{
			printf("进程请求资源数大于所需资源数,不予分配!\n");
			ret = FALSE;
		}
		else if (Request[i] > Available[i]) {
			printf("进程请求资源数大于可用资源数，无法分配!\n");
			ret = FALSE;
		}
		Available[i] -= Request[i];
		Allocate[id][i] += Request[i];
		Need[id][i] -= Request[i];
	}
	return ret;
}
void print_info()
{
	int i, j, k;
	printf("      %-20s%-20s%-20s%-20s%-20s%-20s\n","Work", "最大需求量", "尚需要量","目前分配量","Work+Allocation","Finish");
	printf("      ");
	for (j = 0; j < 4; j++)
	{
		for (i = 0; i < resource_num; i++)
			printf("%c  ", (char)('A' + i));
		printf("%*s", 20-3*i, "");
	}

	printf("\n************************************************************************************************\n");
	for (k = 0; k < progress_num; k++)
	{
		if (flag == TRUE)
			i = safe[k];
		else i = k;

		printf("P%d  ", i); 
		for (j = 0; j < resource_num; j++)
			printf("%3d", work_allocation[i][j] - Allocate[i][j]);
		printf("%*s", 20 - 3 * j, "");

		for (j = 0; j < resource_num; j++)
			printf("%3d", MAX[i][j]);
		printf("%*s", 20 - 3 * j, "");

		for (j = 0; j < resource_num; j++)
			printf("%3d", Need[i][j]);
		printf("%*s", 20 - 3 * j, "");
	
		for (j = 0; j < resource_num; j++)
			printf("%3d", Allocate[i][j]);
		printf("%*s", 20 - 3 * j, "");

		for (j = 0; j < resource_num; j++)
			printf("%3d", work_allocation[i][j]);
		printf("%*s", 20 - 3 * j, "");

		if (Finish[i] == FALSE)
			printf("false\n");
		else printf("true\n");
	}
}
void return_back()
{
	int i, j;
	memset((void*)Finish,0, sizeof(Finish));
	for (i = 0; i < resource_num; i++)
	{
		Available[i] += Request[i];
		Allocate[id][i] -= Request[i];
		Need[id][i] += Request[i];
	}
	memset((void*)work_allocation, 0,sizeof(work_allocation));
	for (i = 0; i < progress_num; i++)
		for (j = 0; j < resource_num; j++)
		{
			work_allocation[i][j] = Allocate[i][j];
		}
}

/*安全性检查函数*/
int is_safe()
{
	int count = 0;
	memset((void*)work_allocation, 0,sizeof(work_allocation));
	printf("**********Work向量**********\n");
	for (int i = 0; i < resource_num; i++)
	{
		Work[i] = Available[i];
		work_allocation[0][i] = Work[i];
	}
	for (int i = 0; i < progress_num; i++)
		Finish[i] = FALSE;

	//此处的退出条件有两个，1.所有进程都可以完成 2.有进程不能完成则直接退出
	for (int i = 0; i < progress_num; i++)
	{
		if (Finish[i] == FALSE)
		{
			int j;
			for (j = 0; j < resource_num; j++)
			{
				if (Need[i][j] > Work[j])
					break;
			}
			if (j == resource_num)   // 若条件满足，则说明当前的资源可以满足这个线程
			{
				printf("P%d\t", i);
				safe[count++] = i;
				for (j = 0; j < resource_num; j++)
				{
					Work[j] += Allocate[i][j];
					work_allocation[i][j] = Work[j];
					printf("%d\t", Work[j]);
				}
				printf("\n");
				Finish[i] = TRUE;  //进程i能顺利完成
				if (i == progress_num - 1)   // 能顺利完成，并且循环到最后一个进程了，则重新开始
					i = -1;
			}
		}
	}
	printf("\n");
	for (int i = 0; i < progress_num; i++)
	{
		if (Finish[i] == FALSE)
		{
			printf("进程P%d不能顺利完成！\n", i);
			return FALSE;
		}
	}
	/*如果前面未返回，则存在安全序列*/
	printf("存在如下安全序列:\n");
	for (int i = 0; i < progress_num; i++)
		printf("P%d ", safe[i]);
	printf("\n");
	return TRUE;
}

int main(void)
{
	int ret = FALSE;
	int choice;
	while (1)
	{
		printf("银行家算法(避免进程死锁算法)实验，请选择你要进行的操作:\n");
		printf("1、初始化系统\n");
		printf("2、请求分配资源\n");
		printf("3、查看系统分配结果\n");
		printf("4、当前状态安全性检查\n");
		printf("0、退出系统\n");
		scanf("%d", &choice);
		if (choice == 1)
			init_system();
		else if (choice == 2)
		{
			if (try_allocate() == TRUE)
			{
				flag = is_safe();
				if (flag == FALSE)
				{
					printf("系统不安全,不能予以分配!\n");
					return_back();
				}
				else {
					printf("系统安全！分配成功!\n");
					print_info();
				}
			}
			else {
				printf("分配失败!\n");
				return_back();
			}
		}
		else if (choice == 3)
			print_info();
		else if (choice == 4)
		{
			if ((flag = is_safe()) == FALSE)
				printf("当前状态不安全!\n");
			else	printf("当前状态安全！\n");
		}
		else if (choice == 0)
			break;
		else
			printf("输入不合法!重新输入!\n");
	}
}