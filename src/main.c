/*
*  Credit is given to Dr. Larry Hughes for providing the reference code
*  that was used both as inspiration and used as is throughout this program.
*/

//engine header includes program operation headers and functions
#include "engine.h"

int main(int argc, char* argv[])
{
    init(); //initializes the console setup

    if (argc > 1)
    {
        //process first argument only
        if (importFile(argv[ARGUMENT]) == false)
        {
            //trim full path to file name
            char* fileName = strrchr(argv[ARGUMENT], SLASH);
            fileName = (fileName == NULL) ? argv[ARGUMENT] : ++fileName;

            printf(MESSAGE ">> Error: could not open file [%s].", fileName);
            filePrompt();
        }
    }
    else
        filePrompt();

    while (status() != QUIT) //while the quit state is not triggered
    {
        pollWindow(); //check if console was resized, update as required
        pollKbInput(); //check if key was pressed
    }
    
    purge(); //free memory from malloc'd linked-lists

    return EXIT_SUCCESS;
}
