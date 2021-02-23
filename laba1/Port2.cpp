#include "Port.h"


int read(HANDLE hFile, char* buffer, const int bufLen) {
	
	 int size = 0;
	DWORD numOfBytesRead = 0;
	int i = 0;
	if (!ReadFile(hFile, &size, 1*sizeof(int), &numOfBytesRead, NULL))
	{
		fputs("\nInfo receiving error", stderr);
		return 0;
	}
	if (size > bufLen || size < 0)
	{
		fputs("\nError : the receiving info larger than buffer can hold", stderr);
		size = 0;
		return size;
	}
	buffer[size] = '\0';
	OVERLAPPED s = { 0 };
	if(!ReadFile(hFile, (LPVOID) buffer, size * sizeof(char), &numOfBytesRead, &s))
	{	//read info
		fputs("\nInfo receiving error", stderr);
		return 0;
	} 

	return numOfBytesRead;

}


void waitInfo(HANDLE hFile, HANDLE readEnd, HANDLE hExit, HANDLE hWork, HANDLE writeEnd) {
	if (WaitForSingleObject(hWork, 1) != WAIT_OBJECT_0) {
		char buffer[BufferTxRxSize] = {""};
		fputs("\n\nWaiting...\n", stdout);
		WaitForSingleObject(writeEnd, INFINITE);
		if (WaitForSingleObject(hExit, 1) == WAIT_OBJECT_0)
			return;
		uint size = read(hFile, buffer, BufferTxRxSize);
		if (size == 0)
		{
			fputs("\n Error : inappropriate info read", stderr);
			_getch();
			exit(1);
		}
		fputs("\nMessage from COM1 :\n", stdout);
		int i = 0;
		while (i < size)
			fputc(buffer[i++], stdout);
		SetEvent(readEnd);
	}
}

void PORT2(HANDLE readEnd, HANDLE hExit, HANDLE hWork, HANDLE writeEnd)
{
	puts("\n\n\t---------COM2---------\n");
	HANDLE hFile = createPort(COM2);
	if (isFileOpen(hFile))
	{
		
		while (WaitForSingleObject(hExit, 1) != WAIT_OBJECT_0)
			waitInfo(hFile, readEnd, hExit, hWork, writeEnd);
		CloseHandle(hFile);
	}
	else
	{
		fputs("\nFile opening problem occured", stderr);
		_getch();
		exit(1);
	}
}