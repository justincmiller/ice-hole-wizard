#include "vterminal.h"

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