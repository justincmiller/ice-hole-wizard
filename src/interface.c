/*
* interface.c
*
* This file contains definitions for UI element display and menu (editor)
* controls. Menu contains a cell structure which behaves as a buffer to 
* hold temporary editor data. Only cells with changes are saved to a 
* linked list.
*/

/*
*  Credit is given to Dr. Larry Hughes for providing the reference code
*  that was used both as inspiration and used as is throughout this program.
*/

#include "interface.h" //alt-screen menu
#include "engine.h" //program operation headers and functions

//static global display pointer and menu structure
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
    dsp = ptr;          //set display pointer
    dsp->menu = &menu;  //update menu pointer
    menu.index = 0;     //initialize menu index

    short y = SECTION; //initial row for printing

    menu.header = createTk(TEXT_X, y);
    y += SECTION; //add space following the header

    //allocate token array for menu text
    menu.text = malloc(MENU_DATA * sizeof(Token*));
    assert((void*)menu.text, APPEND);

    //allocate token array for menu data
    menu.data = malloc(MENU_DATA * sizeof(Token*));
    assert((void*)menu.data, APPEND);

    //allocate token array for menu options
    menu.options = malloc(OPTIONS * sizeof(Token*));
    assert((void*)menu.options, APPEND);

    //create text and data tokens and initialize with x and y positions
    for (int i = 0; i < MENU_DATA; i++)
    {
        //add space following the constant values (cell number and elevation)
        if (i == MENU_FIXED) y++;
        menu.text[i] = createTk(TEXT_X, y);
        menu.data[i] = createTk(DATA_X, y++);
    }

    //copy variable data token pointers to options array
    for (int i = 0; i < MENU_VARS; i++)
    {
        //offset by fixed data tokens
        menu.options[i] = menu.data[i + MENU_FIXED];
    }   

    y += SECTION; //add space following menu body

    //create save and reset tokens and initialize with x and y positions
    menu.options[SAVE_CELL] = createTk(TEXT_X, y++);
    menu.options[RESET_CELL] = createTk(TEXT_X, y);

    menu.message = createTk(TEXT_X, BORDER_ROWS);

    //write header string and format colour
    snprintf(menu.header->string, TOKEN, "Cell Properties");
    formatTk(menu.header, BRIGHT_YELLOW, DEFAULT);

    //write menu text strings
    snprintf(menu.text[CN]->string, TOKEN, "Cell Number");
    snprintf(menu.text[EL]->string, TOKEN, "Elevation (m)");
    snprintf(menu.text[CF]->string, TOKEN, "Friction");
    snprintf(menu.text[TY]->string, TOKEN, "Type (%%)");
    snprintf(menu.text[RL]->string, TOKEN, "Radiation (Bq)");
    snprintf(menu.text[CC]->string, TOKEN, "Ritterbarium (%%)");

    //write save and reset strings
    snprintf(menu.options[SAVE_CELL]->string, TOKEN, "Save");
    snprintf(menu.options[RESET_CELL]->string, TOKEN, "Reset");

    snprintf(menu.message->string, TOKEN, "Cell Data Loaded.");

    //initialize selection pointer using menu index and set highlight format
    menu.selection = menu.options[menu.index];
    formatTk(menu.selection, BLACK, WHITE);
}

Token* createTk(const short x, const short y)
{
    //allocate space for token
    Token* tk = malloc(sizeof(Token));
    assert((void*)tk, APPEND);

    //allocate space for string and initialize to 0
    tk->string = calloc(TOKEN, sizeof(char));
    assert((void*)tk->string, APPEND);

    //initialize default format, x and y positions
    tk->fmt = 0;
    tk->x = x;
    tk->y = y;

    return tk;
}

void printTk(Token* tk)
{
    //variables to extract foreground and background sgr colours
    short fg = 0;
    short bg = 0;

    //evaluate both foreground and background bits
    if (tk->fmt & FMT_BG && tk->fmt & FMT_FG)
    {
        //extract foreground and background colours
        fg = tk->fmt & COLOUR;
        bg = (tk->fmt >> BG) & COLOUR;
        //print token with CUP sequence and SGR sequence
        printf(CSI "%d;%dH" CSI "%d;%dm" "%s" CSI "0m",
               tk->y, tk->x, fg, bg, tk->string);
    }
    //evaluate foreground bit
    else if (tk->fmt & FMT_FG)
    {
        //extract foreground colour
        fg = tk->fmt & COLOUR;
        //print token with CUP sequence and SGR sequence
        printf(CSI "%d;%dH" CSI "%dm" "%s" CSI "0m",
               tk->y, tk->x, fg, tk->string);
    }
    //evaluate background bit
    else if (tk->fmt & FMT_BG)
    {
        //extract background colour
        bg = (tk->fmt >> BG) & COLOUR;
        //print token with CUP sequence and SGR sequence
        printf(CSI "%d;%dH" CSI "%dm" "%s" CSI "0m",
               tk->y, tk->x, bg, tk->string);
    }
    //case when both foreground and background bits are not set
    else
    {
        //print token with CUP sequence
        printf(CSI "%d;%dH%s", tk->y, tk->x, tk->string);
    }
}

void formatTk(Token* tk, const unsigned short fg, const unsigned short bg)
{
    //intitialze format
    tk->fmt = 0;

    //evaluate foreground parameter
    if (fg != DEFAULT)
    {
        //set foreground bit and color from table
        tk->fmt |= FMT_FG;
        tk->fmt |= foreground[fg]; 
    }

    //evaluate background parameter
    if (bg != DEFAULT)
    {
        //set background bit and color from table
        tk->fmt |= FMT_BG;
        tk->fmt |= (background[bg] << BG);
    }
}

void moveToken(Token* tk, const short x, const short y)
{
    //update token x and y positions
    tk->x = x;
    tk->y = y;
}

void overlay()
{
    //initailize variables for brevity
    int x = dsp->cursor->X;
    int y = dsp->cursor->Y;
    char** grid = dsp->map->layer->grid;

    //highlight current cell with marker format
    relCursor(x, y);
    printf(MARKER("%c"), grid[y][x]);

    /********** draw overlay container **********/
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

    //draw the container contents
    container();
}

void container()
{
    //fetch cell data from menu
    Cell* cell = menu.cell;
    if (cell == NULL) return;

    //update data tokens with cell data
    snprintf(menu.data[CN]->string, TOKEN, "%u",  cell->cn);
    snprintf(menu.data[EL]->string, TOKEN, "%d",  cell->el);
    snprintf(menu.data[CF]->string, TOKEN, "%u",  cell->cf);
    snprintf(menu.data[TY]->string, TOKEN, "%d",  cell->ty);
    snprintf(menu.data[RL]->string, TOKEN, "%hu", cell->rl);
    snprintf(menu.data[CC]->string, TOKEN, "%u",  getRB(cell));

    //highlight current menu selection
    formatTk(menu.selection, BLACK, WHITE);

    //print header
    printTk(menu.header);

    //print menu text and data
    for (int i = 0; i < MENU_DATA; i++)
    {
        printTk(menu.text[i]);
        printTk(menu.data[i]);
    }

    //print save and reset options
    printTk(menu.options[SAVE_CELL]);
    printTk(menu.options[RESET_CELL]);

    if (menu.saved)
    {
        printTk(menu.message);
    }
}

void statusBar()
{
    //hide cursor for rendering
    HIDE_CURSOR;

    //initialize display position and convert to metres
    int x = COL_X(dsp->cursor->X) * CELL_WIDTH;
    int y = ROW_Y(dsp->cursor->Y) * CELL_HEIGHT;
    int z = dsp->map->layer->depth;

    #ifdef DEBUG
        printf(CSI "%d;2H" "x, y, z: (%d, %d, %d) (m)" CSI "K", 
              dsp->size.Y, dsp->cursor->X, dsp->cursor->Y, z);
        printf(CSI "%d;%dH" "Display: %d x %d",
               dsp->size.Y, dsp->size.X / 3, dsp->size.X, dsp->size.Y);
        printf(CSI "4C" "T, L, B, R: (%d, %d, %d, %d)",
                dsp->margin.Top, dsp->margin.Left, 
                dsp->margin.Bottom, dsp->margin.Right);
    #else
        //print CUP sequence for bottom row, second column (padding)
        printf(CSI "%d;H" " x, y, z: (%d, %d, %d) (m)" CSI "K", 
               dsp->size.Y, x, y, z);
        
        if (dsp->state & DRAW)
            printf(CSI "%d;%dH" "DRAW", dsp->size.Y, dsp->size.X - 4);
        else if (dsp->state & MOVE)
            printf(CSI "%d;%dH" "MOVE", dsp->size.Y, dsp->size.X - 4);
    #endif

    //restore cursor position
    updateCursor();

    //hide cursor for edit menu
    if (dsp->state & EDIT) HIDE_CURSOR;
    else SHOW_BLOCK;
}

void option(const int code)
{
    short dy = 0;

    //evaluate arrow key code and clamp to menu options
    if (code == ARROW_UP)
        dy = (menu.index > MENU_MIN) ? -1 : dy;
    else if (code == ARROW_DOWN)
        dy = (menu.index < MENU_MAX) ?  1 : dy;

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
    //initialize menu cell once
    if (menu.cell == NULL)
        menu.cell = createCell();

    //compute cell number
    unsigned int cn = CN(dsp->cursor->X, dsp->cursor->Y, dsp->map->layer->depth);

    //check for pre-exisiting cell
    Node* ptr = searchCN(cn);
    if (ptr != NULL && ptr->data != NULL)
    {
        //copy cell data for editing
        Cell* cell = ptr->data;
        memcpy(menu.cell, cell, sizeof(Cell));
        menu.saved = true;
        return;
    }

    //if cell doesn't exist, update x, y and z and reset cell data
    menu.saved = false;
    clearData();
}

void edit()
{
    //move cursor to selected index
    CUP(menu.selection->x, menu.selection->y);

    //determine option action
    switch (menu.index)
    {
        case FRICTION: //edit cell friction
            editCF();
            break;
        case TYPE: //edit cell type
            editTY();
            break;
        case RADIATION: //edit cell radiation
            editRL();
            break;
        case RITTERBARIUM: //edit ritterbarium
            editRB();
            break;
        case SAVE_CELL: //save cell to layer
            saveCell();
            break;
        case RESET_CELL: //clear edited cell data
            clearData();
            break;        
    }

    //update overlay
    overlay();
}

void editCF()
{
    unsigned int cf = 0;

    //validate if cf can be assigned
    if (scanf("%u", &cf))
    {
        //clamp friction to max or min value and update selected token
        cf = (cf > MAX_CF) ? MAX_CF : (cf < 0) ? 0 : cf;
        menu.cell->cf = cf;
        snprintf(menu.selection->string, TOKEN, "%u", cf);
        FLUSH;
    }
}

void editTY()
{
    unsigned char ty = 0;

    //validate if ty can be assigned
    if (scanf("%hhu", &ty))
    {
        //clamp type to max or min percentage and update selected token
        ty = (ty > MAX_PERCENT) ? MAX_PERCENT : (ty < 0) ? 0 : ty;
        menu.cell->ty = ty;
        snprintf(menu.selection->string, TOKEN, "%hhu", ty);
        FLUSH;
    }   
}

void editRL()
{
    unsigned short rl = 0;

    //vaidate if rl can be assigned
    if (scanf("%hu", &rl))
    {
        //update radiation level and selected token
        menu.cell->rl = rl;
        snprintf(menu.selection->string, TOKEN, "%hu", rl);
        FLUSH;
    }
}

void editRB()
{
    int rb = 0;
    int n = 0;

    if (scanf("%d", &rb))
    {
        //clamp RB level to 0-100 pecent
        rb = (rb > MAX_PERCENT) ? MAX_PERCENT : (rb < 0) ? 0 : rb;

        //iterate through cell contents
        while (menu.cell->cc[n].code != LATENT_CC && n < CONTENTS)
        {
            //evaulate mineral for RB code
            if (menu.cell->cc[n].code == RB)
            {
                //update RB level (in %) and update token
                menu.cell->cc[n].qty = rb;
                snprintf(menu.selection->string, TOKEN, "%d", rb);
                //flush remaining characters from input buffer
                FLUSH;
                return;
            }
            n++;
        }

        //evaluate case where RB is not found
        if (menu.cell->cc[n].code == LATENT_CC)
        {
            //add RB to list with qty (in %)
            menu.cell->cc[n].code = RB;
            menu.cell->cc[n].qty = rb;
            snprintf(menu.selection->string, TOKEN, "%d", rb);
        }
        FLUSH;
    }  
}

void saveCell()
{
    //avoid dereferencing NULL pointer
    if (menu.cell == NULL || menu.cell == NULL) return;

    //search for cell with current cell number
    Cell* cell = NULL;
    Node* ptr = searchCN(menu.cell->cn);

    if (ptr != NULL)
    {
        cell = (Cell*)ptr->data; //update cell pointer
        //copy existing cell data
        memcpy(cell, menu.cell, sizeof(Cell));
        return;
    }

    //create cell if cell at CN does not exist
    cell = (Cell*)addCell()->data;

    //copy cell contents from edited cell
    memcpy(cell, menu.cell, sizeof(Cell));
}

void clearData()
{
    //avoid dereferencing NULL pointer
    if (menu.cell == NULL || menu.cell == NULL) return;

    //clear cell data
    memset(menu.cell, 0, sizeof(Cell));

    //set nonzero default values
    menu.cell->cn = CN(dsp->cursor->X, dsp->cursor->Y, dsp->map->layer->depth);
    menu.cell->el = dsp->map->layer->depth;
    menu.cell->cf = DEF_CF;
}