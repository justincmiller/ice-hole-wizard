#include "vterminal.h"

void init(Instance* ins)
{
    CONSOLE_SCREEN_BUFFER_INFO info;
    COORD bufferSize = {100, 100};
    SMALL_RECT window;

    GetConsoleScreenBufferInfo(ins->hd, &info);
    ins->width = info.srWindow.Right - info.srWindow.Left + 1;
    ins->height = info.srWindow.Bottom - info.srWindow.Top + 1;

    ins->left = (bufferSize.X - ins->width) / 2;
    ins->top = (bufferSize.Y - ins->height) / 2;

    SetConsoleScreenBufferSize(ins->hd, bufferSize);
}
