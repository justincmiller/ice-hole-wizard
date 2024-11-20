#include "map.h"

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
    cell->data = malloc(sizeof(Data));
    ASSERT(cell);
    track((void*)cell);

    //initialize position as 0,0
    cell->pos.X = 0;
    cell->pos.Y = 0;

    //initialize cell data
    cell->data->cn = 0; //cell number
    cell->data->el = 0; //cell elevation
    cell->data->cf = 5; //default roughness
    cell->data->ty = 0; //default cell type (rock)
    cell->data->rl = 0; //default radiation level
    //default cell contents
    memset(cell->data->cc, 0, sizeof(cell->data->cc));

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

Cell* editCell()
{
    static Cell* cell = NULL;

    if (cell == NULL) cell = createCell();

    int x = dsp->cursor.X;
    int y = dsp->cursor.Y;
    int z = map.layer->depth;

    cell->pos.X = x;
    cell->pos.Y = y;
    
    cell->data->cn = z * (MAP_ROWS) * (MAP_COLS) + y * MAP_COLS + x;
    cell->data->el = z;

    return cell;
}