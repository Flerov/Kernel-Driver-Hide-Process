#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <tchar.h>
#include <stdio.h>
#include <iostream>
#include <winsvc.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <winioctl.h>

// in main.cpp
unsigned int GetProcessList(char* processname);
int main(int argc, char* argv[]);

// in utils.cpp
int call_kernel_driver(unsigned int pid, HANDLE hDevice);
BOOL load_driver(SC_HANDLE svcHandle);
HANDLE install_driver();

// in handleerror.cpp
const char* GetLastErrorAsString();
