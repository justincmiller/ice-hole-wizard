/*
*  Credit is given to Dr. Larry Hughes for providing the reference code
*  that was used throughout this program. Specifically, the engine.h, vterminal.h, and vterminal.c files
*  which uses much of Dr. Hughes' work.
*/

#ifndef DISPLAY_H
#define DISPLAY_H

#include "engine.h"
#include "cursor.h"

typedef struct Display
{
    COORD cursor;
    COORD size;
    SMALL_RECT margin;
    Node* map;
    int depth;
    int layer;
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