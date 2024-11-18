#include "map.h"
#include "display.h"

static Map map;
static Display* dsp;

void initMap()
{
    //fetch display pointer
    dsp = getDisplay();
    //add initial layer
    addLayer();
    //initialize layer pointers
    map.layer = map.matrix->data;
    map.last = map.layer;
}

Map* getMap()
{
    return &map;
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

void saveLayer()
{

}

Cell* createCell(int x, int y, int z)
{
    //allocate cell
    Cell* cell = malloc(sizeof(Cell));
    ASSERT(cell);
    track((void*)cell);

    //allocate cell data
    cell->data = malloc(sizeof(Data));
    ASSERT(cell);
    track((void*)cell);

    cell->pos.X = x;
    cell->pos.Y = y;

    //initialize cell data
    cell->data->cn = y * MAP_COLS + x;  //cell number
    cell->data->el = z;                 //cell elevation
    cell->data->cf = 5;                 //default roughness
    cell->data->ty = 0;                 //default cell type (rock)
    cell->data->rl = 0;                 //default radiation level
    //default cell contents
    memset(cell->data->cc, 0, sizeof(cell->data->cc));

    return cell;
}

void modifyCell()
{
    Cell* cell = createCell(dsp->cursor.X, dsp->cursor.Y, map.layer->depth);
}