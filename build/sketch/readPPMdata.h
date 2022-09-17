#line 1 "/home/magraina/projects/truckLightAndFunction/readPPMdata.h"
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

#ifndef _READ_PPM_DATA_H_
#define _READ_PPM_DATA_H_

#include "Arduino.h"

#define DIRECTION_UP 1
#define DIRECTION_MID 2
#define DIRECTION_DOWN 3

void initInterrupts(uint8_t pinPPMChannel1, uint8_t pinPPMChannel2, uint8_t pinServoChannel);	// Function to attach all interrupt service routines (ISR)
void ppmMultiInterrupt1();							// ISR for interrupt of first PPM signal
void ppmMultiInterrupt2();							// ISR for interrupt of second PPM signal
void ppmServoInterrupt();							// ISR for interrupt of servo PPM signal
bool checkChannelStatus(uint8_t multiSwitch);		// Function to check the Status of the Channel (If signal is lost)
uint16_t getChannel1Poti(uint8_t channel, uint8_t fallbackValue);	// Function to get the value of the Potis from Channel 1
uint8_t getChannel1Switch(uint8_t channel, uint8_t fallbackValue);	// Function to get the value of the Switches from Channel 1
uint8_t getChannel2Switch(uint8_t channel, uint8_t fallbackValue);	// Function to get the value of the Switches from Channel 2
bool mapSwitchToFunction(uint8_t channel, uint8_t downValue, uint8_t midValue, uint8_t upValue);	// Function to map a Key to a boolean function value

#endif