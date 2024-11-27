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
    //fetch output handle
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    if (out == INVALID_HANDLE_VALUE)
        printf("Error: could not fetch output handle\n");

    //fetch console screen buffer info
    CONSOLE_SCREEN_BUFFER_INFO info;
    if (!GetConsoleScreenBufferInfo(out, &info))
        printf(">> Error: unable to get console screen buffer info.\n");

    return info.srWindow;
}

void setWindow()
{
    SMALL_RECT window = getWindow();

    //compute window width and height
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
    //hide cursor for rendering
    HIDE_CURSOR;
    
    //evaluate display for edit state
    if (dsp.state & EDIT)
    {
        ALT_SCREEN; //switch to alternate screen
        viewport(); //render underlying viewport
        overlay();  //render overlay
    }
    else
    {
        MAIN_SCREEN; //switch to main screen buffer
        CLEAR;       //clear old viewport from display
        viewport();  //render viewport
    }
}

void pollWindow()
{
    //fetch window dimensions
    SMALL_RECT dim = getWindow();

    //compute width and height
    int width = dim.Right - dim.Left + 1;
    int height = dim.Bottom - dim.Top + 1;

    //compute change in width or height
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
    //fetch grid from current layer
    char** grid = dsp.map->layer->grid;
    if (grid == NULL) return;

    //fetch margins for visible portion of the grid
    COORD offset = {dsp.margin.Left, dsp.margin.Top};

    //reset cursor for printing
    RESET;

    //clamp rows and cols to visible boundaries of the grid
    int rows = CLAMP_Y(dsp.size.Y + offset.Y);
    int cols = CLAMP_X(dsp.size.X + offset.X);

    //iterate through rows and columns, only printing if character is not latent
    for (int i = offset.Y; i < rows; i++)
    {
        for (int j = offset.X; j < cols; j++)
        {
            //evalutate grid for portal up
            if (grid[i][j] == PLUS)
            {
                //move cursor to position relative to grid
                relCursor(j, i);
                //print with portal up format
                printf(PORT_UP("%c"), grid[i][j]); 
            }
            //evaluate grid for portal down
            else if (grid[i][j] == MINUS)
            {
                //move cursor to position relative to grid
                relCursor(j, i);
                //print with portal down format
                printf(PORT_DN("%c"), grid[i][j]);
            }
            //evaltuate grid for remaining glyphs
            else if (grid[i][j] != LATENT)
            {
                //move cursor to position relative to grid
                relCursor(j, i);
                //print with path format
                printf(PATH("%c"), grid[i][j]);
            }
        }
    }

    //draw status bar
    statusBar();
}