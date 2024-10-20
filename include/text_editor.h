// text_editor.h
#ifndef TEXT_EDITOR_H
#define TEXT_EDITOR_H

#include <windows.h>

// Maximum text size
#define MAX_TEXT 1024

// Initialize text editor data
void InitializeTextEditor(void);

// Cleanup text editor data
void CleanupTextEditor(void);

// Open a file
BOOL OpenTextFile(HWND hwnd);

// Save a file
BOOL SaveTextFile(HWND hwnd);

// Window Procedure for the text editor
LRESULT CALLBACK TextEditorProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// Register the window class
BOOL RegisterWindowClass(HINSTANCE hInstance, const char *className);

// Create the main window
HWND CreateMainWindow(HINSTANCE hInstance, const char *className, const char *windowTitle, int width, int height);

#endif // TEXT_EDITOR_H