#ifndef MATH_H
#define MATH_H

#include "Header/definitions.h"

typedef struct
{
    f32 X;
    f32 Y;
} v2;

typedef struct
{
    f32 X;
    f32 Y;
    f32 Z;
} v3;

typedef struct
{
    f32 Data[4][4];
} matrix4x4;

#define MAX_TAYLOR_SERIES_TERM_COUNT 10
#define PI32 3.14159265359f
#define DegreesToRadians(angle) ((angle) * (PI32 / 180.0f))
#define Absolute(x) (((x) < 0) ? (-(x)) : (x))
#define Min(a, b) (((a) < (b)) ? (a) : (b))
#define Max(a, b) (((a) > (b)) ? (a) : (b))

#endif