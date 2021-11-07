#include <stdio.h>
#include <process.h>
#include <windows.h>
#include <stdlib.h>
#include <math.h>

//哲学家人数
const int NUM = 6;

//互斥变量
HANDLE Mutext[NUM];
int* Physical;   

// 哲学家就餐线程
unsigned int _stdcall ThreadFun(void* p)
{
	int* current = (int*)p;  //当前线程的哲学家编号
	while (1)
	{
		// 如果是第0到4号哲学家，先拿左筷子，再拿右筷子
		if (*current < (NUM - 1)) {
			//加锁
			WaitForSingleObject(Mutext[*current], INFINITE);
			WaitForSingleObject(Mutext[(*current + 1) % NUM], INFINITE);

			printf("哲学家%d正在就餐.....\n", *current);
			Sleep(2000);

			//解锁
			ReleaseMutex(Mutext[(*current) % NUM]);
			printf("哲学家%d放下左边筷子\n", *current);
			ReleaseMutex(Mutext[(*current + 1) % NUM]);
			printf("哲学家%d放下右边筷子\n", *current);
		}
		// 如果是第5号哲学家，先拿右筷子，再拿左筷子
		else {  
			//加锁
			WaitForSingleObject(Mutext[(*current + 1) % NUM], INFINITE);
			WaitForSingleObject(Mutext[*current], INFINITE);

			printf("哲学家%d正在就餐.....\n", *current);
			Sleep(2000);

			//解锁
			ReleaseMutex(Mutext[(*current + 1) % NUM]);
			printf("哲学家%d放下右边筷子\n", *current);
			ReleaseMutex(Mutext[(*current) % NUM]);
			printf("哲学家%d放下左边筷子\n", *current);
		}
	}
	return 0;
}

int main()
{
	//1.初始化互斥变量以及用餐的哲学家
	int Physical[NUM]; //用餐的哲学家
	for (int i = 0; i < NUM; i++)
	{
		Mutext[i] = CreateMutexA(NULL, false, NULL);
		Physical[i] = i;
	}

	//2.创建线程哲学家就餐线程
	HANDLE* hThread = NULL;
	hThread = new HANDLE[NUM];
	for (int i = 0; i < NUM; i++)
	{
		hThread[i] = (HANDLE)_beginthreadex(NULL, 0, ThreadFun, &Physical[i], 0, NULL);
	}

	//3. 等待上面创建的线程执行完毕
	WaitForMultipleObjects(NUM, hThread, true, INFINITE);//第二个参数为指定对象句柄组合中的第一个元素
	for (int i = 0; i < NUM; i++)
	{
		CloseHandle(hThread[i]);//销毁创建的进程
		CloseHandle(Mutext[i]); //销毁锁
	}
	return 0;
}