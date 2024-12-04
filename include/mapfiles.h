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

#include "utils.h"

#define ARGUMENT   1
#define PATH_LEN   128
#define EXTENSION  ".licemap"
#define TITLE(s)      CSI "2;2H" CSI "2K" CSI "94m" s CSI "0m"
#define SUBTITLE(s)   CSI "3;2H" CSI "2K" CSI "94m" s CSI "0m"
#define LABEL_NEW     CSI "5;2H" CSI "2K"
#define LABEL_LOAD    CSI "6;2H" CSI "2K"
#define LABEL_QUIT    CSI "7;2H" CSI "2K"
#define FILE_INPUT    CSI "9;2H" CSI "2K"
#define MESSAGE       CSI "10;2H" CSI "2K"

#ifdef _WIN32
#define SLASH '\\'
#else
#define SLASH '/'
#endif

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
    UNEDITED, EDITED, DELETED
};


bool importFile(const char *fname);

void filePrompt();

void fileInput();

bool fileStatus();

bool exportFile();

bool importMap();

bool cellCopy(Record *src);

#endif
