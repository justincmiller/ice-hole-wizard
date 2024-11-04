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
#define GET_X(col) ((col) - 50)
#define GET_Y(row) (50 - (row))
#define GET_COL(x) ((x) + 50)
#define GET_ROW(y) (50 - (y))

//three states: cursor movement, map drawing, quit program
enum status
{
    MOVE, DRAW, QUIT
};

//actions to manipulate state and viewport settings
enum wizardActions
{
    GET_STATE, SET_STATE, GET_VIEW, SET_VIEW
};

//function declarations
void init(); //initializes console setup
void reset();
void wizard(const int action, void* data); //manages state and viewport tasks
int getState(); //gets current state
void setState(const int state); //sets state
void toggleState(); //toggles between move and draw states
void update(); //update program based on keyboard input

void draw(const char key); //handles draw state operations
void move(const char key); //handles move state operations
void parseKey(const char key); //handles non-movement keystrokes

#endif