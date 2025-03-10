// Barak Gonen 2019z
// Skeleton code - inject DLL to a running process
#include "pch.h"
int main()
{
	char dllPath[MAX_LEN] = { 0 };
	char dllName[DLLNAME_SIZE] = DLLNAME;
	// Get full path of DLL to inject
	DWORD pathLen = GetFullPathNameA(DLLNAME, MAX_LEN, dllPath, NULL);
	// Get LoadLibrary function address –
	// the address doesn't change at remote process
	PVOID addrLoadLibrary = (PVOID)GetProcAddress(GetModuleHandleW((LPCWSTR)"kernel32.dll"), "LoadLibaryA");
	DWORD pid = 25748;
	printf("%s\n", dllPath);
	// Open remote process
	HANDLE proc = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_WRITE, FALSE, pid);
	// Get a pointer to memory location in remote process,
	// big enough to store DLL path
	PVOID memAddr = (PVOID)VirtualAllocEx(proc, NULL, strlen(dllName) + 1, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (NULL == memAddr) {
		DWORD err = GetLastError();
		printf("%d", err);
		printf("KRUV1");
		return 0;
	}
	// Write DLL name to remote process memory
	BOOL check = WriteProcessMemory(proc, memAddr, dllPath, strlen(dllName) + 1, NULL);
	if (0 == check) {
		DWORD err = GetLastError();
		//printf("%s", err);
		printf("KRUV2");
		return 0;
	}
	// Open remote thread, while executing LoadLibrary
	// with parameter DLL name, will trigger DLLMain
	HANDLE hRemote = CreateRemoteThread(proc, NULL, 0, (LPTHREAD_START_ROUTINE)addrLoadLibrary, memAddr, 0, NULL);
	if (NULL == hRemote) {
		DWORD err = GetLastError();
		return 0;
	}
	WaitForSingleObject(hRemote, INFINITE);
	check = CloseHandle(hRemote);
	CloseHandle(proc);
	return 0;
}