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

static FILE* file;
static Layer** layers;
static Metadata meta;

bool importFile(const char* fname)
{
    file = fopen(fname, "rb+");
    if (file == NULL)
    {
        printf(">> Error: could not open file [%s].\n", fname);
        return false;
    }

    importMap();
    
    return true;
}

bool exportFile()
{

}

bool importMap()
{
    //read file header
    Record buffer;
    if (fread(&buffer, sizeof(Record), 1, file) != 1)
        return false;

    //copy metadata from buffer to metadata struct
    memcpy(&meta, &buffer.meta, sizeof(Metadata));

    //initalize array of layer pointers
    layers = malloc(meta.layers * sizeof(Layer*));
    assert((void*)layers, APPEND);

    //initialize map layers
    for (int i = 0; i < meta.layers; i++)
    {
        layers[i] = addLayer();
    }

    //import map cells
    while (fread(&buffer, sizeof(Record), 1, file) == 1)
    {
        if (buffer.cell.status != DELETED)
        {
            if (!cellCopy(&buffer)) return false;
        }
    }
}

bool cellCopy(Record* src)
{
    //determine layer depth and validate
    int z = src->cell.el;
    if (z < 0 || z > meta.layers) return false;

    //add cell to layer at depth z
    Cell* cell = addCell(layers[z]);

    //copy cell number and determine grid coordinates
    cell->cn = src->cell.cn;
    unsigned short x = CN_X(cell->cn);
    unsigned short y = CN_Y(cell->cn);

    //set grid character
    layers[z]->grid[y][x] = src->cell.glyph;

    //finished processing if status is DEFAULT, return success
    if (src->cell.status == DEFAULT) return true;

    cell->cf = src->cell.cf;
    cell->ty = src->cell.ty;
    cell->rl = src->cell.rl;
    cell->rb = src->cell.rb;

    return true;
}