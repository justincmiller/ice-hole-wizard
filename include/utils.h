#ifndef UTILS_H
#define UTILS_H

#include "vterminal.h"

#define COL_X(col) ((col) - 49)
#define ROW_Y(row) (50 - (row))
#define X_COL(x) ((x) + 49)
#define Y_ROW(y) (50 - (y))

#define CLAMP_X(x) ((x) < 0 ? 0 : ((x) > 99 ? 99 : (x)))
#define CLAMP_Y(y) ((y) < 0 ? 0 : ((y) > 99 ? 99 : (y)))

#define CLEAR printf(CSI "2J" CSI "3J");

//do-while(0) ensures this macro expands consitently
#define ASSERT(ptr) \
    do { \
        if (ptr == NULL) { \
            printf(">> Error: memory failure.\n"); \
            purge(); \
            exit(EXIT_FAILURE); \
        } \
    } while (0)

typedef struct Node
{
    void* data;
    struct Node* next;
    struct Node* prev;
}Node;

void addNode(Node** head, void* data);
void track(void* ptr);
void purge();

#endif