#include "platform.h"

#ifdef WINDOWS_OS
#include <windows.h>
#else
#error
#endif

typedef u8* va_args;

#define VAGet(args, type) *(type *)VAGet_(args)

#include "Code/string.c"

internal_function va_args
VABegin(void *Format)
{
    va_args Result = 0;

    Result = (u8 *)Format + POINTER_SIZE;
    return Result;
}

internal_function va_args
VAEnd()
{
    va_args Result = 0;

    return Result;
}

internal_function void *
VAGet_(va_args *Args)
{
    void *Result = *Args;

    *Args += POINTER_SIZE;

    return Result;
}

void PlatformWriteConsole(char *String)
{
#ifdef WINDOWS_OS
    OutputDebugStringA(String);
#else
    #error
#endif
}

void *PlatformAllocateMemory(uptr Size)
{
    void *Result = 0;

    if(!Size)
    {
        PlatformWriteConsole("Attempt to allocate memory of size 0\n");
        return Result;
    }

#ifdef WINDOWS_OS
    Result = VirtualAlloc(0, Size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if(!Result)
    {
        PlatformWriteConsole("Failed to allocate memory\n");
        return Result;
    }
#else
    #error
#endif

    return Result;
}

bool PlatformFreeMemory(void *Memory)
{
    bool Result = 0;

    if(!Memory)
    {
        PlatformWriteConsole("Attempt to free memory that is 0\n");
        return Result;
    }

#ifdef WINDOWS_OS
    if(!VirtualFree(Memory, 0, MEM_RELEASE))
    {
        PlatformWriteConsole("Failed to free memory\n");
        return Result;
    }
#else
    #error
#endif

    Result = 1;
    return Result;
}

uptr PlatformPrint(char *Format, ...)
{
    uptr Result = 0;

    uptr BufferSize = 1024;
    char *Buffer = (char *)PlatformAllocateMemory(BufferSize);
    if(!Buffer)
    {
        return Result;
    }

    va_args Args = VABegin(&Format);

    char *Octal = "01234567";
    char *Decimal = "0123456789";
    char *HexadecimalLowercase = "0123456789abcdef";
    char *HexadecimalUppercase = "0123456789ABCDEF";
    for(char *c = Format; *c != '\0';)
    {
        if(*c != '%')
        {
            Result += CopyCharacterToBuffer(Buffer + Result, BufferSize - Result, *c);
            c++;
            continue;
        }

        if(*++c == '%')
        {
            Result += CopyCharacterToBuffer(Buffer + Result, BufferSize - Result, *c);
            c++;
        }
        else if(*c == 'c')
        {
            int Value = VAGet(&Args, int);
            Result += CopyCharacterToBuffer(Buffer + Result, BufferSize - Result, Value);
            c++;
        }
        else if((*c == 'd') || (*c == 'i'))
        {
            s32 Value = VAGet(&Args, s32);
            Result += CopySigned32ToBuffer(Buffer + Result, BufferSize - Result, Value, 10, Decimal);
            c++;
        }
        else if(*c == 'f')
        {
            f64 Value = VAGet(&Args, f64);
            Result += CopyFloat64ToBuffer(Buffer + Result, BufferSize - Result, Value, 6);
            c++;
        }
        else if((c[0] == 'l') && (c[1] == 'l') && ((c[2] == 'd') || (c[2] == 'i')))
        {
            s64 Value = VAGet(&Args, s64);
            Result += CopySigned64ToBuffer(Buffer + Result, BufferSize - Result, Value, 10, Decimal);
            c += 3;
        }
        else if((c[0] == 'l') && (c[1] == 'l') && (c[2] == 'u'))
        {
            u64 Value = VAGet(&Args, u64);
            Result += CopyUnsigned64ToBuffer(Buffer + Result, BufferSize - Result, Value, 10, Decimal);
            c += 3;
        }
        else if(*c == 'o')
        {
            u32 Value = VAGet(&Args, u32);
            Result += CopyUnsigned32ToBuffer(Buffer + Result, BufferSize - Result, Value, 8, Octal);
            c++;
        }
        else if(*c == 'p')
        {
            void *Value = VAGet(&Args, void *);
            Result += CopyUnsigned64ToBuffer(Buffer + Result, BufferSize - Result, (u64)Value, 16, HexadecimalLowercase);
            c++;
        }
        else if(*c == 's')
        {
            char *Value = VAGet(&Args, char *);
            Result += CopyStringToBuffer(Buffer + Result, BufferSize - Result, Value, StringLength(Value));
            c++;
        }
        else if(*c == 'u')
        {
            u32 Value = VAGet(&Args, u32);
            Result += CopyUnsigned32ToBuffer(Buffer + Result, BufferSize - Result, Value, 10, Decimal);
            c++;
        }
        else if((*c == 'x') || (*c == 'X'))
        {
            u32 Value = VAGet(&Args, u32);
            
            if(*c == 'x')
            {
                Result += CopyUnsigned32ToBuffer(Buffer + Result, BufferSize - Result, Value, 16, HexadecimalLowercase);
            }
            else
            {
                Result += CopyUnsigned32ToBuffer(Buffer + Result, BufferSize - Result, Value, 16, HexadecimalUppercase);
            }

            c++;
        }
        else
        {
            Assert(0);
            PlatformWriteConsole("Invalid format specifier\n");
            break;
        }
    }

    Args = VAEnd();

    PlatformWriteConsole(Buffer);
    PlatformFreeMemory(Buffer);

    return Result;
}