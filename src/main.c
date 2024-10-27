#include <cave_wizard.h>

int main(void)
{
    Instance ins;
    while (status(GET) != QUIT)
    {
        update();
        render();
    }

    return EXIT_SUCCESS;
}
