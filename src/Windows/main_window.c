#include "main_window.h"
#include <windows.h>
#include <windowsx.h>   // GET_X_LPARAM / GET_Y_LPARAM
#include "settings.h"

// Размеры окна и верхней полосы — в одном месте, чтобы не искать их по коду.
#define WIN_W      340
#define WIN_H      560
#define TITLE_H     37   // высота заголовка: строки y от 0 до 36
#define BTN_W       44
#define BTN_H       32
#define BTN_Y        3

// Объявление оконной процедуры. static — видна только в этом файле.
static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// Залить прямоугольник одним цветом и сразу удалить кисть, чтобы она не утекла.
static void fill_rect(HDC hdc, int left, int top, int right, int bottom, COLORREF color)
{
    RECT   rc;
    HBRUSH brush;

    rc.left   = left;
    rc.top    = top;
    rc.right  = right;
    rc.bottom = bottom;

    brush = CreateSolidBrush(color);
    FillRect(hdc, &rc, brush);
    DeleteObject(brush);            // кисть больше не нужна
}

HWND main_window_create(HINSTANCE hInstance, int nCmdShow)
{
    // Все переменные — в начале функции, как в классическом C.
    const wchar_t CLASS_NAME[] = L"Chord alfa";
    WNDCLASS wc = {0};   // {0} — заполнить всю структуру нулями
    HWND     hwnd;
    int      x, y;

    // 1. Регистрируем «класс окна» — шаблон, по которому создаются окна.
    wc.lpfnWndProc   = WindowProc;   // кто будет получать сообщения
    wc.hInstance     = hInstance;    // наша программа
    wc.lpszClassName = CLASS_NAME;   // имя шаблона

    if (!RegisterClass(&wc))
        return NULL;

    // 2. Считаем позицию сами: у WS_POPUP окна CW_USEDEFAULT даёт левый верхний угол.
    //    (ширина экрана - ширина окна) / 2 — это и есть центр.
    x = (GetSystemMetrics(SM_CXSCREEN) - WIN_W) / 2;
    y = (GetSystemMetrics(SM_CYSCREEN) - WIN_H) / 2;

    // 3. Создаём окно по этому шаблону.
    hwnd = CreateWindowEx(
        0,                              // дополнительные стили — не нужны
        CLASS_NAME,                     // по какому шаблону
        L"Chord",                       // заголовок окна
        WS_POPUP | WS_SYSMENU | WS_MINIMIZEBOX,   // без рамки и заголовка Windows
        x,                              // позицию посчитали выше — по центру экрана
        y, WIN_W, WIN_H,                // размер окна
        NULL,                           // родительского окна нет
        NULL,                           // меню нет
        hInstance,                      // наша программа
        NULL                            // дополнительных данных нет
        );

    if (hwnd == NULL)
        return NULL;

    // 4. Кнопки в правом верхнем углу, слева направо: настройки, свернуть, закрыть.
    settings_create_button(hwnd);       // «...», id 1, на 197,3

    // Кнопка «свернуть», id 2.
    CreateWindowEx(0, L"BUTTON", L"–",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        245, BTN_Y, BTN_W, BTN_H,
        hwnd, (HMENU)2, hInstance, NULL);

    // Кнопка «закрыть», id 3.
    CreateWindowEx(0, L"BUTTON", L"X",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        293, BTN_Y, BTN_W, BTN_H,
        hwnd, (HMENU)3, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);         // показать окно
    return hwnd;
}

// Оконная процедура: сюда Windows присылает все сообщения для окна.
static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:           // окно закрывают
        PostQuitMessage(0);    // просим цикл сообщений завершиться
        return 0;

    case WM_COMMAND:           // нажали одну из наших кнопок
        // В младшем слове wParam лежит id кнопки, который мы задали при создании.
        if (LOWORD(wParam) == 2) ShowWindow(hwnd, SW_MINIMIZE);
        if (LOWORD(wParam) == 3) DestroyWindow(hwnd);
        return 0;

    case WM_NCHITTEST:         // Windows спрашивает: что находится под курсором?
    {
        POINT pt;

        // В lParam лежат координаты курсора на экране — переводим их в координаты окна.
        pt.x = GET_X_LPARAM(lParam);
        pt.y = GET_Y_LPARAM(lParam);
        ScreenToClient(hwnd, &pt);

        // Для верхней полосы отвечаем «это заголовок» — за него окно можно таскать.
        if (pt.y < TITLE_H)
            return HTCAPTION;

        return HTCLIENT;       // остальное — обычная клиентская часть
    }

    case WM_PAINT:             // окно нужно перерисовать
    {
        PAINTSTRUCT ps;
        RECT        rc;
        HDC         hdc;
        int         w, h;

        hdc = BeginPaint(hwnd, &ps);
        GetClientRect(hwnd, &rc);
        w = rc.right;
        h = rc.bottom;

        // Всё рисование — строго между BeginPaint и EndPaint.
        // Сначала заливаем окно целиком цветом рамки, потом поверх — всё остальное
        // с отступом 1 px. Так по краю сама собой остаётся рамка толщиной 1 px.
        fill_rect(hdc, 0, 0, w, h, RGB(0x2A, 0x2E, 0x33));                 // рамка
        fill_rect(hdc, 1, 1, w - 1, TITLE_H, RGB(0x12, 0x14, 0x17));       // заголовок, y 1..36
        fill_rect(hdc, 1, TITLE_H, w - 1, TITLE_H + 1,
                  RGB(0x24, 0x28, 0x2D));                                  // линия под ним, y 37
        fill_rect(hdc, 1, TITLE_H + 1, w - 1, h - 1,
                  RGB(0x16, 0x18, 0x1B));                                  // всё остальное

        EndPaint(hwnd, &ps);
        return 0;
    }
    }                          // конец switch

    // Всё, что мы не обработали сами, Windows сделает по умолчанию.
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
