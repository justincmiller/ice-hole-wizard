#include <cave_wizard.h>

int main(void)
{
    init();

    while (getState() != QUIT)
    {
        update();
    }

    return EXIT_SUCCESS;
}
