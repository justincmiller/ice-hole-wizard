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
    return EXIT_SUCCESS;
}
