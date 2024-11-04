#ifndef DEFINITIONS_HPP
#define DEFINITIONS_HPP

#define internal_function static
#define global_variable static
#define local_variable static

#define Assert(expression) do { if(!(expression)) { *(int *)0 = 0; } } while(0)
#define ArrayCount(array) (sizeof(array) / sizeof((array)[0]))
#define Kilobytes(value) ((value) * 1024LL)
#define Megabytes(value) (Kilobytes(value) * 1024LL)
#define Gigabytes(value) (Megabytes(value) * 1024LL)
#define Terabytes(value) (Gigabytes(value) * 1024LL)

#define WINDOWS_OS
#define POINTER_SIZE sizeof(void *)

typedef char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef long long s64;
typedef unsigned long long u64;
typedef s64 sptr;
typedef u64 uptr;
typedef float f32;
typedef double f64;

#endif