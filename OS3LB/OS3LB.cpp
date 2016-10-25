#include "stdafx.h"
#include <Windows.h>
#include <iostream>
#include <string.h>

using namespace std;

int __cdecl wmain(int argc, wchar_t *argv[], wchar_t *envp[]);

int create_number()
{
	int number = rand() % 9 + 1;
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
	SetClipboardData(CF_TEXT, hMem);
	CloseClipboard();
}

char readFromClipboard() {
	char* data;
	if (OpenClipboard(NULL))
	{
		int d = 1;
		data = (char*)GetClipboardData(CF_TEXT);
		CloseClipboard();
	}
	return *data;
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
	char buf[10];
	//for (int i = 0; i < 10; i++) {
		sprintf_s(buf, "%d", create_number());
		writeToClipboard(buf, strlen(buf));
	//}
	lstrcpy(szPath, TEXT("C:\\Windows\\system32\\cmd.exe"));
	BOOL processP1 = CreateProcess(NULL, szPath, &saProcess, &saThread,
		FALSE, 0, NULL, NULL, &si, &piProcessP1);
	if (processP1) {
		CloseHandle(piProcessP1.hThread); //Закрываем описатель потока, как только в нем необходимость отпадет
		cout << "Process p1 was created successfully!" << endl;
		char* data = (char*)readFromClipboard();
		//for (int i = 0; i < 10; ++i) {
			printf("Number:%c\n", data);
		//}
		GetExitCodeProcess(piProcessP1.hProcess, &dwExitCode); // Дочерний процесс завершился, получаем код его завершения
		CloseHandle(piProcessP1.hProcess); // Закрываем описатель процесса, как только необходимость в нем отпадет
	}
	lstrcpy(szPath, TEXT("C:\\Program Files (x86)\\Notepad++\\notepad++.exe"));
	BOOL processP2 = CreateProcess(NULL, szPath, &saProcess, &saThread,
		FALSE, 0, NULL, NULL, &si, &piProcessP2);
	if (processP2) {
		CloseHandle(piProcessP2.hThread); //Закрываем описатель потока, как только в нем необходимость отпадет
		cout << "Process p2 was created successfully!" << endl;
		const char* data = (const char*)readFromClipboard();
		char s[10] = { (char)data };
		FILE * ptrFile = fopen("numbers.txt", "w");
		if (ptrFile != NULL)
		{
			fputs(s, ptrFile);
			fclose(ptrFile);
		}
		system("pause");
		GetExitCodeProcess(piProcessP2.hProcess, &dwExitCode); // Дочерний процесс завершился, получаем код его завершения
		CloseHandle(piProcessP2.hProcess); // Закрываем описатель процесса, как только необходимость в нем отпадет
	}

	return 0;
}

int main() {
	int nMainRetVal = wmain(__argc, __wargv, _wenviron);
	return 0;
}
