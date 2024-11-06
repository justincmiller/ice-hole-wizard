/*
*  Credit is given to Dr. Larry Hughes for providing the reference code
*  that was used throughout this program. Specifically, the engine.h, vterminal.h, and vterminal.c files
*  which uses much of Dr. Hughes' work.
*/

#ifndef ENGINE_H
#define ENGINE_H

//include the vterminal header for terminal control and key codes
#include "vterminal.h"

//macros for converting between internal array and map
#define X_COL(col) ((col) - 50)
#define Y_ROW(row) (50 - (row))
#define COL_X(x) ((x) + 50)
#define ROW_Y(y) (50 - (y))

#define CLAMP_X(x) ((x) < 0 ? 0 : ((x) > 99 ? 99 : (x)))
#define CLAMP_Y(y) ((y) < 0 ? 0 : ((y) > 99 ? 99 : (y)))

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

void draw(const char key); //handles draw state operations
void move(const char key); //handles move state operations
void parseKey(const char key); //handles non-movement keystrokes

#endif