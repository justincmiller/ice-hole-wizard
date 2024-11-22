/*
*  Credit is given to Dr. Larry Hughes for providing the reference code
*  that was used both as inspiration and used as is throughout this program.
*/

//cave_wizard header includes program operation code
#include "engine.h"

int main(void)
{
    init(); //initializes the console setup

    while (status() != QUIT) //while the quit state is not triggered continuously call update
    {
        pollWindow();
        pollKbInput();
    }
    
    purge();

    return EXIT_SUCCESS;
}
