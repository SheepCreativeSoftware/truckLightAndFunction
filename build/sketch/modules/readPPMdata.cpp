#line 1 "/home/magraina/projects/truckLightAndFunction/modules/readPPMdata.cpp"
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
#include "readPPMdata.h"

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
	volatile uint32_t nDifference = (nMicros - interrrupt1LastMicros); //Calc time since last Change
	interrrupt1LastMicros = nMicros;								//save time for next interrupt
	if((nDifference < PPM_HIGH_MIN) && (nDifference > PPM_HIGH_MAX)) return; 		//Filter HIGH Impulse | HIGH if time is between 700 and 2200 else return
	if((nDifference > PPM_START_MIN) && (nDifference < PPM_START_MAX)) { 				//if time is ~915 then this is the start impulse
		interrupt1Position = 0; 									//then set index to 0
		return;														// And wait for the next impulse
	}
	if(interrupt1Position < INTERRUPT_BUFFERSIZE) {					//if index is out of bound, then wait for next start impulse
		interrupt1Buffer[interrupt1Position] = nDifference;			//save current time difference to value
		interrupt1Position++; 										//increment index by one
		interrupt1LastValidMillis = millis();						//save time of the last valid signal
	}
	return;
}

void ppmMultiInterrupt2(){
	volatile uint32_t nMicros = micros(); 							//Store current time
	volatile uint32_t nDifference = (nMicros - interrrupt1LastMicros); //Calc time since last Change
	interrrupt1LastMicros = nMicros;								//save time for next interrupt
	if((nDifference < PPM_HIGH_MIN) && (nDifference > PPM_HIGH_MAX)) return; 		//Filter HIGH Impulse | HIGH if time is between 700 and 2200 else return
	if((nDifference > PPM_START_MIN) && (nDifference < PPM_START_MAX)) { 				//if time is ~915 then this is the start impulse
		interrupt2Position = 0; 									//then set index to 0
		return;														// And wait for the next impulse
	}
	if(interrupt2Position < INTERRUPT_BUFFERSIZE) {					//if index is out of bound, then wait for next start impulse
		interrupt2Buffer[interrupt2Position] = nDifference;			//save current time difference to value
		interrupt2Position++; 										//increment index by one
		interrupt2LastValidMillis = millis();						//save time of the last valid signal
	}
	return;
}


void ppmServoInterrupt(){
	volatile uint32_t nMicros = micros(); 							// Store current time
	volatile uint32_t nDifference = (nMicros - interrrupt3LastMicros); //Calc time since last Change
	if((nDifference > PPM_HIGH_MIN) && (nDifference < PPM_HIGH_MAX)) { // Filter HIGH Impulse | HIGH if time is between 700 and 2200 
		ppmServoValue = nDifference;								// Store current time difference to value
	}
	interrrupt3LastMicros = nMicros;								//save time for next interrupt
	return;
}

bool ppmChannel1Evaluation() {
	// 16:59:31.637 -> 1504 row below:	1 Poti (0-100% -> 1000-2000)
	// 16:59:31.637 -> 1568	row below:	2 Poti (0-100% -> 1000-2000)
	// 16:59:31.637 -> 1516	row below:	3 switch (up/mid/down 1000/1500/2000)
	// 16:59:31.637 -> 1528	row below: 	4 switch (up/mid/down 1000/1500/2000)
	// 16:59:31.637 -> 1508	row above:		1 button (up/mid/down 1000/1500/2000)
	// 16:59:31.637 -> 1532	row above:		2 switch (up/mid/down 1000/1500/2000)
	// 16:59:31.637 -> 1532	row above:		3 switch (up/mid/down 1000/1500/2000)
	// 16:59:31.637 -> 1528	row above:		4 button/switch (up/mid/down 1000/1500/2000)

	if((millis()-interrupt1LastValidMillis) >= maxTimeForInterrupt) {
		for(uint8_t i = 0; i < 8; i++) {
			interrupt1Buffer[i] = 0;
		}
		return 0;
	} else {
		channel1Poti[0] = ppmServoToRange(filter[0].filterValue(interrupt1Buffer[0]));
		channel1Poti[1] = ppmServoToRange(filter[1].filterValue(interrupt1Buffer[1]));
		channel1Switch[0] = ppmToSwitchStages(interrupt1Buffer[2]);
		channel1Switch[1] = ppmToSwitchStages(interrupt1Buffer[3]);
		channel1Switch[2] = ppmToSwitchStages(interrupt1Buffer[4]);
		channel1Switch[3] = ppmToSwitchStages(interrupt1Buffer[5]);
		channel1Switch[4] = ppmToSwitchStages(interrupt1Buffer[6]);
		channel1Switch[5] = ppmToSwitchStages(interrupt1Buffer[7]);
		return 1;
	}	
}

bool ppmChannel2Evaluation() {
	// 16:59:31.637 -> 1976	row above:		4 button (up/down 2000/1000)
	// 16:59:31.637 -> 1984	row above:		3 switch (up/down 2000/1000)
	// 16:59:31.637 -> 1980	row above:		2 switch (up/down 2000/1000)
	// 16:59:31.637 -> 1980	row above:		1 switch (up/down 2000/1000)
	// 16:59:31.637 -> 1696	row below:	2 switch (up/mid/down 1000/1700/1700)
	// 16:59:31.637 -> 1716	row below:	2 switch (up/mid/down 1700/1700/1000)
	// 16:59:31.637 -> 1692	row below:	1 switch (up/mid/down 1000/1700/1700)
	// 16:59:31.637 -> 1696	row below:	1 switch (up/mid/down 1700/1700/1000)

	if((millis()-interrupt2LastValidMillis) >= maxTimeForInterrupt) {
		for(uint8_t i = 0; i < 8; i++) {
			interrupt2Buffer[i] = 0;
		}
		return 0;
	} else {
		channel2Switch[0] = ppmToSwitchStages(interrupt2Buffer[0], PPM_INVERT);
		channel2Switch[1] = ppmToSwitchStages(interrupt2Buffer[1], PPM_INVERT);
		channel2Switch[2] = ppmToSwitchStages(interrupt2Buffer[2], PPM_INVERT);
		channel2Switch[3] = ppmToSwitchStages(interrupt2Buffer[3], PPM_INVERT);
		channel2Switch[4] = ppm2ToSwitch3Stages(interrupt2Buffer[4], interrupt2Buffer[5]);
		channel2Switch[5] = ppm2ToSwitch3Stages(interrupt2Buffer[6], interrupt2Buffer[7]);
		return 1;
	}	
}
