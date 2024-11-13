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

#define MAP_ROWS 100
#define MAP_COLS 100

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
char** newGrid();
void addLayer();
void initDisplay();
Node* getActiveLayer();
void render(); //renders the console
void pollWindow();

#endif