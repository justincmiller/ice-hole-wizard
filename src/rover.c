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
		.battery = max_battery,
		.ice = false
	};
}

int battery_level() {

}

bool next_move() {

}

bool explore_map() {
	//move function similar to draw() or move() in cursor.c

}

