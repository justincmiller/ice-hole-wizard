/*
* mapfiles.h
*
* This file contains
*/

/*
*  Credit is given to Dr. Larry Hughes for providing the reference code
*  that was used both as inspiration and used as is throughout this program.
*/

#ifndef MAPFILES_H
#define MAPFILES_H

#include <fcntl.h>
#include "engine.h"

typedef struct Header
{
    unsigned short layers;
    unsigned long cells;
    long deleted;
}Header;

typedef struct defaultCell
{
    char glyph;
    unsigned char status;
    unsigned int cn;
    int el;
}defaultCell;

typedef struct editedCell
{
    char glyph;
    unsigned char status;
    unsigned int cn;
    int el;
    unsigned int cf;
    unsigned char ty;
    unsigned short rl;
    Mineral cc[CONTENTS];
}editedCell;

typedef struct deletedCell
{
    unsigned char status;
    long next;
}deletedCell;

union Record
{
    editedCell editcell;
    defaultCell defcell;
    deletedCell delcell;
    Header header;
};

enum recordStatus
{
    DEFAULT, EDITED, DELETED
};

#endif