#include "Header/definitions.hpp"
#include "Library/platform.hpp"
#include <windows.h>

#define MAIN_WINDOW_X 0
#define MAIN_WINDOW_Y 0
#define MAIN_WINDOW_WIDTH 1280
#define MAIN_WINDOW_HEIGHT 720

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

    WNDCLASSEXW WindowClass = {};
    WindowClass.cbSize = sizeof(WNDCLASSEX);
    WindowClass.style  = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    WindowClass.lpfnWndProc = WindowsMainWindowCallback;
    WindowClass.hInstance = Instance;    
    WindowClass.hCursor = LoadCursorA(0, IDC_ARROW);
    WindowClass.lpszClassName = ANSIToUTF16("Game Engine");
    if(!WindowClass.lpszClassName)
    {
        Log("Failed to construct WindowClass.lpszClassName as a UTF-16 string\n");;
        return Result;
    }

    if(!RegisterClassExW(&WindowClass))
    {
        Log("RegisterClassExW() failed: 0x%X\n", GetLastError());
        return Result;
    }

    HWND Window = CreateWindowExW(0, WindowClass.lpszClassName, WindowClass.lpszClassName, WS_OVERLAPPEDWINDOW | WS_VISIBLE, MAIN_WINDOW_X, MAIN_WINDOW_Y, MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT, 0, 0, Instance, 0);
    if(!Window)
    {
        Log("CreateWindowExW() failed: 0x%X\n", GetLastError());
        return Result;
    }

    bool IsRunning = 1;
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
    }
    
    Result = 0;
    return Result;
}