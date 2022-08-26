/************************************ 
 * truckLightAndFunction v0.1.0
 * Date: 2022-08-18T19:48:28Z
 * <Truck Light and function module>
 * Copyright (C) 2020 Marina Egner <info@sheepCreativeStudios.de>
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

#include "tools.h" // get filter class from there

#define INTERRUPT_BUFFERSIZE 8								// Max Buffersize of ISR; Max is 255
#define PPM_START_MIN 850									// Minimun for start impuls
#define PPM_START_MAX 980									// Maximum for start impuls
#define PPM_HIGH_MIN 700									// Minimun for High impuls
#define PPM_HIGH_MAX 2200									// Maximun for High impuls

volatile uint16_t interrupt1Buffer[INTERRUPT_BUFFERSIZE];	// Stores time difference of every channel
volatile uint8_t interrupt1Position = 8;					// Current index for interrupt | Start with 8 to make sure to start with the first start impulse
volatile uint32_t interrrupt1LastMicros = 0;				// Time since last interrupt
volatile uint32_t interrupt1LastValidMillis = 0;			// Time since last valid save in interrupt

volatile uint16_t interrupt2Buffer[INTERRUPT_BUFFERSIZE];	// Stores time difference of every channel
volatile uint8_t interrupt2Position = 8;					// Current index for interrupt | Start with 8 to make sure to start with the first start impulse
volatile uint32_t interrrupt2LastMicros = 0;				// Time since last interrupt
volatile uint32_t interrupt2LastValidMillis = 0;			// Time since last valid save in interrupt

volatile uint8_t ppmServoValue = 0;				//saves time difference of this channel
volatile uint32_t interrrupt3LastMicros = 0;		//time since last interrupt

//Vars for Channel Evaluation
uint32_t maxTimeForInterrupt = 2000; 						//max time (milliseconds) since last valid interrupt
uint8_t channel1Switch[6] = { 0 };
uint16_t channel1Poti[2] = { 0 };
uint8_t channel2Switch[8] = { 0 };

//Classes
Filter filter[2];

void initInterrupts(uint8_t pinPPMChannel1, uint8_t pinPPMChannel2, uint8_t pinServoChannel);		// Function to attach all interrupt service routines (ISR)
void ppmMultiInterrupt1();							// ISR for interrupt of first PPM signal
void ppmMultiInterrupt2();							// ISR for interrupt of second PPM signal
void ppmServoInterrupt();							// ISR for interrupt of servo PPM signal
bool ppmChannel1Evaluation();						// Function to evaluate the ppm signals
bool ppmChannel2Evaluation();						// Function to evaluate the ppm signals

#include "readPPMdata.cpp"
#endif