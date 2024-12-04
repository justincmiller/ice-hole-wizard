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

#define max_battery 100

//structure that holds all the rover information
typedef struct Rover
{
    int x;
    int y;
    int battery = max_battery;
    bool ice = false;
}Rover;

//functions
void rover_init();//initialize rover
int battery_level();//battery cost/recovery
bool next_move();//check if move is valid
bool explore_map();//explore map