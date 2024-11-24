/*
*  Credit is given to Dr. Larry Hughes for providing the reference code
*  that was used both as inspiration and used as is throughout this program.
*/

#ifndef ENGINE_H
#define ENGINE_H

//include headers
#include "utils.h" //VT100 I/O & linked-lists
#include "style.h" //menu formatting
#include "map.h" //layers, map, cells, etc.
#include "display.h" //system information and menus
#include "cursor.h" //cursor movement
//include the vterminal header for terminal control and key codes
#include "utils.h"
#include "map.h"
#include "display.h"
#include "cursor.h"
#include "interface.h"

//function declarations
void init(); //initializes console setup
int status(); //gets current state
void pollKbInput(); //checks to see if key was pressed
void drawControls(const int code); //handles draw state operations
void moveControls(const int code); //handles move state operations
void editControls(const int code); //handles edit state operations

#endif
