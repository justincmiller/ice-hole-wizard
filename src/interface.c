#include "interface.h"
#include "engine.h"

static Display* dsp;
static Menu editor;

const short foreground[] = 
{
    30, //black
    31, //red
    32, //green
    33, //yellow
    34, //blue
    35, //magenta
    36, //cyan
    37, //white
    38, //extended
    39, //default
    90, //bright black
    91, //bright red
    92, //bright green
    93, //bright yellow
    94, //bright blue
    95, //bright magenta
    96, //bright cyan
    97, //bright white
};

const short background[] =
{
    40,  //black
    41,  //red
    42,  //green
    43,  //yellow
    44,  //blue
    45,  //magenta
    46,  //cyan
    47,  //white
    48,  //extended
    49,  //default
    100, //bright black
    101, //bright red
    102, //bright green
    103, //bright yellow
    104, //bright blue
    105, //bright magenta
    106, //bright cyan
    107  //bright white
};

void loadMenu(Display* ptr)
{
    dsp = ptr;
    dsp->editor = &editor;

    editor.index = 0;

    short y = 2;

    editor.header = createTk(TEXT_X, y);
    y += 2; //add space following the header

    editor.text = malloc(MENU_DATA * sizeof(Token*));
    assert((void*)editor.text, APPEND);

    editor.data = malloc(MENU_DATA * sizeof(Token*));
    assert((void*)editor.data, APPEND);

    editor.options = malloc(OPTIONS * sizeof(Token*));
    assert((void*)editor.options, APPEND);

    for (int i = 0; i < MENU_DATA; i++)
    {
        //add space following the constant values (cell number and elevation)
        if (i == 2) y++;
        editor.text[i] = createTk(TEXT_X, y);
        editor.data[i] = createTk(DATA_X, y++);
        if (i > 1)
            editor.options[i-2] = editor.data[i];
    }
    
    y += 2; //add space following menu body

    editor.options[SAVE] = createTk(TEXT_X, y++);
    editor.options[CELL_RESET] = createTk(TEXT_X, y);

    snprintf(editor.header->string, TOKEN, "Cell Properties");
    formatTk(editor.header, BRIGHT_YELLOW, DEFAULT);

    snprintf(editor.text[CN]->string, TOKEN, "Cell Number");
    snprintf(editor.text[EL]->string, TOKEN, "Elevation (m)");
    snprintf(editor.text[CF]->string, TOKEN, "Friction");
    snprintf(editor.text[TY]->string, TOKEN, "Type");
    snprintf(editor.text[RL]->string, TOKEN, "Radiation (Bq)");
    snprintf(editor.text[CC]->string, TOKEN, "Ritterbarium (%%)");

    snprintf(editor.options[SAVE]->string, TOKEN, "Save");
    snprintf(editor.options[CELL_RESET]->string, TOKEN, "Reset");
}

Token* createTk(const short x, const short y)
{
    Token* tk = malloc(sizeof(Token));
    assert((void*)tk->string, APPEND);

    tk->string = calloc(TOKEN, sizeof(char));
    assert((void*)tk->string, APPEND);

    tk->fmt = 0;
    tk->x = x;
    tk->y = y;

    return tk;
}

void printTk(Token* tk)
{
    short fg;
    short bg;

    if (tk->fmt & (FMT_BG | FMT_FG))
    {
        fg = tk->fmt & COLOUR;
        bg = (tk->fmt >> BG) & COLOUR;
        printf(CSI "%d;%dH" CSI "%d;%dm" "%s" CSI "0m",
               tk->y, tk->x, fg, bg, tk->string);
    }
    else if (tk->fmt & FMT_FG)
    {
        fg = tk->fmt & COLOUR;
        printf(CSI "%d;%dH" CSI "%dm" "%s" CSI "0m",
               tk->y, tk->x, fg, tk->string);
    }
    else if (tk->fmt & FMT_BG)
    {
        bg = (tk->fmt >> BG) & COLOUR;
        printf(CSI "%d;%dH" CSI "%dm" "%s" CSI "0m",
               tk->y, tk->x, bg, tk->string);
    }
    else
    {
        printf(CSI "%d;%dH%s", tk->y, tk->x, tk->string);
    }
}

void formatTk(Token* tk, const unsigned short fg, const unsigned short bg)
{
    if (fg != DEFAULT) tk->fmt |= FMT_FG;
    else tk->fmt &= ~FMT_FG;

    if (bg != DEFAULT) tk->fmt |= FMT_BG;
    else tk->fmt &= ~FMT_BG;

    tk->fmt |= (background[bg] << BG) | foreground[fg];
}

void overlay()
{
    //render underlying grid
    viewport();

    /******** draw overlay container ********/
    RESET;

    //initialize string of horizontal lines
    char border[BORDER_COLS] = {0};
    memset(border, LR, sizeof(border)-1);

    //print top border with corners
    printf(LDM("%c%s%c\n"), DR, border, DL);

    //print left and right borders
    for (int i = 0; i < BORDER_ROWS; i++)
    {
        printf(LDM("%c%29c\n"), UD, UD);
    }

    //print bottom border with corners
    printf(LDM("%c%s%c\n"), UR, border, UL);

    container();
}

void container()
{
    short idx = editor.index;
    Data* data = editor.cell->data;

    if (data == NULL) return;

    snprintf(editor.data[CN]->string, TOKEN, "%u",  data->cn);
    snprintf(editor.data[EL]->string, TOKEN, "%d",  data->el);
    snprintf(editor.data[CF]->string, TOKEN, "%u",  data->cf);
    snprintf(editor.data[TY]->string, TOKEN, "%d",  data->ty);
    snprintf(editor.data[RL]->string, TOKEN, "%hu", data->rl);
    snprintf(editor.data[CC]->string, TOKEN, "%u",  getRB(data));

    printTk(editor.header);

    for (int i = 0; i < MENU_DATA; i++)
    {
        printTk(editor.text[i]);
        printTk(editor.data[i]);
    }

    printTk(editor.options[SAVE]);
    printTk(editor.options[CELL_RESET]);
}

void statusBar()
{
    HIDE_CURSOR;

    int x = COL_X(dsp->cursor->X) * CELL_WIDTH;
    int y = ROW_Y(dsp->cursor->Y) * CELL_HEIGHT;
    int z = dsp->map->layer->depth;

    STATUS_BAR_POS(dsp->size.Y);
    printf("x, y, z: (%d, %d, %d) (m) %12c", x, y, z, LATENT);

    updateCursor();

    if (dsp->state == MOVE) SHOW_CURSOR;
}

void option(const int code)
{
    short dy = 0;

    switch (code)
    {
        case ARROW_UP:
            if (editor.index > MENU_MIN) dy = -1;
            break;
        case ARROW_DOWN:
            if (editor.index < MENU_MAX) dy = 1;
            break;
    }

    editor.index += dy;

    if (dy != 0) updateMenu(dy);
}

void updateMenu(const short dy)
{
    formatTk(editor.options[editor.index], BLACK, WHITE);
    formatTk(editor.options[editor.index-dy], DEFAULT, DEFAULT);
    printTk(editor.options[editor.index]);
    printTk(editor.options[editor.index-dy]);
}

void editCell()
{
    if (editor.cell == NULL)
        editor.cell = createCell();

    editor.cell->x = dsp->cursor->X;
    editor.cell->y = dsp->cursor->Y;
    editor.cell->z = dsp->map->layer->depth;

    //skip initializing if the cn matches the current configuration
    if (editor.cell->data->cn == CN(editor.cell->x, editor.cell->y, editor.cell->z)) return;

    clearData();
}

void editValue()
{
    int value = 0;

    switch (editor.index)
    {
        case CF:
            scanf("%d", &value);
            snprintf(editor.data[editor.index]->string, TOKEN, "%d", value);
            editor.cell->data->cf = (unsigned int)value;
            break;
        case TY:
            scanf("%d", &value);
            snprintf(editor.data[editor.index]->string, TOKEN, "%d", value);
            editor.cell->data->ty = (char)value;
            break;
        case RL:
            scanf("%d", &value);
            snprintf(editor.data[editor.index]->string, TOKEN, "%d", value);
            editor.cell->data->rl = (unsigned int)value;
            break;
        case CC:
            scanf("%d", &value);
            snprintf(editor.data[editor.index]->string, TOKEN, "%d", value);
            //editor.cell->data->cf = (char)value;
            break;
    }
}

void saveCell()
{
    Cell* cell = (Cell*)addCell()->data;

    //copy cell contents from edited cell
    memcpy(cell, editor.cell, sizeof(Cell));
}

void clearData()
{
    //clear cell data
    memset(editor.cell->data, 0, sizeof(Data));

    //set nonzero default values
    editor.cell->data->cn = CN(editor.cell->x, editor.cell->y, editor.cell->z);
    editor.cell->data->el = editor.cell->z;
    editor.cell->data->cf = 5;
}