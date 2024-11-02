#ifndef CAVE_WIZARD_H
#define CAVE_WIZARD_H

#include "vterminal.h"

enum status
{
    MOVE, DRAW, QUIT
};

enum actions
{
    GET_STATE, SET_STATE, GET_VIEW, SET_VIEW
};

void wizard(const int action, void* data);
void update();
void render();
void cursorKey(const char key);
void parseKey(const char key);

#endif