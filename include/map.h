/*
* map.h
*
* Contains struct definitions for layers, maps, cells, cell properties.
* 
* Also contains functions for manipulating layers, map, cells, and cell properties.
*/

/*
*  Credit is given to Dr. Larry Hughes for providing the reference code
*  that was used both as inspiration and used as is throughout this program.
*/

#ifndef MAP_H
#define MAP_H

#include "utils.h" //VT100 I/O & linked-lists

//sysyem information struct
struct Display;

#define LATENT_CC -1 //no cell content or end of list
#define MAX_CF  10
#define DEF_CF  5

#define CN(x,y) (x + y * MAP_COLS) //cell number
#define CN_X(cn) ((cn) % MAP_COLS)
#define CN_Y(cn) ((cn) / MAP_COLS)

//struct to hold layer data
typedef struct Layer
{
    Node* cells;    //list to hold cells
    char** grid;    //2d array to hold glyphs
    unsigned short depth;    //current layer depth
    COORD cursor;
}Layer;

//structure to hold map data
typedef struct Map
{
    Node* matrix;   //list to hold layers
    Layer* layer;   //active layer
    Layer* last;    //previous layer
    unsigned short depth;    //layer count
}Map;

//structure to hold cell mineral data
typedef struct Mineral
{
    int code; //type of mineral
    int qty;  //amount of mineral
}Mineral;

#ifdef ENABLE_CC
//structure to hold cell data
typedef struct Cell
{
    unsigned int cn;      // cell number
    int el;               // elevation
    unsigned int cf;      // friction
    unsigned char ty;     // type
    unsigned short rl;    // radiation level
    Mineral cc[CONTENTS]; // cell contents
}Cell;

int getRB(Cell* cell); //gets RB value from cell contents
#else
//structure to hold cell data
typedef struct Cell
{
    unsigned int cn;      // cell number
    int el;               // elevation
    unsigned int cf;      // friction
    unsigned char ty;     // type
    unsigned short rl;    // radiation level
    int rb;               // ritterbarium
}Cell;
#endif

//todo: update with additional mineral types
//valid mineral types
enum minerals
{
    RB, BILLIUM, JUSTINIUM
};

//valid data types for cell properties
enum data
{
    CN, EL, CF, TY, RL, CC
};

//functions
void loadMap(struct Display* ptr); //loads layer and corresponding map data

/********** Layer Functions **********/
char** createGrid(); //creates the character grid
Layer* createLayer(); //creates a new layer
Layer* addLayer(); //adds new layer 
void layerUp(); //saves current layer and moves up one layer
void layerDown(); //saves current layer and moves down one later or creates new layer
void lastLayer(); //moves to the last layer created
void topLayer(); //moves to map layer 0

/********** Cell Functions **********/
Cell* createCell(); //creates the cell properties data holder
Cell* addCell(Layer* layer); //adds cell to map
void remCell(); //deletes cell and cell data from map

Node* searchCN(const unsigned int cn); //gets the cell number

#endif
