#include<Windows.h>
#include<string>
void inject(int pid,char *path);
int main()
{
	//�����������id
	int pid = 1;
	//��������dll�ļ�·��
	char p[] = "x";
	inject(11472, p);
	return 0;
}

void inject(int pid,char *path)
{

	//��ȡ���̾��,�õ����̵�����Ȩ�ޣ������ڽ����д����ӽ�������ȡ������Եڶ���������NULL�ͺ�
	//��������ʾ���̵�ID
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, pid);

	/*
	��������lpStartAddress����
	���������һ������ָ��
	������ʾԶ�̽������̵߳���ʼ��ַ
	�������ʹ��LoadLibrayA����������
	*/

	//��ȡLoadLibraryA��dll
	HMODULE hModule = LoadLibrary(TEXT("Kernel32.dll"));
	//��kernel32.dll�е�LoadLibraryA�����ĵ�ַ��ȡ
	LPTHREAD_START_ROUTINE lpStartAddress = (LPTHREAD_START_ROUTINE)GetProcAddress(hModule,"LoadLibraryA");
	
	//����һ�������ڴ�ĵ�ַ������ĺ�������������Ȼ����Ҫ��������ֵ
	//�൱������һ��������Ҫ��������ֵ����ʹ��
	LPVOID pReturnAddress = VirtualAllocEx(hProcess,NULL,strlen(path)+1, MEM_COMMIT,PAGE_READWRITE);
	WriteProcessMemory(hProcess, pReturnAddress, path, strlen(path) + 1, NULL);
	
	//����Զ���߳�
	CreateRemoteThread(hProcess,NULL,0,lpStartAddress,pReturnAddress,0,NULL);
}