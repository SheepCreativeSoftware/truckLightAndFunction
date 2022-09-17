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
# 21 "/home/magraina/projects/truckLightAndFunction/truckLightAndFunction.ino"
/************************************

 * Configuration Programm

 ************************************/
# 24 "/home/magraina/projects/truckLightAndFunction/truckLightAndFunction.ino"
# 25 "/home/magraina/projects/truckLightAndFunction/truckLightAndFunction.ino" 2
# 26 "/home/magraina/projects/truckLightAndFunction/truckLightAndFunction.ino" 2
/************************************

 * Include Module and Library Files

 ************************************/
# 30 "/home/magraina/projects/truckLightAndFunction/truckLightAndFunction.ino"
# 31 "/home/magraina/projects/truckLightAndFunction/truckLightAndFunction.ino" 2
# 32 "/home/magraina/projects/truckLightAndFunction/truckLightAndFunction.ino" 2

# 34 "/home/magraina/projects/truckLightAndFunction/truckLightAndFunction.ino" 2


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

//Setup Serial and check if Board is UNO with one Serial or Leonardo/Micro with to Serials
# 77 "/home/magraina/projects/truckLightAndFunction/truckLightAndFunction.ino"
void setup() {
 // put your setup code here, to run once:
 /************************************

	* Setup Inputs 

	************************************/
# 82 "/home/magraina/projects/truckLightAndFunction/truckLightAndFunction.ino"
 pinMode(2 /*PPM Signal from Remote Control Extension | Interrupt Needed*/, 0x2);
 pinMode(3 /*PPM Signal from Remote Control Extension | Interrupt Needed*/, 0x2);
 pinMode(7 /*Steering Servo Signal from Receiver  | Interrupt Needed*/, 0x2);
 pinMode(A4 /*Brake Signal from Servonaut Speed Controller*/, 0x2);
 pinMode(A5 /*Reverse Signal from Servonaut Speed Controller*/, 0x2);
 /************************************

	* Setup Outputs 

	************************************/
# 90 "/home/magraina/projects/truckLightAndFunction/truckLightAndFunction.ino"
 initLightOutput();
 setupLightOutput(A3 /*Parking light output pin*/, 200 /* 200ms Fade on time for the Light*/, 200 /* 200ms Fade off time for the Light*/);
 setupLightOutput(10 /*Head light low beam output pin | PWM*/, 200 /* 200ms Fade on time for the Light*/, 200 /* 200ms Fade off time for the Light*/);
 setupLightOutput(11 /*Head light high beam output pin*/, 200 /* 200ms Fade on time for the Light*/, 200 /* 200ms Fade off time for the Light*/);
 setupLightOutput(A2 /*Fog light output pin*/, 200 /* 200ms Fade on time for the Light*/, 200 /* 200ms Fade off time for the Light*/);
 setupLightOutput(8 /*Front left flashing light output pin*/, 200 /* 200ms Fade on time for the Light*/, 200 /* 200ms Fade off time for the Light*/);
 setupLightOutput(9 /*Front right flashing light output pin*/, 200 /* 200ms Fade on time for the Light*/, 200 /* 200ms Fade off time for the Light*/);
 setupLightOutput(5 /*Rear left flashing light output pin | PWM Needed for US*/, 200 /* 200ms Fade on time for the Light*/, 200 /* 200ms Fade off time for the Light*/);
 setupLightOutput(6 /*Rear right flashing light output pin | PWM Needed for US*/, 200 /* 200ms Fade on time for the Light*/, 200 /* 200ms Fade off time for the Light*/);
 setupLightOutput(A1 /*Reverse light output pin*/, 200 /* 200ms Fade on time for the Light*/, 200 /* 200ms Fade off time for the Light*/);
 setupLightOutput(12 /*Brake light output pin | PWM for Parking Light*/, 200 /* 200ms Fade on time for the Light*/, 200 /* 200ms Fade off time for the Light*/);
 setupLightOutput(A0 /*Reserved for Special Auxiliary Light*/, 200 /* 200ms Fade on time for the Light*/, 200 /* 200ms Fade off time for the Light*/);

 /************************************

	* Setup Functions

	************************************/
# 106 "/home/magraina/projects/truckLightAndFunction/truckLightAndFunction.ino"
 initInterrupts(2 /*PPM Signal from Remote Control Extension | Interrupt Needed*/, 3 /*PPM Signal from Remote Control Extension | Interrupt Needed*/, 7 /*Steering Servo Signal from Receiver  | Interrupt Needed*/);

 debuggingInit(3 /*1 = Status LED | >2 = Serial Monitor*/, 13 /*Arduino status LED output Pin*/);

}

void loop() { // put your main code here, to run repeatedly:
 bool errorFlag = false;

 /*

	 * Read Switches and Potis from Multiswitches

	 * Some switches are commented as they are not yet in use.

	 */
# 119 "/home/magraina/projects/truckLightAndFunction/truckLightAndFunction.ino"
 channel1.poti[0] = getChannel1Poti(0, 0);
 channel1.poti[1] = getChannel1Poti(1, 0);
 channel1.lowerSwitch[0] = getChannel1Switch(0, 2); // Function to get the value of the Switches from Channel 1
 channel1.lowerSwitch[1] = getChannel1Switch(1, 2); // Function to get the value of the Switches from Channel 1
 channel1.upperSwitch[0] = getChannel1Switch(2, 3); // Function to get the value of the Switches from Channel 1
 channel1.upperSwitch[1] = getChannel1Switch(3, 3); // Function to get the value of the Switches from Channel 1
 channel1.upperSwitch[2] = getChannel1Switch(4, 3); // Function to get the value of the Switches from Channel 1
 channel1.upperSwitch[3] = getChannel1Switch(5, 2); // Function to get the value of the Switches from Channel 1

 channel2.lowerSwitch[0] = getChannel2Switch(5, 3); // Function to get the value of the Switches from Channel 2
 channel2.lowerSwitch[1] = getChannel2Switch(4, 3); // Function to get the value of the Switches from Channel 2
 channel2.upperSwitch[0] = getChannel2Switch(3, 3); // Function to get the value of the Switches from Channel 2
 channel2.upperSwitch[1] = getChannel2Switch(2, 1); // Function to get the value of the Switches from Channel 2
 channel2.upperSwitch[2] = getChannel2Switch(1, 3); // Function to get the value of the Switches from Channel 2
 channel2.upperSwitch[3] = getChannel2Switch(0, 3); // Function to get the value of the Switches from Channel 2

 /*

	 * Map switches to Functions

	 */
# 138 "/home/magraina/projects/truckLightAndFunction/truckLightAndFunction.ino"
 highBeamLight.state = mapSwitchToFunction(channel1.upperSwitch[3], true, false, false); // Function to map a Key [Down, Mid, Up]
 highBeamLightFlash.state = mapSwitchToFunction(channel1.upperSwitch[3], false, false, true); // Function to map a Key [Down, Mid, Up]
 leftFlashLight.state = mapSwitchToFunction(channel1.lowerSwitch[1], true, false, false); // Function to map a Key [Down, Mid, Up]
 rightFlashLight.state = mapSwitchToFunction(channel1.lowerSwitch[1], false, false, true); // Function to map a Key [Down, Mid, Up]

 parkLight.state = mapSwitchToFunction(channel2.lowerSwitch[0], false, true, true); // Function to map a Key [Down, Mid, Up]
 lowBeamLight.state = mapSwitchToFunction(channel2.lowerSwitch[0], false, false, true); // Function to map a Key [Down, Mid, Up]
 fogLight.state = mapSwitchToFunction(channel2.upperSwitch[0], false, false, true); // Function to map a Key [Down, Mid, Up]
 hazardLight.state = mapSwitchToFunction(channel2.upperSwitch[1], false, false, true); // Function to map a Key [Down, Mid, Up]
 beaconLight.state = mapSwitchToFunction(channel2.upperSwitch[2], false, false, true); // Function to map a Key [Down, Mid, Up]
 auxLight.state = mapSwitchToFunction(channel2.upperSwitch[3], false, false, true); // Function to map a Key [Down, Mid, Up]
 reverseLight.state = !digitalRead(A5 /*Reverse Signal from Servonaut Speed Controller*/);
 brakeLight.state = !digitalRead(A4 /*Brake Signal from Servonaut Speed Controller*/);

 /*

	 * Write Light function state to the output var

	 */
# 155 "/home/magraina/projects/truckLightAndFunction/truckLightAndFunction.ino"
 parkLight.out = directlyToOutput(parkLight.state);
 lowBeamLight.out = directlyToOutput(lowBeamLight.state);
 highBeamLight.out = highBeamFlash(highBeamLight.state, highBeamLightFlash.state, 800 /* Time frequency for head beam to flash*/);
 fogLight.out = directlyToOutput(fogLight.state);
 beaconLight.out = directlyToOutput(beaconLight.state);
 auxLight.out = directlyToOutput(auxLight.state);
 reverseLight.out = directlyToOutput(reverseLight.state);
 brakeLight.out = directlyToOutput(brakeLight.state);
 setFlasherLight(leftFlashLight.state, rightFlashLight.state, hazardLight.state, &leftFlashLight.out, &rightFlashLight.out, 1000 /* Time frequency for Flasher to flash*/);

 /*

	 * Set Outputs

	 */
# 169 "/home/magraina/projects/truckLightAndFunction/truckLightAndFunction.ino"
 setBooleanLight(A3 /*Parking light output pin*/, parkLight.out, 100 /* 0-255 Value for dimming the parking light*/);
# 185 "/home/magraina/projects/truckLightAndFunction/truckLightAndFunction.ino"
 setCombinedHeadlightParkOnly(10 /*Head light low beam output pin | PWM*/,
        parkLight.out,
        lowBeamLight.out,
        5 /* 0-255 Value for combined operation of Headlight*/,
        255 /* 0-255 Value for combined operation of Headlight*/);



 setBooleanLight(11 /*Head light high beam output pin*/, highBeamLight.out);
 setBooleanLight(A2 /*Fog light output pin*/, fogLight.out);
 setBooleanLight(A0 /*Reserved for Special Auxiliary Light*/, auxLight.out);
 setBooleanLight(8 /*Front left flashing light output pin*/, leftFlashLight.out);
 setBooleanLight(9 /*Front right flashing light output pin*/, rightFlashLight.out);
 setBooleanLight(A1 /*Reverse light output pin*/, reverseLight.out);
 setBooleanLight(12 /*Brake light output pin | PWM for Parking Light*/, brakeLight.out);


 setBooleanLight(5 /*Rear left flashing light output pin | PWM Needed for US*/, leftFlashLight.out);
 setBooleanLight(6 /*Rear right flashing light output pin | PWM Needed for US*/, rightFlashLight.out);
 setBooleanLight(12 /*Brake light output pin | PWM for Parking Light*/, brakeLight.out);

 setBooleanLight(A1 /*Reverse light output pin*/, reverseLight.out);

 /*

	 * Setup Debugging

	 */
 controllerStatus(errorFlag, 13 /*Arduino status LED output Pin*/);
# 233 "/home/magraina/projects/truckLightAndFunction/truckLightAndFunction.ino"
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
# 260 "/home/magraina/projects/truckLightAndFunction/truckLightAndFunction.ino"
}
