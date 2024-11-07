/*
*  Credit is given to Dr. Larry Hughes for providing the reference code
*  that was used throughout this program. Specifically, the engine.h, vterminal.h, and vterminal.c files
*  which uses much of Dr. Hughes' work.
*/

#include "engine.h"

//statc global variable to limit scope
static int state;

//initializes the console with screen buffer and window settings
void init()
{
    virtualOutput();
    virtualInput();
    initDisplay();
    state = MOVE;
}

int status()
{
    return state;
}

void pollInput()
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
                if (state == MOVE) move(key); // go to move state operation function
                else if (state == DRAW) draw(key); // go to draw state operation function
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
            moveCursor(UP);
            break;
        case ARROW_DOWN: //moves cursor down columns by 1 unit
            moveCursor(DOWN);
            break;
        case ARROW_RIGHT: //moves cursor right in the row by 1 unit
            moveCursor(RIGHT);
            break;
        case ARROW_LEFT: //moves cursor left in the row by 1 unit
            moveCursor(LEFT);
            break;
        case HOME: //moves cursor to screen position (1,1)
            setCursor(X_COL(1), Y_ROW(1));
            resetMargins();
            break;
        case END: //sets state to QUIT (exits the program)
            state = QUIT;
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
            drawCursor(UP);
            break;
        case ARROW_DOWN: //draws map in the negative latitude direction 1 unit
            drawCursor(DOWN);
            break;
        case ARROW_RIGHT: //draws map in the positive longitude direction 1 unit
            drawCursor(RIGHT);
            break;
        case ARROW_LEFT: //draws map in the negaite longitude direction 1 unit
            drawCursor(LEFT);
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
                    case ARROW_UP: //move map up 1 latitude in screen
                        panViewport(UP);
                        break;
                    case ARROW_DOWN: //move map down 1 latitude in screen
                        panViewport(DOWN);
                        break;
                    case ARROW_RIGHT: //move map right 1 longitude in screen
                        panViewport(RIGHT);
                        break;
                    case ARROW_LEFT: //move map left 1 longitude in screen
                        panViewport(LEFT);
                        break;
                    default:
                        break;
                }
            }
            break;
        case INSERT:
            if (_getch() == '~')
            {
                //toggle between map drawing or cursor movement state
                state = (state == MOVE) ? DRAW : MOVE;
            }
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

void purge()
{
    freeDisplay();
}