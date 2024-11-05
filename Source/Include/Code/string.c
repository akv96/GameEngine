#include "Header/definitions.h"

internal_function uptr
StringLength(char *String)
{
    uptr Result = 0;

    while(*String++ != '\0')
    {
        Result++;
    }

    return Result;
}

internal_function uptr
CopyCharacterToBuffer(char *Buffer, uptr BufferSize, int Character)
{
    uptr Result = 0;

    if(BufferSize < sizeof(char))
    {
        return Result;
    }

    Buffer[0] = (char)Character;

    Result = 1;
    return Result;
}

internal_function uptr
CopyStringToBuffer(char *Buffer, uptr BufferSize, char *String, uptr Length)
{
    uptr Result = 0;

    uptr MaxLength = StringLength(String);
    if(Length > MaxLength)
    {
        Length = MaxLength;
    }

    for(uptr Index = 0; Index < Length; Index++)
    {
        Result += CopyCharacterToBuffer(Buffer + Index, BufferSize - Index, String[Index]);
    }

    return Result;
}

internal_function uptr
CopySigned32ToBuffer(char *Buffer, uptr BufferSize, s32 Value, int Base, char *Representation)
{
    uptr Result = 0;

    if(!Value)
    {
        Result = CopyCharacterToBuffer(Buffer, BufferSize, '0');
        return Result;
    }

    bool IsNegative = Value < 0;
    if(IsNegative)
    {
        Value = -Value;
    }

    char WorkBuffer[32 * 2] = {0};
    int WorkBufferIndex = ArrayCount(WorkBuffer) - 2;
    for(; Value; Value /= Base)
    {
        WorkBuffer[WorkBufferIndex--] = Representation[Value % Base];
    }

    if(IsNegative)
    {
        WorkBuffer[WorkBufferIndex] = '-';
    }
    else
    {
        WorkBufferIndex++;
    }

    Result = CopyStringToBuffer(Buffer, BufferSize, WorkBuffer + WorkBufferIndex, StringLength(WorkBuffer + WorkBufferIndex));
    return Result;
}

internal_function uptr
CopyUnsigned32ToBuffer(char *Buffer, uptr BufferSize, u32 Value, int Base, char *Representation)
{
    uptr Result = 0;

    if(!Value)
    {
        Result = CopyCharacterToBuffer(Buffer, BufferSize, '0');
        return Result;
    }

    char WorkBuffer[32 * 2] = {0};
    int WorkBufferIndex = ArrayCount(WorkBuffer) - 2;
    for(; Value; Value /= Base)
    {
        WorkBuffer[WorkBufferIndex--] = Representation[Value % Base];
    }

    WorkBufferIndex++;

    Result = CopyStringToBuffer(Buffer, BufferSize, WorkBuffer + WorkBufferIndex, StringLength(WorkBuffer + WorkBufferIndex));
    return Result;
}

internal_function uptr
CopySigned64ToBuffer(char *Buffer, uptr BufferSize, s64 Value, int Base, char *Representation)
{
    uptr Result = 0;

    if(!Value)
    {
        Result = CopyCharacterToBuffer(Buffer, BufferSize, '0');
        return Result;
    }

    bool IsNegative = Value < 0;
    if(IsNegative)
    {
        Value = -Value;
    }

    char WorkBuffer[64 * 2] = {0};
    int WorkBufferIndex = ArrayCount(WorkBuffer) - 2;
    for(; Value; Value /= Base)
    {
        WorkBuffer[WorkBufferIndex--] = Representation[Value % Base];
    }

    if(IsNegative)
    {
        WorkBuffer[WorkBufferIndex] = '-';
    }
    else
    {
        WorkBufferIndex++;
    }

    Result = CopyStringToBuffer(Buffer, BufferSize, WorkBuffer + WorkBufferIndex, StringLength(WorkBuffer + WorkBufferIndex));
    return Result;
}

internal_function uptr
CopyUnsigned64ToBuffer(char *Buffer, uptr BufferSize, u64 Value, int Base, char *Representation)
{
    uptr Result = 0;

    if(!Value)
    {
        Result = CopyCharacterToBuffer(Buffer, BufferSize, '0');
        return Result;
    }

    char WorkBuffer[64 * 2] = {0};
    int WorkBufferIndex = ArrayCount(WorkBuffer) - 2;
    for(; Value; Value /= Base)
    {
        WorkBuffer[WorkBufferIndex--] = Representation[Value % Base];
    }

    WorkBufferIndex++;

    Result = CopyStringToBuffer(Buffer, BufferSize, WorkBuffer + WorkBufferIndex, StringLength(WorkBuffer + WorkBufferIndex));
    return Result;
}

internal_function uptr
CopyFloat64ToBuffer(char *Buffer, uptr BufferSize, f64 Value, int Precision)
{
    uptr Result = 0;

    char *Decimal = "0123456789";
    int IntegerPart = (int)Value;
    Result = CopySigned32ToBuffer(Buffer, BufferSize, IntegerPart, 10, Decimal);
    Result += CopyCharacterToBuffer(Buffer + Result, BufferSize - Result, '.');

    if(Value < 0)
    {
        Value = -Value;
        IntegerPart = -IntegerPart;
    }

    Value -=IntegerPart;

    for(int Index = 0; Index < Precision; Index++)
    {
        Value *= 10;
        int Digit = (int)Value;
        Result += CopySigned32ToBuffer(Buffer + Result, BufferSize - Result, Digit, 10, Decimal);
        Value -= Digit;
    }

    return Result;
}