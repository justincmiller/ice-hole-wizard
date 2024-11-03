//cave_wizard header includes program operation code
#include <cave_wizard.h>

int main(void)
{
    init(); //initializes the console setup

    while (getState() != QUIT) //while the quit state is not triggered continuously call update
    {
        update(); //updates program based on keyboard input
    }
    
    return EXIT_SUCCESS;
}
