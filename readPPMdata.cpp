/************************************ 
 * Copyright (C) 2020-2025 Marina Egner <hello@sheepcs.de>
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

#include "readPPMdata.h"							// Include own header file
#include "ppmToSwitches.h"							// Library to evaluate switch position from ppm value
#include "tools.h" 									// Get filter class from there

#define INTERRUPT_BUFFERSIZE 8						// Max Buffersize of ISR; Max is 255
#define PPM_START_MIN 850							// Minimun for start impuls
#define PPM_START_MAX 980							// Maximum for start impuls
#define PPM_HIGH_MIN 700							// Minimun for High impuls
#define PPM_HIGH_MAX 2200							// Maximun for High impuls
#define MULTI1 0									// Array number of Multiswitch
#define MULTI2 1									// Array number of Multiswitch
#define MAX_TIME_SIGNAL 5000						// 2000ms Maximum time for signal to change

// Variables
struct multiswitch {
	volatile uint16_t buffer[INTERRUPT_BUFFERSIZE];	// Stores time difference of every channel
	volatile uint8_t position = 8;					// Current index for interrupt | Start with 8 to make sure to start with the first start impulse
	volatile uint32_t lastMicros = 0;				// Time since last interrupt
	volatile uint32_t lastValidMillis = 0;			// Time since last valid save in interrupt
};

multiswitch interrupt[2];							// Structure for ISR

struct {
	volatile uint16_t servoValue = 0;				// Saves time difference of this channel
	volatile uint32_t lastMicros = 0;				// Time since last interrupt
	volatile uint32_t lastValidMillis = 0;			// Time since last valid save in interrupt
} interrupt3;

// Classes
Filter filter[2];									// Filter Function for Potis

/***************************************************
 * PPM Signal have a range from 1000ms to 2000ms
 * So 3 stages should be 1000/1500/2000 => UP/MID/DOWN
 * invertDirection is optional to turn the direction of UP and DOWN
 ***************************************************/

void initInterrupts(uint8_t pinPPMChannel1, uint8_t pinPPMChannel2, uint8_t pinServoChannel) {
	attachInterrupt(digitalPinToInterrupt(pinPPMChannel1), ppmMultiInterrupt1, CHANGE); 	//Setup Interrupt
	attachInterrupt(digitalPinToInterrupt(pinPPMChannel2), ppmMultiInterrupt2, CHANGE);		//Setup Interrupt
	attachInterrupt(digitalPinToInterrupt(pinServoChannel), ppmServoInterrupt, CHANGE);		//Setup Interrupt
}

void ppmMultiInterrupt1(){
	volatile uint32_t nMicros = micros(); 							//Store current time
	volatile uint32_t nDifference = (nMicros - interrupt[MULTI1].lastMicros); //Calc time since last Change
	if((nDifference > (uint16_t)PPM_HIGH_MIN) && (nDifference < (uint16_t)PPM_HIGH_MAX)) { 		//Filter HIGH Impulse | HIGH if time is between 700 and 2200 
		if((nDifference > (uint16_t)PPM_START_MIN) && (nDifference < (uint16_t)PPM_START_MAX)) { 				//if time is ~915 then this is the start impulse
			interrupt[MULTI1].position = 0; 									//then set index to 0
		} else if(interrupt[MULTI1].position < INTERRUPT_BUFFERSIZE) {					//if index is out of bound, then wait for next start impulse
			interrupt[MULTI1].buffer[interrupt[MULTI1].position] = nDifference;			//save current time difference to value
			interrupt[MULTI1].position++; 										//increment index by one
			interrupt[MULTI1].lastValidMillis = millis();						//save time of the last valid signal
		}
	}
	interrupt[MULTI1].lastMicros = nMicros;								//save time for next interrupt
}

void ppmMultiInterrupt2(){
	volatile uint32_t nMicros = micros(); 							//Store current time
	volatile uint32_t nDifference = (nMicros - interrupt[MULTI2].lastMicros); //Calc time since last Change
	if((nDifference > (uint16_t)PPM_HIGH_MIN) && (nDifference < (uint16_t)PPM_HIGH_MAX)) {		//Filter HIGH Impulse | HIGH if time is between 700 and 2200 else return
		if((nDifference > (uint16_t)PPM_START_MIN) && (nDifference < (uint16_t)PPM_START_MAX)) { 				//if time is ~915 then this is the start impulse
			interrupt[MULTI2].position = 0; 									//then set index to 0
			return;														// And wait for the next impulse
		} else	if(interrupt[MULTI2].position < INTERRUPT_BUFFERSIZE) {					//if index is out of bound, then wait for next start impulse
			interrupt[MULTI2].buffer[interrupt[MULTI2].position] = nDifference;			//save current time difference to value
			interrupt[MULTI2].position++; 										//increment index by one
			interrupt[MULTI2].lastValidMillis = millis();						//save time of the last valid signal
		}
	}
	interrupt[MULTI2].lastMicros = nMicros;								//save time for next interrupt
}


void ppmServoInterrupt(){
	volatile uint32_t nMicros = micros(); 							// Store current time
	volatile uint32_t nDifference = (nMicros - interrupt3.lastMicros); //Calc time since last Change
	if((nDifference > PPM_HIGH_MIN) && (nDifference < PPM_HIGH_MAX)) { // Filter HIGH Impulse | HIGH if time is between 700 and 2200 
		interrupt3.servoValue = nDifference;								// Store current time difference to value
		interrupt3.lastValidMillis = millis();						//save time of the last valid signal
	}
	interrupt3.lastMicros = nMicros;								//save time for next interrupt
	return;
}

bool checkChannelStatus(uint8_t multiSwitch) {
	if((millis()-interrupt[multiSwitch].lastValidMillis) >= (uint16_t)MAX_TIME_SIGNAL) return false;
	return true;
}

uint8_t getChannel1Switch(uint8_t channel, uint8_t fallbackValue) {
	if(!checkChannelStatus(MULTI1)) return fallbackValue;				// return fallback if channel does not respond
	switch (channel) {
	case 0:
		return ppmToSwitchStages(interrupt[MULTI1].buffer[0]);
		break;
	case 1:
		return ppmToSwitchStages(interrupt[MULTI1].buffer[1]);
		break;
	case 2:
		return ppmToSwitchStages(interrupt[MULTI1].buffer[2]);
		break;
	case 3:
		return ppmToSwitchStages(interrupt[MULTI1].buffer[3]);
		break;
	case 4:
		return ppmToSwitchStages(interrupt[MULTI1].buffer[4]);
		break;
	case 5:
		return ppmToSwitchStages(interrupt[MULTI1].buffer[5]);
		break;
	case 6:
		return ppmToSwitchStages(interrupt[MULTI1].buffer[6]);
		break;
	case 7:
		return ppmToSwitchStages(interrupt[MULTI1].buffer[7]);
		break;
	}
	// If something wrong return fallback
	return fallbackValue;
}

uint16_t getChannel2Poti(uint8_t channel, uint16_t fallbackValue) {
	if(!checkChannelStatus(MULTI1)) return fallbackValue;				// return fallback if channel does not respond
	switch (channel) {
	case 0:
		return ppmServoToRange(filter[0].filterValue(interrupt[MULTI2].buffer[0], 10, 50));
		break;
	case 1:
		return ppmServoToRange(filter[1].filterValue(interrupt[MULTI2].buffer[1], 10, 50));
		break;
	}
	// If something wrong return fallback
	return fallbackValue;
}

uint8_t getChannel2Switch(uint8_t channel, uint8_t fallbackValue) {
	if(!checkChannelStatus(MULTI2)) return fallbackValue;				// return fallback if channel does not respond
	switch (channel) {
	case 0:
		return ppmToSwitchStages(interrupt[MULTI2].buffer[2], PPM_INVERT);
		break;
	case 1:
		return ppmToSwitchStages(interrupt[MULTI2].buffer[3], PPM_INVERT);
		break;
	case 2:
		return ppmToSwitchStages(interrupt[MULTI2].buffer[4], PPM_INVERT);
		break;
	case 3:
		return ppmToSwitchStages(interrupt[MULTI2].buffer[5], PPM_INVERT);
		break;
	case 4:
		return ppmToSwitchStages(interrupt[MULTI2].buffer[6], PPM_INVERT);
		break;
	case 5:
		return ppmToSwitchStages(interrupt[MULTI2].buffer[7], PPM_INVERT);
		break;
	}
	// If something wrong return fallback
	return fallbackValue;
}

bool mapSwitchToFunction(uint8_t channel, uint8_t downValue, uint8_t midValue, uint8_t upValue) {
	switch (channel) {
	case DIRECTION_DOWN:
		return downValue;
		break;
	case DIRECTION_MID:
		return midValue;
		break;
	case DIRECTION_UP:
		return upValue;
		break;
	default:
		return false;
		break;
	}
}

uint16_t getChannel3Signal() {
	if((millis()-interrupt3.lastValidMillis) >= (uint16_t)MAX_TIME_SIGNAL) return 0;
	return ppmToSwitchStages(interrupt3.servoValue);
}
