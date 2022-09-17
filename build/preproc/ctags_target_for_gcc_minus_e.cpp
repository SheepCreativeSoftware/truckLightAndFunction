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

# 36 "/home/magraina/projects/truckLightAndFunction/truckLightAndFunction.ino" 2

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
# 83 "/home/magraina/projects/truckLightAndFunction/truckLightAndFunction.ino"
void setup() {
 // put your setup code here, to run once:
 /************************************

	* Setup Inputs 

	************************************/
# 88 "/home/magraina/projects/truckLightAndFunction/truckLightAndFunction.ino"
 pinMode(2 /*PPM Signal from Remote Control Extension | Interrupt Needed*/, 0x2);
 pinMode(3 /*PPM Signal from Remote Control Extension | Interrupt Needed*/, 0x2);
 pinMode(7 /*Soundmodul Servo Signal from Receiver  | Interrupt Needed*/, 0x0);
 pinMode(A4 /*Brake Signal from Servonaut Speed Controller*/, 0x2);
 pinMode(A5 /*Reverse Signal from Servonaut Speed Controller*/, 0x2);
 /************************************

	* Setup Outputs 

	************************************/
# 96 "/home/magraina/projects/truckLightAndFunction/truckLightAndFunction.ino"
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
# 112 "/home/magraina/projects/truckLightAndFunction/truckLightAndFunction.ino"
 initInterrupts(2 /*PPM Signal from Remote Control Extension | Interrupt Needed*/, 3 /*PPM Signal from Remote Control Extension | Interrupt Needed*/, 7 /*Soundmodul Servo Signal from Receiver  | Interrupt Needed*/);

 debuggingInit(6 /*1 = Status LED | >2 = Serial Monitor*/, 13 /*Arduino status LED output Pin*/);



 serialConfigure(&Serial1 /*then define hardware port*/, // Serial interface on arduino
    19200, // Baudrate
    0x06, // e.g. SERIAL_8N1 | start bit, data bit, stop bit
    1000,
    20,
    4 // Pin to switch between Transmit and Receive
 );

}

void loop() { // put your main code here, to run repeatedly:
 bool errorFlag = false;

 /*

	 * Read Switches and Potis from Multiswitches

	 * Some switches are commented as they are not yet in use.

	 */
# 135 "/home/magraina/projects/truckLightAndFunction/truckLightAndFunction.ino"
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

 channel3Switch = getChannel3Signal();

 /*

	 * Map switches to Functions

	 */
# 156 "/home/magraina/projects/truckLightAndFunction/truckLightAndFunction.ino"
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
# 173 "/home/magraina/projects/truckLightAndFunction/truckLightAndFunction.ino"
 parkLight.out = directlyToOutput(parkLight.state);
 lowBeamLight.out = directlyToOutput(lowBeamLight.state);
 highBeamLight.out = highBeamFlash(highBeamLight.state, highBeamLightFlash.state, 800 /* Time frequency for head beam to flash*/);
 fogLight.out = directlyToOutput(fogLight.state);
 beaconLight.out = directlyToOutput(beaconLight.state);
 auxLight.out = directlyToOutput(auxLight.state);
 reverseLight.out = directlyToOutput(reverseLight.state);
 brakeLight.out = directlyToOutput(brakeLight.state);
 setFlasherLight(leftFlashLight.state, rightFlashLight.state, hazardLight.state, &leftFlashLight.out, &rightFlashLight.out, 1000 /* Time frequency for Flasher to flash*/);

 bool starterDiming = false;
 if(channel3Switch == 3) starterDiming = true;

 /*

	 * Set Outputs

	 */
# 189 "/home/magraina/projects/truckLightAndFunction/truckLightAndFunction.ino"
 uint8_t normalDimming = starterDimming(starterDiming, 255, 5 /* Divisor for Dimming function*/, 2 /* 0-255 MAX Value for all light when active starter is activ*/);
 uint8_t parkDimming = starterDimming(starterDiming, 100 /* 0-255 Value for dimming the parking light*/, 5 /* Divisor for Dimming function*/, 2 /* 0-255 MAX Value for all light when active starter is activ*/);

 setBooleanLight(A3 /*Parking light output pin*/, parkLight.out, parkDimming);
# 213 "/home/magraina/projects/truckLightAndFunction/truckLightAndFunction.ino"
 uint8_t headParkDimming = starterDimming(starterDiming, 5 /* 0-255 Value for combined operation of Headlight*/, 5 /* Divisor for Dimming function*/, 2 /* 0-255 MAX Value for all light when active starter is activ*/);
 uint8_t headLowDimming = starterDimming(starterDiming, 255 /* 0-255 Value for combined operation of Headlight*/, 5 /* Divisor for Dimming function*/, 2 /* 0-255 MAX Value for all light when active starter is activ*/);
 setCombinedHeadlightParkOnly(10 /*Head light low beam output pin | PWM*/,
        parkLight.out,
        lowBeamLight.out,
        headParkDimming,
        headLowDimming);



 setBooleanLight(11 /*Head light high beam output pin*/, highBeamLight.out, normalDimming);
 setBooleanLight(A2 /*Fog light output pin*/, fogLight.out, normalDimming);
 setBooleanLight(A0 /*Reserved for Special Auxiliary Light*/, auxLight.out, normalDimming);
 setBooleanLight(8 /*Front left flashing light output pin*/, leftFlashLight.out, normalDimming);
 setBooleanLight(9 /*Front right flashing light output pin*/, rightFlashLight.out, normalDimming);
 setBooleanLight(A1 /*Reverse light output pin*/, reverseLight.out, normalDimming);
 setBooleanLight(12 /*Brake light output pin | PWM for Parking Light*/, brakeLight.out, normalDimming);


 setBooleanLight(5 /*Rear left flashing light output pin | PWM Needed for US*/, leftFlashLight.out, normalDimming);
 setBooleanLight(6 /*Rear right flashing light output pin | PWM Needed for US*/, rightFlashLight.out, normalDimming);
 setBooleanLight(12 /*Brake light output pin | PWM for Parking Light*/, brakeLight.out, normalDimming);

 setBooleanLight(A1 /*Reverse light output pin*/, reverseLight.out, normalDimming);

 /*

	 * Setup serial communication

	 */
# 242 "/home/magraina/projects/truckLightAndFunction/truckLightAndFunction.ino"
 // Function 1 is Light information data which has only one byte
 /* 	0 -> parking light,

		1 -> brake light,

		2 -> reversing lights,

		3 -> right blinker,

		4 -> left blinker,

		5 -> auxiliary light,

		6 -> beacon light

		7 -> dimm light

	*/
# 252 "/home/magraina/projects/truckLightAndFunction/truckLightAndFunction.ino"
 setLightData(0, parkLight.out);
 setLightData(1, brakeLight.out);
 setLightData(2, reverseLight.out);
 setLightData(3, rightFlashLight.out);
 setLightData(4, leftFlashLight.out);
 setLightData(5, auxLight.out);
 setLightData(6, beaconLight.out);
 setLightData(7, starterDiming);
 //setServoData();
 serialUpdate();

 /*

	 * Setup Debugging

	 */
 controllerStatus(errorFlag, 13 /*Arduino status LED output Pin*/);
# 320 "/home/magraina/projects/truckLightAndFunction/truckLightAndFunction.ino"
}
