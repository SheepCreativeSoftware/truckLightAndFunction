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

#define US 0
#define EU 1
/************************************
 * Configuration Programm
 ************************************/
#include "config.h"								// Configuration File
#include "ioMapping.h"							// IO Mapping File
/************************************
 * Include Module and Library Files
 ************************************/

#include "readPPMdata.h"				// read Data from Buffer
#include "lightFunctions.h"
#if (DEBUGLEVEL >=1)
#include "debugging.h"					// Handles debbuging info
#endif

struct {
	uint8_t poti[2];
	uint8_t lowerSwitch[2];
	uint8_t upperSwitch[4];
} channel1;

struct {
	uint8_t lowerSwitch[2];
	uint8_t upperSwitch[4];
} channel2;

struct Lights {
	bool state;
	bool out;
};

Lights parkLight;
Lights lowBeamLight;
Lights highBeamLight;
Lights highBeamLightFlash;
Lights leftFlashLight;
Lights rightFlashLight;
Lights fogLight;
Lights hazardLight;
Lights beaconLight;
Lights auxLight;
Lights brakeLight;
Lights reverseLight;

uint8_t channel3Switch = 0;

//Setup Serial and check if Board is UNO with one Serial or Leonardo/Micro with to Serials
#ifdef HAVE_HWSERIAL1							//if serial ports 1 exist then the arduino has more than one serial port
	#ifndef SerialUSB								//if not allready defined
		#define SerialUSB SERIAL_PORT_MONITOR		//then define monitor port
	#endif
#else
	#if (DEBUGLEVEL >1)								//if serial ports are the same debuging is not possible (for example on UNO)
		#define DEBUGLEVEL 1						//do not change!!!
	#endif
#endif


void setup() {
	// put your setup code here, to run once:
	/************************************
	* Setup Inputs 
	************************************/
	pinMode(inFunction1ControlPPM, INPUT_PULLUP);
	pinMode(inFunction2ControlPPM, INPUT_PULLUP);
	pinMode(inSoundPPM, INPUT);
	pinMode(inBrakeSignal, INPUT_PULLUP);
	pinMode(inReverseSignal, INPUT_PULLUP);
	/************************************
	* Setup Outputs 
	************************************/
	initLightOutput();
	setupLightOutput(outParkingLight, LIGHT_FADE_ON_TIME, LIGHT_FADE_OFF_TIME);
	setupLightOutput(outLowBeamLight, LIGHT_FADE_ON_TIME, LIGHT_FADE_OFF_TIME);
	setupLightOutput(outHighBeamLight, LIGHT_FADE_ON_TIME, LIGHT_FADE_OFF_TIME);
	setupLightOutput(outFogLight, LIGHT_FADE_ON_TIME, LIGHT_FADE_OFF_TIME);
	setupLightOutput(outFrontLeftFlashLight, LIGHT_FADE_ON_TIME, LIGHT_FADE_OFF_TIME);
	setupLightOutput(outFrontRightFlashLight, LIGHT_FADE_ON_TIME, LIGHT_FADE_OFF_TIME);
	setupLightOutput(outRearLeftFlashLight, LIGHT_FADE_ON_TIME, LIGHT_FADE_OFF_TIME);
	setupLightOutput(outRearRightFlashLight, LIGHT_FADE_ON_TIME, LIGHT_FADE_OFF_TIME);
	setupLightOutput(outReverseLight, LIGHT_FADE_ON_TIME, LIGHT_FADE_OFF_TIME);
	setupLightOutput(outBrakeLight, LIGHT_FADE_ON_TIME, LIGHT_FADE_OFF_TIME);
	setupLightOutput(outAuxLight, LIGHT_FADE_ON_TIME, LIGHT_FADE_OFF_TIME);

	/************************************
	* Setup Functions
	************************************/
	initInterrupts(inFunction1ControlPPM, inFunction2ControlPPM, inSoundPPM);
	#if (DEBUGLEVEL >=1)
	debuggingInit(DEBUGLEVEL, outStatusLed);
	#endif
}

void loop() {                             // put your main code here, to run repeatedly:
	bool errorFlag = false;

	/*
	 * Read Switches and Potis from Multiswitches
	 * Some switches are commented as they are not yet in use.
	 */
	channel1.poti[0] = getChannel1Poti(0, 0);
	channel1.poti[1] = getChannel1Poti(1, 0);
	channel1.lowerSwitch[0] = getChannel1Switch(0, DIRECTION_MID);	// Function to get the value of the Switches from Channel 1
	channel1.lowerSwitch[1] = getChannel1Switch(1, DIRECTION_MID);	// Function to get the value of the Switches from Channel 1
	channel1.upperSwitch[0] = getChannel1Switch(2, DIRECTION_DOWN);	// Function to get the value of the Switches from Channel 1
	channel1.upperSwitch[1] = getChannel1Switch(3, DIRECTION_DOWN);	// Function to get the value of the Switches from Channel 1
	channel1.upperSwitch[2] = getChannel1Switch(4, DIRECTION_DOWN);	// Function to get the value of the Switches from Channel 1
	channel1.upperSwitch[3] = getChannel1Switch(5, DIRECTION_MID);	// Function to get the value of the Switches from Channel 1

	channel2.lowerSwitch[0] = getChannel2Switch(5, DIRECTION_DOWN);	// Function to get the value of the Switches from Channel 2
	channel2.lowerSwitch[1] = getChannel2Switch(4, DIRECTION_DOWN);	// Function to get the value of the Switches from Channel 2
	channel2.upperSwitch[0] = getChannel2Switch(3, DIRECTION_DOWN);	// Function to get the value of the Switches from Channel 2
	channel2.upperSwitch[1] = getChannel2Switch(2, DIRECTION_UP);	// Function to get the value of the Switches from Channel 2
	channel2.upperSwitch[2] = getChannel2Switch(1, DIRECTION_DOWN);	// Function to get the value of the Switches from Channel 2
	channel2.upperSwitch[3] = getChannel2Switch(0, DIRECTION_DOWN);	// Function to get the value of the Switches from Channel 2

	channel3Switch = getChannel3Signal();

	/*
	 * Map switches to Functions
	 */
	highBeamLight.state = mapSwitchToFunction(channel1.upperSwitch[3], true, false, false);	// Function to map a Key [Down, Mid, Up]
	highBeamLightFlash.state = mapSwitchToFunction(channel1.upperSwitch[3], false, false, true);	// Function to map a Key [Down, Mid, Up]
	leftFlashLight.state = mapSwitchToFunction(channel1.lowerSwitch[1], true, false, false);	// Function to map a Key [Down, Mid, Up]
	rightFlashLight.state = mapSwitchToFunction(channel1.lowerSwitch[1], false, false, true);	// Function to map a Key [Down, Mid, Up]

	parkLight.state = mapSwitchToFunction(channel2.lowerSwitch[0], false, true, true);	// Function to map a Key [Down, Mid, Up]
	lowBeamLight.state = mapSwitchToFunction(channel2.lowerSwitch[0], false, false, true);	// Function to map a Key [Down, Mid, Up]
	fogLight.state = mapSwitchToFunction(channel2.upperSwitch[0], false, false, true);	// Function to map a Key [Down, Mid, Up]
	hazardLight.state = mapSwitchToFunction(channel2.upperSwitch[1], false, false, true);	// Function to map a Key [Down, Mid, Up]
	beaconLight.state = mapSwitchToFunction(channel2.upperSwitch[2], false, false, true);	// Function to map a Key [Down, Mid, Up]
	auxLight.state = mapSwitchToFunction(channel2.upperSwitch[3], false, false, true);	// Function to map a Key [Down, Mid, Up]
	reverseLight.state = !digitalRead(inReverseSignal);
	brakeLight.state = !digitalRead(inBrakeSignal);

	/*
	 * Write Light function state to the output var
	 */
	parkLight.out = directlyToOutput(parkLight.state);
	lowBeamLight.out = directlyToOutput(lowBeamLight.state);
	highBeamLight.out = highBeamFlash(highBeamLight.state, highBeamLightFlash.state, HIGH_BEAM_FLASH_FREQUENCY);
	fogLight.out = directlyToOutput(fogLight.state);
	beaconLight.out = directlyToOutput(beaconLight.state);
	auxLight.out = directlyToOutput(auxLight.state);
	reverseLight.out = directlyToOutput(reverseLight.state);
	brakeLight.out = directlyToOutput(brakeLight.state);
	setFlasherLight(leftFlashLight.state, rightFlashLight.state, hazardLight.state, &leftFlashLight.out, &rightFlashLight.out, BLINKER_FREQUENCY);

	bool starterDiming = false;
	if(channel3Switch == DIRECTION_DOWN) starterDiming = true;

	/*
	 * Set Outputs
	 */
	uint8_t normalDimming = starterDimming(starterDiming, SOFT_PWM_HIGH, STARTER_DIMM_DIVISOR, STARTER_DIMM_MULTI1);
	uint8_t parkDimming = starterDimming(starterDiming, PARKING_DIMM, STARTER_DIMM_DIVISOR, STARTER_DIMM_MULTI1);

	setBooleanLight(outParkingLight, parkLight.out, parkDimming);
	#if (HEADLIGHT_IS_PARKING && HEADLIGHT_IS_HIGHBEAM)
	uint8_t headParkDimming = starterDimming(starterDiming, HEADLIGHT_PARKING_VALUE, STARTER_DIMM_DIVISOR, STARTER_DIMM_MULTI1); 
	uint8_t headLowDimming = starterDimming(starterDiming, HEADLIGHT_LOWBEAM_VALUE, STARTER_DIMM_DIVISOR, STARTER_DIMM_MULTI1); 
	uint8_t headHighDimming = starterDimming(starterDiming, HEADLIGHT_HIGHBEAM_VALUE, STARTER_DIMM_DIVISOR, STARTER_DIMM_MULTI1); 
	setCombinedHeadlightAll(outLowBeamLight,
							parkLight.out,
							lowBeamLight.out,
							highBeamLight.out,
							headParkDimming,
							headLowDimming,
							headHighDimming);
	#elif (HEADLIGHT_IS_HIGHBEAM)
	uint8_t headLowDimming = starterDimming(starterDiming, HEADLIGHT_LOWBEAM_VALUE, STARTER_DIMM_DIVISOR, STARTER_DIMM_MULTI1); 
	uint8_t headHighDimming = starterDimming(starterDiming, HEADLIGHT_HIGHBEAM_VALUE, STARTER_DIMM_DIVISOR, STARTER_DIMM_MULTI1); 
	setCombinedHeadlightHighOnly(outLowBeamLight,
								lowBeamLight.out,
								highBeamLight.out,
								headLowDimming,
								headHighDimming);
	#elif (HEADLIGHT_IS_PARKING)
	uint8_t headParkDimming = starterDimming(starterDiming, HEADLIGHT_PARKING_VALUE, STARTER_DIMM_DIVISOR, STARTER_DIMM_MULTI1); 
	uint8_t headLowDimming = starterDimming(starterDiming, HEADLIGHT_LOWBEAM_VALUE, STARTER_DIMM_DIVISOR, STARTER_DIMM_MULTI1); 
	setCombinedHeadlightParkOnly(outLowBeamLight,
								parkLight.out,
								lowBeamLight.out,
								headParkDimming,
								headLowDimming);
	#else
	setBooleanLight(outLowBeamLight, lowBeamLight.out, normalDimming);
	#endif
	setBooleanLight(outHighBeamLight, highBeamLight.out, normalDimming);
	setBooleanLight(outFogLight, fogLight.out, normalDimming);
	setBooleanLight(outAuxLight, auxLight.out, normalDimming);
	setBooleanLight(outFrontLeftFlashLight, leftFlashLight.out, normalDimming);
	setBooleanLight(outFrontRightFlashLight, rightFlashLight.out, normalDimming);
	setBooleanLight(outReverseLight, reverseLight.out, normalDimming);
	setBooleanLight(outBrakeLight, brakeLight.out, normalDimming);

	#if (COUNTRY_OPTION == EU)
	setBooleanLight(outRearLeftFlashLight, leftFlashLight.out, normalDimming);
	setBooleanLight(outRearRightFlashLight, rightFlashLight.out, normalDimming);
	setBooleanLight(outBrakeLight, brakeLight.out, normalDimming);
	#endif
	setBooleanLight(outReverseLight, reverseLight.out, normalDimming);

	/*
	 * Setup Debugging
	 */

	#if (DEBUGLEVEL >=1)
	controllerStatus(errorFlag, outStatusLed);
	#endif

	#if (DEBUGLEVEL == 2)
	debugChannelEvaluation(channel1.poti[0],
							channel1.poti[1],
							channel1.lowerSwitch[0],
							channel1.lowerSwitch[1],
							channel1.upperSwitch[0],
							channel1.upperSwitch[1],
							channel1.upperSwitch[2],
							channel1.upperSwitch[3],
							channel2.lowerSwitch[0],
							channel2.lowerSwitch[1],
							channel2.upperSwitch[0],
							channel2.upperSwitch[1],
							channel2.upperSwitch[2],
							channel2.upperSwitch[3]);
	#endif
	#if(DEBUGLEVEL == 3)
	debugFunctionState(parkLight.state,
						lowBeamLight.state,
						highBeamLight.state,
						highBeamLightFlash.state,
						fogLight.state,
						beaconLight.state,
						auxLight.state,
						hazardLight.state,
						leftFlashLight.state,
						rightFlashLight.state,
						reverseLight.state,
						brakeLight.state);
	#endif
	#if(DEBUGLEVEL == 4)
	debugFunctionOut(parkLight.out,
						lowBeamLight.out,
						highBeamLight.out,
						highBeamLightFlash.out,
						fogLight.out,
						beaconLight.out,
						auxLight.out,
						hazardLight.out,
						leftFlashLight.out,
						rightFlashLight.out,
						reverseLight.state,
						brakeLight.state);
	#endif

	#if (DEBUGLEVEL == 5)
	SerialUSB.println(starterDiming);
	//SerialUSB.println(digitalRead(inSoundPPM));
	#endif
}