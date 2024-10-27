#include "cave_wizard.h"

int status(const int action)
{
    static int state = MOVE;

    if (action == GET) return state;
    else state = action;

    return state;
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