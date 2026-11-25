#include "settings.h"

void settings_create_button(HWND parent){ CreateWindowEx(
        0,
        L"BUTTON",
        L"...",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        197, 3,
        44, 32,
        parent,
        (HMENU)1,
        GetModuleHandle(NULL),
        NULL);
};