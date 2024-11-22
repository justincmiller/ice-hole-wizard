/*
*  Credit is given to Dr. Larry Hughes for providing the reference code
*  that was used throughout this program. Specifically, the engine.h, vterminal.h, and vterminal.c files
*  which uses much of Dr. Hughes' work.
*/

#ifndef DISPLAY_H
#define DISPLAY_H

#include "map.h"

#define INPUT 0x01
#define OPTIONS 5

typedef struct Menu
{
    short index;
    char** values;
    char** options;
    Cell* cell;
}Menu;

typedef struct Display
{
    COORD cursor;
    COORD size;
    SMALL_RECT margin;
    Map* map;
    int state;
    Menu edit;
}Display;

SMALL_RECT getWindow();
void setWindow();
void resetMargins();
void loadDisplay(Display** ptr);
void loadMenu();
void render();
void pollWindow();
void viewport();
void overlay();
void container();
void statusBar();

#endif