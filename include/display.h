/*
*  Credit is given to Dr. Larry Hughes for providing the reference code
*  that was used both as inspiration and used as is throughout this program.
*/

#ifndef DISPLAY_H
#define DISPLAY_H

#include "map.h"

#define INPUT 0x01

// //struct to hold all the cell properties
//typedef struct Menu
// {
//     short index;
//     char** values; //cell properties and amounts
//     char** options; 
//     Cell* cell; 
// }Menu;
struct Menu;

//structure that holds all the system information
typedef struct Display
{
    COORD* cursor; //cursor position
    COORD size; //window size
    SMALL_RECT margin; //window margins
    Map* map; //pointer to map structure
    int state; //system state: drawing, moving, quit
    struct Menu* editor; //cell content editing menu
}Display;

SMALL_RECT getWindow();
void setWindow();
void resetMargins();
void loadDisplay(Display** ptr);
//void loadMenu();
void render();
void pollWindow();
void viewport();
//void overlay();
//void container();
//void statusBar();

#endif
