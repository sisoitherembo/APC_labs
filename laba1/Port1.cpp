
#include "Port.h"






bool write(HANDLE hFile,char*  buffer, const int const *messageLen) {
	DWORD numOfBitesWriten = 0;
	if (!WriteFile(hFile, (LPVOID) messageLen, sizeof(int), &numOfBitesWriten, NULL))
	{
		fputs("\nInfo sending error", stderr);
		return false;
	}
	if (!WriteFile(hFile, (LPVOID) buffer, *messageLen * sizeof(char), &numOfBitesWriten, NULL))
	{
		fputs("\nInfo sending error", stderr);
		return false;
	}
	fprintf(stdout, "\nAmout of bites sent to the other COM-port : %d", numOfBitesWriten);
	return true;
}
void sendInfo(HANDLE hFile, HANDLE readEnd, HANDLE hExit, HANDLE hWork, HANDLE writeEnd)
{
	SetEvent(hWork);
	fputs("\nEnter your message :\t", stdout);
	
	char buffer[BufferTxRxSize] ;
	gets_s(buffer, BufferTxRxSize);
	int size = strlen(buffer)+1;
	if (write(hFile, buffer, &size))
	{
		SetEvent(writeEnd);
		WaitForSingleObject(readEnd, INFINITE);
		fputs("\nDelivered!", stdout);
	}
	else
		fputs("\nInfo sending error occured - please contact ypur system administrator", stderr);
}
void PORT1(HANDLE readEnd, HANDLE hExit, HANDLE hWork, HANDLE writeEnd)
{
	puts("\n\t---------COM1---------\n");
	HANDLE hFile = createPort(COM1);
	if (isFileOpen(hFile))
	{
		while (true)
			sendInfo(hFile, readEnd, hExit, hWork, writeEnd);
		CloseHandle(hFile);
	}
	else
	{
		fputs("\nFile opening problem occured", stderr);
		exit(1);
	}

}

	/*char buffer[100];
	int size;
	while (true)
	{
		if (WaitForSingleObject(hWork, 1) != WAIT_TIMEOUT)
		{
			puts("Ожидание. . .");
			WaitForSingleObject(writeEnd, INFINITE);
			size = read(hFile, buffer);
			printf("Сообщение из порта COM2: ");
			for (int i = 0; i < size; i++) printf("%c", buffer[i]);
			printf("\n");
			SetEvent(readEnd);
		}

		if (getch_() == 13)
		{

			SetEvent(hWork);
			printf("Сообщение: ");
			if (!write(hFile, buffer))
			{
				SetEvent(hExit);
				SetEvent(writeEnd);
				return;
			}
			else
			{
				SetEvent(writeEnd);
				WaitForSingleObject(readEnd, INFINITE);
				puts("Доставлено!");
			}
		}
	}*/
//}
//
//
//void PORT2(HANDLE readEnd, HANDLE writeEnd, HANDLE hFile, HANDLE hExit, HANDLE hWork)
//{
//	char buffer[100];
//	int size;
//
//	while (WaitForSingleObject(hExit, 1) == WAIT_TIMEOUT)
//	{
//		if (WaitForSingleObject(hWork, 1) != WAIT_TIMEOUT) //if works
//		{
//			puts("Ожидание. . .");
//			WaitForSingleObject(writeEnd, INFINITE);
//			if (WaitForSingleObject(hExit, 1) != WAIT_TIMEOUT) return;
//
//			size = read(hFile, buffer);
//			printf("Сообщение из порта COM1: ");
//			for (int i = 0; i < size; i++) printf("%c", buffer[i]);
//			printf("\n");
//			SetEvent(readEnd);
//
//		}
//
//		if (getch_() == 13)
//		{
//			SetEvent(hWork);
//			printf("Сообщение: ");
//			write(hFile, buffer);
//			SetEvent(writeEnd);
//			WaitForSingleObject(readEnd, INFINITE);
//			puts("Доставлено!");
//		}
//
//	}
//}
//
//
//
//int read(HANDLE hFile, char* buffer)
//{
//	int size;
//	DWORD numberOfBytesRead;
//
//	ReadFile(hFile, &size, 1 * sizeof(int), &numberOfBytesRead, NULL);	//read size
//	ReadFile(hFile, buffer, size * sizeof(char), &numberOfBytesRead, NULL);	//read info
//
//	return size;
//}
//
//
//bool write(HANDLE hFile, char* buffer)
//{
//	char symb;
//	int i = 0;
//	DWORD numberOfBytesWrite;
//
//	while (true)
//	{
//		scanf("%c", &symb);
//		if (symb == '\n')
//		{
//			buffer[i] = '\0';
//			WriteFile(hFile, &i, 1 * sizeof(int), &numberOfBytesWrite, NULL);	//write i(size)
//			WriteFile(hFile, buffer, i * sizeof(char), &numberOfBytesWrite, NULL);	//write info
//
//			if (!strcmp(buffer, "Выход\0")) {
//				return false;
//			}
//			return true;
//
//		}
//		else {
//			buffer[i] = symb;
//		}
//		i++;
//	}
//}