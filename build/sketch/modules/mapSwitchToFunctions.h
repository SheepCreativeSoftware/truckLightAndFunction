#line 1 "/home/magraina/projects/truckLightAndFunction/modules/mapSwitchToFunctions.h"
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

#ifndef _MAP_SWITCH_TO_FUNCTIONS_H_
#define _MAP_SWITCH_TO_FUNCTIONS_H_

// Include for edge class
#include "tools.h"
#include "Arduino.h"


#ifndef DIRECTION_UP
	#define DIRECTION_UP 1
#endif
#ifndef DIRECTION_MID
	#define DIRECTION_MID 2
#endif
#ifndef DIRECTION_DOWN
	#define DIRECTION_DOWN 3
#endif

//declare vars
bool parkLightState = 0;
bool lowBeamLightState = 0;
bool highBeamLightState = 0;
bool highBeamLightFlashState = 0;
bool fogLightState = 0;
bool leftFlashLightState = 0;
bool RightFlashLightState = 0;
bool hazardLightState = 0;
bool beaconLightState = 0;
bool auxLightState = 0;

//declare fuctions
void runSwitchToFunction();

//declare classes
EdgeEvaluation edgeEval[2];

#endif