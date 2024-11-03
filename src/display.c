#include "display.h"

char** newGrid()
{
    //allocate row pointers
    char** grid = malloc(MAP_ROWS * sizeof(char*));
    if (grid == NULL)
    {
        printf(">> Error: memory failure.\n");
        return NULL;
    }

    //allocate contiguous 2D array of chars
    grid[0] = malloc(MAP_COLS * MAP_ROWS * sizeof(char));
    if (grid[0] == NULL)
    {
        printf(">> Error: memory failure.\n");
        free(grid);
        return NULL;
    }
    
    /*set row pointers and place a newline at the end of every row,
    NUL terminating the last row*/
    for (int i = 0; i < MAP_ROWS; i++)
    {
        grid[i] = grid[0] + i * MAP_COLS;
        grid[i][MAP_COLS - 1] = (i < MAP_ROWS - 1) ? '\n' : '\0';
    }

    return grid;
}

Node* newLayer()
{
    Node* layer = malloc(sizeof(Node));
    layer->grid = newGrid();
    layer->next = NULL;
    layer->prev = NULL;
    return layer;
}

void addLayer(Node** map)
{
    Node* layer = newLayer();
    
    //if map is empty, add layer to map
    if (*map == NULL)
    {
        *map = layer;
    }
    else
    {
        //traverse to end of list and insert layer
        Node* ptr = *map;
        while (ptr->next != NULL)
        {
            ptr = ptr->next;
        }
        ptr->next = layer;
        layer->prev = ptr;
    }
}

void freeLayers(Node** map)
{
    if (map == NULL || *map == NULL) return;

    Node* ptr = *map;
    while (ptr != NULL)
    {
        Node* next = ptr->next;
        free(ptr->grid[0]);
        free(ptr->grid);
        free(ptr);
        ptr = next;
    }

    *map == NULL;
}

void initDisplay()
{

}

//todo: setup display like wizard function
void display(const int action, void*)
{
    static Display;

}