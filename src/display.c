/*
*  Credit is given to Dr. Larry Hughes for providing the reference code
*  that was used throughout this program. Specifically, the engine.h, vterminal.h, and vterminal.c files
*  which uses much of Dr. Hughes' work.
*/

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
        grid[i] = grid[0] + i * (MAP_COLS);
    }

    memset(grid[0], ' ', MAP_ROWS * MAP_COLS);

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
    Display* dsp;
    display(GET_DISPLAY, &dsp);
    addLayer(&(dsp->map));
    getWindow(&(dsp->vp));
    dsp->width = dsp->vp.Right - dsp->vp.Left + 1;
    dsp->height = dsp->vp.Bottom - dsp->vp.Top + 1;

    /*max and min functions clamp margins to grid bounds (0, 99). if the window
    exceeds the width of the grid, it will be aligned top left.*/
    dsp->margin.Left = max((MAP_COLS - dsp->width) / 2, 0);
    dsp->margin.Top = max((MAP_ROWS - dsp->height) / 2, 0);
    dsp->margin.Right = min(dsp->margin.Left + dsp->width, MAP_COLS - 1);
    dsp->margin.Bottom = min(dsp->margin.Top + dsp->height, MAP_ROWS - 1);

    dsp->cursor.X = dsp->width / 2;
    dsp->cursor.Y = dsp->height / 2;

    CUP(dsp->cursor.X, dsp->cursor.Y);
}

void render()
{
    Display* dsp;
    display(GET_DISPLAY, &dsp);
    char** grid = getActiveLayer(dsp)->grid;

    if (grid == NULL) return;

    /***** initialize buffer variables *****/

    //buffer width increased to accomodate \r\n for printing
    int width = dsp->width + 2;
    int left = dsp->margin.Left;
    int top = dsp->margin.Top;
    int row = 0;

    //clamp copy length to the end of the visible portion of the grid
    int len = min(dsp->width, (MAP_COLS - 1) - left);

    char** buffer = malloc(dsp->height * sizeof(char*));
    if (buffer == NULL)
    {
        printf(">> Error: memory failure.\n");
        return;
    }
    
    buffer[0] = malloc(dsp->height * width* sizeof(char));
    if (buffer[0] == NULL)
    {
        printf(">> Error: memory failure.\n");
        free(buffer);
        return;
    }

    for (int i = 0; i < dsp->height; i++)
    {
        //set row pointer
        buffer[i] = buffer[0] + i * width;
        //prevent grid overflow and copy from grid
        row = min(i + top, MAP_ROWS - 1);
        if (row < MAP_ROWS)
        {
            memcpy(buffer[i], &grid[row][left], len * sizeof(char));
            //insert a carriage return and newline starting at len (last two characters)
            buffer[i][len] = '\r';
            buffer[i][len + 1] = '\n';
        }
    }

    //todo: render window
}

Node* getActiveLayer(Display* dsp)
{
    if (dsp == NULL || dsp->map == NULL) return NULL;

    Node* layer = dsp->map;
    for (int i = 0; i < dsp->layer; i++)
    {
        if (layer->next != NULL) layer = layer->next;
        else return NULL;
    }

    return layer;
}

void getWindow(SMALL_RECT* rect)
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    if (out == INVALID_HANDLE_VALUE)
    {
        printf("Error: could not fetch output handle\n");
        return;
    }

    CONSOLE_SCREEN_BUFFER_INFO info;

    if (!GetConsoleScreenBufferInfo(out, &info))
    {
        printf(">> Error: unable to get console screen buffer info.\n");
        return;
    }

    *rect = info.srWindow;
}

void display(const int action, void* data)
{
    static Display display;

    switch (action)
    {
        case GET_VP:
            *(SMALL_RECT*)data = display.vp;
            break;
        case SET_VP:
            display.vp = *(SMALL_RECT*)data;
            break;
        case GET_MAP:
            *(Node**)data = &display.map;
            break;
        case SET_MAP:
            display.map = *(Node**)data;
            break;
        case GET_CUR:
            break;
        case SET_CUR:
            break;
        case GET_DISPLAY:
            *(Display**)data = &display;
            break;
        default:
            break;
    }
}