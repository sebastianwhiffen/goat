#define UNICODE
#include <windows.h>

#define internal static
#define local_persist static
#define global_variable static

global_variable bool _isRunning = false;
global_variable BITMAPINFO _bitmapInfo;
global_variable void *_bitmapMemory;
global_variable HBITMAP _bitmapHandle;
global_variable HDC _bitmapDeviceContext;

internal void Win32ResizeDIBSection(int width, int height) {

    if (_bitmapHandle) {
        DeleteObject(_bitmapHandle);
    }
    if (!_bitmapDeviceContext) {
        _bitmapDeviceContext = CreateCompatibleDC(0);
    }

    _bitmapInfo.bmiHeader.biSize = sizeof(_bitmapInfo.bmiHeader);
    _bitmapInfo.bmiHeader.biWidth = width;
    _bitmapInfo.bmiHeader.biHeight = height;
    _bitmapInfo.bmiHeader.biPlanes = 1;
    _bitmapInfo.bmiHeader.biBitCount = 32;
    _bitmapInfo.bmiHeader.biCompression = BI_RGB;

    HDC deviceContext = CreateCompatibleDC(0);

    _bitmapHandle = CreateDIBSection(deviceContext, &_bitmapInfo, DIB_RGB_COLORS, &_bitmapMemory, 0, 0);
}

internal void Win32UpdateWindow(HDC deviceContext, int x, int y, int width, int height) {

    StretchDIBits(deviceContext, x, y, width, height, x, y, width, height, _bitmapMemory, &_bitmapInfo, DIB_RGB_COLORS,
                  SRCCOPY);
}

LRESULT CALLBACK Win32MainWindowCallback(HWND window, UINT msg, WPARAM wParam, LPARAM lParam) {
    LRESULT result = 0;
    switch (msg) {
        case WM_SIZE: {
            RECT clientRect;
            GetClientRect(window, &clientRect);
            int width = clientRect.right - clientRect.left;
            int height = clientRect.bottom - clientRect.top;

            Win32ResizeDIBSection(width, height);
        } break;
        case WM_DESTROY: {
            _isRunning = false;
            OutputDebugStringA("WM_DESTROY\n");
        } break;
        case WM_CLOSE: {
            _isRunning = false;
            PostQuitMessage(0);
            OutputDebugStringA("WM_CLOSE\n");
        } break;
        case WM_ACTIVATEAPP: {
            OutputDebugStringA("WM_ACTIVATEAPP\n");
        } break;
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC deviceContext = BeginPaint(window, &ps);
            int x = ps.rcPaint.left;
            int y = ps.rcPaint.top;
            int height = ps.rcPaint.bottom - ps.rcPaint.top;
            int width = ps.rcPaint.right - ps.rcPaint.left;

            Win32UpdateWindow(deviceContext, x, y, width, height);

            EndPaint(window, &ps);
        } break;
        default: {
            result = DefWindowProc(window, msg, wParam, lParam);
        } break;
    }
    return result;
}

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE prevInstance, PWSTR cmdLine, int cmdShow) {
    WNDCLASS windowClass = {};
    // style is a set of binary flags for window props
    windowClass.style = CS_OWNDC | CS_HREDRAW;
    // ptr to function that defines how window responds to events
    windowClass.lpfnWndProc = Win32MainWindowCallback;
    // handles instance that contains the window procedure for the class???
    windowClass.hInstance = instance;
    // window icon
    // WindowClass.hIcon = ;
    // little menu like the file edit selection things at the top left
    // WindowClass.lpszMenuName = ;
    windowClass.lpszClassName = L"GoatWindowClass";

    if (RegisterClass(&windowClass)) {
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
        if (windowHandle) {
            MSG msg;
            _isRunning = true;
            while (_isRunning) {

                BOOL res = GetMessage(&msg, 0, 0, 0);
                if (res > 0) {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                } else {
                    break;
                }
            }
        } else {
            OutputDebugStringA("it failed :(");
        }
    } else {
        OutputDebugStringA("it failed :(");
    }

    return 0;
};