#include "windows_main.h"

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

    WNDCLASSEXW WindowClass = {0};
    WindowClass.cbSize = sizeof(WNDCLASSEX);
    WindowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    WindowClass.lpfnWndProc = WindowsMainWindowCallback;
    WindowClass.hInstance = Instance;
    WindowClass.hCursor = LoadCursorA(0, IDC_ARROW);
    WindowClass.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
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