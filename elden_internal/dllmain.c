#include <Windows.h>
#include <stdio.h>

#include "Memory.h"

void Init() {
	// Set stuff up
	FILE* pFile = 0;
	AllocConsole();
	freopen_s(&pFile, "CONOUT$", "w", stdout);

	HMODULE base = GetModuleHandleA(NULL);
	printf("Module Base %p\n", base);

	// Patch ultra wide
	PVOID ultrawide = FindPattern(L"eldenring.exe", "\x74\x4f\x45\x8b\x94\xcc", 0);
	printf("Ultrawide JMP: 0x%p\n", ultrawide);

    DWORD oldprotect;
    VirtualProtect(ultrawide, 1, PAGE_EXECUTE_READWRITE, &oldprotect);
	// Replace JE with JNE 
	char* je = ultrawide;
	*je = '\x75';
    VirtualProtect(ultrawide, 1, oldprotect, &oldprotect);

	printf("Patched !");
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved){
	if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Init, NULL, 0, NULL);
	}

	return TRUE;
}