#include "stdafx.h"  //параллельная работа процессов, работа с объектами ядра, синхронизация, найти замену клипборду
#include <Windows.h>
#include <iostream>
#include <string.h>
#include "ctime"
#include "vars.h"

using namespace std;

int __cdecl wmain(int argc, wchar_t *argv[], wchar_t *envp[]);

int create_number()
{
	int number = rand() %RAND_MAX + RAND_MIN;
	return number;
}

void writeToClipboard(int n[])
{
	char source[M_SIZE + 1] = { "\0" };
	char str1[2], str2[2], str3[2], str4[2], str5[2];
	int j = 0;
	itoa(n[0], str1, 10);
	itoa(n[1], str2, 10);
	itoa(n[2], str3, 10);
	itoa(n[3], str4, 10);
	itoa(n[4], str5, 10);
	for (int i = 0; str1[i] != '\0'; i++)
	{
		source[j] = str1[i];
		j++;
	}
	for (int i = 0; str2[i] != '\0'; i++)
	{
		source[j] = str2[i];
		j++;
	}
	for (int i = 0; str3[i] != '\0'; i++)
	{
		source[j] = str3[i];
		j++;
	}
	for (int i = 0; str4[i] != '\0'; i++)
	{
		source[j] = str4[i];
		j++;
	}
	for (int i = 0; str5[i] != '\0'; i++)
	{
		source[j] = str5[i];
		j++;
	}
	if (OpenClipboard(NULL))
	{
		HGLOBAL hgBuffer;
		EmptyClipboard(); 
		hgBuffer = GlobalAlloc(GMEM_DDESHARE, strlen(source) + 1);//выделяем память
		char* chBuffer;
		chBuffer = (char*)GlobalLock(hgBuffer); //блокируем память
		strcpy(chBuffer, (char const*)LPCSTR(source));
		GlobalUnlock(hgBuffer);//разблокируем память
		SetClipboardData(CF_TEXT, hgBuffer);//помещаем текст в буфер обмена
		CloseClipboard(); 
	}
}

char* readFromClipboard() {
	char* fromClipboard = {"\0"}; //в эту переменную сохраним текст из буфера обмена
	if (OpenClipboard(NULL))
	{
		HANDLE hData = GetClipboardData(CF_TEXT);//извлекаем текст из буфера обмена
		char* chBuffer = (char*)GlobalLock(hData);//блокируем память
		fromClipboard = chBuffer;
		GlobalUnlock(hData);//разблокируем память
		CloseClipboard();
	}
	return fromClipboard;
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
	int numbers[M_SIZE];
	srand(time(NULL));
	for (int i = 0; i < M_SIZE; i++) {
		numbers[i] = create_number();
	}
	writeToClipboard((int*)numbers);

	lstrcpy(szPath, TEXT("C:\\Windows\\system32\\cmd.exe"));
	BOOL processP1 = CreateProcess(NULL, szPath, &saProcess, &saThread,
		FALSE, 0, NULL, NULL, &si, &piProcessP1);
	if (processP1) {
		CloseHandle(piProcessP1.hThread); //Закрываем описатель потока, как только в нем необходимость отпадет
		cout << "Process p1 was created successfully!" << endl;
		char* data = { readFromClipboard() };
		printf("Data from clipboard: ");
		printf(data);
		printf("\n");
		GetExitCodeProcess(piProcessP1.hProcess, &dwExitCode); // Дочерний процесс завершился, получаем код его завершения
		CloseHandle(piProcessP1.hProcess); // Закрываем описатель процесса, как только необходимость в нем отпадет
	}
	lstrcpy(szPath, TEXT("C:\\Program Files (x86)\\Notepad++\\notepad++.exe"));
	BOOL processP2 = CreateProcess(NULL, szPath, &saProcess, &saThread,
		FALSE, 0, NULL, NULL, &si, &piProcessP2);
	if (processP2) {
		CloseHandle(piProcessP2.hThread); //Закрываем описатель потока, как только в нем необходимость отпадет
		cout << "Process p2 was created successfully!" << endl;
		cout << "Writing data from clipboard to numbers.txt ..." << endl;
		char* data = { readFromClipboard() };
		FILE * ptrFile = fopen("numbers.txt", "w");
		if (ptrFile != NULL)
		{
			fputs(data, ptrFile);
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
