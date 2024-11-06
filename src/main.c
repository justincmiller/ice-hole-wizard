/*
*  Credit is given to Dr. Larry Hughes for providing the reference code
*  that was used throughout this program. Specifically, the engine.h, vterminal.h, and vterminal.c files
*  which uses much of Dr. Hughes' work.
*/

//cave_wizard header includes program operation code
#include "engine.h"

int main(void)
{
    init(); //initializes the console setup

    while (status() != QUIT) //while the quit state is not triggered continuously call update
    {
        pollWindow();
        pollInput();
    }
    
    purge();

    return EXIT_SUCCESS;
}
