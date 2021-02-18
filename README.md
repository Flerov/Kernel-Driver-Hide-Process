# Kernel-Driver-Hide-Process
This Kernel Driver makes anny Running Appplication invisible for TaskManager

!This Project was build and tested on a Windows10 Virtual Machin Build Version 19041!
The UniqueProcessId may differ on your machine. You can simply change it to your machine offset by changing the 'OFFSET'-value in ~KernelPart/driver.c
Furthermore you can simply find your UniqueProcessId offset by debugging the nt!_EPROCESS - Structure
- Run 'WinDbgx64.exe -> local Kernel Debugger' with elevated privileges
- set up the symbol path by running -> .sympath srv* -> .symfix -> .reload
- debug the EPROCESS structure by running -> dt nt!_EPROCESS
- search for the UniqueProcessId - Offset
--------------------
![alt text](https://github.com/Flerov/Kernel-Driver-Hide-Process/blob/misc/FlinkBlink.jpeg)
