/*
* display.h
*
* Contains
*/

/*
*  Credit is given to Dr. Larry Hughes for providing the reference code
*  that was used both as inspiration and used as is throughout this program.
*/

#ifndef DISPLAY_H
#define DISPLAY_H

#include "map.h" //layers, map, cells, etc.

#define INPUT 0x01

struct Menu;

//structure that holds all the system information
typedef struct Display
{
    COORD* cursor; //cursor position
    COORD size; //window size
    SMALL_RECT margin; //window margins
    Map* map; //pointer to map structure
    int state; //system state: drawing, moving, quit
    struct Menu* menu; //cell content editing menu
}Display;

void loadDisplay(Display** ptr);
SMALL_RECT getWindow();
void setWindow();
void resetMargins();
void render();
void pollWindow();
void viewport();

#endif
