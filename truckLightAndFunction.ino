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

/************************************
 * Configuration Programm
 ************************************/
#include "config.h"								// Configuration File
#include "ioMapping.h"							// IO Mapping File
#include "globalVars.h"							// Global Vars File
/************************************
 * Include Module and Library Files
 ************************************/
#include <SoftPWM.h>							// https://github.com/bhagman/SoftPWM
#include "modules/ppmToSwitches.h"				// Special functions
#include "modules/readPPMdata.h"				// read Data from Buffer
#include "modules/debugging.h"					// Handles debbuging info
#include "modules/lightFunctions.h"



void setup() {
	SoftPWMBegin();
	// put your setup code here, to run once:
	/************************************
	* Setup Inputs 
	************************************/
	pinMode(inFunction1ControlPPM, INPUT_PULLUP);
	pinMode(inFunction2ControlPPM, INPUT_PULLUP);
	pinMode(inSteerControlPPM, INPUT_PULLUP);
	pinMode(inBrakeSignal, INPUT_PULLUP);
	pinMode(inReverseSignal, INPUT_PULLUP);
	/************************************
	* Setup Outputs 
	************************************/
	pinMode(outParkingLight, OUTPUT);
	pinMode(outLowBeamLight, OUTPUT);
	pinMode(outHighBeamLight, OUTPUT);
	pinMode(outFogLight, OUTPUT);
	pinMode(outFrontLeftFlashLight, OUTPUT);
	pinMode(outFrontRightFlashLight, OUTPUT);
	pinMode(outRearLeftFlashLight, OUTPUT);
	pinMode(outRearRightFlashLight, OUTPUT);
	pinMode(outReverseLight, OUTPUT);
	pinMode(outBrakeLight, OUTPUT);
	pinMode(outAuxLight, OUTPUT);
	/************************************
	* Setup Functions
	************************************/
	initInterrupts(inFunction1ControlPPM, inFunction2ControlPPM, inSteerControlPPM);
}

void loop() {                             // put your main code here, to run repeatedly:
	bool errorFlag = false;
	
	bool dynStatus = ppmChannel1Evaluation();
	if(!dynStatus) errorFlag = true;
	dynStatus = ppmChannel2Evaluation();
	if(!dynStatus) errorFlag = true;
	runLightFunctions();

}