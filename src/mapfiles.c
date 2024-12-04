/*
* mapfiles.c
*
* This file contains
*/

/*
*  Credit is given to Dr. Larry Hughes for providing the reference code
*  that was used both as inspiration and used as is throughout this program.
*/

#include "engine.h"
#include "mapfiles.h"

static FILE* file;
static Layer** layers;
static Metadata meta;
static bool fileOpen;

bool importFile(const char* fname)
{
    file = fopen(fname, "rb+");
    if (file == NULL) return false;

    importMap();
    fileOpen = true;
    return true;
}

void filePrompt()
{

    char key = 0;
    bool valid = false;

    HIDE_CURSOR;

    printf(TITLE("Ice Hole Wizard"));
    printf(SUBTITLE("Lunar Ice Cave Explorer Program"));
    printf(LABEL_NEW  "[N]   New Project");
    printf(LABEL_LOAD "[L]   Load Project");
    printf(LABEL_QUIT "[ESC] Quit");

    while (!valid)
    {
        if (!_kbhit()) continue;

        key = (char)_getch();

        switch (key)
        {
            case 'n':
                SHOW_CURSOR;
                newMap();
                valid = true;
                break;
            case 'l':
                SHOW_CURSOR;
                fileInput();
                valid = true;
                break;
            case '\x1b':
                purge();
                exit(EXIT_SUCCESS);
        }
    }
}

void fileInput()
{
    char buffer[PATH_LEN] = {0};

    printf(FILE_INPUT "File path: >");

    fgets(buffer, PATH_LEN, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';

    char* ext = strrchr(buffer, '.');
    if (ext == NULL)
    {
        printf(MESSAGE ">> Error: file extension not found.");
        filePrompt();
    }
    else if (strcmp(ext, EXTENSION) != 0)
    {
        printf(MESSAGE ">> Error: unexpected file type.");
        filePrompt();
    }
    else if (importFile(buffer) == true)
    {
        return;
    }
    else
    {
        printf(MESSAGE ">> Error: could not open file [%s].", buffer);
        filePrompt();
    }
}

bool fileStatus()
{
    return fileOpen;
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

    //finished processing if status is UNEDITED, return success
    if (src->cell.status == UNEDITED) return true;

    cell->cf = src->cell.cf;
    cell->ty = src->cell.ty;
    cell->rl = src->cell.rl;
    cell->rb = src->cell.rb;

    return true;
}