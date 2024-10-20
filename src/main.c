#include <stdlib.h>
#include <windows.h>
#include "dwm_utils.h"
#include "text_editor.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    (void)hPrevInstance; // Mark as unused
    (void)lpCmdLine;     // Mark as unused

    // Register window class
    if (!RegisterWindowClass(hInstance, "Sample Window Class"))
    {
        MessageBoxA(NULL, "Failed to register window class", "Error", MB_ICONERROR | MB_OK);
        return 0;
    }

    // Create the main window
    HWND hwnd = CreateMainWindow(hInstance, "Sample Window Class", "Text Editor", 800, 600);
    if (hwnd == NULL)
    {
        MessageBoxA(NULL, "Failed to create window", "Error", MB_ICONERROR | MB_OK);
        return 0;
    }

    // Set window attributes
    if (!SetWindowAttributes(hwnd))
    {
        // Non-fatal: Continue even if setting attributes fails
    }

    ShowWindow(hwnd, nCmdShow);

    // Initialize text editor data
    InitializeTextEditor();

    // Message loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Cleanup
    CleanupTextEditor();
    return (int)msg.wParam;
}
