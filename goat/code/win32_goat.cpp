#define UNICODE
#include <windows.h>

LRESULT CALLBACK MainWindowCallback(HWND window, UINT msg, WPARAM wParam, LPARAM lParam)
{
    LRESULT result = 0;
    switch (msg)
    {
    case WM_SIZE:
    {
        OutputDebugStringA("WM_SIZE\n");
    }
    break;
    case WM_DESTROY:
    {
        OutputDebugStringA("WM_DESTROY\n");
    }
    break;
    case WM_CLOSE:
    {
        OutputDebugStringA("WM_CLOSE\n");
    }
    break;
    case WM_ACTIVATEAPP:
    {
        OutputDebugStringA("WM_ACTIVATEAPP\n");
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC deviceContext = BeginPaint(window, &ps);
        int x = ps.rcPaint.left;
        int y = ps.rcPaint.top;
        int height = ps.rcPaint.bottom - ps.rcPaint.top;
        int width = ps.rcPaint.right - ps.rcPaint.left;
        static DWORD operation = WHITENESS;

        PatBlt(deviceContext, x, y, height, width, operation);
        
        if (operation == WHITENESS)
        {
            operation = BLACKNESS;
        }
        else
        {
            operation = WHITENESS;
        }
        EndPaint(window, &ps);
    }
    break;
    default:
    {
        result = DefWindowProc(window, msg, wParam, lParam);
    }
    break;
    }
    return result;
}

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE prevInstance, PWSTR cmdLine, int cmdShow)
{
    WNDCLASS windowClass = {};
    // style is a set of binary flags for window props
    windowClass.style = CS_OWNDC | CS_HREDRAW;
    // ptr to function that defines how window responds to events
    windowClass.lpfnWndProc = MainWindowCallback;
    // handles instance that contains the window procedure for the class???
    windowClass.hInstance = instance;
    // window icon
    // WindowClass.hIcon = ;
    // little menu like the file edit selection things at the top left
    // WindowClass.lpszMenuName = ;
    windowClass.lpszClassName = L"GoatWindowClass";

    if (RegisterClass(&windowClass))
    {
        HWND windowHandle = CreateWindowExW(
            // window style bitflags
            0,
            // classname
            windowClass.lpszClassName,
            // window name
            L"goat",
            // window style
            WS_OVERLAPPEDWINDOW | WS_VISIBLE,
            // window location props
            // x
            CW_USEDEFAULT,
            // y
            CW_USEDEFAULT,
            // nWidth
            CW_USEDEFAULT,
            // nHeight
            CW_USEDEFAULT,
            // parent
            0,
            // menu
            0,
            // instance
            instance,
            // window param, to be used with MainWindowCallback on create
            0);
        if (windowHandle)
        {
            MSG msg;
            for (;;)
            {

                BOOL res = GetMessage(&msg, 0, 0, 0);
                if (res > 0)
                {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }
                else
                {
                    break;
                }
            }
        }
        else
        {
            OutputDebugStringA("it failed :(");
        }
    }
    else
    {
        OutputDebugStringA("it failed :(");
    }

    return 0;
};