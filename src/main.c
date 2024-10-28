#include <cave_wizard.h>

int main(void)
{
    Instance ins;
    while (status(GET) != QUIT)
    {
        update();
        render();
    }
//i edited this
    //i edited this again
    return EXIT_SUCCESS;
}
