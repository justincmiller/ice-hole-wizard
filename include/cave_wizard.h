#ifndef CAVE_WIZARD_H
#define CAVE_WIZARD_H

#include "vterminal.h"

enum status
{
    GET, MOVE, DRAW, QUIT
};

int status(const int action);
void update();
void render();
void cursorKey(const char key);
void parseKey(const char key);

#endif