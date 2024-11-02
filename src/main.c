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
//i edited this
    //i edited this again
    //attempt 3
    return EXIT_SUCCESS;
}
