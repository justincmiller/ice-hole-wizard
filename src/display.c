/*
*  Credit is given to Dr. Larry Hughes for providing the reference code
*  that was used throughout this program. Specifically, the engine.h, vterminal.h, and vterminal.c files
*  which uses much of Dr. Hughes' work.
*/

#include "display.h"

//static global structure to limit scope
static Display dsp;
static int state;

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
    state = status();
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
    state = status();
    HIDE_CURSOR;

    if (state & MODIFY)
    {
        ALT_SCREEN;
        overlay();
    }
    else
    {
        MAIN_SCREEN;
        viewport();
    }

    if (state & MOVE) SHOW_CURSOR;
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
    COORD cursor = dsp.cursor;

    CLEAR;
    RESET;

    int rows = CLAMP_Y(dsp.size.Y + offset.Y);
    int cols = CLAMP_X(dsp.size.X + offset.X);

    for (int i = offset.Y; i < rows; i++)
    {
        for (int j = offset.X; j < cols; j++)
        {
            if (grid[i][j] != LATENT)
            {
                setCursor(j, i);
                if (i == cursor.Y && j == cursor.X)
                {
                    printf(ACTIVE("%c"), grid[i][j]);
                }
                else
                {
                    printf(INACTIVE("%c"), grid[i][j]);
                }
            }
        }
    }

    statusBar();
    setCursor(cursor.X, cursor.Y);
}

void overlay()
{
    //render underlying grid
    viewport();

    /******** draw overlay container ********/
    RESET;

    //initialize string of horizontal lines
    char border[BORDER_COLS] = {0};
    memset(border, 0x71, sizeof(border)-1);

    //print top border with corners
    printf(LDM("%c%s%c\n"), 0x6c, border, 0x6b);

    //print left and right borders
    for (int i = 0; i < BORDER_ROWS; i++)
    {
        printf(LDM("%c%29c\n"), 0x78, 0x78);
    }

    //print bottom border with corners
    printf(LDM("%c%s%c\n"), 0x6d, border, 0x6a);
}

void statusBar()
{
    state = status();
    HIDE_CURSOR;

    int x = COL_X(dsp.cursor.X) * 10;
    int y = ROW_Y(dsp.cursor.Y) * 10;
    int z = dsp.map->layer->depth;

    CUP(PAD_LEFT, dsp.size.Y);
    printf("x, y, z: (%d, %d, %d) (m) %12c", x, y, z, LATENT);

    updateCursor();

    if (state & MOVE) SHOW_CURSOR;
}