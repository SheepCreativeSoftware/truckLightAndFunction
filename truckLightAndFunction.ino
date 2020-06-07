/************************************ 
 * truckLightAndFunction v0.0.7
 * Date: 07.06.2020 | 19:00
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
#define wireCom false                     //Activate Communication to other modules via I2C 
#if (wireCom == true)
	#define truckAdress 1                     //I2C Adress for this Module: Truck
	#define beaconAdress 2                    //I2C Adress for Module: Beacon Lights Extension
	#define trailerAdress 3                   //I2C Adress for Module: Trailer 
	#define extFunctionAdress 4               //I2C Adress for Module: Special function for example Servos Steering
#endif
#define debugLevel 3	
/************************************
 * Include Files
 ************************************/
#if (wireCom == true)
#include <Wire.h>                         		//Include I2C Library
#endif
#include <SoftPWM.h>							//https://github.com/bhagman/SoftPWM
#include "truckFunctions.h"						//Special functions
/************************************
 * Definition IO Pins
 ************************************/
// TODO: setup correct pins
//Pinout Arduino Micro:
//I2C 2+3 | PWM 3, 5, 6, 9, 10, 11, 13 | LED 13
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
bool pulseStatus = false;
unsigned long StatusPreviousMillis = 0;
unsigned long blinkOnTime = 0;

//Vars for Interrupt
volatile int int1Value[8] = {0};		//saves time difference of every channel
volatile int int1Index = 0;				//actual index for interrupt
volatile long int1LastChange = 0;		//time since last interrupt
volatile int int2Value[8] = {0};		//saves time difference of every channel
volatile int int2Index = 0;				//actual index for interrupt
volatile long int2LastChange = 0;		//time since last interrupt
volatile int int3Value = 0;				//saves time difference of this channel
volatile long int3LastChange = 0;		//time since last interrupt

//Vars for debug
#if (debugLevel >=1)
	bool serialIsSent = false;
#endif


//Functions
bool controllerStatus(bool);			//function to signal errorstate
int blink(unsigned int);				//function for blink mechanism
void ppmMultiInterrupt1();				//function for interrupt of first PPM signal
void ppmMultiInterrupt2();				//function for interrupt of second PPM signal
void ppmServoInterrupt();				//function for interrupt of servo PPM signal
void debugInterrupt();					//function to debug the interrupt data


//Classes
outputDefine Output[10];

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
	#if (debugLevel >=1)
	pinMode(outStatusLed, OUTPUT);
	#endif
	
	#if (debugLevel >=2)
	Serial.begin(9600);  // start serial for output
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
		unsigned long currentMillis = millis();
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
int blink(unsigned int blinkTimeMillis) {
	if((blinkOnTime == 0) || (blinkOnTime > millis())){ 	//Reset blinkOnTime on startup and on overflow.
		blinkOnTime = millis();
	}
		unsigned long blinkTime = millis() - blinkOnTime;
	if(blinkTime%blinkTimeMillis >= blinkTimeMillis/2){ 	//ON/OFF Interval at half of Time.
		return 0;
	} else {
		return 1;
	}

}

void ppmMultiInterrupt1(){
	volatile long nMicros = micros(); 						//Save actual time
	volatile long nDifference = (nMicros - int1LastChange); //Calc time since last Change
	if((nDifference > 700) && (nDifference < 2200)) { 		//Filter HIGH Impulse | HIGH if time is between 700 and 2200 
		if((nDifference > 850) && (nDifference < 980)) { 	//if time is ~915 then this is the start impulse
			int1Index = 0; 									//then set index to 0
		} else {
			if (int1Index < 8) {							//if index is out of bound, then wait for next start impulse
				int1Value[int1Index] = nDifference;			//save actual time difference to value
				int1Index++; 								//increment index by one
			}
		}
	}

	int1LastChange = nMicros;								//save time for next interrupt
}

void ppmMultiInterrupt2(){
	volatile long nMicros = micros(); 						//Save actual time
	volatile long nDifference = (nMicros - int2LastChange); //Calc time since last Change
	if((nDifference > 700) && (nDifference < 2200)) { 		//Filter HIGH Impulse | HIGH if time is between 700 and 2200 
		if((nDifference > 850) && (nDifference < 980)) { 	//if time is ~915 then this is the start impulse
			int2Index = 0; 									//then set index to 0
		} else {
			if (int2Index < 8) {							//if index is out of bound, then wait for next start impulse
				int2Value[int2Index] = nDifference;			//save actual time difference to value
				int2Index++; 								//increment index by one
			}
		}
	}

	int2LastChange = nMicros;								//save time for next interrupt
}

void ppmServoInterrupt(){
	volatile long nMicros = micros(); 						//Save actual time
	volatile long nDifference = (nMicros - int3LastChange); //Calc time since last Change
	if((nDifference > 700) && (nDifference < 2200)) { 		//Filter HIGH Impulse | HIGH if time is between 700 and 2200 
		int3Value = nDifference;							//save actual time difference to value
	}

	int3LastChange = nMicros;								//save time for next interrupt
}

#if (debugLevel >=3)
void debugInterrupt() {
	if((millis()%1000 >= 500) && (serialIsSent == false)) {
		Serial.println("--Start---");
		Serial.println(int1Value[0]);
		Serial.println(int1Value[1]);
		Serial.println(int1Value[2]);
		Serial.println(int1Value[3]);
		Serial.println(int1Value[4]);
		Serial.println(int1Value[5]);
		Serial.println(int1Value[6]);
		Serial.println(int1Value[7]);
		Serial.println("---End----");
		Serial.println("--Start2---");
		Serial.println(int2Value[0]);
		Serial.println(int2Value[1]);
		Serial.println(int2Value[2]);
		Serial.println(int2Value[3]);
		Serial.println(int2Value[4]);
		Serial.println(int2Value[5]);
		Serial.println(int2Value[6]);
		Serial.println(int2Value[7]);
		Serial.println("---End----");
		serialIsSent = true;
	} else if((millis()%1000 < 500) && (serialIsSent == true)) {
		serialIsSent = false;
	}
}
	#endif


