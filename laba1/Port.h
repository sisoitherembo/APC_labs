#pragma once
#ifndef PORT_H
#define  PORT_H


#include <windows.h>
#include <iostream>
#include <conio.h>
#include <stdlib.h>
#include <locale.h>
#define _CRT_SECURE_NO_WARNINGS 
#define uint unsigned int
#define COM1_wch L"COM1"
#define COM2_wch L"COM2"
#define BufferTxRxSize 100
enum PORT {
	COM1,
	COM2,
	UNKNOWN
};
void PORT1(HANDLE readEnd, HANDLE hExit, HANDLE hWork, HANDLE writeEnd);
void PORT2(HANDLE readEnd, HANDLE hExit, HANDLE hWork, HANDLE writeEnd);
bool write(HANDLE hFile, char* buffer, const int const* messageLen);
int read(HANDLE hFile, char* buffer, const int bufLen);
bool isFileOpen(HANDLE hFile);
HANDLE createPort(PORT type);
PORT getPortType(char* buffer);
#endif // !1