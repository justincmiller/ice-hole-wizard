#include "cursor.h"

static Display* dsp;

void initCursor()
{
    dsp = getDisplay();
    printf(CSI "?25l");
}

void updateCursor()
{
    int offsetX = CLAMP_X(dsp->cursor.X - dsp->margin.Left);
    int offsetY = CLAMP_Y(dsp->cursor.Y - dsp->margin.Top);
    CUP(offsetX, offsetY);
}

//move cursor using absolute parameters
void setCursor(const short x, const short y)
{
    dsp->cursor.X = x;
    dsp->cursor.Y = y;

    updateCursor();
}

//move cursor using relative parameters
void moveCursor(const int action)
{
    switch(action)
    {
        case UP:
            if (dsp->cursor.Y > 0) 
                dsp->cursor.Y = CLAMP_Y(dsp->cursor.Y - 1);
            break;
        case DOWN:
            if (dsp->cursor.Y < MAP_ROWS - 1) 
                dsp->cursor.Y = CLAMP_Y(dsp->cursor.Y + 1);
            break;
        case LEFT:
            if (dsp->cursor.X > 0) 
                dsp->cursor.X = CLAMP_X(dsp->cursor.X - 1);
            break;
        case RIGHT:
            if (dsp->cursor.X < MAP_COLS - 1) 
                dsp->cursor.X = CLAMP_X(dsp->cursor.X + 1);
            break;
    }
    
    updateCursor();
}

void drawCursor(const int action)
{

    char** grid = (char**)(getActiveLayer()->data);

    //variables for brevity
    int row = dsp->cursor.Y;
    int col = dsp->cursor.X;
    
    switch(action)
    {
        case UP:
            if (dsp->cursor.Y > 0)
            {
                dsp->cursor.Y = CLAMP_Y(dsp->cursor.Y - 1);
                grid[row][col] = 'x';
            }
            break;
        case DOWN:
            if (dsp->cursor.Y < MAP_ROWS - 1)
            {
                dsp->cursor.Y = CLAMP_Y(dsp->cursor.Y + 1);
                grid[row][col] = 'x';
            }
            break;
        case LEFT:
            if (dsp->cursor.X > 0)
            {
                dsp->cursor.X = CLAMP_X(dsp->cursor.X - 1);
                grid[row][col] = 'q';
            }
            break;
        case RIGHT:
            if (dsp->cursor.X < MAP_COLS - 1)
            {
                dsp->cursor.X = CLAMP_X(dsp->cursor.X + 1);
                grid[row][col] = 'q';
            }
            break;
    }
    
    EDLDM
    printf(CSI "43m");
    _putch(grid[row][col]);
    printf(CSI "0m");
    EAM

    updateCursor();
}

void panViewport(const int action)
{
    switch(action)
    {
        case UP:
            if (dsp->margin.Top > 0)
            {
                dsp->margin.Top = CLAMP_Y(dsp->margin.Top - 1);
                dsp->margin.Bottom = CLAMP_Y(dsp->margin.Top + dsp->height);
            }
            break;
        case DOWN:
            if (dsp->margin.Bottom < MAP_ROWS - 1)
            {
                dsp->margin.Top = CLAMP_Y(dsp->margin.Top + 1);
                dsp->margin.Bottom = CLAMP_Y(dsp->margin.Top + dsp->height);
            }
            break;
        case LEFT:
            if (dsp->margin.Left > 0)
            {
                dsp->margin.Left = CLAMP_X(dsp->margin.Left - 1);
                dsp->margin.Right = CLAMP_X(dsp->margin.Left + dsp->width);
            }
            break;
        case RIGHT:
            if (dsp->margin.Right < MAP_COLS - 1)
            {
                dsp->margin.Left = CLAMP_X(dsp->margin.Left + 1);
                dsp->margin.Right = CLAMP_X(dsp->margin.Left + dsp->width);
            }
            break;
    }

    render();
    updateCursor();
}