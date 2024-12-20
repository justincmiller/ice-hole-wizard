/*
* display.h
*
* Contains structure for display that holds all the system information.
* 
* Also contains function definitions for the manipulation of the display and viewport.
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

void loadDisplay(Display** ptr); //loads display
SMALL_RECT getWindow(); //initializes window
void setWindow(); //sets the window dimensions
void resetMargins(); //adjust margins
void render(); //refresh
void pollWindow(); //get window bounds
void viewport(); //viewport function

#endif
