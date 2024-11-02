#include "cave_wizard.h"

//initializes the console with screen buffer and window settings
void init()
{
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE); //handle to console's standard output
    CONSOLE_SCREEN_BUFFER_INFO info; //structure to hold buffer information
    COORD buffer = {100, 100}; //console screen buffer size
    SMALL_RECT window; //defines the area for the console window
    int width, height;

    GetConsoleScreenBufferInfo(console, &info); //get current console screen buffer information
    width = info.srWindow.Right - info.srWindow.Left + 1; //calculate current window width
    height = info.srWindow.Bottom - info.srWindow.Top + 1; //calculate current window height
    
    //center the new window in the buffer 
    window.Left = (buffer.X - width)/2; 
    window.Top = (buffer.Y - height)/2;
    window.Right = (window.Left + width);
    window.Bottom = (window.Top + height);

    //set console screen buffer to the desired size
    SetConsoleScreenBufferSize(console, buffer);

    //adjust the console to the new centered window
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

/*
MAY NOT BE NEEDED: Console buffer may handle itself
char pollWindow()
{
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO info;
    SMALL_RECT window;
    int width, height;

    wizard(GET_VIEW, &window);
    width = window.Right - window.Left;
    height = window.Bottom - window.Top;

    GetConsoleScreenBufferInfo(console, &info);
    if (info.srWindow.Right - info.srWindow.Left != width 
    || info.srWindow.Bottom - info.srWindow.Top != height)
    {
        wizard(SET_VIEW, &info.srWindow);
        return 1;
    }

    return 0;
}*/

//updates program state based on user input
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

//renders the console
void render()
{
    //functionality to be defined as needed
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
        case HOME: //sets cursor to position 1,1
            
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
            if (_getch() == ';' && _getch() == '5')
            {
                //pan window
            }
            break;
        case INSERT:
            if (_getch() == '~') toggleState();
            break;
        case DEL:
            if (_getch() == '~')
                //erase character
            break;
        case PG_UP:
            if (_getch() == '~')
                //save buffer and move up
            break;
        case PG_DN:
            if (_getch() == '~')
                //save buffer and move down
            break;
        default:
            break;
    }
}