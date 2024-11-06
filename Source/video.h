#ifndef VIDEO_H
#define VIDEO_H

#include "Header/definitions.h"
#include "platform.h"
#include "math.h"

typedef struct
{
    f32 R;
    f32 G;
    f32 B;
    f32 A;
} rgba;

typedef struct
{
    v3 Vertices[3];
} triangle;

typedef struct
{
    v3 Vertices[8];
    triangle Faces[12];
} cube;

#endif