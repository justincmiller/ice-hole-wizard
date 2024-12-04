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
	//think he said to ignore powder?
	//needs to using battery_conditions to make calculations
	//friction makes penalty = |friction - 5| / 5
	//battery gain based on RB and battery_gain
	switch () {
		case;
		default: return battery_loss;
	}
}

//check if move is valid
bool next_move() {
	//must be within map bounds
	//must be on a drawn path
}

bool explore_map() {
	//possible directions
	while (rover->battery > battery_warning) {
		//stuff
		
		if (!moved) {
			//rover is broken or stuck
			return false;
		}
	}

	return false;
}

void ice_ice_baby() {
	//print congrats banner if ice is found?
}

