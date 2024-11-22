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

#define MAP_ROWS 100
#define MAP_COLS 100

#define GRID_MAX 99
#define GRID_MIN 0

#define CELL_WIDTH  10
#define CELL_HEIGHT 10

#define CLAMP_X(x) ((x) < 0 ? 0 : ((x) > 99 ? 99 : (x)))
#define CLAMP_Y(y) ((y) < 0 ? 0 : ((y) > 99 ? 99 : (y)))

#define ESC "\x1b"
#define CSI "\x1b["

#define CUP(x,y)        printf(CSI "%d;%dH", (y), (x)) //move cursor to position
#define ECH(n)          printf(CSI "%dX", (n))
#define RESET           printf(CSI "H")
#define CLEAR           printf(CSI "2J" CSI "3J")
#define HIDE_CURSOR     printf(CSI "?25l")
#define SHOW_CURSOR     printf(CSI "?25h")
#define ALT_SCREEN      printf(CSI "?1049h")
#define MAIN_SCREEN     printf(CSI "?1049l")

//label latent character - ASCII space
#define LATENT 0x20

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
#define INACTIVE(string) LDM(CSI "30;43m" string CSI "0m")

/*
* format string for active line
* consists of two escape sequences:
* \x1b[30;43m   - set foreground; background to 30 (black) and 47 (white)
* \x1b[0m       - reset to default colours
*/
#define ACTIVE(string) LDM(CSI "30;47m" string CSI "0m")

//format string to maintain cursor position when printing
#define FIXED CSI "D"

//do-while(0) ensures this macro expands consitently
#define ASSERT(ptr) \
    do { \
        if (ptr == NULL) { \
            printf(">> Error: memory failure.\n"); \
            purge(); \
            exit(EXIT_FAILURE); \
        } \
    } while (0)


#define ARRAY_LEN(arr) (sizeof(arr) / sizeof((arr)[0]))


enum status
{
    MOVE = (1 << 0), 
    DRAW = (1 << 1), 
    EDIT = (1 << 2),
    QUIT = (1 << 3)
};

enum directions
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

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

typedef struct Node
{
    void* data;
    struct Node* next;
    struct Node* prev;
}Node;

bool virtualInput();
bool virtualOutput();

void addNode(Node** head, void* data);
void track(void* ptr);
void purge();

#endif