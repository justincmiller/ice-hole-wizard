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

    /*if width or height do not exceed grid dimensions, set margin offset to 
    center viewport on the grid. Otherwise, align top left*/
    dsp->margin.Left = (dsp->width <= MAP_COLS) ? (MAP_COLS - dsp->width) / 2 : 0;
    dsp->margin.Top = (dsp->height <= MAP_ROWS) ? (MAP_ROWS - dsp->height) / 2 : 0;
    dsp->margin.Right = dsp->margin.Left + dsp->width;
    dsp->margin.Bottom = dsp->margin.Top + dsp->height;

    dsp->cursor.X = dsp->width / 2;
    dsp->cursor.Y = dsp->height / 2;

    CUP(dsp->cursor.X, dsp->cursor.Y);
}

void render()
{
    
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