#ifndef MAP_H
#define MAP_H

#include "utils.h"

struct Display;

#define BUFF_LEN 8

#define CN(x,y,z) (x + y * MAP_COLS + z * MAP_ROWS * MAP_COLS)

typedef struct Layer
{
    Node* cells;    //list to hold cells
    char** grid;    //2d array to hold glyphs
    unsigned short depth;    //current layer depth
    COORD cursor;
}Layer;

typedef struct Map
{
    Node* matrix;   //list to hold layers
    Layer* layer;   //active layer
    Layer* last;    //previous layer
    unsigned short depth;    //layer count
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
    short x;
    short y;
    short z;
    Data* data;     //cell properties
}Cell;

//todo: update with additional mineral types
enum minerals
{
    RB = 1, M2, M3
};

enum data
{
    CN, EL, CF, TY, RL, CC
};

void loadMap(struct Display* ptr);
char** createGrid();
Layer* createLayer();
void addLayer();
Cell* createCell();
Node* addCell();
//void editCell();
//void editValue();
//void saveCell();
int getRB(Data* data);
void layerUp();
void layerDown();
void lastLayer();

#endif