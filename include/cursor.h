#ifndef CURSOR_H
#define CURSOR_H

#include "display.h"

void initCursor();
void updateCursor();
void setCursor(const short x, const short y);
void moveCursor(const int action);
bool connector(int dir, char c);
char lineType(int row, int col);
void drawCursor(const int action);
void activate();
void deactivate();
void panViewport(const int action);
void menu(const int action);

#endif