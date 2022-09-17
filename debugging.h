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

#ifndef _DEBBUGING_H_
#define _DEBBUGING_H_

#include "Arduino.h"


#ifndef SerialUSB								//if not allready defined
#define SerialUSB SERIAL_PORT_MONITOR		//then define monitor port
#endif

void debuggingInit(uint8_t debugLevel, uint8_t outputLED);
void controllerStatus(bool errorFlag, uint8_t outputLED);			//function to signal errorstate
void debugChannelEvaluation(uint8_t channel1Poti1,
							uint8_t channel1Poti2,
							uint8_t channel1Switch1,
							uint8_t channel1Switch2,
							uint8_t channel1Switch3,
							uint8_t channel1Switch4,
							uint8_t channel1Switch5,
							uint8_t channel1Switch6,
							uint8_t channel2Switch1,
							uint8_t channel2Switch2,
							uint8_t channel2Switch3,
							uint8_t channel2Switch4,
							uint8_t channel2Switch5,
							uint8_t channel2Switch6);

void debugFunctionState(bool parkLight,
						bool lowBeamLight,
						bool highBeamLight,
						bool highBeamLightFlash,
						bool fogLight,
						bool beaconLight,
						bool auxLight,
						bool hazardLight,
						bool leftFlashLight,
						bool rightFlashLight,
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
						bool leftFlashLight,
						bool rightFlashLight,
						bool reverseLight,
						bool brakeLight); 

#endif