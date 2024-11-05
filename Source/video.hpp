#ifndef VIDEO_HPP
#define VIDEO_HPP

#include "Header/definitions.hpp"
#include "Library/platform.hpp"
#include "platform.hpp"

struct rectangle
{
    int X;
    int Y;
    int Width;
    int Height;
};

struct rgba
{
    f32 R;
    f32 G;
    f32 B;
    f32 A;
};

struct vertex
{
    f32 X;
    f32 Y;
    f32 Z;
    f32 W;
};

struct polygon
{
    vertex *Vertices;
    int VerticesCount;
    rgba Color;    
};

#include "math.cpp"

#endif