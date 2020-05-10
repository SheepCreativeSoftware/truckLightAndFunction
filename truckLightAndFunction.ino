/************************************ 
 * truckLightAndFunction v0.0.4
 * Date: 10.05.2020 | 15-00
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
#define wireCom true                     //Activate Communication to other modules via I2C 
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
#include <Wire.h>                         //Include I2C Library
#endif

/************************************
 * Definition IO Pins
 ************************************/
// TODO: setup correct pins
//Pinout Arduino Micro:
//I2C 2+3 | PWM 3, 5, 6, 9, 10, 11, 13 | LED 13
//Servo Lib deactivates PWM functionality on pins 9 and 10

//Inputs
#define inFunctionControlPPM 0            //PPM Signal from Remote Control Extension | Interrupt Needed
#define inSteerControlPPM 1               //Steering Servo Signal from Receiver  | Interrupt Needed
// Pin 2+3 reserved for I2C!
#define inBrakeSignal 4                   //Brake Signal from Servonaut Speed Controller
#define inReverseSignal 7                 //Reverse Signal from Servonaut Speed Controller

//Outputs
#if (debugLevel >=1)
	#define outStatusLed 13					//Arduino status LED output Pin
#endif
#define outParkingLight 8                 //Parking light output pin
#define outLowBeamLight 9                 //Head light low beam output pin | PWM
#define outHighBeamLight 10               //Head light high beam output pin
#define outFogLight 12                    //Fog light output pin
#define outFrontLeftFlashLight 14         //Front left flashing light output pin
#define outFrontRightFlashLight 15        //Front right flashing light output pin
#define outRearLeftFlashLight 5           //Rear left flashing light output pin | PWM Needed for US
#define outRearRightFlashLight 6          //Rear right flashing light output pin | PWM Needed for US
#define outReverseLight 16                //Reverse light output pin
#define outBrakeLight 11                  //Brake light output pin | PWM for Parking Light
#define outAuxLight 17                    //Reserved for Special Auxiliary Light
//Free IOs 18, 19
/************************************
 * Definition and Initialisation 
 * Global Vars, Classes and Functions
 ************************************/
bool pulseStatus = false;
unsigned long StatusPreviousMillis = 0;
unsigned long blinkOnTime = 0;
unsigned long waitForAnswer = 0;
bool sendDirection = false;
unsigned int reciveTimeout = 1000;
//Functions
bool controllerStatus(bool);
int blink(unsigned int);
unsigned int wireComunication(int, unsigned int, unsigned int);
unsigned int checkCRC(unsigned int, unsigned int, unsigned int);


void setup() {
	// put your setup code here, to run once:
	#if (wireCom == true)
	Wire.begin(truckAdress);                 // join I2C bus (address optional for master)
	#endif
	#if (debugLevel >=2)
	Serial.begin(9600);  // start serial for output
	#endif
// TODO: Setup IO pins
}
bool pulseTest = false;
void loop() {                             // put your main code here, to run repeatedly:
	#if (debugLevel >=1)
		bool errorFlag = false;                 	// local var for error status
	#endif


	// Example For later Communication with other Module
	// TODO: Setup Communication
	// pulseTest = !pulseTest;
	// delay(5000);
	// #if (wireCom == true)
		// #ifdef beaconAdress
			// Wire.beginTransmission(beaconAdress); 		// transmit to Beacon Module
			// Wire.write(pulseTest);       				// send bool
			// Wire.endTransmission();        				// stop transmitting
		// #endif
	#if (wireCom == true)
		unsigned int tempStatus = wireComunication(beaconAdress, 0x01, 0x01);
	#if (debugLevel >=3)
		Serial.println("Com Status:");      				// print the character
		Serial.println(tempStatus);         				// com Status 
	#endif
	#endif
	#if (debugLevel >=1)
		controllerStatus(errorFlag);
	#endif
}
#if (debugLevel >=1)
bool controllerStatus(bool errorFlag) {
	if(errorFlag) {
		return true;
	} else {
		unsigned long currentMillis = millis();
    if (currentMillis - StatusPreviousMillis >= 1000) { //Zeitverzoegerte Abfrage
		StatusPreviousMillis = currentMillis;
		pulseStatus = !pulseStatus;
    } else if (currentMillis < StatusPreviousMillis) {  //Reset
		StatusPreviousMillis = currentMillis; 
    }
    return pulseStatus;                 //Flash if everything is OK
	}
}
#endif
int blink(unsigned int blinkTimeMillis) {
	if((blinkOnTime == 0) || (blinkOnTime > millis())){ //Reset blinkOnTime on startup and on overflow.
		blinkOnTime = millis();
	}
		unsigned long blinkTime = millis() - blinkOnTime;
	if(blinkTime%blinkTimeMillis >= blinkTimeMillis/2){ //ON/OFF Interval at half of Time.
		return 0;
	} else {
		return 1;
	}

}

#if (wireCom == true)
unsigned int wireComunication(int sendAdress, unsigned int RegisterAdress, unsigned int data) {
	unsigned int sendError = 0;
	if(sendDirection == false) {
		unsigned int dataCRC = checkCRC(RegisterAdress, data);
		Wire.beginTransmission(sendAdress); 		// transmit to Beacon Module
		Wire.write(RegisterAdress);       			// send bytes
		Wire.write(data);       					// send bytes
		Wire.write(dataCRC);       					// send bytes
		sendError = Wire.endTransmission();        	// stop transmitting
		#if (debugLevel >=3)
		Serial.println("Send Error Code:");         // print the character
		Serial.println(sendError);         			// print the character
		#endif
		if(sendError == 0) {
			Wire.requestFrom(sendAdress, 1);    // request 6 bytes from slave device #8
			sendDirection = true;
			waitForAnswer = millis();
		}
	} else {
		while (Wire.available()) { 					// slave may send less than requested
			unsigned int tempData = Wire.read(); 	// receive a byte as character
			#if (debugLevel >=3)
			Serial.println("Recived Answer:");      // print the character
			Serial.println(tempData);         		// print the character
			#endif
			sendDirection = false;
			if(tempData == 0x01) {
				sendError = 0;
			} else {
				sendError = tempData;
			}
		}
		if(millis() >= (waitForAnswer+reciveTimeout)) {
			sendDirection = false;
			sendError = 0x99;
			#if (debugLevel >=3)
			Serial.println("Answer Timeout");      // print the character
			#endif
		}
		
	}
	
	return sendError;
}
	
unsigned int checkCRC(unsigned int registerAdress, unsigned int data){
	return 1;
}
#endif