#ifndef STYLE_H
#define STYLE_H

#include "utils.h"

#define STATUS_BAR_POS(y) CUP(2, (y))

#define MENU_ROWS 20
#define MENU_COLS 30

#define MENU_STR 32

#define MENU_MAX 4
#define MENU_MIN 0

#define DATA_ROWS 6
#define DATA_COLS 32

//row offset of 2 to accomodate top/bottom borders
#define BORDER_ROWS (MENU_ROWS - 2)
//column offset of 1 to accomodate for corners and NUL terminator
#define BORDER_COLS (MENU_COLS - 1)

#define SELECT(string) CSI "30;47m" string CSI "0m"

#define EDIT_X 20
#define EDIT_Y 7

#define EDIT_ROWS 4

#define TEXT_POS    CUP(3,2)
#define DATA_POS    CUP(20,4)

#define DBL_LN      CSI "2E"
#define SGL_LN      CSI "1E"

#define TEXT_X      CSI "2C"
#define DATA_X      CSI "18C"

#define FG_BY(fmt)  CSI "93m" fmt CSI "0m"

enum menuPos
{
    CF_Y = 7, TY_Y, RL_Y, CC_Y
};

#endif