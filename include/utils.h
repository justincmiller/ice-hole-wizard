#ifndef UTILS_H
#define UTILS_H

#define COL_X(col) ((col) - 50)
#define ROW_Y(row) (50 - (row))
#define X_COL(x) ((x) + 50)
#define Y_ROW(y) (50 - (y))

#define CLAMP_X(x) ((x) < 0 ? 0 : ((x) > 99 ? 99 : (x)))
#define CLAMP_Y(y) ((y) < 0 ? 0 : ((y) > 99 ? 99 : (y)))

//do-while(0) ensures this macro expands consitently
#define ASSERT(ptr) \
    do { \
        if (ptr == NULL) { \
            printf(">> Error: memory failure.\n"); \
            purge(); \
            exit(EXIT_FAILURE); \
        } \
    } while (0)

#endif