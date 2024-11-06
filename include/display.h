/*
*  Credit is given to Dr. Larry Hughes for providing the reference code
*  that was used throughout this program. Specifically, the engine.h, vterminal.h, and vterminal.c files
*  which uses much of Dr. Hughes' work.
*/

#ifndef DISPLAY_H
#define DISPLAY_H

#include "vterminal.h"
#include "map.h"

typedef struct Display
{
    int state, layer;
    SMALL_RECT vp;
    SMALL_RECT margin;
    int width, height;
    Node* map;
    COORD cursor;
}Display;

SMALL_RECT getWindow();
void resetMargins();
void initDisplay();
void render(); //renders the console
void pollWindow();
void setCursor(const short x, const short y);
void moveCursor(const int action);

#endif