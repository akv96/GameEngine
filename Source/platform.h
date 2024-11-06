#ifndef PLATFORM_H
#define PLATFORM_H

#include "Header/definitions.h"

typedef struct
{
    uptr PermanentMemorySize;
    void *PermanentMemory;
    uptr TemporaryMemorySize;
    void *TemporaryMemory;
} platform_memory;

typedef struct
{
    void *Memory;
    int Width;
    int Height;
    int BytesPerPixel;
    int Pitch;
} platform_video;

typedef struct
{
    bool IsDown;
} platform_button;

typedef struct
{
    platform_button Button[256];
    int MouseX;
    int MouseY;
} platform_controller;

typedef struct
{
    platform_controller Controller[1];
} platform_input;

#define GAME_UPDATE_AND_RENDER(name) void name(platform_memory *Memory, platform_video *Video, platform_input *Input)
typedef GAME_UPDATE_AND_RENDER(game_update_and_render);

#endif