#include "Port.h"


bool isFileOpen(HANDLE hFile) {
	if (hFile == INVALID_HANDLE_VALUE) {
		return false;
	}
	return true;
}
HANDLE createPort(PORT type) {
	HANDLE file;
	if (type == COM1)
		file = CreateFile(COM1_wch, GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	else if (type == COM2)
		file = CreateFile(COM2_wch, GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	else
	{
		fputs("Incorrect File creation", stderr);
		exit(1);
	}
	return file;
}

PORT getPortType(char* buffer) {
	PORT retVal;
	if (!strcmp(buffer, "COM1"))
		retVal = COM1;
	else if (!strcmp(buffer, "COM2"))
		retVal = COM2;
	else retVal = UNKNOWN;
	return retVal;
}
