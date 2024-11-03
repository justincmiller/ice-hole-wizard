#ifndef DISPLAY_H
#define DISPLAY_H

#include "vterminal.h"

#define MAP_ROWS 100
#define MAP_COLS 101

typedef struct Node
{
    char** grid;
    struct Node* next;
    struct Node* prev;
}Node;

typedef struct Display
{
    int state;
    SMALL_RECT vp;
    Node* map;
    COORD cursor;
}Display;

void render(); //renders the console
char** newGrid();
Node* newLayer();
Node* addLayer(Node** map);
void freeLayers(Node** map);

#endif