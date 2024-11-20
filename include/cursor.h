#ifndef CURSOR_H
#define CURSOR_H

#include "engine.h"

struct Display;

void loadCursor(struct Display* ptr);
void updateCursor();
void setCursor(const short x, const short y);
void move(const int code);
void draw(const int code);
bool connector(int dir, char c);
char lineType(char** grid, int row, int col);
void panViewport(const int code);

#endif