// text_editor.c
#include "text_editor.h"
#include <stdlib.h>
#include <windows.h>

// Global buffer to store the text
static char g_text[MAX_TEXT] = "";
static int g_text_length = 0;

// Function to convert VK code to character considering the keyboard state
static char VKCodeToChar(WPARAM wParam, LPARAM lParam)
{
    BYTE keyboardState[256];
    WCHAR unicodeChar[5];
    int result;

    // Get the current keyboard state
    if (!GetKeyboardState(keyboardState))
    {
        return '\0';
    }

    // Handle extended keys
    UINT scanCode = (lParam >> 16) & 0xFF;
    if (lParam & (1 << 24))
    {
        scanCode |= 0x100;
    }

    // Convert virtual key code to Unicode character
    result = ToUnicode((UINT)wParam, scanCode, keyboardState, unicodeChar, 4, 0);
    if (result > 0)
    {
        unicodeChar[result] = '\0';
        return (char)unicodeChar[0];
    }

    return '\0';
}

void InitializeTextEditor(void)
{
    g_text[0] = '\0';
    g_text_length = 0;
}

void CleanupTextEditor(void)
{
    // Currently, no dynamic resources to clean up
}

// Window Procedure
LRESULT CALLBACK TextEditorProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // Fill background
        FillRect(hdc, &ps.rcPaint, (HBRUSH)GetStockObject(BLACK_BRUSH));

        // Set text properties
        SetTextColor(hdc, RGB(255, 255, 255));
        SetBkColor(hdc, RGB(0, 0, 0));

        // Draw the text
        RECT rect;
        GetClientRect(hwnd, &rect);
        DrawTextA(hdc, g_text, -1, &rect, DT_LEFT | DT_TOP | DT_WORDBREAK);

        EndPaint(hwnd, &ps);
        return 0;
    }

    case WM_KEYDOWN:
    {
        BOOL textChanged = FALSE;

        switch (wParam)
        {
        case VK_BACK:
            if (g_text_length > 0)
            {
                g_text_length--;
                g_text[g_text_length] = '\0';
                textChanged = TRUE;
            }
            break;

        case VK_SPACE:
            if (g_text_length < MAX_TEXT - 1)
            {
                g_text[g_text_length++] = ' ';
                g_text[g_text_length] = '\0';
                textChanged = TRUE;
            }
            break;

        case VK_RETURN:
            if (g_text_length < MAX_TEXT - 1)
            {
                g_text[g_text_length++] = '\n';
                g_text[g_text_length] = '\0';
                textChanged = TRUE;
            }
            break;

        default:
            // Filter out non-character keys
            if (!(wParam == VK_SHIFT || wParam == VK_CONTROL || wParam == VK_MENU))
            {
                char c = VKCodeToChar(wParam, lParam);
                if (c != '\0' && g_text_length < MAX_TEXT - 1)
                {
                    g_text[g_text_length++] = c;
                    g_text[g_text_length] = '\0';
                    textChanged = TRUE;
                }
            }
            break;
        }

        if (textChanged)
        {
            InvalidateRect(hwnd, NULL, TRUE);
        }
        return 0;
    }

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

BOOL RegisterWindowClass(HINSTANCE hInstance, const char *className)
{
    WNDCLASSA wc = {0};
    wc.lpfnWndProc = TextEditorProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = className;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    return RegisterClassA(&wc);
}

HWND CreateMainWindow(HINSTANCE hInstance, const char *className, const char *windowTitle, int width, int height)
{
    return CreateWindowExA(
        0,                   // Optional window styles
        className,           // Window class
        windowTitle,         // Window text
        WS_OVERLAPPEDWINDOW, // Window style
        CW_USEDEFAULT, CW_USEDEFAULT, width, height,
        NULL,      // Parent window
        NULL,      // Menu
        hInstance, // Instance handle
        NULL       // Additional application data
    );
}
