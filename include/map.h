#ifndef MAP_H
#define MAP_H

#include "vterminal.h"

#define MAP_ROWS 100
#define MAP_COLS 100

typedef struct Node
{
    char** grid;
    struct Node* next;
    struct Node* prev;
}Node;

/*linked list functions for map layers*/
char** newGrid();
Node* newLayer();
void addLayer(Node** map);
Node* getActiveLayer(const Node* map, const int n);
void freeLayers(Node** map);

#endif