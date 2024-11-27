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

#define CN(x,y,z) (x + y * MAP_COLS + z * MAP_ROWS * MAP_COLS) //cell number

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
    short x;
    short y;
    short z;
    Data* data;     //cell properties
}Cell;

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
void addLayer(); //adds new layer 
void layerUp(); //saves current layer and moves up one layer
void layerDown(); //saves current layer and moves down one later or creates new layer
void lastLayer(); //moves to the last layer created
void topLayer(); //moves to map layer 0

/********** Cell Functions **********/
Cell* createCell(); //creates the cell properties data holder
Node* addCell(); //adds cell to map
void remCell(); //deletes cell and cell data from map

Node* searchCN(const unsigned int cn); //gets the cell number
int getRB(Data* data); //gets RB value from cell contents

#endif
