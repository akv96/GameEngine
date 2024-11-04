#ifndef PLATFORM_LIBRARY_HPP
#define PLATFORM_LIBRARY_HPP

#include "Header/definitions.hpp"

void PlatformWriteConsole(char *String);
void *PlatformAllocateMemory(uptr Size);
bool PlatformFreeMemory(void *Memory);
uptr PlatformFormatString(char *Format, ...);

#define Print(format, ...) do { PlatformFormatString(format, __VA_ARGS__); } while(0)

#endif