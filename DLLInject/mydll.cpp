#include "pch.h"
#define DLL_EXPORT
#include "mydll.h"

DWORD WINAPI createCmdThread(LPVOID lpParam)
{
	system("start cmd /k echo Hello, this is DLL injection!");
	return 0;
}

extern "C"
{
	DECLDIR void Share()
	{
		printf("I am an exported function, can be called outside the DLL\n");
	}
	void Keep()
	{
		//printf("I am not exported, can be called only within the DLL\n");
		HANDLE hThread = CreateThread(nullptr, 0, createCmdThread, nullptr, 0, nullptr);
		CloseHandle(hThread);
	}
}

BOOL APIENTRY DllMain(HANDLE hModule, // Handle to DLL module

	DWORD ul_reason_for_call,
	LPVOID lpReserved) // Reserved

{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		// A process is loading the DLL.
		//Share();
		//Keep();
		break;
	case DLL_THREAD_ATTACH:
		// A process is creating a new thread.
		break;
	case DLL_THREAD_DETACH:
		// A thread exits normally.
		break;
	case DLL_PROCESS_DETACH:
		// A process unloads the DLL.
		break;
	}
	return TRUE;
}