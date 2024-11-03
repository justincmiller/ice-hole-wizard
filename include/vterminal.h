#ifndef VTERMINAL_H
#define VTERMINAL_H

#include <stdio.h>
#include <stdlib.h>
#include <conio.h> //keyboard handling
#include <windows.h> //Windows API functions
#include <stdbool.h>

//supress security warnings
#define _CRT_SECURE_NO_WARNINGS

//ANSI escape sequence for Control Sequence Introducer for terminal controls
#define ESC "\x1b"
#define CSI "\x1b["
#define CUU(n)      printf(CSI "%dA", (n)); //cursor up by n
#define CUD(n)      printf(CSI "%dB", (n)); //cursor down by n
#define CUF(n)      printf(CSI "%dC", (n)); //cursor forward (right) by n
#define CUB(n)      printf(CSI "%dD", (n)); //cursor backward (left) by n
#define CUP(x,y)    printf(CSI "%d;%dH", (y), (x)); //move cursor to position

/*
*  Credit is given to Dr. Larry Hughes for providing the reference code
*  that was used throughout this program specifically the DEC Line Drawing Mode
*  which uses much of Dr. Hughes' work.
*  Credit to Dr. Hughes for the following code: lines 28 - 86
*/
/* Some commonly used VT-100 commands */
#define EL(r)		printf(CSI "%d;1H" CSI "K", (r)); /* Erase in Line */
#define CLRSCR		printf(CSI "2J");

/* Cursor keys */
#define KPNM	printf(ESC ">");	/* Enable Keypad Numeric Mode '[' */
#define ECKM	printf(CSI "?1h");	/* Enable Cursor Keys Application Mode 'O' */
#define DCKM	printf(CSI "?1l");	/* Disable Cursor Keys Application Mode */
#define EDLDM	printf(ESC "(0");	/* Enable DEC Line Drawing Mode */
#define EAM		printf(ESC "(B");	/* Enable ASCII Mode */

/* DEC colour: ESC [ <n> m */
enum VT100_COLOURS {
    FGBLACK = 30,
    FGRED, FGGREEN, FGYELLOW, FGBLUE, FGMAGENTA, FGCYAN, FGWHITE,
    BGBLACK = 40,
    BGRED, BGGREEN, BGYELLOW, BGBLUE, BGMAGENTA, BGCYAN, BGWHITE
};

/* DEC line drawing mode characters*/
enum symbol { LR, UR, UL, LL, XX, HR, TR, TL, TU, TD, VR, EM };

#define START_SYM '*'

enum direction { NORTH, SOUTH, EAST, WEST, IDLE };

/* Tunnel symbol to display [new dir][old dir] */
char cell_sym[4][5] = {
    /*	Old: NO  SO  ET  WT  IDL    New:*/
            {VR, VR, LR, LL, VR}, /* NO */
            {VR, VR, UR, UL, VR}, /* SO */
            {UL, LL, HR, HR, HR}, /* ET */
            {UR, LR, HR, HR, HR}  /* WT */
};

short del_y[] = { -1, 1, 0, 0 };
short del_x[] = { 0, 0, 1, -1 };

/* ASCII-to-DEC graphic characters */
enum symbol asc_dec[] = {
    'j', 'k', 'l', 'm', 'n', 'q', 't', 'u', 'v', 'w', 'x', ' ' };

int draw_object(int col, int MIN_COL, int MAX_COL, int row, int MIN_ROW, int MAX_ROW, int symbol)
{
    /*
     - Attempts to draw symbol at position col, row
     - Returns -1 if illegal location, 0 otherwise
    */
    if (col <= MIN_COL || row <= MIN_ROW ||
        col >= MAX_COL || row >= MAX_ROW)
        /* Outside view screen - do not draw */
        return -1;

    CUP(col, row);
    _putch(symbol);

    return 0;

}
//End of credited material.

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

#endif