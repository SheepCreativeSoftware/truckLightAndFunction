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

#ifndef _DEBBUGING_H_
#define _DEBBUGING_H_

#include "Arduino.h"


#ifndef SerialUSB								//if not allready defined
#define SerialUSB SERIAL_PORT_MONITOR		//then define monitor port
#endif

void debuggingInit(uint8_t debugLevel, uint8_t outputLED);
void controllerStatus(bool errorFlag, uint8_t outputLED);			//function to signal errorstate
void debugChannelEvaluation(
	uint16_t channelNumber,
	uint16_t channel1,
	uint16_t channel2,
	uint16_t channel3,
	uint16_t channel4,
	uint16_t channel5,
	uint16_t channel6,
	uint16_t channel7,
	uint16_t channel8
);

void debugFunctionState(bool parkLight,
						bool lowBeamLight,
						bool highBeamLight,
						bool highBeamLightFlash,
						bool fogLight,
						bool beaconLight,
						bool auxLight,
						bool hazardLight,
						bool leftIndicatorLight,
						bool rightIndicatorLight,
						bool reverseLight,
						bool brakeLight); 
void debugFunctionOut(bool parkLight,
						bool lowBeamLight,
						bool highBeamLight,
						bool highBeamLightFlash,
						bool fogLight,
						bool beaconLight,
						bool auxLight,
						bool hazardLight,
						bool leftIndicatorLight,
						bool rightIndicatorLight,
						bool reverseLight,
						bool brakeLight); 

#endif