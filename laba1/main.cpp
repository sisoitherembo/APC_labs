#include "Port.h"
#define EXECUTABLE_PATH L"D:\\visual studio\\4 semestr\\APC\\laba1\\Debug\\laba1.exe"

static void (*portAction[])(HANDLE readEnd, HANDLE hExit, HANDLE hWork, HANDLE writeEnd) = { PORT1, PORT2 };

void createNewProcess(char* buffer, PORT type, HANDLE readEnd, HANDLE hExit, HANDLE hWork, HANDLE writeEnd);

int main(int argc, char* argv[])
{

	char buffer[] = { "?????" };
	uint bufferLenth = strlen(buffer);
	HANDLE hWork = CreateEvent(NULL, FALSE, FALSE, L"work");
	HANDLE readEnd = CreateEvent(NULL, FALSE, FALSE, L"readEnd");
	HANDLE writeEnd = CreateEvent(NULL, FALSE, FALSE, L"writeEnd");
	HANDLE hExit = CreateEvent(NULL, FALSE, FALSE, L"exit");
	PORT type = UNKNOWN;
	fputs("Welcome!\nEnter port you'r going to use :\t", stdout);
	
	if (wcslen(GetCommandLine()) == 55)
		do {
		gets_s(buffer, bufferLenth);
		type = getPortType(buffer);
		if (type == UNKNOWN)
			fputs("Error : unknown port entered, try one more time :\t", stdout);
	} while (type == UNKNOWN);
	if (wcslen(GetCommandLine())==55)
	{
		createNewProcess(buffer, type, readEnd, hExit, hWork, writeEnd);
		puts("\n\nProgram ending\n\n");
		_getch();
		return 0;
	}
	else
	{
		wchar_t* str = GetCommandLine();
		size_t size = 100;
		char commandLine[100];
		wcstombs_s(&size, commandLine, size, str, size);
			if (!strcmp(commandLine, "COM1"))
				type = COM2;
			else
				type = COM1;
		
		switch (type)
		{
		case COM1: portAction[COM1](readEnd, hExit, hWork, writeEnd); break;
		case COM2: portAction[COM2](readEnd, hExit, hWork, writeEnd); break;
		}
	}
	puts("\n\nProgram ending\n\n");
	_getch();
	return 0;
}

void createNewProcess(char* buffer, PORT type, HANDLE readEnd, HANDLE hExit, HANDLE hWork, HANDLE writeEnd)
{
	size_t num;
	wchar_t wtext[20];
	mbstowcs_s(&num, (wtext), buffer, (strlen(buffer) + 1));//Plus null
	LPWSTR commandLineStr = wtext;
	STARTUPINFO StInf = { 0 };
	StInf.cb = sizeof(StInf);
	PROCESS_INFORMATION ProcInf = { 0 };
	if (!CreateProcess(EXECUTABLE_PATH, commandLineStr, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &StInf, &ProcInf))
	{
		fputs("Error : fail when creating new process\n", stderr);
		exit(1);
	}

	switch (type)
	{
	case COM1: portAction[COM1](readEnd, hExit, hWork, writeEnd); break;
	case COM2: portAction[COM2](readEnd, hExit, hWork, writeEnd); break;
	}
}
