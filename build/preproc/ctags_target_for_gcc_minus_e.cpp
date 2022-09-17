# 1 "/home/magraina/projects/truckLightAndFunction/truckLightAndFunction.ino"
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
# 19 "/home/magraina/projects/truckLightAndFunction/truckLightAndFunction.ino"
/************************************

 * Configuration Programm

 ************************************/
# 22 "/home/magraina/projects/truckLightAndFunction/truckLightAndFunction.ino"
# 23 "/home/magraina/projects/truckLightAndFunction/truckLightAndFunction.ino" 2
# 24 "/home/magraina/projects/truckLightAndFunction/truckLightAndFunction.ino" 2
/************************************

 * Include Module and Library Files

 ************************************/
# 27 "/home/magraina/projects/truckLightAndFunction/truckLightAndFunction.ino"
# 28 "/home/magraina/projects/truckLightAndFunction/truckLightAndFunction.ino" 2
# 29 "/home/magraina/projects/truckLightAndFunction/truckLightAndFunction.ino" 2
//#include <SoftPWM.h>							// https://github.com/bhagman/SoftPWM
//#include "debugging.h"					// Handles debbuging info

//#include "tools.h"




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
# 73 "/home/magraina/projects/truckLightAndFunction/truckLightAndFunction.ino"
 pinMode(2 /*PPM Signal from Remote Control Extension | Interrupt Needed*/, 0x2);
 pinMode(3 /*PPM Signal from Remote Control Extension | Interrupt Needed*/, 0x2);
 pinMode(7 /*Steering Servo Signal from Receiver  | Interrupt Needed*/, 0x2);
 pinMode(A4 /*Brake Signal from Servonaut Speed Controller*/, 0x2);
 pinMode(A5 /*Reverse Signal from Servonaut Speed Controller*/, 0x2);
 /************************************

	* Setup Outputs 

	************************************/
# 81 "/home/magraina/projects/truckLightAndFunction/truckLightAndFunction.ino"
 pinMode(A0 /*Parking light output pin*/, 0x1);
 pinMode(10 /*Head light low beam output pin | PWM*/, 0x1);
 pinMode(11 /*Head light high beam output pin*/, 0x1);
 pinMode(A1 /*Fog light output pin*/, 0x1);
 pinMode(8 /*Front left flashing light output pin*/, 0x1);
 pinMode(9 /*Front right flashing light output pin*/, 0x1);
 pinMode(5 /*Rear left flashing light output pin | PWM Needed for US*/, 0x1);
 pinMode(6 /*Rear right flashing light output pin | PWM Needed for US*/, 0x1);
 pinMode(A2 /*Reverse light output pin*/, 0x1);
 pinMode(12 /*Brake light output pin | PWM for Parking Light*/, 0x1);
 pinMode(A3 /*Reserved for Special Auxiliary Light*/, 0x1);
 /************************************

	* Setup Functions

	************************************/
# 95 "/home/magraina/projects/truckLightAndFunction/truckLightAndFunction.ino"
 initInterrupts(2 /*PPM Signal from Remote Control Extension | Interrupt Needed*/, 3 /*PPM Signal from Remote Control Extension | Interrupt Needed*/, 7 /*Steering Servo Signal from Receiver  | Interrupt Needed*/);
 //debuggingInit();
 Serial.begin(9600);
}

void loop() { // put your main code here, to run repeatedly:
 bool errorFlag = false;

 // Some switches are commented as they are not yet in use.
 //channel1.poti[0] = getChannel1Poti(0, 0);
 //channel1.poti[1] = getChannel1Poti(1, 0);
 //channel1.lowerSwitch[0] = getChannel1Switch(0, DIRECTION_MID);	// Function to get the value of the Switches from Channel 1
 channel1.lowerSwitch[1] = getChannel1Switch(1, 2); // Function to get the value of the Switches from Channel 1
 //channel1.upperSwitch[0] = getChannel1Switch(2, DIRECTION_DOWN);	// Function to get the value of the Switches from Channel 1
 //channel1.upperSwitch[1] = getChannel1Switch(3, DIRECTION_DOWN);	// Function to get the value of the Switches from Channel 1
 //channel1.upperSwitch[2] = getChannel1Switch(4, DIRECTION_DOWN);	// Function to get the value of the Switches from Channel 1
 channel1.upperSwitch[3] = getChannel1Switch(5, 2); // Function to get the value of the Switches from Channel 1
 highBeamLight.state = mapSwitchToFunction(channel1.upperSwitch[3], true, false, false); // Function to map a Key [Down, Mid, Up]
 highBeamLightFlash.state = mapSwitchToFunction(channel1.upperSwitch[3], false, false, true); // Function to map a Key [Down, Mid, Up]
 leftFlashLight.state = mapSwitchToFunction(channel1.lowerSwitch[1], true, false, false); // Function to map a Key [Down, Mid, Up]
 rightFlashLight.state = mapSwitchToFunction(channel1.lowerSwitch[1], false, false, true); // Function to map a Key [Down, Mid, Up]

 channel2.lowerSwitch[0] = getChannel2Switch(5, 3); // Function to get the value of the Switches from Channel 2
 //channel2.lowerSwitch[1] = getChannel2Switch(4, DIRECTION_DOWN);	// Function to get the value of the Switches from Channel 2
 channel2.upperSwitch[0] = getChannel2Switch(3, 3); // Function to get the value of the Switches from Channel 2
 channel2.upperSwitch[1] = getChannel2Switch(2, 1); // Function to get the value of the Switches from Channel 2
 channel2.upperSwitch[2] = getChannel2Switch(1, 3); // Function to get the value of the Switches from Channel 2
 channel2.upperSwitch[3] = getChannel2Switch(0, 3); // Function to get the value of the Switches from Channel 2
 parkLight.state = mapSwitchToFunction(channel2.lowerSwitch[0], false, true, true); // Function to map a Key [Down, Mid, Up]
 lowBeamLight.state = mapSwitchToFunction(channel2.lowerSwitch[0], false, false, true); // Function to map a Key [Down, Mid, Up]
 fogLight.state = mapSwitchToFunction(channel2.upperSwitch[0], false, false, true); // Function to map a Key [Down, Mid, Up]
 hazardLight.state = mapSwitchToFunction(channel2.upperSwitch[1], false, false, true); // Function to map a Key [Down, Mid, Up]
 beaconLight.state = mapSwitchToFunction(channel2.upperSwitch[2], false, false, true); // Function to map a Key [Down, Mid, Up]
 auxLight.state = mapSwitchToFunction(channel2.upperSwitch[3], false, false, true); // Function to map a Key [Down, Mid, Up]

 parkLight.out = directlyToOutput(parkLight.state);
 lowBeamLight.out = directlyToOutput(lowBeamLight.state);
 highBeamLight.out = highBeamFlash(highBeamLight.state, highBeamLightFlash.state);
 fogLight.out = directlyToOutput(fogLight.state);
 beaconLight.out = directlyToOutput(beaconLight.state);
 auxLight.out = directlyToOutput(auxLight.state);
 setFlasherLight(leftFlashLight.state, rightFlashLight.state, hazardLight.state, &leftFlashLight.out, &rightFlashLight.out);

 digitalWrite(A0 /*Parking light output pin*/, parkLight.out);
 digitalWrite(10 /*Head light low beam output pin | PWM*/, lowBeamLight.out);
 digitalWrite(11 /*Head light high beam output pin*/, highBeamLight.out);
 digitalWrite(A1 /*Fog light output pin*/, fogLight.out);
 digitalWrite(A3 /*Reserved for Special Auxiliary Light*/, auxLight.out);

 if((millis()%1000 >= 500) && (serialIsSent == false)) {
  Serial.println("--Multiswitch 1--");
  Serial.print("lowerSwitch 1: ");
  Serial.println(channel1.lowerSwitch[1]);
  Serial.print("upperSwitch 3: ");
  Serial.println(channel1.upperSwitch[3]);
  Serial.println("--Multiswitch 2--");
  Serial.print("lowerSwitch 0: ");
  Serial.println(channel2.lowerSwitch[0]);
  Serial.print("upperSwitch 0: ");
  Serial.println(channel2.upperSwitch[0]);
  Serial.print("upperSwitch 1: ");
  Serial.println(channel2.upperSwitch[1]);
  Serial.print("upperSwitch 2: ");
  Serial.println(channel2.upperSwitch[2]);
  Serial.print("upperSwitch 3: ");
  Serial.println(channel2.upperSwitch[3]);
  Serial.println("-- Light State --");
  Serial.print("parkLight: ");
  Serial.println(parkLight.state);
  Serial.print("lowBeamLight: ");
  Serial.println(lowBeamLight.state);
  Serial.print("highBeamLight: ");
  Serial.println(highBeamLight.state);
  Serial.print("highBeamLightFlash: ");
  Serial.println(highBeamLightFlash.state);
  Serial.print("fogLight: ");
  Serial.println(fogLight.state);
  Serial.print("beaconLight: ");
  Serial.println(beaconLight.state);
  Serial.print("auxLight: ");
  Serial.println(auxLight.state);
  Serial.print("hazardLight: ");
  Serial.println(hazardLight.state);
  Serial.print("leftFlashLight: ");
  Serial.println(leftFlashLight.state);
  Serial.print("rightFlashLight: ");
  Serial.println(rightFlashLight.state);
  Serial.println("-- Light Out --");
  Serial.print("parkLight: ");
  Serial.println(parkLight.out);
  Serial.print("lowBeamLight: ");
  Serial.println(lowBeamLight.out);
  Serial.print("highBeamLight: ");
  Serial.println(highBeamLight.out);
  Serial.print("highBeamLightFlash: ");
  Serial.println(highBeamLightFlash.out);
  Serial.print("fogLight: ");
  Serial.println(fogLight.out);
  Serial.print("beaconLight: ");
  Serial.println(beaconLight.out);
  Serial.print("auxLight: ");
  Serial.println(auxLight.out);
  Serial.print("hazardLight: ");
  Serial.println(hazardLight.out);
  Serial.print("leftFlashLight: ");
  Serial.println(leftFlashLight.out);
  Serial.print("rightFlashLight: ");
  Serial.println(rightFlashLight.out);
  Serial.println("-------End-------");
  serialIsSent = true;
 } else if((millis()%1000 < 500) && (serialIsSent == true)) {
  serialIsSent = false;
 }
}
