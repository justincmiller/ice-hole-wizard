#ifndef CAVE_WIZARD_H
#define CAVE_WIZARD_H

#include "vterminal.h"

#define XPOS(col) (col - 50)
#define YPOS(row) (row - 50)

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