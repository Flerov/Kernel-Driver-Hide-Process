#include "main.h"

const char* GetLastErrorAsString()
{
	DWORD errorID = GetLastError();
	if (errorID == 0) { return NULL; }
	char* messageBuffer = NULL;
	size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, errorID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);
	return messageBuffer;
}
