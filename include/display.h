/*
*  Credit is given to Dr. Larry Hughes for providing the reference code
*  that was used throughout this program. Specifically, the engine.h, vterminal.h, and vterminal.c files
*  which uses much of Dr. Hughes' work.
*/

#ifndef DISPLAY_H
#define DISPLAY_H

#include "engine.h"

#define MENU_ROWS 20
#define MENU_COLS 30

//row offset of 2 to accomodate top/bottom borders
#define BORDER_ROWS (MENU_ROWS - 2)
//column offset of 1 to accomodate for corners and NUL terminator
#define BORDER_COLS (MENU_COLS - 1)

#define CONTAINER_X 3
#define CONTAINER_Y 3

#define PAD_LEFT 2

struct Map;

typedef struct Display
{
    COORD cursor;
    COORD size;
    SMALL_RECT margin;
    struct Map* map;
    int state;
}Display;

SMALL_RECT getWindow();
void setWindow();
void resetMargins();
void loadDisplay(Display** ptr);
void render();
void refresh();
void pollWindow();
void viewport();
void overlay();
void statusBar();

#endif