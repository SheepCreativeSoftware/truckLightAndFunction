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
#include <SoftPWM.h>							// https://github.com/bhagman/SoftPWM


Blink flasher[3];

bool directlyToOutput(bool lightState) {
	return lightState;
}

bool highBeamFlash(bool lightState, bool lightFlashState, uint16_t flashFrequency) {
	if(lightState) return true;
	if(lightFlashState) return flasher[0].blink(flashFrequency);
	// else then reset
	flasher[0].resetBlink();
	return false;
}

void setFlasherLight(bool leftFlasherState, bool rightFlasherState, bool hazardState, bool* outLeftLight, bool* outRightLight, uint16_t flashFrequency) {
	if(hazardState) {
		bool blinkerState = flasher[1].blink(flashFrequency);
		*outLeftLight = blinkerState;
		*outRightLight = blinkerState;
	} else if(leftFlasherState) {
		*outLeftLight = flasher[1].blink(flashFrequency);
		*outRightLight = false;
	} else if(rightFlasherState) {
		*outLeftLight = false;
		*outRightLight = flasher[1].blink(flashFrequency);
	} else if((*outLeftLight || *outRightLight) && !flasher[1].blink(flashFrequency)) {
		*outLeftLight = false;
		*outRightLight = false;
	} else if (!*outLeftLight && !*outRightLight) {
		flasher[1].resetBlink();
	}
}

void initLightOutput() {
	SoftPWMBegin(); 					//Init Soft PWM Lib
}

void setupLightOutput(uint8_t pin, uint16_t fadeOnTime, uint16_t fadeOffTime) {
	SoftPWMSet(pin, SOFT_PWM_LOW);     	//Create and set pin to 0
	SoftPWMSetFadeTime(pin, fadeOnTime, fadeOffTime); //Set Fade on/off time for output
}

void setBooleanLight(uint8_t pin, bool state, uint8_t highValue = SOFT_PWM_HIGH) {
	if(state) SoftPWMSet(pin, highValue);
	if(!state) SoftPWMSet(pin, SOFT_PWM_LOW);
}

void setCombinedHeadlightAll(uint8_t pin,
							uint8_t parkingState,
							uint8_t lowBeamState,
							uint8_t highBeamState,
							uint8_t parkingOutValue,
							uint8_t lowBeamOutValue,
							uint8_t highBeamOutValue) {
	if(highBeamState) {
		SoftPWMSet(pin, highBeamOutValue);
	} else if(lowBeamState) {
		SoftPWMSet(pin, lowBeamOutValue);
	} else if(parkingState) {
		SoftPWMSet(pin, parkingOutValue);
	} else {
		SoftPWMSet(pin, SOFT_PWM_LOW);
	}
}
void setCombinedHeadlightHighOnly(uint8_t pin,
							uint8_t lowBeamState,
							uint8_t highBeamState,
							uint8_t lowBeamOutValue,
							uint8_t highBeamOutValue) {
	if(highBeamState) {
		SoftPWMSet(pin, highBeamOutValue);
	} else if(lowBeamState) {
		SoftPWMSet(pin, lowBeamOutValue);
	} else {
		SoftPWMSet(pin, SOFT_PWM_LOW);
	}
}

void setCombinedHeadlightParkOnly(uint8_t pin,
							uint8_t parkingState,
							uint8_t lowBeamState,
							uint8_t parkingOutValue,
							uint8_t lowBeamOutValue) {
	if(lowBeamState) {
		SoftPWMSet(pin, lowBeamOutValue);
	} else if(parkingState) {
		SoftPWMSet(pin, parkingOutValue);
	} else {
		SoftPWMSet(pin, SOFT_PWM_LOW);
	}
}

uint8_t starterDimming(bool active, uint8_t defaultDimValue, uint8_t divisor, uint8_t multiplier1) {
	if(!active) return defaultDimValue;
	if(active) return defaultDimValue / divisor * multiplier1;
}