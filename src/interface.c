#include "interface.h"
#include "engine.h"

static Display* dsp;
static Menu menu;

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
    dsp->menu = &menu;
    menu.index = 0;

    short y = 2;

    menu.header = createTk(TEXT_X, y);
    y += 2; //add space following the header

    menu.text = malloc(MENU_DATA * sizeof(Token*));
    assert((void*)menu.text, APPEND);

    menu.data = malloc(MENU_DATA * sizeof(Token*));
    assert((void*)menu.data, APPEND);

    menu.options = malloc(OPTIONS * sizeof(Token*));
    assert((void*)menu.options, APPEND);

    for (int i = 0; i < MENU_DATA; i++)
    {
        //add space following the constant values (cell number and elevation)
        if (i == MENU_FIXED) y++;
        menu.text[i] = createTk(TEXT_X, y);
        menu.data[i] = createTk(DATA_X, y++);
    }

    for (int i = 0; i < MENU_VARS; i++)
    {
        menu.options[i] = menu.data[i + MENU_FIXED];
    }   

    y += 2; //add space following menu body

    menu.options[SAVE_CELL] = createTk(TEXT_X, y++);
    menu.options[RESET_CELL] = createTk(TEXT_X, y);

    snprintf(menu.header->string, TOKEN, "Cell Properties");
    formatTk(menu.header, BRIGHT_YELLOW, DEFAULT);

    snprintf(menu.text[CN]->string, TOKEN, "Cell Number");
    snprintf(menu.text[EL]->string, TOKEN, "Elevation (m)");
    snprintf(menu.text[CF]->string, TOKEN, "Friction");
    snprintf(menu.text[TY]->string, TOKEN, "Type");
    snprintf(menu.text[RL]->string, TOKEN, "Radiation (Bq)");
    snprintf(menu.text[CC]->string, TOKEN, "Ritterbarium (%%)");

    snprintf(menu.options[SAVE_CELL]->string, TOKEN, "Save");
    snprintf(menu.options[RESET_CELL]->string, TOKEN, "Reset");

    menu.selection = menu.options[menu.index];
    formatTk(menu.selection, BLACK, WHITE);
}

Token* createTk(const short x, const short y)
{
    Token* tk = malloc(sizeof(Token));
    assert((void*)tk, APPEND);

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

    if (tk->fmt & FMT_BG && tk->fmt & FMT_FG)
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
    tk->fmt = 0;

    if (fg != DEFAULT)
    {
        tk->fmt |= FMT_FG;
        tk->fmt |= foreground[fg]; 
    }

    if (bg != DEFAULT)
    {
        tk->fmt |= FMT_BG;
        tk->fmt |= (background[bg] << BG);
    }
}

void moveToken(Token* tk, const short x, const short y)
{
    tk->x = x;
    tk->y = y;
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
    Data* data = menu.cell->data;

    if (data == NULL) return;

    snprintf(menu.data[CN]->string, TOKEN, "%u",  data->cn);
    snprintf(menu.data[EL]->string, TOKEN, "%d",  data->el);
    snprintf(menu.data[CF]->string, TOKEN, "%u",  data->cf);
    snprintf(menu.data[TY]->string, TOKEN, "%d",  data->ty);
    snprintf(menu.data[RL]->string, TOKEN, "%hu", data->rl);
    snprintf(menu.data[CC]->string, TOKEN, "%u",  getRB(data));

    formatTk(menu.selection, BLACK, WHITE);

    printTk(menu.header);

    for (int i = 0; i < MENU_DATA; i++)
    {
        printTk(menu.text[i]);
        printTk(menu.data[i]);
    }

    printTk(menu.options[SAVE_CELL]);
    printTk(menu.options[RESET_CELL]);

    setCursor(dsp->cursor->X, dsp->cursor->Y);
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

    //if (dsp->state == MOVE) SHOW_CURSOR;
    if (dsp->state != EDIT) SHOW_BLOCK;
}

void option(const int code)
{
    short dy = 0;

    switch (code)
    {
        case ARROW_UP:
            if (menu.index > MENU_MIN) dy = -1;
            break;
        case ARROW_DOWN:
            if (menu.index < MENU_MAX) dy = 1;
            break;
    }

    if (!dy) return;

    //reformat current selection to default and print
    formatTk(menu.selection, DEFAULT, DEFAULT);
    printTk(menu.selection);

    //update selection
    menu.index += dy;
    menu.selection = menu.options[menu.index];

    //format new selection and print
    formatTk(menu.selection, BLACK, WHITE); 
    printTk(menu.selection);
}

void editor()
{
    if (menu.cell == NULL)
        menu.cell = createCell();

    unsigned int cn = CN(dsp->cursor->X, dsp->cursor->Y, dsp->map->layer->depth);

    //check for pre-exisiting cell
    Node* ptr = searchCN(cn);
    if (ptr != NULL && ptr->data != NULL)
    {
        //copy cell data for editing
        Cell* cell = ptr->data;
        memcpy(menu.cell->data, cell->data, sizeof(Data));
        menu.cell->x = cell->x;
        menu.cell->y = cell->y;
        menu.cell->z = cell->z;

        render();
        return;
    }

    menu.cell->x = dsp->cursor->X;
    menu.cell->y = dsp->cursor->Y;
    menu.cell->z = dsp->map->layer->depth;

    //skip initializing if the cn matches the current configuration
    if (menu.cell->data->cn == CN(menu.cell->x, menu.cell->y, menu.cell->z)) return;

    clearData();
    render();
}

void edit()
{
    Token* option = menu.options[menu.index];

    CUP(option->x, option->y);

    switch (menu.index)
    {
        case FRICTION:
            editCF();
            break;
        case TYPE:
            editTY();
            break;
        case RADIATION:
            editRL();
            break;
        case RITTERBARIUM:
            editRB();
            break;
        case SAVE_CELL:
            saveCell();
            break;
        case RESET_CELL:
            clearData();
            break;        
    }

    overlay();
}

void editCF()
{
    unsigned int cf = 0;

    if (scanf("%u", &cf))
    {
        cf = (cf > 10) ? 10 : (cf < 0) ? 0 : cf;
        menu.cell->data->cf = cf;
        snprintf(menu.selection->string, TOKEN, "%u", cf);
    }
}

void editTY()
{
    unsigned char ty = 0;

    if (scanf("%hhu", &ty))
    {
        ty = (ty > 100) ? 100 : (ty < 10) ? 0 : ty;
        menu.cell->data->ty = ty;
        snprintf(menu.selection->string, TOKEN, "%hhu", ty);
    }
}

void editRL()
{
    unsigned short rl = 0;

    if (scanf("%hu", &rl))
    {
        menu.cell->data->rl = rl;
        snprintf(menu.selection->string, TOKEN, "%hu", rl);
    }
}

void editRB()
{
    int rb = 0;

    if (scanf("%d", &rb))
    {
        rb = (rb > 100) ? 100 : (rb < 0) ? 0 : rb;

        for (int i = 0; i < CONTENTS; i++)
        {
            if (menu.cell->data->cc[i].code == RB)
                menu.cell->data->cc[i].qty = rb;
        }
    }
}

void saveCell()
{
    if (menu.cell == NULL || menu.cell->data == NULL) return;

    Cell* cell = NULL;
    Node* ptr = searchCN(menu.cell->data->cn);

    if (ptr != NULL)
    {
        cell = (Cell*)ptr->data;
        memcpy(cell->data, menu.cell->data, sizeof(Data));
        cell->x = menu.cell->x;
        cell->y = menu.cell->y;
        cell->z = menu.cell->z;
        return;
    }

    cell = (Cell*)addCell()->data;

    //copy cell contents from edited cell
    memcpy(cell->data, menu.cell->data, sizeof(Data));
    cell->x = menu.cell->x;
    cell->y = menu.cell->y;
    cell->z = menu.cell->z;
}

void clearData()
{
    if (menu.cell == NULL || menu.cell->data == NULL) return;

    //clear cell data
    memset(menu.cell->data, 0, sizeof(Data));

    //set nonzero default values
    menu.cell->data->cn = CN(menu.cell->x, menu.cell->y, menu.cell->z);
    menu.cell->data->el = menu.cell->z;
    menu.cell->data->cf = 5;
}