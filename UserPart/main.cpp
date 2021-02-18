#include "main.h"

int main(int argc, char* argv[])
{
	HANDLE hDevice;

	_tprintf("[REQUIRES ELEVATED PRIVILEGES]\n");
	if (argc != 2)
	{
		_tprintf("Usage Error: [PROVIDE A PROCESS NAME] (example: cmd.exe)\n");
		exit(EXIT_FAILURE);
	}
	_tprintf("~Hide a Process~\n{Usage: UserMode.exe [process name]\n");

	unsigned int pid = GetProcessList(argv[1]);
	if (pid == 0)
	{
		_tprintf("[-] Process [%s] not found!\n", argv[1]);
		exit(2);
	}
	_tprintf("[+] Discovered PID of process [%s|%d]\n", argv[1], pid);

	hDevice = install_driver();
	if (hDevice == NULL) { exit(1); }
	_tprintf("[+] Recieved driver handle!\n");

	char buffer[10];
	sprintf_s(buffer, "%ld", pid);
	call_kernel_driver(pid, hDevice);

	return 0;
}

// [IMPORTANT_BUILD_NOTE]:_tprintf -> either fix it or build as Debug
