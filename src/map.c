/*
*  Credit is given to Dr. Larry Hughes for providing the reference code
*  that was used both as inspiration and used as is throughout this program.
*/

#include "map.h"
#include "engine.h"

static Display* dsp;
static Map map;

void loadMap(Display* ptr)
{
    addLayer();
    map.layer = map.matrix->data;
    map.last = map.layer;
    dsp = ptr;
    dsp->map = &map;
    dsp->cursor = &map.layer->cursor;
}

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

void addLayer()
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
}

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

    if (dsp->state == DRAW)
    {
        int x = dsp->cursor->X;
        int y = dsp->cursor->Y;
        dsp->map->last->grid[y][x] = MINUS;
        printf(PORT_DN("%c") FIXED, dsp->map->last->grid[y][x]);
        dsp->map->layer->grid[y][x] = PLUS;
        printf(PORT_UP("%c") FIXED, dsp->map->layer->grid[y][x]);
    }

    //update cursor pointer
    dsp->cursor = &map.layer->cursor;

    render();
}

void layerUp()
{
    Node* ptr = getNode(map.matrix, map.layer->depth);

    if (ptr == NULL || ptr->prev == NULL) return;

    map.layer = ptr->prev->data;
    map.last = ptr->data;

    map.layer->cursor = map.last->cursor;

    if (dsp->state == DRAW)
    {
        int x = dsp->cursor->X;
        int y = dsp->cursor->Y;
        dsp->map->last->grid[y][x] = PLUS;
        printf(PORT_UP("%c") FIXED, dsp->map->last->grid[y][x]);
        dsp->map->layer->grid[y][x] = MINUS;
        printf(PORT_DN("%c") FIXED, dsp->map->layer->grid[y][x]);
    }

    dsp->cursor = &map.layer->cursor;

    render();
}

void lastLayer()
{
    Layer* last = map.layer;
    map.layer = dsp->map->last;
    map.last = last;

    dsp->cursor = &map.layer->cursor;

    render();
}

void topLayer()
{
    Layer* last = map.layer;
    
    //fetch head of matrix, representing first layer
    Node* ptr = dsp->map->matrix;

    map.layer = (Layer*)ptr->data;
    map.last = last;

    dsp->cursor = &map.layer->cursor;

    render();
}

Cell* createCell()
{
    //allocate cell
    Cell* cell = malloc(sizeof(Cell));
    assert((void*)cell, APPEND);

    //allocate cell data
    cell->data = calloc(1, sizeof(Data));
    assert((void*)cell->data, APPEND);

    //initialize position as 0,0
    cell->x = 0;
    cell->y = 0;

    //initialize cell data
    cell->data->cf = DEF_CF; //default roughness
    
    for (int i = 0; i < CONTENTS; i++)
    {
        cell->data->cc[i].code = LATENT_CC;
    }

    return cell;
}

Node* addCell()
{
    Node* node = malloc(sizeof(Node));
    assert((void*)node, APPEND);

    node->data = (void*)createCell();
    node->next = NULL;
    node->prev = NULL;
    
    //if list is empty, add node to list
    if (map.layer->cells == NULL)
        map.layer->cells = node;
    else
    {
        //traverse to end of list and insert node
        Node* ptr = map.layer->cells;
        while (ptr->next != NULL)
        {
            ptr = ptr->next;
        }
        ptr->next = node;
        node->prev = ptr;
    }

    return node;
}

void remCell()
{
    short x = dsp->cursor->X;
    short y = dsp->cursor->Y;
    unsigned int cn = CN(x, y, map.layer->depth);

    ECH(1);
    map.layer->grid[y][x] = LATENT;

    Node* ptr = searchCN(cn);
    if (ptr == NULL) return;

    Cell* cell = (Cell*)ptr->data;
    
    removeNode(&map.layer->cells, ptr);
    forget(cell->data); //remove and free allocated cell data
    forget(cell);       //remove and free allocated cell
    forget(ptr);        //remove and free allocated node
}

Node* searchCN(const unsigned int cn)
{
    if (map.layer == NULL || map.layer->cells == NULL) return NULL;

    Node* ptr = map.layer->cells;
    Cell* cell = NULL;

    while (ptr != NULL)
    {
        cell = (Cell*)ptr->data;
        if (cell && cell->data->cn == cn) return ptr;

        ptr = ptr->next;
    }

    return NULL;
}

int getRB(Data* data)
{
    int rb = 0;
    int n = 0;

    //loop until end of list, either LATENT_CC or maximum number of contents
    while (data->cc[n].code != LATENT_CC && n < CONTENTS)
    {
        if (data->cc[n].code == RB)
        {
            rb = data->cc[n].qty;
            break;
        }
        n++;
    }

    return rb;
}