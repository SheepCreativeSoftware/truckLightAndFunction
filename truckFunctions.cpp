/************************************ 
 * truckLightAndFunction v0.0.6
 * Date: 03.06.2020 | 22:16
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
#include <SoftPWM.h>							//https://github.com/bhagman/SoftPWM
#include "truckFunctions.h"

/***************************************************
 * PPM Signal have a range from 1000ms to 2000ms
 * So 3 stages should be 1000/1500/2000
 ***************************************************/
int ppmToSwitchStages(unsigned int signal) {
	if((signal >= 750) && (signal <= 1250)) {	//if signal is at 1000ms ±250ms 
		return 1;								//return 1 if signal is at lower end
	} else if(signal <= 1750) {					//else if signal is at 1500ms ±250ms
		return 2;								//return 2 if signal is at middle
	} else if(signal <= 2250) {					//else if signal is at 2000ms ±250ms
		return 3;								//return 3 if signal is at upper end
	} else {									//else signal is <750 or >2250
		return 0;								//return 0 cause signal is out of bound | error
	}
}

/***************************************************
 * PPM Signal have a range from 1000ms to 2000ms
 * input Range of signal is defined by inMin (~1000) inMax (~2000)
 * returns a value from outMin to outMax
 * if calculation fails a zero returns
 ***************************************************/
int ppmServoToRange(int signal, int inMin, int inMax, int outMin, int outMax) {
	if(inMin != inMax) {						//if Min and Max are equal abbort calculation cause of divide by zero
		return (signal - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
	} else {
		return 0;
	}
}

bool edgeEvaluation::readEdge(int input){
	if((input) && (!lastEdge)){
		lastEdge = true;
		return true;
	} else if((!input) && (lastEdge)){
		lastEdge = false;
		return false;
	} 
	return false;
}

void outputDefine::outputMode(int outPin, unsigned char modus){
	pinMode(outPin, OUTPUT);
	outPinModus = modus;
	switch (modus) {
		case OUT_HARD_PWM:
		analogWrite(outPin, 0);
		break;
		case OUT_SOFT_PWM:
		SoftPWMSet(outPin, 0);
		break;
		case OUT_DIGITAL:
		//nothing to do
		break;
		default:
		//error
		break;		
	}
}
void outputDefine::outputMode(int outPin, unsigned char modus, int fadeUpTime, int fadeDownTime){
	pinMode(outPin, OUTPUT);
	outPinModus = modus;
	switch (modus) {
		case OUT_SOFT_FADE:
		SoftPWMSet(outPin, 0);     									//Create and set pin to 0
		SoftPWMSetFadeTime(outPin, fadeUpTime, fadeDownTime);       //Set fade time for pin 1000 ms fade-up time, and 1000 ms fade-down time
		break;
		default:
		//error
		break;		
	}
}

