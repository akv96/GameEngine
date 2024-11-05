#include "windows_main.hpp"

#define GAME_DLL_FILE_NAME L"main.dll"

struct windows_main_dll
{
    HMODULE Library;
    game_update_and_render *GameUpdateAndRender;
};

struct windows_video
{
    BITMAPINFO Info;    
    platform_video State;    
};

struct window_dimension
{
    int X;
    int Y;
    int Width;
    int Height;
};

internal_function bool
WindowsGetWindowDimension(HWND Window, window_dimension *WindowDimension)
{
    bool Result = 0;

    RECT ClientRect = {};
    if(!GetClientRect(Window, &ClientRect))
    {
        Log("GetClientRect() failed: 0x%X\n", GetLastError());
        return Result;
    }

    WindowDimension->Width = ClientRect.right - ClientRect.left;
    WindowDimension->Height = ClientRect.bottom - ClientRect.top;

    Result = 1;
    return Result;
}

internal_function WCHAR *
ANSIToUTF16(char *ANSI)
{
    WCHAR *Result = 0;

    int UTF16Size = MultiByteToWideChar(CP_ACP, 0, ANSI, -1, 0, 0);

    Result = (WCHAR *)PlatformAllocateMemory(UTF16Size);
    if(!Result)
    {
        Log("Failed to allocate memory for UTF16 string\n");
        return Result;
    }

    if(!MultiByteToWideChar(CP_ACP, 0, ANSI, -1, Result, UTF16Size))
    {
        Log("MultiByteToWideChar() failed: 0x%X\n", GetLastError());
        PlatformFreeMemory(Result);
        Result = 0;
        return Result;
    }

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

    windows_main_dll WindowsMainDLL = {};
    WindowsMainDLL.Library = LoadLibraryW(GAME_DLL_FILE_NAME);
    if(!WindowsMainDLL.Library)
    {
        Log("LoadLibraryW() failed: 0x%X\n", GetLastError());
        return Result;
    }

    WindowsMainDLL.GameUpdateAndRender = (game_update_and_render *)GetProcAddress(WindowsMainDLL.Library, "GameUpdateAndRender");
    if(!WindowsMainDLL.GameUpdateAndRender)
    {
        Log("GetProcAddress() failed: 0x%X\n", GetLastError());
        return Result;
    }

    platform_memory WindowsMainDLLMemory = {};
    WindowsMainDLLMemory.PermanentMemorySize = Megabytes(256);
    WindowsMainDLLMemory.PermanentMemory = PlatformAllocateMemory(WindowsMainDLLMemory.PermanentMemorySize);
    if(!WindowsMainDLLMemory.PermanentMemory)
    {
        Log("Failed to allocate memory for main dll\n");
        return Result;
    }

    WindowsMainDLLMemory.TemporaryMemorySize = Megabytes(256);
    WindowsMainDLLMemory.TemporaryMemory = PlatformAllocateMemory(WindowsMainDLLMemory.TemporaryMemorySize);
    if(!WindowsMainDLLMemory.TemporaryMemory)
    {
        Log("Failed to allocate memory for main dll\n");
        return Result;
    }

    windows_video WindowsVideo = {};
    platform_video *VideoState = &WindowsVideo.State;
    VideoState->Width = MAIN_WINDOW_WIDTH;
    VideoState->Height = MAIN_WINDOW_HEIGHT;
    VideoState->BytesPerPixel = 4;
    VideoState->Pitch = VideoState->Width * VideoState->BytesPerPixel;
    WindowsVideo.Info.bmiHeader.biSize = sizeof(WindowsVideo.Info.bmiHeader);
    WindowsVideo.Info.bmiHeader.biWidth = VideoState->Width;
    WindowsVideo.Info.bmiHeader.biHeight = VideoState->Height;
    WindowsVideo.Info.bmiHeader.biPlanes = 1;
    WindowsVideo.Info.bmiHeader.biBitCount = (WORD)(VideoState->BytesPerPixel * 8);
    WindowsVideo.Info.bmiHeader.biCompression = BI_RGB;
    VideoState->Memory = PlatformAllocateMemory(VideoState->Pitch * VideoState->Height);
    if(!VideoState->Memory)
    {
        Log("Failed to allocate memory for video output\n");
        return Result;
    }

    WNDCLASSEXW WindowClass = {};
    WindowClass.cbSize = sizeof(WNDCLASSEX);
    WindowClass.style  = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    WindowClass.lpfnWndProc = WindowsMainWindowCallback;
    WindowClass.hInstance = Instance;    
    WindowClass.hCursor = LoadCursorA(0, IDC_ARROW);
    WindowClass.lpszClassName = L"Game Engine";
    
    if(!WindowClass.lpszClassName)
    {
        Log("Failed to construct WindowClass.lpszClassName as a UTF-16 string\n");
        return Result;
    }

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

    RECT WindowRectangle = {};
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
    platform_input Input[2] = {}; 
    platform_input *OldInput = &Input[0];
    platform_input *NewInput = &Input[1];
    while(IsRunning)
    {
        MSG Message = {};
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

        POINT CursorPosition = {};
        if(GetCursorPos(&CursorPosition) != 0)
        {
            NewInput->Controller[0].MouseX = (CursorPosition.x < 0) ? (0) : (CursorPosition.x);
            NewInput->Controller[0].MouseY = (CursorPosition.y < 0) ? (0) : (CursorPosition.y);
        }

        if(WindowsMainDLL.GameUpdateAndRender)
        {
            platform_memory MainDLLMemory = WindowsMainDLLMemory;
            platform_video MainDLLVideo = WindowsVideo.State;        
            WindowsMainDLL.GameUpdateAndRender(&MainDLLMemory, &MainDLLVideo, NewInput);
        }

        window_dimension WindowDimension = {};
        if(WindowsGetWindowDimension(Window, &WindowDimension))
        {
            StretchDIBits(DeviceContext, 0, 0, WindowDimension.Width, WindowDimension.Height, 0, 0, WindowsVideo.State.Width, WindowsVideo.State.Height, WindowsVideo.State.Memory, &WindowsVideo.Info, DIB_RGB_COLORS, SRCCOPY);
        }

        for(int Index = 0; Index < ArrayCount(OldInput->Controller[0].Button); Index++)        
        {
            OldInput->Controller[0].Button[Index].IsDown = NewInput->Controller[0].Button[Index].IsDown;
        }

        platform_input *TemporaryInput = OldInput;
        OldInput = NewInput;
        NewInput = TemporaryInput;
    }
    
    Result = 0;
    return Result;
}