/*
*  Credit is given to Dr. Larry Hughes for providing the reference code
*  that was used throughout this program. Specifically, the engine.h, vterminal.h, and vterminal.c files
*  which uses much of Dr. Hughes' work.
*/

#include "engine.h"

//static global variable to limit scope
static Display* dsp;

//initializes the console with screen buffer and window settings
void init()
{
    //enable virtual terminal I/O
    virtualOutput();
    virtualInput();

    //load modules
    loadDisplay(&dsp);
    loadMap(dsp);
    loadCursor(dsp);
    
    //set initial state to movement
    dsp->state = MOVE;
    //set cursor to origin and set window size and margins
    setCursor(X_COL(1), Y_ROW(1));
    setWindow();
}

int status()
{
    return dsp->state;
}

void pollKbInput()
{
    if (!_kbhit()) return;
    
    char buffer[SQLEN] = {0};
    int code = 0;
    int n = 0;

    const char* keys[] =
    {
        "p",        //p key
        "\r",       //enter
        CSI "A",    //up arrow
        CSI "B",    //down arrow
        CSI "C",    //right arrow
        CSI "D",    //left arrow
        CSI "H",    //home
        CSI "F",    //end
        CSI "2~",   //insert
        CSI "3~",   //delete
        CSI "5~",   //page up
        CSI "6~",   //page down
        CSI "1;5A", //ctrl + up arrow
        CSI "1;5B", //ctrl + down arrow
        CSI "1;5C", //ctrl + right arrow
        CSI "1;5D", //ctrl + left arrow
        ESC         //escape
    };

    while (_kbhit() && n < SQLEN - 1)
    {
        buffer[n++] = (char)_getch();
    }

    for (int i = 0; i < CONTROLS; i++)
    {
        if (strcmp(buffer, keys[i]) == 0)
        {
            code = i + 1;
            break;
        }
    }

    if (dsp->state & EDIT)
    {
        editControls(code);
        return;
    }
    else if (dsp->state & MOVE)
    {
        moveControls(code);
        return;
    }
    else if (dsp->state & DRAW)
    {
        drawControls(code);
        return;
    }
}

void moveControls(const int code)
{
    switch (code)
    {
        case ARROW_UP:
        case ARROW_DOWN:
        case ARROW_RIGHT:
        case ARROW_LEFT:
            move(code);
            return;
        case HOME:
            setCursor(X_COL(1), Y_ROW(1));
            resetMargins();
            return;
        case END:
            dsp->state = QUIT;
            return;
        case INSERT:
            dsp->state ^= ( MOVE | DRAW);
            refresh();
            return;
        case DEL:
            ECH(1);
            return;
        case PG_UP:
            return;
        case PG_DN:
            return;
        case CTRL_UP:
        case CTRL_DOWN:
        case CTRL_RIGHT:
        case CTRL_LEFT:
            panViewport(code);
            return;
        case P:
            dsp->state ^= EDIT;
            editCell();
            refresh();
            return;
    }
}

void drawControls(const int code)
{
    switch (code)
    {
        case ARROW_UP:
        case ARROW_DOWN:
        case ARROW_RIGHT:
        case ARROW_LEFT:
            draw(code);
            return;
        case HOME:
            break;
        case END:
            break;
        case INSERT:
            dsp->state ^= ( MOVE | DRAW);
            refresh();
            return;
        case DEL:
            ECH(1);
            break;
        case PG_UP:
            break;
        case PG_DN:
            break;
        case CTRL_UP:
        case CTRL_DOWN:
        case CTRL_RIGHT:
        case CTRL_LEFT:
            panViewport(code);
            return;
        case P:
            dsp->state ^= EDIT;
            editCell();
            refresh();
            break;
    }
}

void editControls(const int code)
{
    switch (code)
    {
        case ARROW_UP:
        case ARROW_DOWN:
            option(code);
            break;
        case ARROW_RIGHT:
        case ARROW_LEFT:
        case HOME:
            break;
        case END:
            break;
        case INSERT:
            break;
        case DEL:
            break;
        case PG_UP:
            break;
        case PG_DN:
            break;
        case P:
            dsp->state ^= EDIT;
            editCell();
            refresh();
            break;
    }
}