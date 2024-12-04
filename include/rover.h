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
#define battery_warning battery_max/1.8
#define no_directions 4

//directions
int dx[] = {0, 1, 0, -1};
int dy[] = {1, 0, -1, 0};

//structure to hold battery gain data from RB
typedef enum {
    R0 = 0,
    R1 = 0.5,
    R2 = 2.5,
    R3 = 5,
} battery_gain;

//structure that holds all the rover information
typedef struct Rover
{
    int x;
    int y;
    int battery = battery_max;
    bool ice = false;
}Rover;

//functions
void rover_init(); //initialize rover
int battery_impact(); //battery cost/recovery
bool next_move(int x, int y); //check if move is valid
void ice_ice_baby(); //ice found
bool explore_map(); //explore map