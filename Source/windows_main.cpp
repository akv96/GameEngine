#include "Header/definitions.hpp"
#include "Library/platform.hpp"
#include <windows.h>

int APIENTRY 
WinMain(HINSTANCE Instance, HINSTANCE PreviousInstance, PSTR CommandLine, int ShowCode)
{
    Print("%% Hello, world! %%\n");
    Print("%% Hello, world! %%\n");
    Print("%% Hello, world! %%\n");
    
    Print("%c %c %c\n", 'A', 'B', 'C');
    Print("%c %c %c\n", 'A', 'B', 'C');
    Print("%c %c %c\n", 'A', 'B', 'C');

    Print("%d %i %d\n", 0, -1, 1234);
    Print("%d %i %d\n", 0, -1, 1234);
    Print("%d %i %d\n", 0, -1, 1234);

    Print("%f %f %f\n", 0.0, -1.0, 1.234);
    Print("%f %f %f\n", 0.0, -1.0, 1.234);
    Print("%f %f %f\n", 0.0, -1.0, 1.234);

    Print("%lld %lli\n",  9223372036854775807, 9223372036854775807);
    
    Print("%llu\n", 18446744073709551615);

    Print("%o %o %o\n", 0, 8, 16);
    Print("%o %o %o\n", 0, 8, 16);
    Print("%o %o %o\n", 0, 8, 16);

    Print("%p %p %p\n", 0, WinMain, Instance);
    Print("%p %p %p\n", 0, WinMain, Instance);
    Print("%p %p %p\n", 0, WinMain, Instance);

    Print("%s %s %s\n", "ABC", "DEF", "GHI");
    Print("%s %s %s\n", "ABC", "DEF", "GHI");
    Print("%s %s %s\n", "ABC", "DEF", "GHI");
    
    Print("%u %u %u\n", 0, -1, 1234);
    Print("%u %u %u\n", 0, -1, 1234);
    Print("%u %u %u\n", 0, -1, 1234);

    Print("%x %X %x\n", 0, -1, 255);
    Print("%x %X %x\n", 0, -1, 255);
    Print("%x %X %x\n", 0, -1, 255);

    return 0;
}