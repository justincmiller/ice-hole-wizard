/*
* engine.c
*
* Contains
*/

/*
*  Credit is given to Dr. Larry Hughes for providing the reference code
*  that was used both as inspiration and used as is throughout this program.
*/

#include "engine.h" //program operation headers & functions

//static global variable for display (limited scope)
static Display* dsp;

//initializes the console setup
void init()
{
    //enable virtual terminal I/O
    virtualOutput(); //VT100 drawings
    virtualInput();  //VT100 Esc commands

    //load modules
    loadDisplay(&dsp);
    loadMap(dsp);
    loadMenu(dsp);
    loadCursor(dsp);
    
    //set initial state to movement
    dsp->state = MOVE;
    
    //set cursor to origin and set window size and margins
    setCursor(X_COL(1), Y_ROW(1));
    setWindow();
}

//get program state: draw, move, quit
int status()
{
    return dsp->state;
}

//checks to see if key was pressed, calls addional system functions as necessary
void pollKbInput()
{
    if (!_kbhit()) return; //if no key pressed return to main
    
    char buffer[SQLEN] = {0};
    int code = 0;
    int n = 0;

    //stucture to hold key press strings
    const char* keys[] =
    {
        "p",        //p key
        "\r",       //enter
        CSI "A",    //up arrow
        CSI "B",    //down arrow
        CSI "C",    //right arrow
        CSI "D",    //left arrow
        CSI "H",    //home
        CSI "F",    //end
        CSI "2~",   //insert
        CSI "3~",   //delete
        CSI "5~",   //page up
        CSI "6~",   //page down
        CSI "1;5A", //ctrl + up arrow
        CSI "1;5B", //ctrl + down arrow
        CSI "1;5C", //ctrl + right arrow
        CSI "1;5D", //ctrl + left arrow
        ESC         //escape
    };

    //while keys are being pressed and do not exceed sequence limit
    while (_kbhit() && n < SQLEN)
    {
        buffer[n++] = (char)_getch(); //store key strokes as a string
    }

    //compare keyboard input string to saved string options
    for (int i = 0; i < CONTROLS; i++)
    {
        if (strcmp(buffer, keys[i]) == 0)
        {
            code = i + 1; //if the comparison if found set string code value
            break;
        }
    }

    //send string code value to appropriate function based on current system state
    if (dsp->state & EDIT)
    {
        editControls(code); //edit state (not mutually exclusive)
        return;
    }
    else if (dsp->state & MOVE)
    {
        moveControls(code); //move state
        return;
    }
    else if (dsp->state & DRAW)
    {
        drawControls(code); //draw state
        return;
    }
}

//move state cursor controller
void moveControls(const int code)
{
    switch (code) //string code value from pollKbInput
    {
        case ARROW_UP:
        case ARROW_DOWN:
        case ARROW_RIGHT:
        case ARROW_LEFT:
            move(code); //move cursor
            return;
        case HOME:
            setCursor(X_COL(1), Y_ROW(1)); //move cursor to origin
            resetMargins(); //update screen to reflect move
            return;
        case END:
            dsp->state = QUIT; //quits the program
            return;
        case INSERT:
            dsp->state ^= ( MOVE | DRAW); //changes system state
            render(); //hides cursor in draw state
            return;
        case DEL:
            remCell(); //erase character at cursor position
            return;
        case PG_UP:
            layerUp(); //saves current layer, moves up one layer
            return;
        case PG_DN:
            layerDown(); //saves current layer, creates new map layer or uses existing
            return;
        case CTRL_UP:
        case CTRL_DOWN:
        case CTRL_RIGHT:
        case CTRL_LEFT:
            panViewport(code); //moves viewport in appropriate direction
            return;
        case P:
            dsp->state |= EDIT; //set state to edit
            editor(); //call function to edit cell properties
            render();
            return;
    }
}

//draw state controller
void drawControls(const int code)
{
    switch (code) //string code value from pollKbInput
    {
        case ARROW_UP:
        case ARROW_DOWN:
        case ARROW_RIGHT:
        case ARROW_LEFT:
            draw(code); //move and draw map
            return;
        case HOME:
            topLayer(); //move to map layer 0
            break;
        case END:
            lastLayer(); //move to the last map layer created
            break;
        case INSERT:
            dsp->state ^= ( MOVE | DRAW); //change system state
            render(); //enable cursor for move state
            return;
        case DEL:
            remCell(); //erase character at cursor position
            break;
        case PG_UP:
            layerUp(); //saves current layer, moves up one layer
            break;
        case PG_DN:
            layerDown(); //saves current layer, creates new map layer or uses existing
            break;
        case CTRL_UP:
        case CTRL_DOWN:
        case CTRL_RIGHT:
        case CTRL_LEFT:
            panViewport(code); //moves viewport in appropriate direction
            return;
        case P:
            dsp->state |= EDIT; //set state to edit
            editor(); //call function to edit cell properties
            render();
            break;
    }
}

//edit state controller
void editControls(const int code)
{
    switch (code) //string code value from pollKbInput
    {
        case ARROW_UP:
        case ARROW_DOWN:
            option(code); //move through menu options
            break;
        case P:
            dsp->state &= ~EDIT; //set state to edit
            render();
            break;
        case ENTER:
            edit(); //select option to edit and update as necessary
            break;
    }
}
