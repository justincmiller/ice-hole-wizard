/*
*  Credit is given to Dr. Larry Hughes for providing the reference code
*  that was used throughout this program. Specifically, the engine.h, vterminal.h, and vterminal.c files
*  which uses much of Dr. Hughes' work.
*/

#include "display.h"
#include "engine.h"

//static global structure to limit scope
static Display dsp;

SMALL_RECT getWindow()
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    if (out == INVALID_HANDLE_VALUE)
    {
        printf("Error: could not fetch output handle\n");
        return;
    }

    CONSOLE_SCREEN_BUFFER_INFO info;

    if (!GetConsoleScreenBufferInfo(out, &info))
    {
        printf(">> Error: unable to get console screen buffer info.\n");
        return;
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
    dsp.vp = getWindow();
    dsp.width = dsp.vp.Right - dsp.vp.Left + 1;
    dsp.height = dsp.vp.Bottom - dsp.vp.Top + 1;
    dsp.cursor.X = X_COL(1);
    dsp.cursor.Y = Y_ROW(1);
    resetMargins();

    addLayer(&(dsp.map));

    CUP(dsp.cursor.X, dsp.cursor.Y);
}

void render()
{
    char** grid = getActiveLayer(dsp.map, dsp.layer)->grid;

    if (grid == NULL) return;

    /***** initialize buffer variables *****/

    //buffer width increased to accomodate \r\n for printing
    int width = dsp.width + 2;
    int left = dsp.margin.Left;
    int top = dsp.margin.Top;
    int row = 0;
    int bufsize = width * dsp.height;

    //clamp copy length to the end of the visible portion of the grid
    int len = min(dsp.width, (MAP_COLS - 1) - left);

    char** buffer = malloc(dsp.height * sizeof(char*));
    if (buffer == NULL)
    {
        printf(">> Error: memory failure.\n");
        return;
    }
    
    buffer[0] = malloc(dsp.height * width* sizeof(char));
    if (buffer[0] == NULL)
    {
        printf(">> Error: memory failure.\n");
        free(buffer);
        return;
    }

    for (int i = 0; i < dsp.height; i++)
    {
        //set row pointer
        buffer[i] = buffer[0] + i * width;
        //prevent grid overflow and copy from grid
        row = min(i + top, MAP_ROWS - 1);
        if (row < MAP_ROWS)
        {
            memcpy(buffer[i], &grid[row][left], len * sizeof(char));
            //insert a carriage return and newline starting at len (last two characters)
            buffer[i][len] = '\r';
            buffer[i][len + 1] = '\n';
        }
    }

    //move cursor to 1,1 for printing
    CUP(1,1);
    CLRSCR;
    
    fwrite(buffer, sizeof(char), bufsize, stdout);
    fflush(stdout);

    //return cursor to position on grid
    CUP(dsp.cursor.X, dsp.cursor.Y);

    free(buffer[0]);
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

//absolute
void setCursor(const short x, const short y)
{
    dsp.cursor.X = x;
    dsp.cursor.Y = y;

    int offsetX = CLAMP_X(dsp.cursor.X - dsp.margin.Left);
    int offsetY = CLAMP_Y(dsp.cursor.Y - dsp.margin.Top);
    CUP(offsetX, offsetY);
}

//relative
void moveCursor(const int action)
{
    switch(action)
    {
        case UP:
            dsp.cursor.Y = CLAMP_Y(dsp.cursor.Y - 1);
            break;
        case DOWN:
            dsp.cursor.Y = CLAMP_Y(dsp.cursor.Y + 1);
            break;
        case LEFT:
            dsp.cursor.X = CLAMP_X(dsp.cursor.X - 1);
            break;
        case RIGHT:
            dsp.cursor.X = CLAMP_X(dsp.cursor.X + 1);
            break;
    }
    
    int offsetX = CLAMP_X(dsp.cursor.X - dsp.margin.Left);
    int offsetY = CLAMP_Y(dsp.cursor.Y - dsp.margin.Top);
    CUP(offsetX, offsetY);
}