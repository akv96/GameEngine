#ifndef PLATFORM_HPP
#define PLATFORM_HPP

struct platform_memory
{
    uptr PermanentMemorySize;
    void *PermanentMemory;
    uptr TemporaryMemorySize;
    void *TemporaryMemory;
};

struct platform_video
{
    void *Memory;
    int Width;
    int Height;
    int BytesPerPixel;
    int Pitch;
};

struct platform_button
{
    bool IsDown;
};

struct platform_controller
{
    platform_button Button[256];
};

struct platform_input
{
    platform_controller Controller[1];
};

#define GAME_UPDATE_AND_RENDER(name) void name(platform_memory *Memory, platform_video *Video, platform_input *Input)
typedef GAME_UPDATE_AND_RENDER(game_update_and_render);

#endif