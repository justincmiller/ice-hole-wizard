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
    ASSERT(cell);
    track((void*)cell);

    //allocate cell data
    cell->data = calloc(1, sizeof(Data));
    ASSERT(cell->data);
    track((void*)cell->data);

    //initialize position as 0,0
    cell->pos.X = 0;
    cell->pos.Y = 0;

    //initialize cell data
    cell->data->cf = 5; //default roughness

    return cell;
}

void addCell()
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
}

void editCell()
{
    Cell* cell = dsp->edit.cell;

    if (cell == NULL)
    {
        cell = createCell();
        dsp->edit.cell = cell;
    }

    int x = dsp->cursor.X;
    int y = dsp->cursor.Y;
    int z = map.layer->depth;

    cell->pos.X = x;
    cell->pos.Y = y;
    
    //calculate cell number from x, y and z
    int cn = z * (MAP_ROWS) * (MAP_COLS) + y * MAP_COLS + x;

    //skip initializing if the cn matches the current configuration
    if (cell->data->cn == cn) return;

    cell->data->cn = cn;
    cell->data->el = z;
    cell->data->cf = 5;
    cell->data->ty = 0;
    cell->data->rl = 0;

    for (int i = 0; i < 3; i++)
    {
        cell->data->cc[i].code = 0;
        cell->data->cc[i].qty = 0;
    }
}

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

void editValue()
{
    int option = dsp->edit.index + 2;
    int value = 0;

    switch (option)
    {
        case CF:
            CUP(EDIT_X, CF_Y);
            scanf("%d", &value);
            snprintf(dsp->edit.values[option], BUFF_LEN, "%d", value);
            dsp->edit.cell->data->cf = (unsigned int)value;
            break;
        case TY:
            CUP(EDIT_X, TY_Y);
            scanf("%d", &value);
            snprintf(dsp->edit.values[option], BUFF_LEN, "%d", value);
            dsp->edit.cell->data->ty = (char)value;
            break;
        case RL:
            CUP(EDIT_X, RL_Y);
            scanf("%d", &value);
            snprintf(dsp->edit.values[option], BUFF_LEN, "%d", value);
            dsp->edit.cell->data->rl = (unsigned int)value;
            break;
        case CC:
            CUP(EDIT_X, CC_Y);
            scanf("%d", &value);
            snprintf(dsp->edit.values[option], BUFF_LEN, "%d", value);
            //dsp->edit.cell->data->cf = (char)value;
            break;
    }
}