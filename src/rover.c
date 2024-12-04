/*
* rover.c
*
* This file contains rover initialization functions as well as
* functions to automate.
*/

/*
*  Credit is given to Dr. Larry Hughes for providing the reference code
*  that was used both as inspiration and used as is throughout this program.
*/

#include "rover.h"

void rover_init() {
	Rover rover = {
		.x = 0,
		.y = 0,
		.battery = battery_max,
		.ice = false
	};

	//get coords from file and map them to .x and .y
}

int battery_level() {
	//needs to using battery_conditions to make calculations
	//friction makes penalty = |friction - 5| / 5
}

bool next_move() {

}

bool explore_map() {
	//move function similar to draw() or move() in cursor.c

}

