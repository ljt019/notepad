#include <windows.h>
#include <stdlib.h>

// Define the maximum text size
#define MAX_TEXT 1024

// Create a Global Buffer to store the text
char g_text[MAX_TEXT] = "";
int g_text_length = 0;

// Function to convert VK code to character considering the keyboard state
char VKCodeToChar(WPARAM wParam, LPARAM lParam) {
    BYTE keyboardState[256];
    WCHAR unicodeChar[5];
    int result;

    // Get the current keyboard state
    if (!GetKeyboardState(keyboardState)) {
        return '\0';
    }

    // Handle extended keys
    UINT scanCode = (lParam >> 16) & 0xFF;
    // Check if the key is an extended key
    if (lParam & (1 << 24)) {
        scanCode |= 0x100;
    }

    // Convert virtual key code to Unicode character
    result = ToUnicode((UINT)wParam, scanCode, keyboardState, unicodeChar, 4, 0);
    if (result > 0) {
        // Null-terminate the string
        unicodeChar[result] = '\0';
        // Convert WCHAR to char (assuming ASCII for simplicity)
        return (char)unicodeChar[0];
    }

    return '\0';
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_DESTROY: {
            PostQuitMessage(0);
            return 0;
        }
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            FillRect(hdc, &ps.rcPaint, (HBRUSH) GetStockObject(BLACK_BRUSH));

            // Set text properties
            SetTextColor(hdc, RGB(255, 255, 255));
            SetBkColor(hdc, RGB(0, 0, 0));

            // Draw the text 
            TextOutA(hdc, 10, 10, g_text, g_text_length);

            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_KEYDOWN: {
            switch (wParam) {
                case VK_BACK: {
                    if (g_text_length > 0) {
                        g_text_length--;
                        g_text[g_text_length] = '\0';
                        InvalidateRect(hwnd, NULL, TRUE);
                    }
                    return 0;
                } 
                case VK_SPACE: {
                    if (g_text_length < MAX_TEXT - 1) {
                        g_text[g_text_length] = ' ';
                        g_text_length++;
                        g_text[g_text_length] = '\0';
                        InvalidateRect(hwnd, NULL, TRUE);
                    }
                    return 0;
                }
                default: {
                    // Filter out non-character keys
                    if (wParam == VK_SHIFT || wParam == VK_CONTROL || wParam == VK_MENU) {
                        return 0;
                    }

                    char c = VKCodeToChar(wParam, lParam);
                    if (c != '\0') {
                        if (g_text_length < MAX_TEXT - 1) {
                            g_text[g_text_length] = c;
                            g_text_length++;
                            g_text[g_text_length] = '\0';
                            InvalidateRect(hwnd, NULL, TRUE);
                        }
                    }
                    return 0;
                }
            }
        }
        default: {
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }
}

// hInstance : instance handle
// hPrevInstance : previous instance handle
// lpCmdLine : command line arguments
// nCmdShow : how the window is to be shown
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Register the window class
    const char basic_window[] = "Sample Window Class";

    WNDCLASSA windows_class = {0};

    windows_class.lpfnWndProc = WindowProc; // Windows Procedure callback function
    windows_class.hInstance = hInstance; // handle for application instance
    windows_class.lpszClassName = basic_window;
    windows_class.hCursor = LoadCursor(NULL, IDC_ARROW);
    windows_class.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);

    if (!RegisterClassA(&windows_class)) {
        return 0;
    }

    // Create the window
    HWND hwnd = CreateWindowExA(
        0,                   // Optional window styles
        basic_window,        // Window class
        "Text Editor",       // Window text
        WS_OVERLAPPEDWINDOW, // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,

        NULL,      // Parent window
        NULL,      // Menu
        hInstance, // Instance handle
        NULL       // Additional application data
    );

    // If window creation failed, return 0
    if (hwnd == NULL) {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    // Message loop
    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
