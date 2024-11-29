/*
* interface.h
*
* Updated Nov. 27 2024
* 
* Contains Menu and Token structure definitions, Token, UI element and editor
* function declarations, and menu and editor constants.
*
*  Credit is given to Dr. Larry Hughes for providing the reference code
*  that was used both as inspiration and used as is throughout this program.
*/

#ifndef INTERFACE_H
#define INTERFACE_H

#include "map.h" //layers, map, cells, etc.

#define MENU_ROWS 20 //total overlay rows
#define MENU_COLS 30 //total overlay columns

//number of middle rows offset by 2 to accomodate top/bottom borders
#define BORDER_ROWS (MENU_ROWS - 2)
//number of columns offset by 1 to accomodate for corners and NUL terminator
#define BORDER_COLS (MENU_COLS - 1)

#define SECTION 2 //section break value

#define COLOUR     0x7F       //bitmask for colour extraction
#define FMT_BG     (1 << 14)  //bitmask for foreground bit
#define FMT_FG     (1 << 15)  //bitmask for background bit
#define BG         7          //bit shift for background
#define TOKEN      32         //maximum token string size for

#define MENU_MAX   5   //maximum array index for menu options
#define MENU_MIN   0   //minimum array index for menu options

#define MENU_FIXED 2   //amount of fixed menu data items
#define MENU_VARS  4   //amount of variable menu data items

#define MENU_DATA  6   //amount of menu data items
#define OPTIONS    6   //number of selectable menu options
#define TEXT_X     3   //starting column for text printing
#define DATA_X     20  //starting column for data printing

#define MAX_PERCENT 100 //maximum percent value

struct Display;

typedef struct Token
{
    char* string;       //string to hold text for printing
    unsigned short fmt; //format value with encoded foreground and background
    short x;            //x position
    short y;            //y position
}Token;

typedef struct Menu
{
    short index;      //menu option index
    Token* header;    //header token
    Token** text;     //array of fixed menu labels
    Token** data;     //array of variable menu data
    Token** options;  //array of menu selections
    Token* selection; //pointer to current selected option
    Token* message;
    Cell* cell;       //cell buffer for editing
    bool saved;
}Menu;

//Array indices for foreground and background SGR values
enum colours
{
    BLACK,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN,
    WHITE,
    EXTENDED,
    DEFAULT,
    BRIGHT_BLACK,
    BRIGHT_RED,
    BRIGHT_GREEN,
    BRIGHT_YELLOW,
    BRIGHT_BLUE,
    BRIGHT_MAGENTA,
    BRIGHT_CYAN,
    BRIGHT_WHITE
};

/*
* Box drawing character values for line drawing mode
* defined by line connectiors:
* U: up connector
* D: down connector
* L: left connector
* R: right connector
*/
enum lines
{
    UL = 0x6a, DL, DR, UR, UDLR,
    LR = 0x71,
    UDR = 0x74, UDL, ULR, DLR, UD
};

//Labels for menu options
enum options
{
    FRICTION, TYPE, RADIATION, RITTERBARIUM, SAVE_CELL, RESET_CELL
};

/*
* Function to initialize menu module. Accepts a pointer to the system display
* structure and updates it with a pointer to the menu structure. Menu contents
* are allocated and initialized.
*/
void loadMenu(struct Display* ptr);

/********** Token Functions **********/

/*
* Function to allocate and initialize a new token using parameters to set x 
* and y positions for printing. Returns a pointer to the token it allocates.
*/
Token* createTk(const short x, const short y);

/*
* Function to print a token accepted by parameter. If foreground or background
* colours are set, they are extracted and token will be printed at its 
* coordinates with SGR colour formatting.
*/
void printTk(Token* tk);

/*
* Function to format a token using foreground and background parameters, which
* represent array indices from the foreground and background lookup tables.
* Foreground and background bits will be set if the value is not default.
*/
void formatTk(Token* tk, const unsigned short fg, const unsigned short bg);

/********** UI Element Functions **********/

/*
* Function to render the alt screen overlay. Borders are printed along with
* the highlighted current cell. Calls container() to render menu contents.
*/
void overlay();

/*
* Function to update tokens using cell data and render menu text, data and 
* options and highlighting the currently selected option.
*/
void container();

/*
* Function to update and render the status bar which is positioned at the
* bottom of the screen. Prints current x, y and z coordinates in metres.
* Restores the cursor position, as well as visibility for draw and move 
* states.
*/
void statusBar();

/********* Cell Editor Functions **********/

/*
* Function to update menu selection using key code parameter. Updates menu
* index as well as menu selection token. Previous highlighted option is
* reformatted and current menu option is highlighted.
*/
void option(const int code);

/*
* Function to Initialize the menu cell buffer. Searches for pre-existing cell 
* value using cell number. If no saved data exists, data is cleared and reset 
* to default values.
*/
void editor();

/*
* Function to edit the menu option at the current menu index. Calls the
* respective function to modify cell properties, save cell data, or clear the 
* cell buffer.
*/
void edit();

/*
* Function to edit cell friction. Validates input and clamps to specified 
* maximum or minimum value before updating menu cell buffer.
*/
void editCF();

/*
* Function to edit cell type. Validates input and clamps to specified 
* maximum or minimum value before updating menu cell buffer.
*/
void editTY();

/*
* Function to edit cell radiation level. Validates input and clamps to 
* specified maximum or minimum value before updating menu cell buffer.
*/
void editRL();

/*
* Function to edit cell ritterbarium. Validates input and clamps to 
* specified maximum or minimum value. Searches for existing RB within cell
* contents, updates quanitity if it exists or creates an entry with quantity
* if it does not exist.
*/
void editRB();

/*
* Function to save edited cell buffer to layer. Overwrites cell data if it 
* exists or creates a new cell with copied cell buffer data.
*/
void saveCell();

/*
* Function to erase edited cell data and set nonzero default values.
*/
void clearData();

#endif