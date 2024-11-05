#ifndef PLATFORM_LIBRARY_H
#define PLATFORM_LIBRARY_H

#include "Header/definitions.h"

void PlatformWriteConsole(char *String);
void *PlatformAllocateMemory(uptr Size);
bool PlatformFreeMemory(void *Memory);
uptr PlatformPrint(char *Format, ...);

#define Print(format, ...) do { PlatformPrint(format, __VA_ARGS__); } while(0)
#define Log Print

#endif