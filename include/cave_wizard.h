#ifndef CAVE_WIZARD_H
#define CAVE_WIZARD_H

#include "vterminal.h"

#define GET_X(col) ((col) - 50)
#define GET_Y(row) (50 - (row))
#define GET_COL(x) ((x) + 50)
#define GET_ROW(y) (50 - (y))

enum status
{
    MOVE, DRAW, QUIT
};

enum actions
{
    GET_STATE, SET_STATE, GET_VIEW, SET_VIEW
};

void init();
void wizard(const int action, void* data);
int getState();
void setState(const int state);
void toggleState();
void update();
void render();
void draw(const char key);
void move(const char key);
void parseKey(const char key);

#endif