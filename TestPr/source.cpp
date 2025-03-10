#include <stdio.h>
#include <windows.h>
#define LIBRARY "DLLInject.dll"
typedef void(*PFUNC)(void);
int main()
{
	HMODULE hModule = LoadLibraryA(LIBRARY);
	if (NULL == hModule) {
		printf("Failed to load DLL\n");
		return 0;
	}
	PFUNC pFunc = (PFUNC)GetProcAddress(hModule, "Share");
	if (NULL != pFunc) {
		(*pFunc)();
	}
	else {
		printf("Failed to load function\n");
	}
	return 0;
}