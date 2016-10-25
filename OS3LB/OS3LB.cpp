#include "stdafx.h"
#include <Windows.h>
#include <iostream>

using namespace std;

int __cdecl wmain(int argc, wchar_t *argv[], wchar_t *envp[]);

int create_number()
{
	int number = rand();
	return number;
}

void writeToClipboard(char*str, int n) {
	size_t len;
	HGLOBAL hMem;
	len = n + 1;
	hMem = GlobalAlloc(GMEM_MOVEABLE, len);
	memcpy(GlobalLock(hMem), str, len);
	GlobalUnlock(hMem);
	OpenClipboard(0);
	EmptyClipboard();
	SetClipboardData(CF_TEXT, hMem);
	CloseClipboard();
}

void readFromClipboard() {
	if (OpenClipboard(NULL))
	{
		int d = 1;
		char* data = (char*)GetClipboardData(CF_TEXT);
		for (int i = 0; i < 1; ++i) {
			printf("Number:%c\n", data[i]); d++;
		}
		CloseClipboard();
	}
}

int wmain(int argc, wchar_t * argv[], wchar_t * envp[])
{
	STARTUPINFO si = { sizeof(si) };
	SECURITY_ATTRIBUTES saProcess, saThread;
	PROCESS_INFORMATION piProcessP1, piProcessP2;
	TCHAR szPath[MAX_PATH];
	DWORD dwExitCode;
	saProcess.nLength = sizeof(saProcess);
	saProcess.lpSecurityDescriptor = NULL;
	saProcess.bInheritHandle = TRUE;
	saThread.nLength = sizeof(saThread);
	saThread.lpSecurityDescriptor = NULL;
	saThread.bInheritHandle = FALSE;
	lstrcpy(szPath, TEXT("C:\\Windows\\system32\\cmd.exe"));
	BOOL fSuccess = CreateProcess(NULL, szPath, &saProcess, &saThread,
		FALSE, 0, NULL, NULL, &si, &piProcessP1);
	if (fSuccess) {
		CloseHandle(piProcessP1.hThread); //Закрываем описатель потока, как только в нем необходимость отпадет
		cout << "Success!!!" << endl;
		char buf[10];
		for (int i = 0; i < 10; i++) {
			sprintf_s(buf, "%d", create_number());
			writeToClipboard(buf, strlen(buf));
			readFromClipboard();
		}
		//WaitForSingleObject(piProcessP1.hProcess, INFINITE); //Приостанавливаем выполнение родительского процессв, пока не завершится дочерний процесс
		GetExitCodeProcess(piProcessP1.hProcess, &dwExitCode); // Дочерний процесс завершился, получаем код его завершения
		CloseHandle(piProcessP1.hProcess); // Закрываем описатель процесса, как только необходимость в нем отпадет
	}
	/*lstrcpy(szPath, TEXT("ProcessP2"));
	CreateProcess(NULL, szPath, &saProcess, &saThread,
		FALSE, DEBUG_PROCESS, NULL, NULL, &sti, &piProcessP2);*/
	//system("pause");
	return 0;
}

int main() {
	int nMainRetVal = wmain(__argc, __wargv, _wenviron);
	return 0;
}
