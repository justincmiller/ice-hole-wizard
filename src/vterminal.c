#include "vterminal.h"

void init()
{
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO info;
    COORD buffer = {100, 100};
    SMALL_RECT window;
    int width, height;

    GetConsoleScreenBufferInfo(console, &info);
    width = info.srWindow.Right - info.srWindow.Left + 1;
    height = info.srWindow.Bottom - info.srWindow.Top + 1;
    window.Left = (buffer.X - width)/2;
    window.Top = (buffer.Y - height)/2;
    window.Right = (window.Left + width);
    window.Bottom = (window.Top + height);

    SetConsoleScreenBufferSize(console, buffer);
    SetConsoleWindowInfo(console, TRUE, &window);
}
