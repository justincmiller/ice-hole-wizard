/*
*  Credit is given to Dr. Larry Hughes for providing the reference code
*  that was used both as inspiration and used as is throughout this program.
*/

#ifndef MAP_H
#define MAP_H

#include "utils.h" //VT100 I/O & linked-lists

//sysyem information struct
struct Display;

#define BUFF_LEN 8

//structure to hold layer data
typedef struct Layer
{
    Node* cells;    //list to hold cells
    char** grid;    //2d array to hold glyphs
    short depth;    //current layer depth
}Layer;

//structure to hold map data
typedef struct Map
{
    Node* matrix;   //list to hold layers
    Layer* layer;   //active layer
    Layer* last;    //previous layer
    short depth;    //layer count
}Map;

//structure to hold cell mineral data
typedef struct Mineral
{
    int code; //type of mineral
    int qty; //amount of mineral
}Mineral;

//structure to hold cell properties data
typedef struct Data
{
    unsigned int cn;      // cell number
    int el;               // elevation
    unsigned int cf;      // friction
    unsigned char ty;     // type
    unsigned short rl;    // radiation level
    Mineral cc[CONTENTS]; // cell contents
}Data;

//structure to hold cell data
typedef struct Cell
{
    COORD pos;      //position
    Data* data;     //cell properties
}Cell;

//todo: update with additional mineral types
//valid mineral types
enum minerals
{
    RB = 1, M2, M3
};

//valid data types for cell properties
enum data
{
    CN, EL, CF, TY, RL, CC
};

//functions
void loadMap(struct Display* ptr); //layer operations and corresponding maps
char** createGrid(); 
Layer* createLayer();
void addLayer();
Cell* createCell();
void addCell();
void editCell();
void editValue();
int getRB(Data* data);

#endif
