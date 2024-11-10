/*
*  Credit is given to Dr. Larry Hughes for providing the reference code
*  that was used throughout this program. Specifically, the engine.h, vterminal.h, and vterminal.c files
*  which uses much of Dr. Hughes' work.
*/

#ifndef ENGINE_H
#define ENGINE_H

//include the vterminal header for terminal control and key codes
#include "vterminal.h"
#include "display.h"
#include "utils.h"

//macros for converting between internal array and map

#define CLEAR_SCREEN        printf(CSI "2J");
#define CLEAR_SCROLLBACK    printf(CSI "3J");

//three states: cursor movement, map drawing, quit program
enum status
{
    MOVE, DRAW, QUIT
};

enum directions
{
    UP, DOWN, LEFT, RIGHT
};

//function declarations
void init(); //initializes console setup
int status(); //gets current state
void pollInput();

void draw(const char key); //handles draw state operations
void move(const char key); //handles move state operations
void parseKey(const char key); //handles non-movement keystrokes

void purge();
#endif