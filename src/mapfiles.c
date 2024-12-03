/*
* mapfiles.c
*
* This file contains
*/

/*
*  Credit is given to Dr. Larry Hughes for providing the reference code
*  that was used both as inspiration and used as is throughout this program.
*/

#include "mapfiles.h"

static FILE* import;

void importFile(const char* fname)
{
    _set_fmode(_O_BINARY);
}