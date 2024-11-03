#include "cave_wizard.h"

//initializes the console with screen buffer and window settings
void init()
{
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE); //handle to the console's standard output
    CONSOLE_SCREEN_BUFFER_INFO info; //structure to hold information about the console screen buffer
    COORD buffer = { 100, 100 }; //console screen buffer size
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

    //sets the console window size
    if (!(SetConsoleWindowInfo(output, TRUE, &window)))
    {
        //print error if it fails
        printf(">> Error: unable to set console window size. Error code: %lu\n", GetLastError());
        setState(QUIT); //exits program
        return;
    }

    //sets the console screen buffer size
    if (!(SetConsoleScreenBufferSize(output, buffer)))
    {
        //print error if it fails
        printf(">> Error: unable to set console buffer size. Error code: %lu\n", GetLastError());
        setState(QUIT); //exits program
        return;
    }

    //set the screen buffer size and window position for the console
    SetConsoleScreenBufferSize(console, buffer);
    SetConsoleWindowInfo(console, TRUE, &window);


    /*
    *  Credit is given to Dr. Larry Hughes for providing the reference code
    *  that was used throughout this program specifically the DEC Line Drawing Mode
    *  which uses much of Dr. Hughes' work.
    *  Credit to Dr. Hughes for the following code: lines 28 - 65
    */

    //initialize cursor to center of the screen
    int initial_x = width / 2;
    int initial_y = height / 2;
    int current_direction = IDLE;

    printf(CSI "%dm" CSI "%dm", BGYELLOW, FGBLACK);

    draw_object(initial_x, window.Bottom, window.Top, initial_y, window.Left, window.Right, START_SYM);
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
            case INSERT:
                if (getState() == MOVE) move(key); // go to move state operation function
                else if (getState == DRAW) draw(key); // go to draw state operation function
                break;
            case DEL: //both states: erase character at cursor position
            case PG_UP: //both states: save current layer, move up one layer.
            case PG_DN: //both states: save current layer,move down one layer by either creating a new layer or using an existing layer if available
            default: //if follows logically that if its non of these then its either a Ctrl key sequence or an invalid key
                parseKey(key); //if input was not an arrow or movement key
                break;
        }
    }
}

void render()
{

}

//move state operations
void move(const char key)
{
    switch (key)
    {
        case ARROW_UP: //moves cursor up columns by 1 unit
            CUU(1);
            break;
        case ARROW_DOWN: //moves cursor down columns by 1 unit
            CUD(1);
            break;
        case ARROW_RIGHT: //moves cursor right in the row by 1 unit
            CUF(1);
            break;
        case ARROW_LEFT: //moves cursor left in the row by 1 unit
            CUB(1);
            break;
        case HOME: //moves cursor to screen position (1,1)
            CUP(1, 1);
            break;
        case END: //sets state to QUIT (exits the program)
            setState(QUIT);
            return;
        default:
            break;
    }
}

//draw state operations
void draw(const char key)
{

    EDLDM; //enables DEC Line Drawing Mode

    switch (key)
    {
        case ARROW_UP: //draws map in the positive latitude direction 1 unit
            CUU(1);
            _putch('x'); //vertical line
            break;
        case ARROW_DOWN: //draws map in the negative latitude direction 1 unit
            CUD(1);
            _putch('x');
            break;
        case ARROW_RIGHT: //draws map in the positive longitude direction 1 unit
            CUF(1);
            _putch('q'); //horizontal line
            break;
        case ARROW_LEFT: //draws map in the negaite longitude direction 1 unit
            CUB(1);
            _putch('q');
            break;
        case HOME: //moves to map layer 0
            CUP(0,0);
            break;
        case END: //move to the last map layer created
            //?
            break;
        default: //handles unforseen input
            break;
    }

    EAM; //enables ASCII Mode
}

//used for Ctrl and both-state commands
void parseKey(const char key)
{
    switch (key)
    {
        case CTRL:
            if ((char)_getch() == ';' && (char)_getch() == '5')
            {
                char c_key = (char)_getch();
                switch (c_key) {
                    case ARROW_UP: //move up 1 cell latitude
                        break;
                    case ARROW_DOWN: //move down 1 cell latitude
                        break;
                    case ARROW_RIGHT: //move right 1 cell longitude
                        break;
                    case ARROW_LEFT: //move left 1 cell longitude
                        break;
                    default:
                        break;
                }
            }
            break;
        case INSERT:
            if (_getch() == '~') toggleState(); //swaps states between map drawing mode and cursor movement mode
            break;
        case DEL:
            if (_getch() == '~') //erase character at curosr position
            break;
        case PG_UP:
            if (_getch() == '~') //save current layer, move up one layer
            break;
        case PG_DN:
            if (_getch() == '~') //save current layer, move down one layer by either creating a new layer or using an existing layer if available
            break;
        default:
            break;
    }
}