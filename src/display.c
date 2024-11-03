#include "display.h"

char** newLayer()
{
    char** layer = malloc(MAP_ROWS * sizeof(char*));
    if (layer == NULL)
    {
        printf(">> Error: memory failure.\n");
        return;
    }
}