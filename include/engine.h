/*
*  Credit is given to Dr. Larry Hughes for providing the reference code
*  that was used both as inspiration and used as is throughout this program.
*/

#ifndef ENGINE_H
#define ENGINE_H

//include the vterminal header for terminal control and key codes
#include "utils.h"
#include "style.h"
#include "map.h"
#include "display.h"
#include "cursor.h"

//function declarations
void init(); //initializes console setup
int status(); //gets current state
void pollKbInput();

void drawControls(const int code); //handles draw state operations
void moveControls(const int code); //handles move state operations
void editControls(const int code);

#endif
