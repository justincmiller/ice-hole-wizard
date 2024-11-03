#include "engine.h"
#include "display.h"
#include "vterminal.h"

//initializes the console with screen buffer and window settings
void init()
{
    virtualOutput();
    virtualInput();
    
    reset();
    setState(MOVE);
}

void reset()
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    if (out == INVALID_HANDLE_VALUE)
    {
        printf("Error: could not fetch output handle\n");
        setState(QUIT);
        return;
    }

    CONSOLE_SCREEN_BUFFER_INFO info; //structure to hold information about the console screen buffer

    //retrieve current console screen buffer information
    if (!GetConsoleScreenBufferInfo(out, &info))
    {
        //display error if it fails
        printf(">> Error: unable to get console screen buffer info.\n");
        setState(QUIT); //exits program
        return;
    }

    SMALL_RECT window = info.srWindow;

    //calculate the width and height of the console window, offset of 1 due to indexing
    int width = window.Right - window.Left + 1;
    int height = window.Bottom - window.Top + 1;
    int left, top;

    left = width / 2;
    top = height / 2;

    CUP(left, top);
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
    wizard(GET_STATE, (void*)&state); //called with get_state to retrieve current state
    return state;
}

//sets new state using the wizard function
void setState(const int state)
{
    wizard(SET_STATE, (void*)&state); //called with set_state to update the state
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
    else if (key == '\x1b' && (char)_getch() == '[') //check for escape sequence for special keys
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
                if (getState() == MOVE) move(key); // go to move state operation function
                else if (getState() == DRAW) draw(key); // go to draw state operation function
                break;
            case INSERT:
            case DEL: //both states: erase character at cursor position
            case PG_UP: //both states: save current layer, move up one layer.
            case PG_DN: //both states: save current layer,move down one layer by either creating a new layer or using an existing layer if available
            default: //if follows logically that if its non of these then its either a Ctrl key sequence or an invalid key
                parseKey(key); //if input was not an arrow or movement key
                break;
        }
    }
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
    switch (key)
    {
        case ARROW_UP: //draws map in the positive latitude direction 1 unit
        case ARROW_DOWN: //draws map in the negative latitude direction 1 unit
        case ARROW_RIGHT: //draws map in the positive longitude direction 1 unit
        case ARROW_LEFT: //draws map in the negaite longitude direction 1 unit
            draw_cursor_move(key);
            break;
        case HOME: //moves to map layer 0
            //
            break;
        case END: //move to the last map layer created
            //
            break;
        default: //handles unforseen input
            break;
    }
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
            if (_getch() == '~') EL(1); //erase character at curosr position
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