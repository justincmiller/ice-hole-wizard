#include <cave_wizard.h>

int main(void)
{
    while (status(GET) != QUIT)
    {
        update();
        render();
    }

    return EXIT_SUCCESS;
}