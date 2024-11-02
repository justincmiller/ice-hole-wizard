#include "cave_wizard.h"

void wizard(const int action, void* data)
{
    static int state;
    static SMALL_RECT viewport;

    switch (action)
    {
        case GET_STATE:
            *(int*)data = state;
            return;
        case SET_STATE:
            state = *(int*)data;
            return;
        case GET_VIEW:
            *(SMALL_RECT*)data = viewport;
            return;
        case SET_VIEW:
            viewport = *(SMALL_RECT*)data;
            break;
        default:
            return;
    }
}

/*
MAY NOT BE NEEDED: Console buffer may handle itself
char pollWindow()
{
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO info;
    SMALL_RECT window;
    int width, height;

    wizard(GET_VIEW, &window);
    width = window.Right - window.Left;
    height = window.Bottom - window.Top;

    GetConsoleScreenBufferInfo(console, &info);
    if (info.srWindow.Right - info.srWindow.Left != width 
    || info.srWindow.Bottom - info.srWindow.Top != height)
    {
        wizard(SET_VIEW, &info.srWindow);
        return 1;
    }

    return 0;
}*/

void update()
{
    if (_kbhit() == 0) return;
    
    char key = (char)_getch();

    //update as necessary for addtional alphanumeric entries
    if (key == 'p')
    {
        //edit cell properties
    }
    else if (key == ESC && (char)_getch() == '[')
    {
        key = (char)_getch();

        switch (key)
        {
            case ARROW_UP:
            case ARROW_DOWN:
            case ARROW_RIGHT:
            case ARROW_LEFT:
            case HOME:
            case END:
                cursorKey(key);
                break;
            default:
                parseKey(key);
                break;
        }
    }
}

void render()
{

}

void cursorKey(const char key)
{

}

void parseKey(const char key)
{
    switch (key)
    {
        case CTRL:
            if (_getch() == ';' && _getch() == '5')
            {
                //pan window
            }
            break;
        case INSERT:
        case DEL:
        case PG_UP:
        case PG_DN:
    }
}