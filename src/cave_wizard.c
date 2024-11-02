#include "cave_wizard.h"

//initializes the console with screen buffer and window settings
void init()
{
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE); //handle to the console's standard output
    CONSOLE_SCREEN_BUFFER_INFO info; //structure to hold information about the console screen buffer
    COORD buffer = {100, 100}; //console screen buffer size
    SMALL_RECT window; //defines area for console window
    int width, height;

    //retrieve current console screen buffer information
    if (!(GetConsoleScreenBufferInfo(output, &info)))
    {
        //display error if it fails
        printf(">> Error: unable to get console screen buffer info.\n");
        setState(QUIT); //exits program
        return;
    }

    //calculate the width and height of the console window
    width = info.srWindow.Right - info.srWindow.Left + 1;
    height = info.srWindow.Bottom - info.srWindow.Top + 1;

    //adjust buffer if it is smaller than the current window dimensions
    if (buffer.X < width || buffer.Y < height)
    {
        buffer.X = width;
        buffer.Y = height;
    }

    //set the window size parameters starting from (0,0)
    window.Left = 0;
    window.Top = 0;
    window.Right = width - 1;
    window.Bottom = height - 1;

    //sets the console screen buffer size
    if (!(SetConsoleWindowInfo(output, TRUE, &window)))
    {
        printf(">> Error: unable to set console window size. Error code: %lu\n", GetLastError());
        setState(QUIT);
        return;
    }

    if (!(SetConsoleScreenBufferSize(output, buffer)))
    {
        printf(">> Error: unable to set console buffer size. Error code: %lu\n", GetLastError());
        setState(QUIT);
        return;
    }

    SetConsoleScreenBufferSize(console, buffer);
    SetConsoleWindowInfo(console, TRUE, &window);
}

//manages state data and viewport data
void wizard(const int action, void* data)
{
    //static variables to retain stat and viewport between function calls
    static int state;
    static SMALL_RECT viewport;

    //determine action based on the input parameter
    switch (action)
    {
        case GET_STATE: //retrieve current state
            *(int*)data = state;
            return;
        case SET_STATE: //update state
            state = *(int*)data;
            return;
        case GET_VIEW: //retrieve current viewport
            *(SMALL_RECT*)data = viewport;
            return;
        case SET_VIEW: //update viewport
            viewport = *(SMALL_RECT*)data;
            return;
        default: //incase of unexpected input
            return;
    }
}

//get current state using the wizard function
int getState()
{
    int state = 0;
    wizard(GET_STATE, &state); //called with get_state to retrieve current state
    return state;
}

//sets new state using the wizard function
void setState(const int state)
{
    wizard(SET_STATE, &state); //called with set_state to update the state
}

//toggles the current state between move and draw modes
void toggleState()
{
    if (getState() == MOVE)
    {
        setState(DRAW); //if in move state, set to draw
    }
    else if (getState() == DRAW)
    {
        setState(MOVE); //if in draw, set to move state
    }
}

    return 0;
}*/

void update()
{
    //check if key has been pressed
    if (_kbhit() == 0) return;
    
    //if a key was pressed, get key character
    char key = (char)_getch();

    //update as necessary for addtional alphanumeric entries
    if (key == 'p')
    {
        //edit cell properties
    }
    else if (key == ESC && (char)_getch() == '[') //check for escape sequence for special keys
    {
        //get the next character to feed into the switch case
        key = (char)_getch();

        switch (key)
        {
            case ARROW_UP:
            case ARROW_DOWN:
            case ARROW_RIGHT:
            case ARROW_LEFT:
            case HOME:
            case END:
                if (getState() == MOVE) move(key); //move cursor if in move state
                else if (getState() == DRAW) draw(key); //draw if in draw state
                break;
            default:
                parseKey(key); //if input was not an arrow or movement key
                break;
        }
    }
}

void render()
{

}

//handles cursor movement in move state
void move(const char key)
{
    switch (key)
    {
        case ARROW_UP: //moves cursor up by 1 unit
            CUU(1);
            break;
        case ARROW_DOWN: //moves cursor down by 1 unit
            CUD(1);
            break;
        case ARROW_RIGHT: //moves cursor right by 1 unit
            CUF(1);
            break;
        case ARROW_LEFT: //moves cursor left by 1 unit
            CUB(1);
            break;
        case HOME:
            
            break;
        case END: //sets state to QUIT
            setState(QUIT);
            return;
        default:
            break;
    }
}

//handles drawing operations in draw state
void draw(const char key)
{
    switch (key)
    {
        case ARROW_UP:
            break;
        case ARROW_DOWN:
            break;
        case ARROW_RIGHT:
            break;
        case ARROW_LEFT:
            break;
        case HOME:
            break;
        case END:
            break;
        default:
            break;
    }
}

//parses additional key inputs, used for control commands
void parseKey(const char key)
{
    switch (key)
    {
        case CTRL:
            if (_getch() == ';' && _getch() == '5') //Ctrl + 5
            {
                //pan window
            }
            break;
        case INSERT:
            if (_getch() == '~') toggleState(); //Insert + ~
            break;
        case DEL:
            if (_getch() == '~') //Delete + ~
                //erase character
            break;
        case PG_UP:
            if (_getch() == '~') //Page Up + ~
                //save buffer and move up
            break;
        case PG_DN:
            if (_getch() == '~') //Page Down + ~
                //save buffer and move down
            break;
        default:
            break;
    }
}