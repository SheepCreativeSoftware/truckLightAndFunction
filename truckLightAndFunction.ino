/************************************ 
 * truckLightAndFunction v0.0.10
 * Date: 09.06.2020 | 19:27
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
#define ContryOption EU                   //Setup Region EU or US for Truck
#define HeadLightCombine false            //High => Low and High Beam on both Head Light output Pins | False => Seperate Pins for High and Low Beam 
#define serialCom false                     //Activate Communication to other modules via Serial 
#if (serialCom == true)
	#define truckAdress 1                 //Serial Adress for this Module: Truck
	#define beaconAdress 2                //Serial Adress for Module: Beacon Lights Extension
	#define trailerAdress 3               //Serial Adress for Module: Trailer 
	#define extFunctionAdress 4           //Serial Adress for Module: Special function for example Servos Steering
#endif
//Change this value for different debuging levels
#define debugLevel 3						//1 = Status LED | >2 = Serial Monitor

/************************************
 * Include Files
 ************************************/
#include <SoftPWM.h>							//https://github.com/bhagman/SoftPWM
#include "truckFunctions.h"						//Special functions
/************************************
 * Definition IO Pins
 ************************************/
// TODO: setup correct pins
//Pinout Arduino Micro:
//HW Serial 0+1 | I2C 2+3 | PWM 3, 5, 6, 9, 10, 11, 13 | LED 13
//Servo Lib deactivates PWM functionality on pins 9 and 10

//Inputs
#define inFunction1ControlPPM 2            //PPM Signal from Remote Control Extension | Interrupt Needed
#define inFunction2ControlPPM 3            //PPM Signal from Remote Control Extension | Interrupt Needed
#define inSteerControlPPM 7               //Steering Servo Signal from Receiver  | Interrupt Needed
// Pin 0+1+4 reserved for RS485!
#define inBrakeSignal A4                   //Brake Signal from Servonaut Speed Controller
#define inReverseSignal A5                 //Reverse Signal from Servonaut Speed Controller

//Outputs
#if (debugLevel >=1)
	#define outStatusLed 13					//Arduino status LED output Pin
#endif
#define outParkingLight A0                 //Parking light output pin
#define outLowBeamLight 10                 //Head light low beam output pin | PWM
#define outHighBeamLight 11               //Head light high beam output pin
#define outFogLight A1                    //Fog light output pin
#define outFrontLeftFlashLight 8         //Front left flashing light output pin
#define outFrontRightFlashLight 9        //Front right flashing light output pin
#define outRearLeftFlashLight 5           //Rear left flashing light output pin | PWM Needed for US
#define outRearRightFlashLight 6          //Rear right flashing light output pin | PWM Needed for US
#define outReverseLight A2                //Reverse light output pin
#define outBrakeLight 12                  //Brake light output pin | PWM for Parking Light
#define outAuxLight A3                    //Reserved for Special Auxiliary Light
//Free IOs 18, 19
/************************************
 * Definition and Initialisation 
 * Global Vars, Classes and Functions
 ************************************/
// These serial port names are intended to allow libraries and architecture-neutral
// sketches to automatically default to the correct port name for a particular type
// of use.  For example, a GPS module would normally connect to SERIAL_PORT_HARDWARE_OPEN,
// the first hardware serial port whose RX/TX pins are not dedicated to another use.
//
// SERIAL_PORT_MONITOR        Port which normally prints to the Arduino Serial Monitor
//
// SERIAL_PORT_USBVIRTUAL     Port which is USB virtual serial
//
// SERIAL_PORT_LINUXBRIDGE    Port which connects to a Linux system via Bridge library
//
// SERIAL_PORT_HARDWARE       Hardware serial port, physical RX & TX pins.
//
// SERIAL_PORT_HARDWARE_OPEN  Hardware serial ports which are open for use.  Their RX & TX
//                            pins are NOT connected to anything by default.
//Setup Serial and check if Board is UNO with one Serial or Leonardo/Micro with to Serials
#if defined(HAVE_HWSERIAL1)							//if serial ports 1 exist then the arduino has more than one serial port
	#ifndef SerialUSB								//if not allready defined
		#define SerialUSB SERIAL_PORT_MONITOR		//then define monitor port
	#endif
#else
	#if (debugLevel >1)								//if serial ports are the same debuging is not possible (for example on UNO)
		#define debugLevel 1						//do not change!!!
	#endif
#endif
#ifndef SerialHW									//if not allready defined
	#define SerialHW SERIAL_PORT_HARDWARE			//then define hardware port
#endif


bool pulseStatus = false;
uint32_t StatusPreviousMillis = 0;
uint32_t blinkOnTime = 0;

//Vars for Interrupt
volatile uint16_t int1Value[8] = {0};		//saves time difference of every channel
volatile uint8_t int1Index = 0;				//actual index for interrupt
volatile uint32_t int1LastChange = 0;		//time since last interrupt
volatile uint32_t int1LastSave = 0;			//time since last valid save in interrupt
volatile uint16_t int2Value[8] = {0};		//saves time difference of every channel
volatile uint8_t int2Index = 0;				//actual index for interrupt
volatile uint32_t int2LastChange = 0;		//time since last interrupt
volatile uint32_t int2LastSave = 0;			//time since last valid save in interrupt
volatile uint8_t int3Value = 0;				//saves time difference of this channel
volatile uint32_t int3LastChange = 0;		//time since last interrupt

//Vars for debug
#if (debugLevel >=1)
	bool serialIsSent = false;
#endif


//Functions
bool controllerStatus(bool);			//function to signal errorstate
uint8_t blink(uint16_t);				//function for blink mechanism
void ppmMultiInterrupt1();				//function for interrupt of first PPM signal
void ppmMultiInterrupt2();				//function for interrupt of second PPM signal
void ppmServoInterrupt();				//function for interrupt of servo PPM signal
void debugInterrupt();					//function to debug the interrupt data
bool ppmChannel1Evaluation();				//function to evaluate the ppm signals
bool ppmChannel2Evaluation();				//function to evaluate the ppm signals

//Classes

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
	
	attachInterrupt(digitalPinToInterrupt(inFunction1ControlPPM), ppmMultiInterrupt1, CHANGE); 	//Setup Interrupt
	attachInterrupt(digitalPinToInterrupt(inFunction2ControlPPM), ppmMultiInterrupt2, CHANGE);	//Setup Interrupt
	attachInterrupt(digitalPinToInterrupt(inSteerControlPPM), ppmServoInterrupt, CHANGE);		//Setup Interrupt
	#if (serialCom == true)
	SerialHW.begin(115200);  // start Serial for Communication
	#endif
	#if (debugLevel >=1)
	pinMode(outStatusLed, OUTPUT);
	#endif
	#if (debugLevel >=2)
	SerialUSB.begin(9600);  // start Serial for Monitoring
	#endif
	delay(5000);
// TODO: Setup IO pins
}

void loop() {                             // put your main code here, to run repeatedly:
	#if (debugLevel >=1)
		bool errorFlag = false;                 				// local var for error status
	#endif
	bool dynBrakeSignal = digitalRead(inBrakeSignal);			//Read digital Input and copy to dynamic var
	bool dynReverseSignal = digitalRead(inReverseSignal);		//Read digital Input and copy to dynamic var
	// bool dynParkingLight = 0;
	// bool dynLowBeamLight = 0;
	// bool dynHighBeamLight = 0;
	// bool dynFogLight = 0;
	// bool dynLeftFlashLight = 0;
	// bool dynRightFlashLight = 0;
	// bool dynAuxLight = 0;
	bool dynStatus = ppmChannel1Evaluation();
	if(!dynStatus) errorFlag = true;
	dynStatus = ppmChannel2Evaluation();
	if(!dynStatus) errorFlag = true;
	digitalWrite(outBrakeLight, dynBrakeSignal);				//Copy var to Output
	digitalWrite(outReverseLight, dynReverseSignal);			//Copy var to Output
	
	
	#if (debugLevel >=1)
		controllerStatus(errorFlag);
	#endif
	#if (debugLevel >=3)
		debugInterrupt();
	#endif
}

#if (debugLevel >=1)
bool controllerStatus(bool errorFlag) {
	if(errorFlag) {
		return true;
	} else {
		uint32_t currentMillis = millis();
    if (currentMillis - StatusPreviousMillis >= 1000) { 	//Zeitverzoegerte Abfrage
		StatusPreviousMillis = currentMillis;
		pulseStatus = !pulseStatus;
    } else if (currentMillis < StatusPreviousMillis) {  	//Reset
		StatusPreviousMillis = currentMillis; 
    }
    return pulseStatus;                 					//Flash if everything is OK
	}
}
#endif
uint8_t blink(uint16_t blinkTimeMillis) {
	if((blinkOnTime == 0) || (blinkOnTime > millis())){ 	//Reset blinkOnTime on startup and on overflow.
		blinkOnTime = millis();
	}
		uint32_t blinkTime = millis() - blinkOnTime;
	if(blinkTime%blinkTimeMillis >= blinkTimeMillis/2){ 	//ON/OFF Interval at half of Time.
		return 0;
	} else {
		return 1;
	}

}

void ppmMultiInterrupt1(){
	volatile uint32_t nMicros = micros(); 						//Save actual time
	volatile uint32_t nDifference = (nMicros - int1LastChange); //Calc time since last Change
	if((nDifference > 700) && (nDifference < 2200)) { 		//Filter HIGH Impulse | HIGH if time is between 700 and 2200 
		if((nDifference > 850) && (nDifference < 980)) { 	//if time is ~915 then this is the start impulse
			int1Index = 0; 									//then set index to 0
		} else {
			if (int1Index < 8) {							//if index is out of bound, then wait for next start impulse
				int1Value[int1Index] = nDifference;			//save actual time difference to value
				int1Index++; 								//increment index by one
				int1LastSave = millis();					//save time of the last valid signal
			}
		}
	}

	int1LastChange = nMicros;								//save time for next interrupt
}

void ppmMultiInterrupt2(){
	volatile uint32_t nMicros = micros(); 						//Save actual time
	volatile uint32_t nDifference = (nMicros - int2LastChange); //Calc time since last Change
	if((nDifference > 700) && (nDifference < 2200)) { 		//Filter HIGH Impulse | HIGH if time is between 700 and 2200 
		if((nDifference > 850) && (nDifference < 980)) { 	//if time is ~915 then this is the start impulse
			int2Index = 0; 									//then set index to 0
		} else {
			if (int2Index < 8) {							//if index is out of bound, then wait for next start impulse
				int2Value[int2Index] = nDifference;			//save actual time difference to value
				int2Index++; 								//increment index by one
				int2LastSave = millis();					//save time of the last valid signal
			}
		}
	}

	int2LastChange = nMicros;								//save time for next interrupt
}

void ppmServoInterrupt(){
	volatile uint32_t nMicros = micros(); 						//Save actual time
	volatile uint32_t nDifference = (nMicros - int3LastChange); //Calc time since last Change
	if((nDifference > 700) && (nDifference < 2200)) { 		//Filter HIGH Impulse | HIGH if time is between 700 and 2200 
		int3Value = nDifference;							//save actual time difference to value
	}

	int3LastChange = nMicros;								//save time for next interrupt
}

#if (debugLevel >=3)
void debugInterrupt() {
	if((millis()%1000 >= 500) && (serialIsSent == false)) {
		SerialUSB.println("--Start---");
		SerialUSB.println(int1Value[0]);
		SerialUSB.println(int1Value[1]);
		SerialUSB.println(int1Value[2]);
		SerialUSB.println(int1Value[3]);
		SerialUSB.println(int1Value[4]);
		SerialUSB.println(int1Value[5]);
		SerialUSB.println(int1Value[6]);
		SerialUSB.println(int1Value[7]);
		SerialUSB.println("---End----");
		SerialUSB.println("--Start2---");
		SerialUSB.println(int2Value[0]);
		SerialUSB.println(int2Value[1]);
		SerialUSB.println(int2Value[2]);
		SerialUSB.println(int2Value[3]);
		SerialUSB.println(int2Value[4]);
		SerialUSB.println(int2Value[5]);
		SerialUSB.println(int2Value[6]);
		SerialUSB.println(int2Value[7]);
		SerialUSB.println("---End----");
		serialIsSent = true;
	} else if((millis()%1000 < 500) && (serialIsSent == true)) {
		serialIsSent = false;
	}
}
	#endif
uint32_t maxTimeForInterrupt = 2000; //max time in milliseconds
uint8_t channel1Switch[6] = { 0 };
uint16_t channel1Poti[2] = { 0 };
bool ppmChannel1Evaluation() {
	// 16:59:31.637 -> 1504 	row below:	1 Poti (0-100% -> 1000-2000)
	// 16:59:31.637 -> 1568	row below:	2 Poti (0-100% -> 1000-2000)
	// 16:59:31.637 -> 1516	row below:	3 switch (up/mid/down 1000/1500/2000)
	// 16:59:31.637 -> 1528	row below: 	4 switch (up/mid/down 1000/1500/2000)
	// 16:59:31.637 -> 1508	row below:		1 button (up/mid/down 1000/1500/2000)
	// 16:59:31.637 -> 1532	row below:		2 switch (up/mid/down 1000/1500/2000)
	// 16:59:31.637 -> 1532	row below:		3 switch (up/mid/down 1000/1500/2000)
	// 16:59:31.637 -> 1528	row below:		4 button/switch (up/mid/down 1000/1500/2000)

	if((millis()-int1LastSave) >= maxTimeForInterrupt) {
		for(uint8_t i = 0; i < 8; i++) {
			int1Value[i] = 0;
		}
		return 0;
	} else {
		channel1Poti[0] = ppmServoToRange(int1Value[0], 1000, 2000, 0, 1023);
		channel1Poti[1] = ppmServoToRange(int1Value[1], 1000, 2000, 0, 1023);
		channel1Switch[0] = ppmToSwitchStages(int1Value[2]);
		channel1Switch[1] = ppmToSwitchStages(int1Value[3]);
		channel1Switch[2] = ppmToSwitchStages(int1Value[4]);
		channel1Switch[3] = ppmToSwitchStages(int1Value[5]);
		channel1Switch[4] = ppmToSwitchStages(int1Value[6]);
		channel1Switch[5] = ppmToSwitchStages(int1Value[7]);
		return 1;
	}	
}

uint8_t channel2Switch[8] = { 0 };
bool ppmChannel2Evaluation() {
	// 16:59:31.637 -> 1976	row below:		4 button (up/down 2000/1000)
	// 16:59:31.637 -> 1984	row below:		3 switch (up/down 2000/1000)
	// 16:59:31.637 -> 1980	row below:		2 switch (up/down 2000/1000)
	// 16:59:31.637 -> 1980	row below:		1 switch (up/down 2000/1000)
	// 16:59:31.637 -> 1696	row below:	2 switch (up/mid/down 1000/1700/1700)
	// 16:59:31.637 -> 1716	row below:	2 switch (up/mid/down 1700/1700/1000)
	// 16:59:31.637 -> 1692	row below:	1 switch (up/mid/down 1000/1700/1700)
	// 16:59:31.637 -> 1696	row below:	1 switch (up/mid/down 1700/1700/1000)

	if((millis()-int2LastSave) >= maxTimeForInterrupt) {
		for(uint8_t i = 0; i < 8; i++) {
			int2Value[i] = 0;
		}
		return 0;
	} else {
		channel2Switch[0] = ppmToSwitchStages(int2Value[0], PPM_INVERT);
		channel2Switch[1] = ppmToSwitchStages(int2Value[1], PPM_INVERT);
		channel2Switch[2] = ppmToSwitchStages(int2Value[2], PPM_INVERT);
		channel2Switch[3] = ppmToSwitchStages(int2Value[3], PPM_INVERT);
		channel2Switch[4] = ppm2ToSwitch3Stages(int2Value[4], int2Value[5]);
		channel2Switch[5] = ppm2ToSwitch3Stages(int2Value[6], int2Value[7]);
		return 1;
	}	
}
