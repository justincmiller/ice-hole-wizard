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

#include "engine.h"

typedef struct Metadata
{
    unsigned short layers;
    unsigned long cells;
    long deleted;
}Metadata;

typedef struct editedCell
{
    unsigned char status;
    char glyph;
    unsigned int cn;
    int el;
    unsigned int cf;
    unsigned char ty;
    unsigned short rl;
    int rb;
}editedCell;

typedef struct deletedCell
{
    unsigned char status;
    long next;
}deletedCell;

typedef union Record
{
    editedCell cell;
    deletedCell del;
    Metadata meta;
}Record;

enum recordStatus
{
    DEFAULT, EDITED, DELETED
};

#endif