/*
* cursor.c
*
* Contains
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
    int offsetX = 1 + CLAMP_X(dsp->cursor->X - dsp->margin.Left);
    int offsetY = 1 + CLAMP_Y(dsp->cursor->Y - dsp->margin.Top);

    //prevent cursor from entering status bar
    offsetY = (offsetY >= dsp->size.Y) ? (dsp->size.Y - 1) : offsetY;

    CUP(offsetX, offsetY);
}

//move cursor using absolute parameters
void setCursor(const short x, const short y)
{
    dsp->cursor->X = x;
    dsp->cursor->Y = y;

    updateCursor();
}

void relCursor(const short x, const short y)
{
    int offsetX = 1 + CLAMP_X(x - dsp->margin.Left);
    int offsetY = 1 + CLAMP_Y(y - dsp->margin.Top);
    CUP(offsetX, offsetY);
}

//move cursor using relative parameters
void move(const int code)
{
    switch(code)
    {
        case ARROW_UP:
            if (dsp->cursor->Y > GRID_MIN) 
                dsp->cursor->Y = CLAMP_Y(dsp->cursor->Y - 1);
            break;
        case ARROW_DOWN:
            if (dsp->cursor->Y < GRID_MAX) 
                dsp->cursor->Y = CLAMP_Y(dsp->cursor->Y + 1);
            break;
        case ARROW_LEFT:
            if (dsp->cursor->X > GRID_MIN) 
                dsp->cursor->X = CLAMP_X(dsp->cursor->X - 1);
            break;
        case ARROW_RIGHT:
            if (dsp->cursor->X < GRID_MAX) 
                dsp->cursor->X = CLAMP_X(dsp->cursor->X + 1);
            break;
    }
    
    updateCursor();
    statusBar();
}

//map drawing operations
void draw(const int code)
{
    //variables for brevity
    char** grid = dsp->map->layer->grid;
    int x = dsp->cursor->X;
    int y = dsp->cursor->Y;
    
    int dx = 0;
    int dy = 0;
    
    switch(code) //from pollKbInput
    {
        case ARROW_UP:
            if (y > GRID_MIN)
            {
                dy = -1;
                GLYPH(grid[y+dy][x], UD);
            }
            break;
        case ARROW_DOWN:
            if (y < GRID_MAX)
            {
                dy = 1;
                GLYPH(grid[y+dy][x], UD);
            }
            break;
        case ARROW_LEFT:
            if (x > GRID_MIN)
            {
                dx = -1;
                GLYPH(grid[y][x+dx], LR);
            }
            break;
        case ARROW_RIGHT:
            if (x < GRID_MAX)
            {
                dx = 1;
                GLYPH(grid[y][x+dx], LR);
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

//handles intersection drawing
bool connector(int dir, char c)
{
    //table of connection options
    const char connections[4][10] =
    {
        {0x5F, 0x6b, 0x6c, 0x6e, 0x74, 0x75, 0x77, 0x78, PLUS, MINUS},
        {0x5F, 0x6a, 0x6d, 0x6e, 0x74, 0x75, 0x76, 0x78, PLUS, MINUS},
        {0x5F, 0x6c, 0x6d, 0x6e, 0x71, 0x74, 0x76, 0x77, PLUS, MINUS},
        {0x5F, 0x6a, 0x6b, 0x6e, 0x71, 0x75, 0x76, 0x77, PLUS, MINUS}
    };

    for (int i = 0; i < 10; i++)
    {
        if (connections[dir][i] == c) return true;
    }

    return false;
}

//handles what sytle of line to draw on map
char lineType(char** grid, int row, int col)
{
    //line types
    const char line[] = 
    {
        0x5F, // 0000 - none
        UD,  // 0001 - up
        UD,  // 0010 - down
        UD,  // 0011 - up | down
        LR,  // 0100 - left
        UL,  // 0101 - left | up
        DL,  // 0110 - left | down
        UDL, // 0111 - left | down | up
        LR,  // 1000 - right
        UR,  // 1001 - right | up
        DR,  // 1010 - right | down
        UDR, // 1011 - right | down | up
        LR,  // 1100 - right | left
        ULR, // 1101 - right | left | up
        DLR, // 1110 - right | left | down
        UDLR // 1111 - right | left | up | down
    };

    int type = 0;

    //conditions to validate if connection can be made in each direction
    bool up    = (row > GRID_MIN) ? connector(UP,    grid[row-1][col]) : false;
    bool down  = (row < GRID_MAX) ? connector(DOWN,  grid[row+1][col]) : false;
    bool left  = (col > GRID_MIN) ? connector(LEFT,  grid[row][col-1]) : false;
    bool right = (col < GRID_MAX) ? connector(RIGHT, grid[row][col+1]) : false;

    type |= up    ? 1 << UP    : 0;
    type |= down  ? 1 << DOWN  : 0;
    type |= left  ? 1 << LEFT  : 0;
    type |= right ? 1 << RIGHT : 0;

    return line[type];
}

//handles moving the viewport in any state
void panViewport(const int code)
{
    switch(code) //from pollKbInput
    {
        case CTRL_UP:
            if (dsp->margin.Top > GRID_MIN)
            {
                dsp->margin.Top = CLAMP_Y(dsp->margin.Top - 1);
                dsp->margin.Bottom = CLAMP_Y(dsp->margin.Top + dsp->size.Y);
            }
            break;
        case CTRL_DOWN:
            if (dsp->margin.Bottom < GRID_MAX)
            {
                dsp->margin.Top = CLAMP_Y(dsp->margin.Top + 1);
                dsp->margin.Bottom = CLAMP_Y(dsp->margin.Top + dsp->size.Y);
            }
            break;
        case CTRL_LEFT:
            if (dsp->margin.Left > GRID_MIN)
            {
                dsp->margin.Left = CLAMP_X(dsp->margin.Left - 1);
                dsp->margin.Right = CLAMP_X(dsp->margin.Left + dsp->size.X);
            }
            break;
        case CTRL_RIGHT:
            if (dsp->margin.Right < GRID_MAX)
            {
                dsp->margin.Left = CLAMP_X(dsp->margin.Left + 1);
                dsp->margin.Right = CLAMP_X(dsp->margin.Left + dsp->size.X);
            }
            break;
    }

    render(); //forces update to map corresponding to the viewport update
}