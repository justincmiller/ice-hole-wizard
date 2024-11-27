/*
*  Credit is given to Dr. Larry Hughes for providing the reference code
*  that was used both as inspiration and used as is throughout this program.
*/

#include "display.h"
#include "engine.h"

//static global structure (limited scope)
static Display dsp;

//loads structure that holds all the system information
void loadDisplay(Display** ptr)
{
    //update display pointer
    *ptr = &dsp;
}

SMALL_RECT getWindow()
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    if (out == INVALID_HANDLE_VALUE)
    {
        printf("Error: could not fetch output handle\n");
    }

    CONSOLE_SCREEN_BUFFER_INFO info;
    if (!GetConsoleScreenBufferInfo(out, &info))
    {
        printf(">> Error: unable to get console screen buffer info.\n");
    }

    return info.srWindow;
}

void setWindow()
{
    SMALL_RECT window = getWindow();

    dsp.size.X = window.Right - window.Left + 1;
    dsp.size.Y = window.Bottom - window.Top + 1;

    resetMargins();
}

void resetMargins()
{
    /*CLAMP macros use max and min functions clamp margins to grid 
    bounds (0, 99). if the window exceeds the width of the grid, it 
    will be aligned top left.*/
    dsp.margin.Left = CLAMP_X(dsp.cursor->X - dsp.size.X / 2);
    dsp.margin.Top =  CLAMP_Y(dsp.cursor->Y - dsp.size.Y / 2);
    dsp.margin.Right = CLAMP_X(dsp.margin.Left + dsp.size.X);
    dsp.margin.Bottom = CLAMP_Y(dsp.margin.Top + dsp.size.Y);

    //render window with new margins
    render();
}

void render()
{
    HIDE_CURSOR;

    if (dsp.state & EDIT)
    {
        ALT_SCREEN;
        overlay();
    }
    else
    {
        MAIN_SCREEN;
        CLEAR;
        viewport();
    }
}

void pollWindow()
{
    SMALL_RECT dim = getWindow();

    int width = dim.Right - dim.Left + 1;
    int height = dim.Bottom - dim.Top + 1;

    int dx = dsp.size.X - width;
    int dy = dsp.size.Y - height;

    //check if either dx or dy are nonzero
    if (dx || dy)
    {
        //set new dimensions
        dsp.size.X = width;
        dsp.size.Y = height;
        //recenter viewport on cursor
        resetMargins();
    }
}

void viewport()
{
    char** grid = dsp.map->layer->grid;

    if (grid == NULL) return;

    COORD offset = {dsp.margin.Left, dsp.margin.Top};

    RESET;

    int rows = CLAMP_Y(dsp.size.Y + offset.Y);
    int cols = CLAMP_X(dsp.size.X + offset.X);

    for (int i = offset.Y; i < rows; i++)
    {
        for (int j = offset.X; j < cols; j++)
        {
            if (grid[i][j] == PLUS)
            {
                relCursor(j, i);
                printf(PORT_UP("%c"), grid[i][j]);
            }
            else if (grid[i][j] == MINUS)
            {
                relCursor(j, i);
                printf(PORT_DN("%c"), grid[i][j]);
            }
            else if (grid[i][j] != LATENT)
            {
                relCursor(j, i);
                printf(PATH("%c"), grid[i][j]);
            }
        }
    }

    updateCursor();
    statusBar();
}