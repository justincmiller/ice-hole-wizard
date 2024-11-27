/*
* interface.h
* 
* Contains Menu and Token structure definitions, Token, UI element and editor
* function declarations, and menu and editor constants.
*/
#ifndef INTERFACE_H
#define INTERFACE_H

#include "map.h" //layers, map, cells, etc.

#define MENU_ROWS 20
#define MENU_COLS 30

//number of middle rows offset by 2 to accomodate top/bottom borders
#define BORDER_ROWS (MENU_ROWS - 2)
//number of columns offset by 1 to accomodate for corners and NUL terminator
#define BORDER_COLS (MENU_COLS - 1)

#define SECTION 2 //section break value

#define COLOUR     0x7F         //bitmask for colour extraction
#define FMT_BG     (1 << 14)    //bitmask for foreground bit
#define FMT_FG     (1 << 15)    //bitmask for background bit
#define BG         7            //bit shift for background
#define TOKEN      32           //maximum token string size for

#define MENU_MAX   5  //maximum array index for menu options
#define MENU_MIN   0  //minimum array index for menu options

#define MENU_FIXED 2  //amount of fixed menu data items
#define MENU_VARS  4  //amount of variable menu data items

#define MENU_DATA  6  //amount of menu data items
#define TEXT_X     3  //starting column for text printing
#define DATA_X     20 //starting column for data printing

#define OPTIONS    6  //number of selectable menu options

#define MAX_PERCENT 100 //maximum percent value

struct Display;

typedef struct Token
{
    char* string;
    unsigned short fmt;
    short x;
    short y;
}Token;

typedef struct Menu
{
    short index;
    Token* header;
    Token** text;
    Token** data;
    Token** options;
    Token* selection;
    Cell* cell;
}Menu;

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

enum lines
{
    UL = 0x6a, DL, DR, UR, UDLR,
    LR = 0x71,
    UDR = 0x74, UDL, ULR, DLR, UD
};

enum options
{
    FRICTION, TYPE, RADIATION, RITTERBARIUM, SAVE_CELL, RESET_CELL
};

void loadMenu(struct Display* ptr);

/********** Token Functions **********/
Token* createTk(const short x, const short y);
void printTk(Token* tk);
void formatTk(Token* tk, const unsigned short fg, const unsigned short bg);

/********** UI Element Functions **********/
void overlay();
void container();
void statusBar();

/********* Cell Editor Functions **********/
void option(const int code);
void editor();
void edit();
void editCF();
void editTY();
void editRL();
void editRB();
void saveCell();
void clearData();

#endif