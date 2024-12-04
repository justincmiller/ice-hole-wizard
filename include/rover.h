/*
* rover.h
*
* This file contains the structures and definitions used by the program for
* the rover.
*/

/*
*  Credit is given to Dr. Larry Hughes for providing the reference code
*  that was used both as inspiration and used as is throughout this program.
*/

#ifndef ROVER_H
#define ROVER_H

#inlcude "engine.h"

#define battery_max 150
#define battery_loss 0.05

//structure to hold battery loss and gain data
typedef enum {
    NORMAL = 0,
    A_SLOPE = 1,
    D_SLOPE = 2,
    RB = 3,
    RAD = 4,    
} BATTERY_CONDITIONS;

//structure that holds all the rover information
typedef struct Rover
{
    int x;
    int y;
    int battery = battery_max;
    bool ice = false;
}Rover;

//functions
void rover_init();//initialize rover
int battery_level();//battery cost/recovery
bool next_move();//check if move is valid
bool explore_map();//explore map