/*
*  Credit is given to Dr. Larry Hughes for providing the reference code
*  that was used throughout this program. Specifically, the engine.h, vterminal.h, and vterminal.c files
*  which uses much of Dr. Hughes' work.
*/

#ifndef DISPLAY_H
#define DISPLAY_H

#include "vterminal.h"

#define MAP_ROWS 100
#define MAP_COLS 100

typedef struct Node
{
    char** grid;
    struct Node* next;
    struct Node* prev;
}Node;

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
void setMargins();
void initDisplay();
void render(); //renders the console
void pollWindow();
COORD getCursor();
void setCursor(short x, short y);

/*linked list functions for map layers*/
char** newGrid();
Node* newLayer();
void addLayer(Node** map);
Node* getActiveLayer();
void freeLayers(Node** map);

#endif