#ifndef VTERMINAL_H
#define VTERMINAL_H

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <stdbool.h>

#define _CRT_SECURE_NO_WARNINGS

#define CSI "\x1B["

enum keyCodes
{
    ESC = '\x1B',
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

void init();

#endif