#include "windows_main.h"

internal_function bool
WindowsGetWindowDimension(HWND Window, window_dimension *WindowDimension)
{
    bool Result = 0;

    RECT ClientRectangle = {0};
    if(!GetClientRect(Window, &ClientRectangle))
    {
        Log("GetClientRect() failed: 0x%X\n", GetLastError());
        return Result;
    }

    WindowDimension->Width = ClientRectangle.right - ClientRectangle.left;
    WindowDimension->Height = ClientRectangle.bottom - ClientRectangle.top;

    Result = 1;
    return Result;
}

internal_function bool
WindowsLoadGameDLL(windows_game_dll *GameDLL, platform_memory *Memory, windows_video *Video)
{
    bool Result = 0;

    GameDLL->Library = LoadLibraryW(GAME_DLL_FILE_NAME);
    if(!GameDLL->Library)
    {
        Log("LoadLibraryW() failed: 0x%X\n", GetLastError());
        return Result;
    }

    GameDLL->GameUpdateAndRender = (game_update_and_render *)GetProcAddress(GameDLL->Library, "GameUpdateAndRender");
    if(!GameDLL->GameUpdateAndRender)
    {
        Log("GetProcAddress() failed: 0x%X\n", GetLastError());
        FreeLibrary(GameDLL->Library);
        return Result;
    }

    Memory->PermanentMemorySize = Megabytes(256);
    Memory->PermanentMemory = PlatformAllocateMemory(Memory->PermanentMemorySize);
    if(!Memory->PermanentMemory)
    {
        FreeLibrary(GameDLL->Library);
        return Result;
    }

    Memory->TemporaryMemorySize = Megabytes(256);
    Memory->TemporaryMemory = PlatformAllocateMemory(Memory->TemporaryMemorySize);
    if(!Memory->TemporaryMemory)
    {
        PlatformFreeMemory(Memory->PermanentMemory);
        FreeLibrary(GameDLL->Library);
        return Result;
    }    

    platform_video *VideoState = &Video->State;
    VideoState->Width = MAIN_WINDOW_WIDTH;
    VideoState->Height = MAIN_WINDOW_HEIGHT;
    VideoState->BytesPerPixel = 4;
    VideoState->Pitch = VideoState->Width * VideoState->BytesPerPixel;
    Video->Info.bmiHeader.biSize = sizeof(Video->Info.bmiHeader);
    Video->Info.bmiHeader.biWidth = VideoState->Width;
    Video->Info.bmiHeader.biHeight = VideoState->Height;
    Video->Info.bmiHeader.biPlanes = 1;
    Video->Info.bmiHeader.biBitCount = (WORD)(VideoState->BytesPerPixel * 8);
    Video->Info.bmiHeader.biCompression = BI_RGB;
    VideoState->Memory = PlatformAllocateMemory(VideoState->Pitch * VideoState->Height);
    if(!VideoState->Memory)
    {
        PlatformFreeMemory(Memory->TemporaryMemory);
        PlatformFreeMemory(Memory->PermanentMemory);
        FreeLibrary(GameDLL->Library);
        return Result;
    }

    Result = 1;
    return Result;
}

LRESULT WindowsMainWindowCallback(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam)
{
    LRESULT Result = 0;

    switch(Message)
    {
        case WM_CLOSE:
        {
            DestroyWindow(Window);
        } break;

        case WM_DESTROY:
        {
            PostQuitMessage(0);
        } break;

        //@TODO: WM_PAINT        

        default:
        {
            Result = DefWindowProcW(Window, Message, WParam, LParam);
        } break;
    }

    return Result;
}
int APIENTRY
WinMain(HINSTANCE Instance, HINSTANCE PreviousInstance, PSTR CommandLine, int ShowCode)
{
    int Result = -1;

    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

    windows_game_dll GameDLL = {0};
    platform_memory WindowsMemory = {0};
    windows_video WindowsVideo = {0};
    if(!WindowsLoadGameDLL(&GameDLL, &WindowsMemory, &WindowsVideo))
    {
        Log("Failed to load game dll\n");
        return Result;
    }

    WNDCLASSEXW WindowClass = {0};
    WindowClass.cbSize = sizeof(WNDCLASSEX);
    WindowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    WindowClass.lpfnWndProc = WindowsMainWindowCallback;
    WindowClass.hInstance = Instance;
    WindowClass.hCursor = LoadCursorA(0, IDC_ARROW);
    WindowClass.lpszClassName = L"Game Engine";
    if(!RegisterClassExW(&WindowClass))
    {
        Log("RegisterClassExW() failed: 0x%X\n", GetLastError());
        return Result;
    }

    HWND Window = CreateWindowExW(0, WindowClass.lpszClassName, WindowClass.lpszClassName, WS_OVERLAPPEDWINDOW, MAIN_WINDOW_X, MAIN_WINDOW_Y, MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT, 0, 0, Instance, 0);
    if(!Window)
    {
        Log("CreateWindowExW() failed: 0x%X\n", GetLastError());
        return Result;
    }

    HDC DeviceContext = GetDC(Window);
    if(!DeviceContext)
    {
        Log("GetDC() failed\n");
        return Result;
    }

    RECT WindowRectangle = {0};
    WindowRectangle.right = MAIN_WINDOW_WIDTH;
    WindowRectangle.bottom = MAIN_WINDOW_HEIGHT;
    if(!AdjustWindowRectExForDpi(&WindowRectangle, WS_OVERLAPPEDWINDOW, FALSE, 0, GetDpiForWindow(Window)))
    {
        Log("AdjustWindowRectExForDpi() failed: 0x%X\n", GetLastError());
        return Result;
    }

    if(!SetWindowPos(Window, 0, 0, 0, WindowRectangle.right - WindowRectangle.left, WindowRectangle.bottom - WindowRectangle.top, SWP_NOMOVE | SWP_NOZORDER | SWP_SHOWWINDOW))
    {
        Log("SetWindowPos() failed: 0x%X\n", GetLastError());
        return Result;
    }

    bool IsRunning = 1;
    platform_input Input[2] = {0};
    platform_input *OldInput = &Input[0];
    platform_input *NewInput = &Input[1];
    while(IsRunning)
    {
        MSG Message = {0};
        while(PeekMessageW(&Message, 0, 0, 0, PM_REMOVE))
        {
            switch(Message.message)
            {
                case WM_QUIT:
                {
                    TranslateMessage(&Message);
                    DispatchMessageW(&Message);

                    IsRunning = 0;
                } break;

                case WM_MOUSEMOVE:
                {
                    int X = GET_X_LPARAM(Message.lParam); 
                    int Y = GET_Y_LPARAM(Message.lParam);

                    NewInput->Controller[0].MouseX = X;
                    NewInput->Controller[0].MouseY = (WindowsVideo.State.Height - 1) - Y;
                } break;

                default:
                {
                    TranslateMessage(&Message);
                    DispatchMessageW(&Message);
                } break;
            }            
        }

        if(!IsRunning)
        {
            break;
        }


        if(GameDLL.GameUpdateAndRender)
        {
            platform_memory GameMemory = WindowsMemory;
            platform_video GameVideo = WindowsVideo.State;        
            GameDLL.GameUpdateAndRender(&GameMemory, &GameVideo, NewInput);
        }

        window_dimension WindowDimension = {0};
        if(WindowsGetWindowDimension(Window, &WindowDimension))
        {
            StretchDIBits(DeviceContext, 0, 0, WindowDimension.Width, WindowDimension.Height, 0, 0, WindowsVideo.State.Width, WindowsVideo.State.Height, WindowsVideo.State.Memory, &WindowsVideo.Info, DIB_RGB_COLORS, SRCCOPY);
        }        

        for(int Index = 0; Index < ArrayCount(OldInput->Controller[0].Button); Index++)
        {
            OldInput->Controller[0].Button[Index].IsDown = NewInput->Controller[0].Button[Index].IsDown;
            OldInput->Controller[0].MouseX = NewInput->Controller[0].MouseX;
            OldInput->Controller[0].MouseY = NewInput->Controller[0].MouseY;
        }

        platform_input *TemporaryInput = OldInput;
        OldInput = NewInput;
        NewInput = TemporaryInput;
    }
    Result = 0;
    return Result;
}