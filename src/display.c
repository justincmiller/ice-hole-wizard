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
    dsp.margin.Left = CLAMP_X(dsp.cursor.X - dsp.width / 2);
    dsp.margin.Top =  CLAMP_Y(dsp.cursor.Y - dsp.height / 2);
    dsp.margin.Right = CLAMP_X(dsp.margin.Left + dsp.width);
    dsp.margin.Bottom = CLAMP_Y(dsp.margin.Top + dsp.height);

    //render window with new margins
    render();
}

void initDisplay()
{
    initCursor();
    addLayer(&(dsp.map));
    dsp.vp = getWindow();
    dsp.width = dsp.vp.Right - dsp.vp.Left + 1;
    dsp.height = dsp.vp.Bottom - dsp.vp.Top + 1;
    dsp.cursor.X = X_COL(1);
    dsp.cursor.Y = Y_ROW(1);
    resetMargins();
}

Node* getActiveLayer()
{
    if (dsp.map == NULL) return NULL;

    Node* layer = dsp.map;
    for (int i = 0; i < dsp.layer; i++)
    {
        if (layer->next != NULL) layer = layer->next;
        else return NULL;
    }

    return layer;
}

void render()
{
    char** grid = getActiveLayer()->grid;

    if (grid == NULL) return;

    /***** initialize buffer variables *****/

    int left = dsp.margin.Left;
    int top = dsp.margin.Top;
    int size = dsp.width * dsp.height;

    char* buffer = malloc(size * sizeof(char));
    ASSERT(buffer);

    //initialize buffer with spaces
    memset(buffer, ' ', size);

    /*clamp copy length to the end of the visible portion of the grid with
    space for a newline for printing*/
    int cols = CLAMP_X(dsp.width - 1);
    int rows = CLAMP_Y(dsp.height);
    char* index = buffer;

    for (int i = 0; i < rows; i++)
    {
        index = buffer + i * dsp.width;
        if (i + top < MAP_ROWS)
        {
            
        }
    }

    //move cursor to 1,1 for printing
    printf(CSI "H");
    CLEAR_SCREEN
    CLEAR_SCROLLBACK
    EDLDM
    
    fwrite(buffer, sizeof(char), size, stdout);
    fflush(stdout);

    EAM

    //return cursor to position on grid
    updateCursor();
    free(buffer);
}

void pollWindow()
{
    SMALL_RECT win = getWindow();

    int width = win.Right - win.Left + 1;
    int height = win.Bottom - win.Top + 1;

    int dx = dsp.width - width;
    int dy = dsp.height - height;

    //check if either dx or dy are nonzero
    if (dx || dy)
    {
        //set new dimensions
        dsp.vp = win;
        dsp.width = width;
        dsp.height = height;
        resetMargins();
        //render with new dimensions
        render();
    }
}

void freeDisplay()
{
    freeLayers(&(dsp.map));
}