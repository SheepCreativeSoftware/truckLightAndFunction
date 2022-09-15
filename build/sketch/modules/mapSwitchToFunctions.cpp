#line 1 "/home/magraina/projects/truckLightAndFunction/modules/mapSwitchToFunctions.cpp"
/************************************ 
 * Copyright (C) 2020 Marina Egner <info@sheepindustries.de>
 *
 * This program is free software: you can redistribute it and/or modify it 
 * under the terms of the GNU General Public License as published by the 
 * Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see <https://www.gnu.org/licenses/>.
 ************************************/

#include "mapSwitchToFunctions.h"
#include "readPPMdata.h" // Get Channel vars from there

void runSwitchToFunction() {

	// channel1Poti[0] row below:	1 Poti (0-100% -> 1000-2000)
	// channel1Poti[1]	row below:	2 Poti (0-100% -> 1000-2000)
	// channel1Switch[0]	row below:	3 switch (up/mid/down 1000/1500/2000)
	// channel1Switch[1]	row below: 	4 switch (up/mid/down 1000/1500/2000)
	// channel1Switch[2]	row above:	1 button (up/mid/down 1000/1500/2000)
	// channel1Switch[3]	row above:	2 switch (up/mid/down 1000/1500/2000)
	// channel1Switch[4]	row above:	3 switch (up/mid/down 1000/1500/2000)
	// channel1Switch[5]	row above:	4 button/switch (up/mid/down 1000/1500/2000)

	// Fourth from left switch in upper row
	switch (channel1Switch[5]) {
	case DIRECTION_DOWN:
		highBeamLightFlashState = false;
		highBeamLightState = true;
		break;
	case DIRECTION_MID:
		highBeamLightFlashState = false;
		highBeamLightState = false;
		break;
	case DIRECTION_UP:
		highBeamLightFlashState = true; // Only catch high edge
		highBeamLightState = false;
		break;
	default:
		highBeamLightFlashState = false;
		highBeamLightState = false;
		break;
	}

	// Fourth from left switch in bottom row
	switch (channel1Switch[1]) {
	case DIRECTION_DOWN:
		leftFlashLightState = true;
		RightFlashLightState = false;
		break;
	case DIRECTION_MID:
		leftFlashLightState = false;
		RightFlashLightState = false;
		break;
	case DIRECTION_UP:
		leftFlashLightState = false;
		RightFlashLightState = true;
		break;
	default:
		highBeamLightFlashState = false;
		highBeamLightState = false;
		break;
	}

	// channel2Switch[0]	row above:	4 button (up/down 2000/1000)
	// channel2Switch[1]	row above:	3 switch (up/down 2000/1000)
	// channel2Switch[2]	row above:	2 switch (up/down 2000/1000)
	// channel2Switch[3]	row above:	1 switch (up/down 2000/1000)
	// channel2Switch[4]	row below:	2 switch (up/mid/down 1000/1700/1700)
	// 						row below:	2 switch (up/mid/down 1700/1700/1000)
	// channel2Switch[5]	row below:	1 switch (up/mid/down 1000/1700/1700)
	// 						row below:	1 switch (up/mid/down 1700/1700/1000)
	
	// First from left switch in bottom row
	switch (channel2Switch[5]) {
	case DIRECTION_DOWN:
		parkLightState = false;
		lowBeamLightState = false;
		break;
	case DIRECTION_MID:
		parkLightState = true;
		lowBeamLightState = false;
		break;
	case DIRECTION_UP:
		parkLightState = true;
		lowBeamLightState = true;
		break;
	default:
		parkLightState = false;
		lowBeamLightState = false;
		break;
	}

	// First from left switch in upper row
	switch (channel2Switch[3]) {
	case DIRECTION_DOWN:
		fogLightState = false;
		break;
	case DIRECTION_UP:
		fogLightState = true;
		break;
	default:
		fogLightState = false;
		break;
	}

	// Second from left switch in upper row
	switch (channel2Switch[2]) {
	case DIRECTION_DOWN:
		hazardLightState = false;
		break;
	case DIRECTION_UP:
		hazardLightState = true;
		break;
	default:
		hazardLightState = false;
		break;
	}

	// Third from left switch in upper row
	switch (channel2Switch[1]) {
	case DIRECTION_DOWN:
		beaconLightState = false;
		break;
	case DIRECTION_UP:
		beaconLightState = true;
		break;
	default:
		beaconLightState = false;
		break;
	}

	// Fourth from left switch in upper row
	switch (channel2Switch[0]) {
	case DIRECTION_DOWN:
		auxLightState = false;
		break;
	case DIRECTION_UP:
		auxLightState = true;
		break;
	default:
		auxLightState = false;
		break;
	}


}