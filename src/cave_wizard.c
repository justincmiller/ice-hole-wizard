#include "cave_wizard.h"

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
            return;
        default:
            return;
    }
}

int getState()
{
    int state = 0;
    wizard(GET_STATE, &state);
    return state;
}

void setState(const int state)
{
    wizard(SET_STATE, &state);
}

void toggleState()
{
    if (getState() == MOVE)
    {
        setState(DRAW);
    }
    else if (getState() == DRAW)
    {
        setState(MOVE);
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
                if (getState() == MOVE) move(key);
                else if (getState() == DRAW) draw(key);
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

void move(const char key)
{
    switch (key)
    {
        case ARROW_UP:
            CUU(1);
            break;
        case ARROW_DOWN:
            CUD(1);
            break;
        case ARROW_RIGHT:
            CUF(1);
            break;
        case ARROW_LEFT:
            CUB(1);
            break;
        case HOME:
            
            break;
        case END:
            setState(QUIT);
            return;
        default:
            break;
    }
}

void draw(const char key)
{
    switch (key)
    {
        case ARROW_UP:
            break;
        case ARROW_DOWN:
            break;
        case ARROW_RIGHT:
            break;
        case ARROW_LEFT:
            break;
        case HOME:
            break;
        case END:
            break;
        default:
            break;
    }
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
            if (_getch() == '~') toggleState();
            break;
        case DEL:
            if (_getch() == '~')
                //erase character
            break;
        case PG_UP:
            if (_getch() == '~')
                //save buffer and move up
            break;
        case PG_DN:
            if (_getch() == '~')
                //save buffer and move down
            break;
        default:
            break;
    }
}