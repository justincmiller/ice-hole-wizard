#include "cursor.h"

static Display* dsp;

void initCursor()
{
    dsp = getDisplay();
}

void updateCursor()
{
    int offsetX = 1 + CLAMP_X(dsp->cursor.X - dsp->margin.Left);
    int offsetY = 1 + CLAMP_Y(dsp->cursor.Y - dsp->margin.Top);
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

bool connector(int dir, char c)
{
    const char connections[4][8] =
    {
        {0x5F, 0x6b, 0x6c, 0x6e, 0x74, 0x75, 0x77, 0x78},
        {0x5F, 0x6a, 0x6d, 0x6e, 0x74, 0x75, 0x76, 0x78},
        {0x5F, 0x6c, 0x6d, 0x6e, 0x71, 0x74, 0x76, 0x77},
        {0x5F, 0x6a, 0x6b, 0x6e, 0x71, 0x75, 0x76, 0x77}
    };

    for (int i = 0; i < 8; i++)
    {
        if (connections[dir][i] == c) return true;
    }

    return false;
}

char lineType(char** grid, int row, int col)
{
    const char line[] = 
    {
        0x5F, // 0000 - none
        0x78, // 0001 - up
        0x78, // 0010 - down
        0x78, // 0011 - up | down
        0x71, // 0100 - left
        0x6a, // 0101 - left | up
        0x6b, // 0110 - left | down
        0x75, // 0111 - left | down | up
        0x71, // 1000 - right
        0x6d, // 1001 - right | up
        0x6c, // 1010 - right | down
        0x74, // 1011 - right | down | up
        0x71, // 1100 - right | left
        0x76, // 1101 - right | left | up
        0x77, // 1110 - right | left | down
        0x6e  // 1111 - right | left | up | down
    };

    int type = 0;

    bool up    = (row > 0)            ? connector(UP, grid[row-1][col])    : false;
    bool down  = (row < MAP_ROWS - 1) ? connector(DOWN, grid[row+1][col])  : false;
    bool left  = (col > 0)            ? connector(LEFT, grid[row][col-1])  : false;
    bool right = (col < MAP_COLS - 1) ? connector(RIGHT, grid[row][col+1]) : false;

    type |= up    ? 1 << UP    : 0;
    type |= down  ? 1 << DOWN  : 0;
    type |= left  ? 1 << LEFT  : 0;
    type |= right ? 1 << RIGHT : 0;

    return line[type];
}

void drawCursor(const int action)
{

    char** grid = (char**)(getActiveLayer()->data);

    //variables for brevity
    int x = dsp->cursor.X;
    int y = dsp->cursor.Y;
    
    int dx = 0;
    int dy = 0;
    
    switch(action)
    {
        case UP:
            if (y > 0)
            {
                dy = -1;
                grid[y+dy][x] = 0x78;
                grid[y][x] = lineType(grid, y, x);
            }
            break;
        case DOWN:
            if (y < MAP_ROWS - 1)
            {
                dy = 1;
                grid[y+dy][x] = 0x78;
                grid[y][x] = lineType(grid, y, x);
            }
            break;
        case LEFT:
            if (x > 0)
            {
                dx = -1;
                grid[y][x+dx] = 0x71;
                grid[y][x] = lineType(grid, y, x);
            }
            break;
        case RIGHT:
            if (x < MAP_COLS - 1)
            {
                dx = 1;
                grid[y][x+dx] = 0x71;
                grid[y][x] = lineType(grid, y, x);
            }
            break;
    }

    render();
    setCursor(x+dx, y+dy);
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

    //clear screen to remove old grid
    CLEAR;
    render();
    updateCursor();
}