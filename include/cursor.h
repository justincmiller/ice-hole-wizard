/*
*  Credit is given to Dr. Larry Hughes for providing the reference code
*  that was used both as inspiration and used as is throughout this program.
*/

#ifndef CURSOR_H
#define CURSOR_H

#include "engine.h" //program operations

void loadCursor(struct Display* ptr); //loads cursor onto map
void updateCursor(); //updates cursor position
void setCursor(const short x, const short y); //move cursor using absolute parameters
void move(const int code); //cursor movement operations
void draw(const int code); //map drawing operations
bool connector(int dir, char c); //intersection handling
char lineType(char** grid, int row, int col); //line type handling
void panViewport(const int code); //handles moving viewport
void option(const int code); //alt-screen menu options
void updateMenu(const short dy); //highlighting alt-screen menu choices

#endif
