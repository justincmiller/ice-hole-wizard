#ifndef INTERFACE_H
#define INTERFACE_H

#include "map.h"

#define MENU_ROWS 20
#define MENU_COLS 30

//row offset of 2 to accomodate top/bottom borders
#define BORDER_ROWS (MENU_ROWS - 2)
//column offset of 1 to accomodate for corners and NUL terminator
#define BORDER_COLS (MENU_COLS - 1)

#define SECTION 2

#define COLOUR 0x7F
#define FMT_BG (1 << 14)
#define FMT_FG (1 << 15)
#define BG     7
#define TOKEN  32

#define MENU_MAX 5
#define MENU_MIN 0

#define MENU_FIXED 2
#define MENU_VARS  4

#define MENU_DATA 6
#define TEXT_X    3
#define DATA_X    20

#define OPTIONS   6

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
Token* createTk(const short x, const short y);
void printTk(Token* tk);
void formatTk(Token* tk, const unsigned short fg, const unsigned short bg);
void overlay();
void container();
void statusBar();
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