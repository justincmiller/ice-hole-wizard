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

static Display* dsp;
static FILE* file;
static Layer** layers;
static Metadata meta;
static bool fileOpen;

void loadFiles(Display* ptr)
{
    dsp = ptr;
    //set file operations to binary
    _set_fmode(_O_BINARY);
}

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

    //get file path from input
    fgets(buffer, PATH_LEN, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';

    //check for presence of an extension
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
    if (fileOpen)
        updateFile();
    else
        createFile();
}

bool updateFile()
{

}

bool createFile()
{
    char fname[PATH_LEN] = {0};

    printf("File name: >");

    //get filename from input
    fgets(fname, PATH_LEN, stdin);
    fname[strcspn(fname, "\n")] = '\0';

    //check for presence of an extention
    char* ext = strrchr(fname, '.');

    //none found, append extension
    if (ext == NULL)
        strcat(fname, EXTENSION);
    //extension found, verify extension
    else if (strcmp(ext, EXTENSION) != 0)
    {
        //replace extension when invalid
        strcpy(ext, EXTENSION);
    }

    //check if file exists
    file = fopen(fname, "rb");

    //prompt to overwrite if true
    if (file != NULL)
    {
        fclose(file);
        printf("File [%s] already exists. Would you like to overwrite it?\n", fname);
        printf("[ENTER] Continue\n");
        printf("[ESC]   Cancel");

        char key = 0;
        while (key != '\x1b' && key != '\r')
        {
            if (!_kbhit()) continue;
            key = (char)_getch();
        }
        //skip exporting and return fail condition
        if (key == '\x1b') return false;
    }

    writeFile(fname);
}

bool writeFile(const char* fname)
{
    file = fopen(fname, "wb");
    if (file == NULL) return false;

    unsigned long offset = 0;

    Record buffer;
    //initialize file header
    meta.layers = dsp->map->depth;
    meta.cells = 0;
    meta.deleted = 0;

    memcpy(&buffer.meta, &meta, sizeof(Metadata));

    //write initial file header
    fwrite(&buffer, sizeof(Record), 1, file);
    offset += sizeof(Record);

    //initialize layer matrix pointer
    Node* layerPtr = dsp->map->matrix;
    if (layerPtr == NULL) 
    {
        fclose(file);
        return false;
    }

    //initialize loop pointers
    Layer* layer = NULL;
    Node* cellPtr = NULL;
    Cell* cell = NULL;

    unsigned short x = 0;
    unsigned short y = 0;

    //iterate over each layer
    while (layerPtr != NULL)
    {
        //update layer pointer
        layer = (Layer*)layerPtr->data;
        if (layer == NULL)
        {
            fclose(file);
            return false;
        }

        //update cell list pointer, skipping if layer is emptys
        cellPtr = layer->cells;
        if (cellPtr == NULL) continue;

        //iterate over each edited cell
        while (cellPtr != NULL)
        {
            //update cell pointer
            cell = (Cell*)cellPtr->data;
            if (cell == NULL) 
            {
                fclose(file);
                return false;
            }

            //decode cell number
            x = CN_X(cell->cn);
            y = CN_Y(cell->cn);

            buffer.cell.status = EDITED;
            buffer.cell.glyph = layer->grid[y][x];
            buffer.cell.cn = cell->cn;
            buffer.cell.el = cell->el;
            buffer.cell.cf = cell->cf;
            buffer.cell.ty = cell->ty;
            buffer.cell.rl = cell->rl;
            buffer.cell.rb = cell->rb;
            
            if (cellPtr->next == NULL)
                buffer.cell.next = 0;
            else
                buffer.cell.next = offset + sizeof(Record);

            fwrite(&buffer, sizeof(Record), 1, file);
            offset += sizeof(Record);
            
            //increment cell total
            meta.cells++;
            cellPtr = cellPtr->next;
        }

        layerPtr = layerPtr->next;
    }

    //update metadata
    fseek(file, 0, SEEK_SET);
    memcpy(&buffer.meta, &meta, sizeof(Metadata));
    fwrite(&buffer, sizeof(Record), 1, file);

    fclose(file);
    return true;
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

    cell->cf = src->cell.cf;
    cell->ty = src->cell.ty;
    cell->rl = src->cell.rl;
    cell->rb = src->cell.rb;

    return true;
}