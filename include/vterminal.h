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
#define CSI "\x1B["
#define CUU(n)      printf(CSI "%dA", (n)); //cursor up by n
#define CUD(n)      printf(CSI "%dB", (n)); //cursor down by n
#define CUF(n)      printf(CSI "%dC", (n)); //cursor forward (right) by n
#define CUB(n)      printf(CSI "%dD", (n)); //cursor backward (left) by n
#define CUP(x,y)    printf(CSI "%d;%dH", (y), (x)); //move cursor to position

//ANSI escape sequence for DEC Line Drawing Mode
#define EDLDM       printf(ESC "(0"); //starts DEC Line Drawing Mode
#define EAM         printf(ESC "(B"); //starts ASCII Mode


enum keyCodes
{
    ESC = '\x1B', //escape key ASCII code
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