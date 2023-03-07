#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <Windows.h>
#include <Psapi.h>

#include "Memory.h"

void* FindPattern(PCWSTR module, PCSTR pattern, SIZE_T offset)
{
    MODULEINFO moduleInfo;
    HMODULE moduleHandle = GetModuleHandleW(module);

    if (moduleHandle && GetModuleInformation(GetCurrentProcess(), moduleHandle, &moduleInfo, sizeof(moduleInfo))) {
        PCHAR begin = moduleInfo.lpBaseOfDll;
        PCHAR end = begin + moduleInfo.SizeOfImage;

        for (PCHAR c = begin; c != end; c++) {
            bool matched = true;

            for (PCSTR patternIt = pattern, it = c; *patternIt; patternIt++, it++) {
                if (*patternIt != '?' && *it != *patternIt) {
                    matched = false;
                    break;
                }
            }
            if (matched)
                return c + offset;
        }
    }
    WCHAR buf[100];
    swprintf(buf, sizeof(buf) / sizeof(WCHAR), L"Failed to find pattern in module: %s!", module);
    MessageBoxW(NULL, buf, L"Error", MB_OK | MB_ICONERROR);
    exit(EXIT_FAILURE);
}