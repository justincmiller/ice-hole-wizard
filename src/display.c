/*
*  Credit is given to Dr. Larry Hughes for providing the reference code
*  that was used both as inspiration and used as is throughout this program.
*/

#include "display.h"
#include "engine.h"

//static global structure (limited scope)
static Display dsp;

void loadDisplay(Display** ptr)
{
    *ptr = &dsp;
}

void loadMenu()
{
    dsp.edit.values = malloc(DATA_ROWS * sizeof(char*));
    ASSERT(dsp.edit.values);
    track((void*)dsp.edit.values);

    dsp.edit.values[0] = calloc(DATA_ROWS * MENU_STR, sizeof(char));
    ASSERT(dsp.edit.values[0]);
    track((void*)dsp.edit.values[0]);
    
    for (int i = 0; i < DATA_ROWS; i++)
    {
        dsp.edit.values[i] = dsp.edit.values[0] + i * MENU_STR;
    }

    dsp.edit.options = malloc(OPTIONS * sizeof(char*));
    ASSERT(dsp.edit.options);
    track((void*)dsp.edit.options);

    dsp.edit.options[0] = calloc(OPTIONS * MENU_STR, sizeof(char));
    ASSERT(dsp.edit.options[0]);
    track((void*)dsp.edit.options);

    for (int i = 0; i < OPTIONS; i++)
    {
        dsp.edit.options[i] = dsp.edit.options[0] + i * MENU_STR;
    }
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

void setWindow()
{
    SMALL_RECT window = getWindow();

    dsp.size.X = window.Right - window.Left + 1;
    dsp.size.Y = window.Bottom - window.Top + 1;

    resetMargins();
}

void resetMargins()
{
    /*CLAMP macros use max and min functions clamp margins to grid 
    bounds (0, 99). if the window exceeds the width of the grid, it 
    will be aligned top left.*/
    dsp.margin.Left = CLAMP_X(dsp.cursor.X - dsp.size.X / 2);
    dsp.margin.Top =  CLAMP_Y(dsp.cursor.Y - dsp.size.Y / 2);
    dsp.margin.Right = CLAMP_X(dsp.margin.Left + dsp.size.X);
    dsp.margin.Bottom = CLAMP_Y(dsp.margin.Top + dsp.size.Y);

    //render window with new margins
    render();
}

void render()
{
    HIDE_CURSOR;

    if (dsp.state & EDIT)
    {
        ALT_SCREEN;
        overlay();
    }
    else
    {
        MAIN_SCREEN;
        viewport();
    }
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

    setCursor(cursor.X, cursor.Y);
    statusBar();
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

    container();
}

void container()
{
    short idx = dsp.edit.index;
    Data* data = dsp.edit.cell->data;

    if (data == NULL) return;

    const char* text[] =
    {
        FG_BY("Cell Properties"),
        "Cell Number",
        "Elevation",
        "Friction",
        "Type",
        "Radiation",
        "Ritterbarium"
    };

    const char* offsets[] =
    {
        DBL_LN TEXT_X, //header
        SGL_LN TEXT_X, //cell number
        DBL_LN TEXT_X, //elevation
        SGL_LN TEXT_X, //friction
        SGL_LN TEXT_X, //type
        SGL_LN TEXT_X, //radiation
        SGL_LN TEXT_X  //ritterbarium
    };

    TEXT_POS;
    
    for (int i = 0; i < ARRAY_LEN(text); i++)
    {
        printf("%s%s", text[i], offsets[i]);
    }

    DATA_POS;

    snprintf(dsp.edit.values[CN], DATA_COLS, "%u"  SGL_LN DATA_X, data->cn);
    snprintf(dsp.edit.values[EL], DATA_COLS, "%d"  DBL_LN DATA_X, data->el);
    snprintf(dsp.edit.values[CF], DATA_COLS, "%u"  SGL_LN DATA_X, data->cf);
    snprintf(dsp.edit.values[TY], DATA_COLS, "%d"  SGL_LN DATA_X, data->ty);
    snprintf(dsp.edit.values[RL], DATA_COLS, "%hu" SGL_LN DATA_X, data->rl);
    snprintf(dsp.edit.values[CC], DATA_COLS, "%u"  SGL_LN DATA_X, getRB(data));

    printf("%s %s %s %s %s %s",
            dsp.edit.values[CN],
            dsp.edit.values[EL],
            dsp.edit.values[CF],
            dsp.edit.values[TY],
            dsp.edit.values[RL],
            dsp.edit.values[CC]);
}

void statusBar()
{
    HIDE_CURSOR;

    int x = COL_X(dsp.cursor.X) * CELL_WIDTH;
    int y = ROW_Y(dsp.cursor.Y) * CELL_HEIGHT;
    int z = dsp.map->layer->depth;

    STATUS_BAR_POS(dsp.size.Y);
    printf("x, y, z: (%d, %d, %d) (m) %12c", x, y, z, LATENT);

    updateCursor();

    if (dsp.state == MOVE) SHOW_CURSOR;
}
