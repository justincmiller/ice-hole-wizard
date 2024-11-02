#include <cave_wizard.h>

int main(void)
{
    int state = MOVE;
    init();

    while (state != QUIT)
    {
        update();
        render();
        wizard(GET_STATE, &state);
    }

    return EXIT_SUCCESS;
}
