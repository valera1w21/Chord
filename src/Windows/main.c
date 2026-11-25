#include "main_window.h"

#pragma comment(lib, "user32.lib")   // окна и сообщения
#pragma comment(lib, "gdi32.lib")    // рисование

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PWSTR pCmdLine, int nCmdShow)
{
    MSG msg = {0};

    (void)hPrevInstance;
    (void)pCmdLine;

    if (main_window_create(hInstance, nCmdShow) == NULL)
        return 0;

    // Цикл сообщений: берём сообщение из очереди и отдаём окну.
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}