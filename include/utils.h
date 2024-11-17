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

#define CLAMP_X(x) ((x) < 0 ? 0 : ((x) > 99 ? 99 : (x)))
#define CLAMP_Y(y) ((y) < 0 ? 0 : ((y) > 99 ? 99 : (y)))

#define ESC "\x1b"
#define CSI "\x1b["

#define CUP(x,y)        printf(CSI "%d;%dH", (y), (x)) //move cursor to position
#define CLEAR           printf(CSI "2J" CSI "3J")
#define HIDE_CURSOR     printf(CSI "?25l")
#define SHOW_CURSOR     printf(CSI "?25h")

#define SPACE 0x20

/*
* format strings for line drawing - consists of 4 escape sequences.
* \x1b(0        - enable line drawing mode
* \x1b[<n>;<n>m - set foreground and background colour respectively
* \x1b[0m       - reset to default colours
* \x1b(B        - enable ASCII mode
* inactive colours: 30 (black), 43 (yellow)
* active colours:   37 (white), 43 (yellow)
*/
#define INACTIVE_LINE   "\x1b(0\x1b[30;43m%c\x1b[0m\x1b(B"
#define ACTIVE_LINE     "\x1b(0\x1b[37;43m%c\x1b[0m\x1b(B"

//format string to maintain cursor position when printing
#define FIXED           "\x1b[D"

//do-while(0) ensures this macro expands consitently
#define ASSERT(ptr) \
    do { \
        if (ptr == NULL) { \
            printf(">> Error: memory failure.\n"); \
            purge(); \
            exit(EXIT_FAILURE); \
        } \
    } while (0)

enum status
{
    MOVE, DRAW, QUIT
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
    CTRL = '1',
    INSERT = '2',
    DEL = '3',
    PG_UP = '5',
    PG_DN = '6',
    ARROW_UP = 'A',
    ARROW_DOWN = 'B',
    ARROW_RIGHT = 'C',
    ARROW_LEFT = 'D',
    HOME = 'H',
    END = 'F'
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