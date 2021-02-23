
#include "Port.h"






bool write(HANDLE hFile,char*  buffer,  int messageLen) {
	DWORD numOfBitesWriten = 0;
	if (!WriteFile(hFile, (LPVOID) &messageLen, sizeof(int), &numOfBitesWriten, NULL))
	{
		fputs("\nInfo sending error", stderr);
		return false;
	}
	if (!WriteFile(hFile, (LPVOID) buffer, messageLen * sizeof(char), &numOfBitesWriten, NULL))
	{
		fputs("\nInfo sending error", stderr);
		return false;
	}
	fprintf(stdout, "Amout of bites sent to the other COM-port : %d", numOfBitesWriten);
	return true;
}
bool sendInfo(HANDLE hFile, HANDLE readEnd, HANDLE hExit, HANDLE hWork, HANDLE writeEnd)
{
	SetEvent(hWork);
	fputs("\nEnter your message (enter \"EXIT123\" to stop sending): \t", stdout);

	char buffer[BufferTxRxSize];
	gets_s(buffer, BufferTxRxSize);
	int size = strlen(buffer) + 1;
	if (strcmp(buffer, "EXIT123")==0)
		return false;
	if (write(hFile, buffer, size))
	{
		SetEvent(writeEnd);
		WaitForSingleObject(readEnd, INFINITE);
		fputs("\nDelivered!\n", stdout);
		return true;
	}
	else
	{
		fputs("\nInfo sending error occured - please contact ypur system administrator", stderr);
		return false;
	}

}
void PORT1(HANDLE readEnd, HANDLE hExit, HANDLE hWork, HANDLE writeEnd)
{
	puts("\n\n\t---------COM1---------\n");
	HANDLE hFile = createPort(COM1);
	if (isFileOpen(hFile))
	{
		while (sendInfo(hFile, readEnd, hExit, hWork, writeEnd));
		CloseHandle(hFile);
		SetEvent(hExit);
		SetEvent(writeEnd);
	}
	else
	{
		fputs("\nFile opening problem occured", stderr);
		_getch();
		exit(1);
	}

}

	