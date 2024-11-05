#ifndef WINDOWS_MAIN_H
#define WINDOWS_MAIN_H

#include "Header/definitions.h"
#include "Library/platform.h"
#include "platform.h"
#include <windows.h>

#define MAIN_WINDOW_X 0
#define MAIN_WINDOW_Y 0
#define MAIN_WINDOW_WIDTH 1280
#define MAIN_WINDOW_HEIGHT 720
#define GAME_DLL_FILE_NAME L"game.dll"

typedef struct
{
    HMODULE Library;
    game_update_and_render *GameUpdateAndRender;
} windows_game_dll;

typedef struct
{
    platform_video State;
    BITMAPINFO Info;
} windows_video;

typedef struct
{
    int X;
    int Y;
    int Width;
    int Height;
} window_dimension;

#endif