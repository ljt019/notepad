// dwm_utils.c
#include "dwm_utils.h"

// Define DWM Attributes manually if not defined
#ifndef DWMWA_CAPTION_COLOR
#define DWMWA_CAPTION_COLOR 35
#endif

#ifndef DWMWA_USE_IMMERSIVE_DARK_MODE
#define DWMWA_USE_IMMERSIVE_DARK_MODE 20
#endif

BOOL SetWindowAttributes(HWND hwnd)
{
    // Set the title bar color to dark grey
    COLORREF darkGreyColor = RGB(64, 64, 64);
    HRESULT hr = DwmSetWindowAttribute(hwnd, DWMWA_CAPTION_COLOR, &darkGreyColor, sizeof(darkGreyColor));
    if (FAILED(hr))
    {
        // Handle error (optional)
    }

    // Enable dark mode for the window
    BOOL useDarkMode = TRUE;
    hr = DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &useDarkMode, sizeof(useDarkMode));
    if (FAILED(hr))
    {
        // Handle error (optional)
    }

    return SUCCEEDED(hr);
}
