/*
*  Credit is given to Dr. Larry Hughes for providing the reference code
*  that was used throughout this program. Specifically, the engine.h, vterminal.h, and vterminal.c files
*  which uses much of Dr. Hughes' work.
*/

#ifndef DISPLAY_H
#define DISPLAY_H

#include "engine.h"
#include "cursor.h"
#include "map.h"

typedef struct Display
{
    COORD cursor;
    COORD size;
    SMALL_RECT margin;
    Map* map;
}Display;

Display* getDisplay();
SMALL_RECT getWindow();
void resetMargins();
void initDisplay();
void render(); //renders the console
void pollWindow();

#endif