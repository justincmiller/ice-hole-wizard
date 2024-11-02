#include <cave_wizard.h>

int main(void)
{
    init();

    while (getState() != QUIT)
    {
        update();
        render();
    }

    return EXIT_SUCCESS;
}
