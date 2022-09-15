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
/************************************
 * Include Module and Library Files
 ************************************/
#include "readPPMdata.h"				// read Data from Buffer
#include "lightFunctions.h"
//#include <SoftPWM.h>							// https://github.com/bhagman/SoftPWM
//#include "debugging.h"					// Handles debbuging info

//#include "tools.h"

#define DIRECTION_UP 1
#define DIRECTION_MID 2
#define DIRECTION_DOWN 3
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

bool serialIsSent;


void setup() {
	//SoftPWMBegin();
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
	//debuggingInit();
	SerialUSB.begin(9600);
}

void loop() {                             // put your main code here, to run repeatedly:
	bool errorFlag = false;

	// Some switches are commented as they are not yet in use.
	//channel1.poti[0] = getChannel1Poti(0, 0);
	//channel1.poti[1] = getChannel1Poti(1, 0);
	//channel1.lowerSwitch[0] = getChannel1Switch(0, DIRECTION_MID);	// Function to get the value of the Switches from Channel 1
	channel1.lowerSwitch[1] = getChannel1Switch(1, DIRECTION_MID);	// Function to get the value of the Switches from Channel 1
	//channel1.upperSwitch[0] = getChannel1Switch(2, DIRECTION_DOWN);	// Function to get the value of the Switches from Channel 1
	//channel1.upperSwitch[1] = getChannel1Switch(3, DIRECTION_DOWN);	// Function to get the value of the Switches from Channel 1
	//channel1.upperSwitch[2] = getChannel1Switch(4, DIRECTION_DOWN);	// Function to get the value of the Switches from Channel 1
	channel1.upperSwitch[3] = getChannel1Switch(5, DIRECTION_MID);	// Function to get the value of the Switches from Channel 1
	highBeamLight.state = mapSwitchToFunction(channel1.upperSwitch[3], true, false, false);	// Function to map a Key [Down, Mid, Up]
	highBeamLightFlash.state = mapSwitchToFunction(channel1.upperSwitch[3], false, false, true);	// Function to map a Key [Down, Mid, Up]
	leftFlashLight.state = mapSwitchToFunction(channel1.lowerSwitch[1], true, false, false);	// Function to map a Key [Down, Mid, Up]
	rightFlashLight.state = mapSwitchToFunction(channel1.lowerSwitch[1], false, false, true);	// Function to map a Key [Down, Mid, Up]

	channel2.lowerSwitch[0] = getChannel2Switch(5, DIRECTION_DOWN);	// Function to get the value of the Switches from Channel 2
	//channel2.lowerSwitch[1] = getChannel2Switch(4, DIRECTION_DOWN);	// Function to get the value of the Switches from Channel 2
	channel2.upperSwitch[0] = getChannel2Switch(3, DIRECTION_DOWN);	// Function to get the value of the Switches from Channel 2
	channel2.upperSwitch[1] = getChannel2Switch(2, DIRECTION_UP);	// Function to get the value of the Switches from Channel 2
	channel2.upperSwitch[2] = getChannel2Switch(1, DIRECTION_DOWN);	// Function to get the value of the Switches from Channel 2
	channel2.upperSwitch[3] = getChannel2Switch(0, DIRECTION_DOWN);	// Function to get the value of the Switches from Channel 2
	parkLight.state = mapSwitchToFunction(channel2.lowerSwitch[0], false, true, true);	// Function to map a Key [Down, Mid, Up]
	lowBeamLight.state = mapSwitchToFunction(channel2.lowerSwitch[0], false, false, true);	// Function to map a Key [Down, Mid, Up]
	fogLight.state = mapSwitchToFunction(channel2.upperSwitch[0], false, false, true);	// Function to map a Key [Down, Mid, Up]
	hazardLight.state = mapSwitchToFunction(channel2.upperSwitch[1], false, false, true);	// Function to map a Key [Down, Mid, Up]
	beaconLight.state = mapSwitchToFunction(channel2.upperSwitch[2], false, false, true);	// Function to map a Key [Down, Mid, Up]
	auxLight.state = mapSwitchToFunction(channel2.upperSwitch[3], false, false, true);	// Function to map a Key [Down, Mid, Up]

	parkLight.out = directlyToOutput(parkLight.state);
	lowBeamLight.out = directlyToOutput(lowBeamLight.state);
	highBeamLight.out = highBeamFlash(highBeamLight.state, highBeamLightFlash.state, HIGH_BEAM_FLASH_FREQUENCY);
	fogLight.out = directlyToOutput(fogLight.state);
	beaconLight.out = directlyToOutput(beaconLight.state);
	auxLight.out = directlyToOutput(auxLight.state);

	digitalWrite(outParkingLight, parkLight.out);
	digitalWrite(outLowBeamLight, lowBeamLight.out);
	digitalWrite(outHighBeamLight, highBeamLight.out);
	digitalWrite(outFogLight, fogLight.out);
	digitalWrite(outAuxLight, auxLight.out);

	if((millis()%1000 >= 500) && (serialIsSent == false)) {
		SerialUSB.println("--Multiswitch 1--");
		SerialUSB.print("lowerSwitch 1: ");
		SerialUSB.println(channel1.lowerSwitch[1]);
		SerialUSB.print("upperSwitch 3: ");
		SerialUSB.println(channel1.upperSwitch[3]);
		SerialUSB.println("--Multiswitch 2--");
		SerialUSB.print("lowerSwitch 0: ");
		SerialUSB.println(channel2.lowerSwitch[0]);
		SerialUSB.print("upperSwitch 0: ");
		SerialUSB.println(channel2.upperSwitch[0]);
		SerialUSB.print("upperSwitch 1: ");
		SerialUSB.println(channel2.upperSwitch[1]);
		SerialUSB.print("upperSwitch 2: ");
		SerialUSB.println(channel2.upperSwitch[2]);
		SerialUSB.print("upperSwitch 3: ");
		SerialUSB.println(channel2.upperSwitch[3]);
		SerialUSB.println("-- Light State --");
		SerialUSB.print("parkLight: ");
		SerialUSB.println(parkLight.state);
		SerialUSB.print("lowBeamLight: ");
		SerialUSB.println(lowBeamLight.state);
		SerialUSB.print("highBeamLight: ");
		SerialUSB.println(highBeamLight.state);
		SerialUSB.print("highBeamLightFlash: ");
		SerialUSB.println(highBeamLightFlash.state);
		SerialUSB.print("fogLight: ");
		SerialUSB.println(fogLight.state);
		SerialUSB.print("beaconLight: ");
		SerialUSB.println(beaconLight.state);
		SerialUSB.print("auxLight: ");
		SerialUSB.println(auxLight.state);
		SerialUSB.print("hazardLight: ");
		SerialUSB.println(hazardLight.state);
		SerialUSB.print("leftFlashLight: ");
		SerialUSB.println(leftFlashLight.state);
		SerialUSB.print("rightFlashLight: ");
		SerialUSB.println(rightFlashLight.state);
		SerialUSB.println("-- Light Out --");
		SerialUSB.print("parkLight: ");
		SerialUSB.println(parkLight.out);
		SerialUSB.print("lowBeamLight: ");
		SerialUSB.println(lowBeamLight.out);
		SerialUSB.print("highBeamLight: ");
		SerialUSB.println(highBeamLight.out);
		SerialUSB.print("highBeamLightFlash: ");
		SerialUSB.println(highBeamLightFlash.out);
		SerialUSB.print("fogLight: ");
		SerialUSB.println(fogLight.out);
		SerialUSB.print("beaconLight: ");
		SerialUSB.println(beaconLight.out);
		SerialUSB.print("auxLight: ");
		SerialUSB.println(auxLight.out);
		SerialUSB.print("hazardLight: ");
		SerialUSB.println(hazardLight.out);
		SerialUSB.print("leftFlashLight: ");
		SerialUSB.println(leftFlashLight.out);
		SerialUSB.print("rightFlashLight: ");
		SerialUSB.println(rightFlashLight.out);
		SerialUSB.println("-------End-------");
		serialIsSent = true;
	} else if((millis()%1000 < 500) && (serialIsSent == true)) {
		serialIsSent = false;
	}
}