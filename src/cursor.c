#include "cursor.h"

static Display* dsp;
static char** grid;

void initCursor()
{
    dsp = getDisplay();
    grid = dsp->map->layer->grid;
}

void updateCursor()
{
    int offsetX = 1 + CLAMP_X(dsp->cursor.X - dsp->margin.Left);
    int offsetY = 1 + CLAMP_Y(dsp->cursor.Y - dsp->margin.Top);

    offsetY = (offsetY >= dsp->size.Y) ? (dsp->size.Y - 1) : offsetY;

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
            if (dsp->cursor.Y > GRID_MIN) 
                dsp->cursor.Y = CLAMP_Y(dsp->cursor.Y - 1);
            break;
        case DOWN:
            if (dsp->cursor.Y < GRID_MAX) 
                dsp->cursor.Y = CLAMP_Y(dsp->cursor.Y + 1);
            break;
        case LEFT:
            if (dsp->cursor.X > GRID_MIN) 
                dsp->cursor.X = CLAMP_X(dsp->cursor.X - 1);
            break;
        case RIGHT:
            if (dsp->cursor.X < GRID_MAX) 
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

char lineType(int row, int col)
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

    //conditions to validate if connection can be made in each direction
    bool up    = (row > GRID_MIN) ? connector(UP, grid[row-1][col])    : false;
    bool down  = (row < GRID_MAX) ? connector(DOWN, grid[row+1][col])  : false;
    bool left  = (col > GRID_MIN) ? connector(LEFT, grid[row][col-1])  : false;
    bool right = (col < GRID_MAX) ? connector(RIGHT, grid[row][col+1]) : false;

    type |= up    ? 1 << UP    : 0;
    type |= down  ? 1 << DOWN  : 0;
    type |= left  ? 1 << LEFT  : 0;
    type |= right ? 1 << RIGHT : 0;

    return line[type];
}

void drawCursor(const int action)
{
    //variables for brevity
    int x = dsp->cursor.X;
    int y = dsp->cursor.Y;
    
    int dx = 0;
    int dy = 0;
    
    switch(action)
    {
        case UP:
            if (y > GRID_MIN)
            {
                dy = -1;
                grid[y+dy][x] = 0x78;
                grid[y][x] = lineType(y, x);
            }
            break;
        case DOWN:
            if (y < GRID_MAX)
            {
                dy = 1;
                grid[y+dy][x] = 0x78;
                grid[y][x] = lineType(y, x);
            }
            break;
        case LEFT:
            if (x > GRID_MIN)
            {
                dx = -1;
                grid[y][x+dx] = 0x71;
                grid[y][x] = lineType(y, x);
            }
            break;
        case RIGHT:
            if (x < GRID_MAX)
            {
                dx = 1;
                grid[y][x+dx] = 0x71;
                grid[y][x] = lineType(y, x);
            }
            break;
    }

    //setCursor(x, y);
    printf(INACTIVE("%c") FIXED, grid[y][x]);
    setCursor(x+dx, y+dy);
    printf(ACTIVE("%c") FIXED, grid[y+dy][x+dx]);
}

void activate()
{
    //update grid pointer
    grid = dsp->map->layer->grid;
    HIDE_CURSOR;
}

void deactivate()
{
    //print inactive character on cursor position
    printf(INACTIVE("%c") FIXED, grid[dsp->cursor.Y][dsp->cursor.X]);
    SHOW_CURSOR;
}

void panViewport(const int action)
{
    switch(action)
    {
        case UP:
            if (dsp->margin.Top > GRID_MIN)
            {
                dsp->margin.Top = CLAMP_Y(dsp->margin.Top - 1);
                dsp->margin.Bottom = CLAMP_Y(dsp->margin.Top + dsp->size.Y);
            }
            break;
        case DOWN:
            if (dsp->margin.Bottom < GRID_MAX)
            {
                dsp->margin.Top = CLAMP_Y(dsp->margin.Top + 1);
                dsp->margin.Bottom = CLAMP_Y(dsp->margin.Top + dsp->size.Y);
            }
            break;
        case LEFT:
            if (dsp->margin.Left > GRID_MIN)
            {
                dsp->margin.Left = CLAMP_X(dsp->margin.Left - 1);
                dsp->margin.Right = CLAMP_X(dsp->margin.Left + dsp->size.X);
            }
            break;
        case RIGHT:
            if (dsp->margin.Right < GRID_MAX)
            {
                dsp->margin.Left = CLAMP_X(dsp->margin.Left + 1);
                dsp->margin.Right = CLAMP_X(dsp->margin.Left + dsp->size.X);
            }
            break;
    }

    render();
}

void menu(const int action)
{
    // switch(action)
    // {
        
    // }
}