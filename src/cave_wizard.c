#include "cave_wizard.h"

void init()
{

    HANDLE input = GetStdHandle(STD_INPUT_HANDLE);
    if (input == INVALID_HANDLE_VALUE)
    {
        printf(">> Error: unable to get input handle.\n");
        setState(QUIT);
        return;
    }

    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    if (output == INVALID_HANDLE_VALUE)
    {
        printf(">> Error: unable to get output handle.\n");
        setState(QUIT);
        return;
    }    

    DWORD inmode, outmode;

    if (!(GetConsoleMode(input, &inmode)))
    {
        printf(">> Error: unable to get input mode.\n");
        setState(QUIT);
        return;
    }

    if (!(GetConsoleMode(output, &outmode)))
    {
        printf(">> Error: unable to get output mode.\n");
        setState(QUIT);
        return;
    }

    outmode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!(SetConsoleMode(output, outmode)))
    {
        printf(">> Error: unable to set output mode.\n");
        setState(QUIT);
        return;
    }

    inmode |= ENABLE_VIRTUAL_TERMINAL_INPUT;
    if (!(SetConsoleMode(input, inmode)))
    {
        printf(">> Error: unable to set input mode.\n");
        setState(QUIT);
        return;
    }

    CONSOLE_SCREEN_BUFFER_INFO info;
    COORD buffer = {100, 100};
    SMALL_RECT window;
    int width, height;

    if (!(GetConsoleScreenBufferInfo(output, &info)))
    {
        printf(">> Error: unable to get console screen buffer info.\n");
        setState(QUIT);
        return;
    }

    width = info.srWindow.Right - info.srWindow.Left + 1;
    height = info.srWindow.Bottom - info.srWindow.Top + 1;

    if (buffer.X < width || buffer.Y < height)
    {
        buffer.X = width;
        buffer.Y = height;
    }

    window.Left = 0;
    window.Top = 0;
    window.Right = width - 1;
    window.Bottom = height - 1;

    if (!(SetConsoleWindowInfo(output, TRUE, &window)))
    {
        printf(">> Error: unable to set console window size. Error code: %lu\n", GetLastError());
        setState(QUIT);
        return;
    }

    if (!(SetConsoleScreenBufferSize(output, buffer)))
    {
        printf(">> Error: unable to set console buffer size. Error code: %lu\n", GetLastError());
        setState(QUIT);
        return;
    }

    CUP(GET_ROW(1), GET_COL(1));
    setState(MOVE);
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

/*
void render()
{

}*/

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
            //move cursor to position 1,1
            CUP(GET_COL(1), GET_ROW(1));
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