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

int battery_impact() {
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
bool next_move(int x, int y) {
	//must be within map bounds
	//must be on a drawn path
}

//print layer # and coords of ice
void ice_ice_baby() {
	//print congrats banner if ice is found?
}

bool explore_map() {
	//possible directions
	while (rover->battery > battery_warning) {
		//check adjacent cells for ice
		//move towards cell with highest % ice
		
		//if 100% ice
		if () { //rover->x == 100% ice && rover->y == 100% ice
			rover->ice = true;
			ice_ice_baby();
		}
		
		//make move
		bool moved = false;
		for (int i = 0; i < no_directions; i++) { //loop directions
			int new_x = rover->x + dx[i];
			int new_y = rover->y + dy[i];

			//check if move is valid
			if (next_move(new_x, new_y)) {
				//cell properties
				//need cell type

				//wall (or no path)
				if () continue; // cell == wall

				//update battery
				int battery_cost = battery_impact();

				//check if we have enough battery
				if (rover->battery > battery_warning) {
					rover->x = new_x;
					rover->y = new_y;
					rover->battery -= battery_cost;
					moved = true;
					break;
				}
			}
		}

		//if no move was made
		if (!moved) {
			//rover is broken or stuck
			return false;
		}
	}
	
	//if battery hits threshold to return
	if (rover->battery <= battery_warning) {
		//abandon mission
	}

	//default if failure
	return false;
}

