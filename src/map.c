/*
* map.c
*
* Contains function implementations for the manipulation of 
* layers, maps, cells, cell properties stuctures and information.
*/

/*
*  Credit is given to Dr. Larry Hughes for providing the reference code
*  that was used both as inspiration and used as is throughout this program.
*/

#include "map.h" //layers, map, cells, etc.
#include "engine.h" //program operation headers and functions

static Display* dsp; //static global variable for display (limited scope)
static Map map; //map data structure

//loads layer and corresponding map data
void loadMap(Display* ptr)
{
    dsp = ptr;
    dsp->map = &map;
}

void newMap()
{
    map.layer = addLayer();
    map.last = map.layer;
    dsp->cursor = &map.layer->cursor;

    //set cursor to origin and set window size and margins
    setCursor(X_COL(0), Y_ROW(0));
    setWindow();
}

//created character grid to display characters on screen
char** createGrid()
{
    //allocate row pointers
    char** grid = malloc(MAP_ROWS * sizeof(char*));
    assert((void*)grid, APPEND);

    //allocate contiguous 2D array of chars
    grid[0] = malloc(MAP_COLS * MAP_ROWS * sizeof(char));
    assert((void*)grid[0], APPEND);
    
    //initialize row pointers
    for (int i = 0; i < MAP_ROWS; i++)
    {
        grid[i] = grid[0] + i * (MAP_COLS);
    }

    //initialize empty grid (space characters)
    memset(grid[0], LATENT, MAP_ROWS * MAP_COLS);

    return grid;
}

//creates new layer in memory
Layer* createLayer()
{
    //allocate layer
    Layer* layer = malloc(sizeof(Layer));
    assert((void*)layer, APPEND);

    //initialize grid
    layer->grid = createGrid();
    layer->cells = NULL;
    layer->cursor.X = X_COL(1);
    layer->cursor.Y = Y_ROW(1);

    //increment depth
    layer->depth = map.depth;
    map.depth++;
    return layer;
}

//adds newly created layer to cave system
Layer* addLayer()
{
    Node* node = malloc(sizeof(Node));
    assert((void*)node, APPEND);

    node->data = (void*)createLayer();
    node->next = NULL;
    node->prev = NULL;
    
    //if list is empty, add node to list
    if (map.matrix == NULL)
        map.matrix = node;
    else
    {
        //traverse to end of list and insert node
        Node* ptr = map.matrix;
        while (ptr->next != NULL)
        {
            ptr = ptr->next;
        }
        ptr->next = node;
        node->prev = ptr;
    }

    return node->data;
}

//saves current layer and moves down one layer or adds new layer
void layerDown()
{
    //initialize ptr to head of map matrix
    Node* ptr = getNode(map.matrix, map.layer->depth);

    //return if empty
    if (ptr == NULL) return;

    //if next layer does not exist, add layer
    if (ptr->next == NULL)
    {
        addLayer();
        //validate if addLayer() creates a new layer
        if (ptr->next == NULL) return;
    }
    
    //update current layer and previous layer
    map.layer = ptr->next->data;
    map.last = ptr->data;

    map.layer->cursor = map.last->cursor;

    //if in drawing state add portals at entrance and exit
    if (dsp->state == DRAW)
    {
        int x = dsp->cursor->X;
        int y = dsp->cursor->Y;
        dsp->map->last->grid[y][x] = MINUS;
        printf(PORT_DN("%c") FIXED, dsp->map->last->grid[y][x]);
        dsp->map->layer->grid[y][x] = PLUS;
        printf(PORT_UP("%c") FIXED, dsp->map->layer->grid[y][x]);
    }

    saveLayer(map.last);
    //update cursor pointer
    dsp->cursor = &map.layer->cursor;

    render();
}

//saves current layer and moves up one layer if possible
void layerUp()
{
    Node* ptr = getNode(map.matrix, map.layer->depth);

    //if no higher layer exists, then do nothing
    if (ptr == NULL || ptr->prev == NULL) return;

    map.layer = ptr->prev->data;
    map.last = ptr->data;

    map.layer->cursor = map.last->cursor;

    //if in drawing state save/load portal positions
    if (dsp->state == DRAW)
    {
        int x = dsp->cursor->X;
        int y = dsp->cursor->Y;
        dsp->map->last->grid[y][x] = PLUS;
        printf(PORT_UP("%c") FIXED, dsp->map->last->grid[y][x]);
        dsp->map->layer->grid[y][x] = MINUS;
        printf(PORT_DN("%c") FIXED, dsp->map->layer->grid[y][x]);
    }

    saveLayer(map.last);
    dsp->cursor = &map.layer->cursor;

    render();
}

//move to the last layer created
void lastLayer()
{
    Layer* last = map.layer;
    map.layer = dsp->map->last;
    map.last = last;

    dsp->cursor = &map.layer->cursor;

    saveLayer(map.last);

    render();
}

//moves to layer 0
void topLayer()
{
    Layer* last = map.layer;
    
    //fetch head of matrix, representing first layer
    Node* ptr = dsp->map->matrix;

    map.layer = (Layer*)ptr->data;
    map.last = last;

    saveLayer(map.last);

    dsp->cursor = &map.layer->cursor;

    render();
}

void saveLayer(Layer* layer)
{
    if (map.layer == NULL || map.layer->cells == NULL) return;

    bool* saved = calloc(MAP_ROWS * MAP_COLS, sizeof(char));
    assert((void*)saved, IGNORE);

    Node* ptr = map.layer->cells;
    Cell* cell = NULL;

    while (ptr != NULL)
    {
        cell = (Cell*)ptr->data;
        if (cell)
            saved[cell->cn] = true;

        ptr = ptr->next;
    }

    unsigned short x = 0;
    unsigned short y = 0;

    for (int i = 0; i < MAP_ROWS * MAP_COLS; i++)
    {
        x = CN_X(i);
        y = CN_Y(i);
        if (saved[i] = false && map.layer->grid[y][x] != LATENT)
        {
            addCell(map.layer);
        }
    }

    free(saved);
}

//creates a cell data node within the map
Cell* createCell()
{
    //allocate cell
    Cell* cell =calloc(1, sizeof(Cell));
    assert((void*)cell, APPEND);

    //initialize cell data
    cell->cf = DEF_CF; //default roughness
    
    #ifdef ENABLE_CC
    for (int i = 0; i < CONTENTS; i++)
    {
        cell->cc[i].code = LATENT_CC;
    }
    #endif

    return cell;
}

//adds a newly created cell data struct to map
Cell* addCell(Layer* layer)
{
    Node* node = malloc(sizeof(Node));
    assert((void*)node, APPEND);

    node->data = (void*)createCell();
    node->next = NULL;
    node->prev = NULL;
    
    //if list is empty, add node to list
    if (layer->cells == NULL)
        layer->cells = node;
    else
    {
        //traverse to end of list and insert node
        Node* ptr = layer->cells;
        while (ptr->next != NULL)
        {
            ptr = ptr->next;
        }
        ptr->next = node;
        node->prev = ptr;
    }

    layer->cellCount++;

    return node->data;
}

//removes cell and cell data from the map
void remCell()
{
    short x = dsp->cursor->X;
    short y = dsp->cursor->Y;
    unsigned int cn = CN(x, y);

    ECH(1);
    map.layer->grid[y][x] = LATENT;

    Node* ptr = searchCN(cn);
    if (ptr == NULL) return;

    Cell* cell = (Cell*)ptr->data;
    
    removeNode(&map.layer->cells, ptr); //removes cell node
    forget(cell);       //remove and free allocated cell
    forget(ptr);        //remove and free allocated node
}

//seaches node by cell number
Node* searchCN(const unsigned int cn)
{
    if (map.layer == NULL || map.layer->cells == NULL) return NULL;

    Node* ptr = map.layer->cells;
    Cell* cell = NULL;

    while (ptr != NULL)
    {
        cell = (Cell*)ptr->data;
        if (cell && cell->cn == cn) return ptr;

        ptr = ptr->next;
    }

    return NULL;
}

#ifdef ENABLE_CC
int getRB(Cell* cell)
{
    int n = 0;
    //loop until end of list, either LATENT_CC or maximum number of contents
    while (cell->cc[n].code != LATENT_CC && n < CONTENTS)
    {
        if (cell->cc[n].code == RB)
        {
            return cell->cc[n].qty;
        }
        n++;
    }
}
#endif