/*
*  Credit is given to Dr. Larry Hughes for providing the reference code
*  that was used throughout this program. Specifically, the engine.h, vterminal.h, and vterminal.c files
*  which uses much of Dr. Hughes' work.
*/

#ifndef ENGINE_H
#define ENGINE_H

//include the vterminal header for terminal control and key codes
#include "utils.h"
#include "display.h"

//function declarations
void init(); //initializes console setup
int status(); //gets current state
void pollInput();

void draw(const char key); //handles draw state operations
void move(const char key); //handles move state operations
void modify(const char key);
void parseKey(const char key); //handles non-movement keystrokes

#endif