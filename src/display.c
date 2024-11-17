/*
*  Credit is given to Dr. Larry Hughes for providing the reference code
*  that was used throughout this program. Specifically, the engine.h, vterminal.h, and vterminal.c files
*  which uses much of Dr. Hughes' work.
*/

#include "display.h"

//static global structure to limit scope
static Display dsp;

Display* getDisplay()
{
    return &dsp;
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

void resetMargins()
{
    /*CLAMP macros use max and min functions clamp margins to grid bounds (0, 99). if the window
    exceeds the width of the grid, it will be aligned top left.*/
    dsp.margin.Left = CLAMP_X(dsp.cursor.X - dsp.size.X / 2);
    dsp.margin.Top =  CLAMP_Y(dsp.cursor.Y - dsp.size.Y / 2);
    dsp.margin.Right = CLAMP_X(dsp.margin.Left + dsp.size.X);
    dsp.margin.Bottom = CLAMP_Y(dsp.margin.Top + dsp.size.Y);

    //render window with new margins
    render();
}

void initDisplay()
{
    initMap();
    dsp.map = getMap();
    initCursor();
    SMALL_RECT dim = getWindow();
    dsp.size.X = dim.Right - dim.Left + 1;
    dsp.size.Y = dim.Bottom - dim.Top + 1;
    setCursor(X_COL(1), Y_ROW(1));
    resetMargins();
}

void render()
{
    //fetch grid from active layer
    char** grid = dsp.map->layer->grid;

    if (grid == NULL) return;

    /***** initialize buffer variables *****/
    int offsetX = dsp.margin.Left;
    int offsetY = dsp.margin.Top;
    int size = dsp.size.X * dsp.size.Y;
    char* row = NULL;

    char* buffer = malloc(size * sizeof(char));
    ASSERT(buffer);

    //initialize buffer with spaces
    memset(buffer, SPACE, size);

    //clamp copy length to the end of the visible portion of the grid
    int cols = CLAMP_X(dsp.size.X);
    int rows = CLAMP_Y(dsp.size.Y);

    //start copying at top margin
    for (int i = 0; i < rows; i++)
    {
        row = buffer + i * dsp.size.X;
        if (offsetY + i < MAP_ROWS)
        {
            memcpy(row, &grid[offsetY + i][offsetX], cols);
        }
    }

    //move cursor to 1,1 for printing
    CLEAR;
    printf(CSI "H");

    int cursor = (dsp.cursor.Y - offsetY) * dsp.size.X + (dsp.cursor.X - offsetX);

    for (int i = 0; i < size; i++)
    {
        if (i == cursor && buffer[i] != SPACE)
        {
            printf(ACTIVE_LINE, buffer[i]);
        }
        else if (buffer[i] != SPACE)
        {
            printf(INACTIVE_LINE, buffer[i]);
        }
        else
        {
            _putch(buffer[i]);
        }
    }

    //return cursor to position on grid
    updateCursor();
    free(buffer);
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