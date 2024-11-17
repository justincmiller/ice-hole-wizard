#ifndef CURSOR_H
#define CURSOR_H

#include "display.h"

void initCursor();
void updateCursor();
void setCursor(const short x, const short y);
void moveCursor(const int action);
bool connector(int dir, char c);
char lineType(char** grid, int row, int col);
void drawCursor(const int action);
void deactivate();
void panViewport(const int action);

#endif