#include <windows.h>
#include <iostream>
#define _CRT_SECURE_NO_WARNINGS 
using namespace std;

HANDLE hSerial;

int main(int argc, WCHAR* argv[])
{
	LPCWSTR sPortName =  L"COM1";
	hSerial = ::CreateFile(sPortName, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,0);
    if (hSerial == INVALID_HANDLE_VALUE)
    {
        if (GetLastError() == ERROR_FILE_NOT_FOUND)
        {
            cout << "serial port does not exist.\n";
        }
        cout << "some other error occurred.\n";
    }
    return 0;
}