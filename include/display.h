#ifndef DISPLAY_H
#define DISPLAY_H

#include "vterminal.h"

#define MAP_ROWS 100
#define MAP_COLS 100

typedef struct Node
{
    char** grid;
    struct Node* next;
}Node;

typedef struct Display
{
    int state;
    SMALL_RECT vp;
    Node* map;
}Display;

void render(); //renders the console
char** newLayer();

#endif