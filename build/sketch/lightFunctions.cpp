#line 1 "/home/magraina/projects/truckLightAndFunction/lightFunctions.cpp"
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

#include "lightFunctions.h"
#include "tools.h"

#ifndef BLINKER_FREQUENCY
#define BLINKER_FREQUENCY 1000
#endif

#ifndef HIGH_BEAM_FLASH_FREQUENCY
#define HIGH_BEAM_FLASH_FREQUENCY 500
#endif

Blink flasher[2];

bool directlyToOutput(bool lightState) {
	return lightState;
}

bool highBeamFlash(bool lightState, bool lightFlashState) {
	if(lightState) return true;
	if(lightFlashState) return flasher[0].blink(HIGH_BEAM_FLASH_FREQUENCY);
	// else then reset
	flasher[0].resetBlink();
	return false;
}

void setFlasherLight(bool leftFlasherState, bool rightFlasherState, bool hazardState, bool* outLeftLight, bool* outRightLight) {
	if(hazardState) {
		bool blinkerState = flasher[1].blink(BLINKER_FREQUENCY);
		*outLeftLight = blinkerState;
		*outRightLight = blinkerState;
	} else if(leftFlasherState) {
		*outLeftLight = flasher[1].blink(BLINKER_FREQUENCY);
	} else if(rightFlasherState) {
		*outRightLight = flasher[1].blink(BLINKER_FREQUENCY);
	} else if((*outLeftLight || *outRightLight) && flasher[1].blink(BLINKER_FREQUENCY)) {
		*outLeftLight = false;
		*outRightLight = false;
		flasher[1].resetBlink();
	}
}

