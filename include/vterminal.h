/*
*  Credit is given to Dr. Larry Hughes for providing the reference code
*  that was used throughout this program specifically the DEC Line Drawing Mode
*  and virtual terminal code which uses much of Dr. Hughes' work.
*/

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

/* Tunnel symbol to display [new dir][old dir] */
extern char cell_sym[4][5];

extern short del_y[];
extern short del_x[];

/* ASCII-to-DEC graphic characters */
extern enum symbol asc_dec[];

int draw_object(int col, int row, int symbol); //Dr. Hughe's function

typedef struct Draw_Cursor
{
    int x;
    int y;
    int curr_dir;
    int old_x;
    int old_y;
    int old_dir;
}Draw_Cursor;

extern void draw_cursor_init();
extern void draw_cursor_move(char);

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

bool virtualInput();
bool virtualOutput();

#endif