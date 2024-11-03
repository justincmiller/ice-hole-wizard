#include "vterminal.h"

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

/* ASCII-to-DEC graphic characters */
enum symbol asc_dec[] = {
    'j', 'k', 'l', 'm', 'n', 'q', 't', 'u', 'v', 'w', 'x', ' ' };


int draw_object(int col, int row, int symbol)
{
    /*
     - Attempts to draw symbol at position col, row
     - Returns -1 if illegal location, 0 otherwise
    */
    if (col <= /*min col value*/ || row <= /*min row value*/ ||
        col >= /*max col value*/ || row >= /*max row value*/)
        /* Outside view screen - do not draw */
        return -1;

    CUP(col, row);
    _putch(symbol);

    return 0;

}

bool virtualInput()
{
    HANDLE input = GetStdHandle(STD_INPUT_HANDLE);
    if (input == INVALID_HANDLE_VALUE)
    {
        printf("Error: could not fetch input handle\n");
        return false;
    }
    
    DWORD mode;

    if (!(GetConsoleMode(input, &mode)))
    {
        printf(">> Error: could not fetch input mode\n");
        return false;
    }

    mode |= ENABLE_VIRTUAL_TERMINAL_INPUT;

    if (!(SetConsoleMode(input, mode)))
    {
        printf(">> Error: could not set input mode\n");
        return false;
    }

    return true;
}

bool virtualOutput()
{
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE); //handle to the console's standard output
    if (output == INVALID_HANDLE_VALUE)
    {
        printf("Error: could not fetch output handle\n");
        return false;
    }

    DWORD mode;

    if (!(GetConsoleMode(output, &mode)))
    {
        printf(">> Error: could not fetch input mode\n");
        return false;
    }

    mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    
    if (!(SetConsoleMode(output, mode)))
    {
        printf(">> Error: could not set input mode\n");
        return false;
    }

    return true;
}