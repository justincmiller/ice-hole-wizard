#ifndef CURSOR_H
#define CURSOR_H

#include "display.h"

void initCursor();
void updateCursor();
void setCursor(const short x, const short y);
void moveCursor(const int action);
void drawCursor(const int action);
void panViewport(const int action);

#endif