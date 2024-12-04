/*
* cursor.c
*
* Contains function implementations to manipulate the cursor in
* all states, and map drawing functions for lines and interesctions on the map.
* 
* Also contains function to pan the viewport.
*/

/*
*  Credit is given to Dr. Larry Hughes for providing the reference code
*  that was used both as inspiration and used as is throughout this program.
*/

#include "cursor.h" //cursor movement

//static global variable for display (limited scope)
static Display* dsp;

//loads cursor onto map
void loadCursor(Display* ptr)
{
    dsp = ptr;
}

//update cursor position
void updateCursor()
{
    unsigned short x = 1 + CLAMP_X(dsp->cursor->X - dsp->margin.Left);
    unsigned short y = 1 + CLAMP_Y(dsp->cursor->Y - dsp->margin.Top);

    CUP(x, y);
}

//move cursor using absolute parameters
void setCursor(const short x, const short y)
{
    dsp->cursor->X = x;
    dsp->cursor->Y = y;

    updateCursor();
}

//reloads cursor on window resizing
void relCursor(const short x, const short y)
{
    unsigned short offsetX = 1 + CLAMP_X(x - dsp->margin.Left);
    unsigned short offsetY = 1 + CLAMP_Y(y - dsp->margin.Top);

    CUP(offsetX, offsetY);
}

//move cursor using relative parameters
void move(const int code)
{
    unsigned short x = dsp->cursor->X;
    unsigned short y = dsp->cursor->Y;

    unsigned short up = max(dsp->margin.Top, GRID_MIN);
    unsigned short down = min(dsp->margin.Bottom, GRID_MAX);
    unsigned short left = max(dsp->margin.Left, GRID_MIN);
    unsigned short right = min(dsp->margin.Right, GRID_MAX);

    switch (code)
    {
        case ARROW_UP:
            if (y > up) y--;
            else
            {
                panViewport(UP);
            }
            break;
        case ARROW_DOWN:
            if (y < down) y++;
            else
            {
                panViewport(DOWN);
            }
            break;
        case ARROW_LEFT:
            if (x > left) x--;
            else
            {
                panViewport(LEFT);
            }
            break;
        case ARROW_RIGHT:
            if (x < right) x++;
            else
            {
                panViewport(RIGHT);
            }
            break;
    }

    setCursor(x, y);
    statusBar();
}

//map drawing operations
void draw(const int code)
{
    //variables for brevity
    char** grid = dsp->map->layer->grid;
    unsigned short x = dsp->cursor->X;
    unsigned short y = dsp->cursor->Y;

    unsigned short up = max(dsp->margin.Top, GRID_MIN);
    unsigned short down = min(dsp->margin.Bottom, GRID_MAX);
    unsigned short left = max(dsp->margin.Left, GRID_MIN);
    unsigned short right = min(dsp->margin.Right, GRID_MAX);
    
    short dx = 0;
    short dy = 0;
    
    switch(code) //from pollKbInput
    {
        case ARROW_UP:
            if (y > up)
            {
                dy = -1;
                GLYPH(grid[y+dy][x], UD);
            }
            else
            {
                panViewport(UP);
            }
            break;
        case ARROW_DOWN:
            if (y < down)
            {
                dy = 1;
                GLYPH(grid[y+dy][x], UD);
            }
            else
            {
                panViewport(DOWN);
            }
            break;
        case ARROW_LEFT:
            if (x > left)
            {
                dx = -1;
                GLYPH(grid[y][x+dx], LR);
            }
            else
            {
                panViewport(LEFT);
            }
            break;
        case ARROW_RIGHT:
            if (x < right)
            {
                dx = 1;
                GLYPH(grid[y][x+dx], LR);
            }
            else
            {
                panViewport(RIGHT);
            }
            break;
    }

    GLYPH(grid[y][x], lineType(grid, y, x));

    //update cursor after drawing
    if (grid[y][x] != PLUS && grid[y][x] != MINUS)
        printf(PATH("%c") FIXED, grid[y][x]);

    setCursor(x+dx, y+dy);
    if (grid[y+dy][x+dx] != PLUS && grid[y+dy][x+dx] != MINUS)
        printf(PATH("%c") FIXED, grid[y+dy][x+dx]);

    statusBar();
}

//handles what sytle of line to draw on map
char lineType(char** grid, int row, int col)
{
    //line types
    const char line[] = 
    {
        NONE, // 0000 - none
        UD,   // 0001 - up
        UD,   // 0010 - down
        UD,   // 0011 - up | down
        LR,   // 0100 - left
        UL,   // 0101 - left | up
        DL,   // 0110 - left | down
        UDL,  // 0111 - left | down | up
        LR,   // 1000 - right
        UR,   // 1001 - right | up
        DR,   // 1010 - right | down
        UDR,  // 1011 - right | down | up
        LR,   // 1100 - right | left
        ULR,  // 1101 - right | left | up
        DLR,  // 1110 - right | left | down
        UDLR  // 1111 - right | left | up | down
    };

    int type = 0;

    bool up =    (row > GRID_MIN) ? upConnectors(grid[row-1][col])    : false;
    bool down =  (row < GRID_MAX) ? downConnectors(grid[row+1][col])  : false;
    bool left =  (col > GRID_MIN) ? leftConnectors(grid[row][col-1])  : false;
    bool right = (col < GRID_MAX) ? rightConnectors(grid[row][col+1]) : false;

    type |= (up    ? 1 : 0) << UP;
    type |= (down  ? 1 : 0) << DOWN;
    type |= (left  ? 1 : 0) << LEFT;
    type |= (right ? 1 : 0) << RIGHT;

    return line[type];
}

//line connections in the up direction
bool upConnectors(const char c)
{
    switch (c)
    {
        case UL:
        case UR:
        case LR:
        case ULR:
        case LATENT:
            return false;
        default:
            return true;
    }
}

//line connections in the down direction
bool downConnectors(const char c)
{
    switch (c)
    {
        case DL:
        case DR:
        case LR:
        case DLR:
        case LATENT:
            return false;
        default:
            return true;
    }
}

//line connections in the left direction
bool leftConnectors(const char c)
{
    switch (c)
    {
        case UL:
        case DL:
        case UDL:
        case UD:
        case LATENT:
            return false;
        default:
            return true;
    }
}

//line connections in the right direction
bool rightConnectors(const char c)
{
    switch (c)
    {
        case DR:
        case UR:
        case UDR:
        case UD:
        case LATENT:
            return false;
        default:
            return true;
    }
}

//handles moving the viewport in any state
void panViewport(const int code)
{
    short dx = 0;
    short dy = 0;

    switch(code) //from pollKbInput
    {
        case UP:
        case CTRL_UP:
            if (dsp->margin.Top > GRID_MIN) dy = -1;
            break;
        case DOWN:
        case CTRL_DOWN:
            if (dsp->margin.Bottom < GRID_MAX) dy = 1;
            break;
        case LEFT:
        case CTRL_LEFT:
            if (dsp->margin.Left > GRID_MIN) dx = -1;
            break;
        case RIGHT:
        case CTRL_RIGHT:
            if (dsp->margin.Right < GRID_MAX) dx = 1;
            break;
    }

    if (dx)
    {
        dsp->margin.Left += dx;
        dsp->margin.Right += dx;
        render();
    }

    if (dy)
    {
        dsp->margin.Top += dy;
        dsp->margin.Bottom += dy;
        render();
    }
}