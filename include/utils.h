/*
*  Credit is given to Dr. Larry Hughes for providing the reference code
*  that was used both as inspiration and used as is throughout this program.
*/

#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <conio.h> //keyboard handling
#include <windows.h> //Windows API functions
#include <stdbool.h>
#include <string.h>

//supress security warnings
#define _CRT_SECURE_NO_WARNINGS

//coordinate conversions
#define COL_X(col) ((col) - 49)
#define ROW_Y(row) (50 - (row))
#define X_COL(x) ((x) + 49)
#define Y_ROW(y) (50 - (y))

//map size
#define MAP_ROWS 100
#define MAP_COLS 100

//viewport size
#define GRID_MAX 99
#define GRID_MIN 0

//cell size
#define CELL_WIDTH  10
#define CELL_HEIGHT 10

//boundries
#define CLAMP_X(x) ((x) < 0 ? 0 : ((x) > 99 ? 99 : (x)))
#define CLAMP_Y(y) ((y) < 0 ? 0 : ((y) > 99 ? 99 : (y)))

//VT100 definitions
#define ESC "\x1b"
#define CSI "\x1b["
#define CUP(x,y)        printf(CSI "%d;%dH", (y), (x))
#define ECH(n)          printf(CSI "%dX", (n))
#define RESET           printf(CSI "H")
#define CLEAR           printf(CSI "2J" CSI "3J")
#define HIDE_CURSOR     printf(CSI "?25l")
#define SHOW_CURSOR     printf(CSI "?25h")
#define SAVE_CURSOR     printf(CSI "7")
#define RESTORE_CURSOR  printf(CSI "8")
#define BLOCK_CURSOR    printf(CSI "2 q")
#define SHOW_BLOCK      printf(CSI "?25h" CSI "2 q")
#define ALT_SCREEN      printf(CSI "?1049h")
#define MAIN_SCREEN     printf(CSI "?1049l")

//Special ASCII characters
#define LATENT    0x20
#define PLUS      0x2B
#define MINUS     0x2D
#define NONE      0x5F

//Keyboard Input values for pollKbInput function
#define SQLEN 7 
#define CONTROLS 17
#define CONTENTS 3

/* 
* format string for line drawing mode
* consists of two escape sequences:
* \x1b(0  - enable line drawing mode
* \x1b(B  - enable ASCII mode
*/
#define LDM(string) ESC "(0" string ESC "(B"

/*
* format string for inactive line
* consists of two escape sequences:
* \x1b[30;43m  - set foreground; background to 30 (black) and 43 (yellow)
* \x1b[0m      - reset to default colours
*/
#define PATH(string)    LDM(CSI "30;43m" string CSI "0m")

#define PORT_DN(string) LDM(CSI "37;42m" string CSI "0m")
#define PORT_UP(string) LDM(CSI "37;41m" string CSI "0m")

#define MARKER(string)  LDM(CSI "30;103m" string CSI "0m")

#define GLYPH(a, b) (a) = ((a) == PLUS || (a) == MINUS) ? (a) : (b)

/*
* format string for active line
* consists of two escape sequences:
* \x1b[30;43m   - set foreground; background to 30 (black) and 47 (white)
* \x1b[0m       - reset to default colours
*/
#define ACTIVE(string) LDM(CSI "30;47m" string CSI "0m")

//format string to maintain cursor position when printing
#define FIXED CSI "D"

#define ARRAY_LEN(arr) (sizeof(arr) / sizeof((arr)[0]))
#define FLUSH          while (getchar() != '\n')

//system state options
#define APPEND 1

enum status
{
    MOVE = (1 << 0), 
    DRAW = (1 << 1), 
    EDIT = (1 << 2),
    QUIT = (1 << 3)
};

//movement directions
enum directions
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

//possible valid key press options
enum keyCodes
{
    P = 1,
    ENTER,
    ARROW_UP,
    ARROW_DOWN,
    ARROW_RIGHT,
    ARROW_LEFT,
    HOME,
    END,
    INSERT,
    DEL,
    PG_UP,
    PG_DN,
    CTRL_UP,
    CTRL_DOWN,
    CTRL_RIGHT,
    CTRL_LEFT,
    ESCAPE
};

//struct for link-list node
typedef struct Node
{
    void* data;
    struct Node* next;
    struct Node* prev;
}Node;

//functions
bool virtualInput(); //initializes VT100 Input
bool virtualOutput(); //initializes VT100 Output
void addNode(Node** head, void* data); //handles adding to linked-lists
void removeNode(Node** head, Node* node);
Node* getNode(Node* head, const unsigned int n);
void track(void* ptr); //handles updating linked-list "heap"
void assert(void* ptr, const short action);
void forget(void* ptr);
void purge(); //frees memory from malloc'd linked-lists

#endif
