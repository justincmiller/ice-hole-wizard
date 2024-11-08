/*
*  Credit is given to Dr. Larry Hughes for providing the reference code
*  that was used throughout this program. Specifically, the engine.h, vterminal.h, and vterminal.c files
*  which uses much of Dr. Hughes' work.
*/

#ifndef DISPLAY_H
#define DISPLAY_H

#include "vterminal.h"
#include "engine.h"
#include "cursor.h"
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

Display* getDisplay();
SMALL_RECT getWindow();
void resetMargins();
void initDisplay();
Node* getActiveLayer();
void render(); //renders the console
void pollWindow();
void freeDisplay();

#endif