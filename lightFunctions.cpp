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


Blink flasher[2];

bool directlyToOutput(bool lightState) {
	return lightState;
}

bool highBeamFlash(bool lightState, bool lightFlashState, uint16_t flashFrequency) {
	if(lightState) return true;
	if(lightFlashState) return flasher[0].blink(flashFrequency);
	// else the reset
	flasher[0].resetBlink();
	return false;
}

