#line 1 "/home/magraina/projects/truckLightAndFunction/modules/lightFunctions.cpp"
/************************************ 
 * truckLightAndFunction v0.0.11
 * Date: 10.06.2020 | 00:25
 * <Truck Light and function module>
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
#include "Arduino.h"
#include "lightFunctions.h"
#include "mapSwitchToFunctions.h" // get mapping from there
#include "../config.h"


// parkLightState = 0;
// lowBeamLightState = 0;
// highBeamLightState = 0;
// highBeamLightFlashState = 0;
// fogLightState = 0;
// leftFlashLightState = 0;
// RightFlashLightState = 0;
// hazardLightState = 0;
// beaconLightState = 0;
// auxLightState = 0;

void runLightFunctions() {
	parkLightOut = parkLightState;
	lowBeamLightOut = lowBeamLightOut;
	if(highBeamLightState) {
		highBeamLightOut = true;
	} else if(highBeamLightFlashState) {
		highBeamLightOut = flasher[0].blink(HIGH_BEAM_FLASH_FREQUENCY);
	} else {
		highBeamLightOut = false;
		flasher[0].resetBlink();
	}
}

