/*
*  Credit is given to Dr. Larry Hughes for providing the reference code
*  that was used throughout this program. Specifically, the engine.h, vterminal.h, and vterminal.c files
*  which uses much of Dr. Hughes' work.
*/

#include "vterminal.h"
#include "display.h"
#include "engine.h"

#define START_SYM '*'

enum direction { NORTH, SOUTH, EAST, WEST, IDLE };

/* Tunnel symbol to display [new dir][old dir] */
char cell_sym[4][5] = {
    /*	Old: NO  SO  ET  WT  IDL    New:*/
            {VR, VR, LR, LL, VR}, /* NO */
            {VR, VR, UR, UL, VR}, /* SO */
            {UL, LL, HR, HR, HR}, /* ET */
            {UR, LR, HR, HR, HR}  /* WT */
};

short del_y[] = { -1, 1, 0, 0 };
short del_x[] = { 0, 0, 1, -1 };

Draw_Cursor draw_cur;

/* ASCII-to-DEC graphic characters */
enum symbol asc_dec[] = {
    'j', 'k', 'l', 'm', 'n', 'q', 't', 'u', 'v', 'w', 'x', ' ' };

int draw_object(int col, int row, int symbol)
{
    //our screen is going to follow the cursor so were not worried about it printing off screen
    /*
     - Attempts to draw symbol at position col, row
     - Returns -1 if illegal location, 0 otherwise
    */
    //if (col <= /*min col value*/ || row <= /*min row value*/ ||
        //col >= /*max col value*/ || row >= /*max row value*/)
        /* Outside view screen - do not draw */
        //return -1;

    CUP(col, row);
    _putch(symbol);

    return 0;

}

void draw_cursor_init() {
    draw_cur.curr_dir = IDLE;

    printf(CSI "%dm" CSI "%dm", BGGREEN, FGBLACK);

    draw_object(draw_cur.x, draw_cur.y, START_SYM);
}

void draw_cursor_move(char ch) {
    enum direction new_dir;
    enum symbol new_off; /* Offset into asc_dec[] */
    char new_sym;	/* Symbol from asc_dec[] */
    int new_x;
    int new_y;

    /* DEC Keypad sequence (U, D, R, L) */
    new_x = draw_cur.x + del_x[ch - 'A'];
    new_y = draw_cur.y + del_y[ch - 'A'];

    /* Should check for edge of world */

    /* New direction? */
    if (new_y < draw_cur.y)
        new_dir = NORTH;
    else if (new_y > draw_cur.y)
        new_dir = SOUTH;
    else if (new_x < draw_cur.x)
        new_dir = WEST;
    else
        new_dir = EAST;

    /* Get symbol offset */
    new_off = cell_sym[new_dir][draw_cur.curr_dir];
    new_sym = asc_dec[new_off];

    /* Draw symbol if possible */

    EDLDM /* Enable DEC line drawing mode */

        if (draw_object(draw_cur.x, draw_cur.y, new_sym) == 0)
        {
            /* Draw worked */
            /* Remember last valid position */
            draw_cur.old_x = draw_cur.x;
            draw_cur.old_y = draw_cur.y;
            draw_cur.old_dir = draw_cur.curr_dir;
            draw_cur.curr_dir = new_dir;
            /* Possible overshoot to invalid position */
            draw_cur.x = new_x;
            draw_cur.y = new_y;
        }
        else
        {
            /* Restore robot to last valid position */
            draw_cur.x = draw_cur.old_x;
            draw_cur.y = draw_cur.old_y;
            draw_cur.curr_dir = draw_cur.old_dir;
        }

    EAM /* Enable ASCII mode */
}

