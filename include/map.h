#ifndef MAP_H
#define MAP_H

#include "utils.h"

struct Display;

typedef struct Layer
{
    Node* cells;    //list to hold cells
    char** grid;    //2d array to hold glyphs
    short depth;    //current layer depth
}Layer;

typedef struct Map
{
    Node* matrix;   //list to hold layers
    Layer* layer;   //active layer
    Layer* last;    //previous layer
    short depth;    //layer count
}Map;

typedef struct Mineral
{
    int code;
    int qty;
}Mineral;

typedef struct Data
{
    unsigned int cn;      // cell number
    int el;               // elevation
    unsigned int cf;      // friction
    unsigned char ty;     // type
    unsigned short rl;    // radiation level
    Mineral cc[CONTENTS]; // cell contents
}Data;

typedef struct Cell
{
    COORD pos;      //position
    Data* data;     //cell properties
}Cell;



//todo: update with additional mineral types
enum minerals
{
    RB, M2, M3
};

void loadMap(struct Display* ptr);
char** createGrid();
Layer* createLayer();
void addLayer();
Cell* createCell();
void addCell();
void editCell();

#endif