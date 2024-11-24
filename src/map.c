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
    ASSERT(grid);
    track((void*)grid);

    //allocate contiguous 2D array of chars
    grid[0] = malloc(MAP_COLS * MAP_ROWS * sizeof(char));
    ASSERT(grid[0]);
    track((void*)grid[0]);
    
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
    ASSERT(layer);
    track((void*)layer);

    //initialize grid
    layer->grid = createGrid();

    //increment depth
    layer->depth = map.depth;
    map.depth++;
    return layer;
}

void addLayer()
{
    Node* node = malloc(sizeof(Node));
    ASSERT(node);
    track((void*)node);

    node->data = (void*)createLayer();
    node->next = NULL;
    node->prev = NULL;
    
    //if list is empty, add node to list
    if (map.matrix == NULL)
    {
        map.matrix = node;
    }
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
    cell->data->cf = 5; //default roughness

    return cell;
}

Node* addCell()
{
    Node* node = malloc(sizeof(Node));
    ASSERT(node);
    track((void*)node);

    node->data = (void*)createCell();
    node->next = NULL;
    node->prev = NULL;
    
    //if list is empty, add node to list
    if (map.layer->cells == NULL)
    {
        map.layer->cells = node;
    }
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

// void editCell()
// {
//     Cell* cell = dsp->edit.cell;

//     if (cell == NULL)
//     {
//         cell = createCell();
//         dsp->edit.cell = cell;
//     }

//     int x = dsp->cursor->X;
//     int y = dsp->cursor->Y;
//     int z = map.layer->depth;

//     cell->pos.X = x;
//     cell->pos.Y = y;
    
//     //calculate cell number from x, y and z
//     int cn = z * (MAP_ROWS) * (MAP_COLS) + y * MAP_COLS + x;

//     //skip initializing if the cn matches the current configuration
//     if (cell->data->cn == cn) return;

//     cell->data->cn = cn;
//     cell->data->el = z;
//     cell->data->cf = 5;
//     cell->data->ty = 0;
//     cell->data->rl = 0;

//     for (int i = 0; i < 3; i++)
//     {
//         cell->data->cc[i].code = 0;
//         cell->data->cc[i].qty = 0;
//     }
// }

// void editValue()
// {
//     int option = dsp->edit.index + 2;
//     int value = 0;

//     switch (option)
//     {
//         case CF:
//             CUP(EDIT_X, CF_Y);
//             scanf("%d", &value);
//             snprintf(dsp->edit.values[option], BUFF_LEN, "%d", value);
//             dsp->edit.cell->data->cf = (unsigned int)value;
//             break;
//         case TY:
//             CUP(EDIT_X, TY_Y);
//             scanf("%d", &value);
//             snprintf(dsp->edit.values[option], BUFF_LEN, "%d", value);
//             dsp->edit.cell->data->ty = (char)value;
//             break;
//         case RL:
//             CUP(EDIT_X, RL_Y);
//             scanf("%d", &value);
//             snprintf(dsp->edit.values[option], BUFF_LEN, "%d", value);
//             dsp->edit.cell->data->rl = (unsigned int)value;
//             break;
//         case CC:
//             CUP(EDIT_X, CC_Y);
//             scanf("%d", &value);
//             snprintf(dsp->edit.values[option], BUFF_LEN, "%d", value);
//             //dsp->edit.cell->data->cf = (char)value;
//             break;
//     }
// }

// void saveCell()
// {
//     Cell* cell = (Cell*)addCell()->data;
//     Cell* edited = dsp->edit.cell;

//     //copy cell contents from edited cell
//     cell->pos.X = edited->pos.X;
//     cell->pos.Y = edited->pos.Y;

//     cell->data->cn = edited->data->cn;
//     cell->data->el = edited->data->el;
//     cell->data->cf = edited->data->cf;
//     cell->data->ty = edited->data->ty;
//     cell->data->rl = edited->data->rl;
    
//     for (int i = 0; i < 3; i++)
//     {
//         cell->data->cc[i].code = edited->data->cc[i].code;
//         cell->data->cc[i].code = edited->data->cc[i].qty;
//     }
// }

int getRB(Data* data)
{
    int rb = 0;

    for (int i = 0; i < CONTENTS; i++)
    {
        if (data->cc[i].code == RB)
            rb = data->cc[i].qty;
    }

    return rb;
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

    //update cursor pointer
    dsp->cursor = &map.layer->cursor;

    render();
}

void layerUp()
{
    Node* ptr = getNode(map.matrix, map.layer->depth);

    if (ptr == NULL) return;

    if (ptr->prev == NULL) return;

    map.layer = ptr->prev->data;
    map.last = ptr->data;

    dsp->cursor = &map.layer->cursor;

    render();
}

void lastLayer()
{
    Layer* ptr = map.layer;
    map.layer = dsp->map->last;
    map.last = ptr;

    dsp->cursor = &map.layer->cursor;

    render();
}
