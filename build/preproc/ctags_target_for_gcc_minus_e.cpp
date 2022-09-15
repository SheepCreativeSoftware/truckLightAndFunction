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
# 25 "/home/magraina/projects/truckLightAndFunction/truckLightAndFunction.ino" 2
/************************************

 * Include Module and Library Files

 ************************************/
# 28 "/home/magraina/projects/truckLightAndFunction/truckLightAndFunction.ino"
# 29 "/home/magraina/projects/truckLightAndFunction/truckLightAndFunction.ino" 2
//#include <SoftPWM.h>							// https://github.com/bhagman/SoftPWM
//#include "debugging.h"					// Handles debbuging info

//#include "ppmToSwitches.h"				// Special functions
//#include "mapSwitchToFunctions.h"
//#include "tools.h"



void setup() {
 //SoftPWMBegin();
 // put your setup code here, to run once:
 /************************************

	* Setup Inputs 

	************************************/
# 44 "/home/magraina/projects/truckLightAndFunction/truckLightAndFunction.ino"
 pinMode(2 /*PPM Signal from Remote Control Extension | Interrupt Needed*/, 0x2);
 pinMode(3 /*PPM Signal from Remote Control Extension | Interrupt Needed*/, 0x2);
 pinMode(7 /*Steering Servo Signal from Receiver  | Interrupt Needed*/, 0x2);
 pinMode(A4 /*Brake Signal from Servonaut Speed Controller*/, 0x2);
 pinMode(A5 /*Reverse Signal from Servonaut Speed Controller*/, 0x2);
 /************************************

	* Setup Outputs 

	************************************/
# 52 "/home/magraina/projects/truckLightAndFunction/truckLightAndFunction.ino"
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
# 66 "/home/magraina/projects/truckLightAndFunction/truckLightAndFunction.ino"
 initInterrupts(2 /*PPM Signal from Remote Control Extension | Interrupt Needed*/, 3 /*PPM Signal from Remote Control Extension | Interrupt Needed*/, 7 /*Steering Servo Signal from Receiver  | Interrupt Needed*/);
 //debuggingInit();
}

void loop() { // put your main code here, to run repeatedly:
 bool errorFlag = false;

 // 16:59:31.637 -> 1504 row below:	1 Poti (0-100% -> 1000-2000)
// 16:59:31.637 -> 1568	row below:	2 Poti (0-100% -> 1000-2000)
// 16:59:31.637 -> 1516	row below:	3 switch (up/mid/down 1000/1500/2000)
// 16:59:31.637 -> 1528	row below: 	4 switch (up/mid/down 1000/1500/2000)
// 16:59:31.637 -> 1508	row above:		1 button (up/mid/down 1000/1500/2000)
// 16:59:31.637 -> 1532	row above:		2 switch (up/mid/down 1000/1500/2000)
// 16:59:31.637 -> 1532	row above:		3 switch (up/mid/down 1000/1500/2000)
// 16:59:31.637 -> 1528	row above:		4 button/switch (up/mid/down 1000/1500/2000)

 //getChannel1Switch( channel,  fallbackValue);

}
