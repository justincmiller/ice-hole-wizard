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
    int state;
    SMALL_RECT vp;
    SMALL_RECT margin;
    int width, height;
    Node* map;
    COORD cursor;
}Display;

enum displayActions
{
    GET_VP, SET_VP, GET_MAP, SET_MAP, GET_CUR, SET_CUR, GET_DISPLAY
};

void display(const int action, void* data);
void getWindow(SMALL_RECT* vp);
void render(); //renders the console
char** newGrid();
Node* newLayer();
void addLayer(Node** map);
void freeLayers(Node** map);

#endif