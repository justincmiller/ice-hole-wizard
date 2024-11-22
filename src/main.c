/*
*  Credit is given to Dr. Larry Hughes for providing the reference code
*  that was used both as inspiration and used as is throughout this program.
*/

//engine header includes program operation headers and functions
#include "engine.h"

int main(void)
{
    init(); //initializes the console setup

    while (status() != QUIT) //while the quit state is not triggered
    {
        pollWindow(); //check if console was resized, update as required
        pollKbInput(); //check if key was pressed
    }
    
    purge(); //free memory from malloc'd linked-lists

    return EXIT_SUCCESS;
}
