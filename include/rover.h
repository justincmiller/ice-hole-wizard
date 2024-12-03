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
    //x
    //y
    //battery life = max_battery
    //ice_found = false
}Rover;

//functions
//initialize rover
//battery cost/recovery
//check if move is valid
//explore map
//retreat low battery
//retreat ice found