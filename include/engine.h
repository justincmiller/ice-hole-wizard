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

//three states: cursor movement, map drawing, quit program
enum status
{
    MOVE, DRAW, QUIT
};

//function declarations
void init(); //initializes console setup
void reset();
int getState(); //gets current state
void toggleState(); //toggles between move and draw states
void update(); //update program based on keyboard input

void draw(const char key); //handles draw state operations
void move(const char key); //handles move state operations
void parseKey(const char key); //handles non-movement keystrokes

#endif