#include<Windows.h>
#include<string>
void inject(int pid,char *path);
int main()
{
	//这里输入进程id
	int pid = 1;
	//这里输入dll文件路径
	char p[] = "x";
	inject(11472, p);
	return 0;
}

void inject(int pid,char *path)
{

	//获取进程句柄,得到进程的所有权限，不用在进程中创建子进程来获取句柄所以第二个参数传NULL就好
	//第三个表示进程的ID
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, pid);

	/*
	调用生成lpStartAddress变量
	这个变量是一个函数指针
	用来表示远程进程中线程的起始地址
	这里采用使用LoadLibrayA函数来处理
	*/

	//获取LoadLibraryA的dll
	HMODULE hModule = LoadLibrary(TEXT("Kernel32.dll"));
	//将kernel32.dll中的LoadLibraryA函数的地址获取
	LPTHREAD_START_ROUTINE lpStartAddress = (LPTHREAD_START_ROUTINE)GetProcAddress(hModule,"LoadLibraryA");
	
	//申请一块虚拟内存的地址给传入的函数参数来处理，然后还需要给参数赋值
	//相当于申明一个变量后还要给变量赋值才能使用
	LPVOID pReturnAddress = VirtualAllocEx(hProcess,NULL,strlen(path)+1, MEM_COMMIT,PAGE_READWRITE);
	WriteProcessMemory(hProcess, pReturnAddress, path, strlen(path) + 1, NULL);
	
	//创建远程线程
	CreateRemoteThread(hProcess,NULL,0,lpStartAddress,pReturnAddress,0,NULL);
}