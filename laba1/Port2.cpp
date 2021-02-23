#include "Port.h"


int read(HANDLE hFile, char* buffer, const int bufLen) {
	long long int size = 0;
	DWORD numOfBytesRead = 0;
	char temp = '\0';
	int i = 0;
	if (!ReadFile(hFile, (LPVOID)&size, 1*sizeof(long long int), &numOfBytesRead, NULL))
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
	if(!ReadFile(hFile, (LPVOID) buffer, size * sizeof(char), &numOfBytesRead, NULL))
	{	//read info
		fputs("\nInfo receiving error", stderr);
		return 0;
	} 

	return numOfBytesRead;

}


void waitInfo(HANDLE hFile, HANDLE readEnd, HANDLE hExit, HANDLE hWork, HANDLE writeEnd) {
	if (WaitForSingleObject(hWork, 1) != WAIT_TIMEOUT) {
		char buffer[BufferTxRxSize];
		WaitForSingleObject(writeEnd, INFINITE);
		uint size = read(hFile, buffer, BufferTxRxSize);
		if (size == 0)
		{
			fputs("\n Error : inappropriate info read", stderr);
			exit(1);
		}
		fputs("\nMessage from COM1 :\n", stdout);
		fputs(buffer, stdout);
		SetEvent(readEnd);
	}
}

void PORT2(HANDLE readEnd, HANDLE hExit, HANDLE hWork, HANDLE writeEnd)
{
	puts("\n\t---------COM2---------\n");
	HANDLE hFile = createPort(COM2);
	if (isFileOpen(hFile))
	{
		fputs("\nWaiting...\n", stdout);
		while (true)
			waitInfo(hFile, readEnd, hExit, hWork, writeEnd);
		CloseHandle(hFile);
	}
	else
	{
		fputs("\nFile opening problem occured", stderr);
		exit(1);
	}
}