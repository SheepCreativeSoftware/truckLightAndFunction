/************************************ 
 * truckLightAndFunction v0.0.1
 * Date: 08.05.2020 | 21-07-10
 * by M.Egner
 * Truck Light and function module
 ************************************/

/************************************
 * Configuration Programm
 ************************************/
#define ContryOption EU                   //Setup Region EU or US for Truck
#define HeadLightCombine false            //High => Low and High Beam on both Head Light output Pins | False => Seperate Pins for High and Low Beam 
#define WireCom false                     //Activate Communication to other modules via I2C 
#if (WireCom == true)
#define TruckAdress 1                     //I2C Adress for this Module: Truck
#define BeaconAdress 2                    //I2C Adress for Module: Beacon Lights Extension
#define TrailerAdress 3                   //I2C Adress for Module: Trailer 
#define ExtFunctionAdress 4               //I2C Adress for Module: Special function for example Servos Steering
#endif
                        
/************************************
 * Include Files
 ************************************/
#if (WireCom == true)
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
#define outStatusLed 13                   //Arduino status LED output Pin
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

//Functions
bool controllerStatus(bool);
int blink(unsigned int);


void setup() {
  // put your setup code here, to run once:
  #if (WireCom == true)
  Wire.begin(TruckAdress);                 // join I2C bus (address optional for master)
  #endif
// TODO: Setup IO pins
}

void loop() {                             // put your main code here, to run repeatedly:
  bool errorFlag = false;                 // local var for error status
  
  
  // Example For later Communication with other Module
  // TODO: Setup Communication
  #if (WireCom == true)
  Wire.beginTransmission(8);             // transmit to device #8
  Wire.write("Need to Setup COM");       // sends five bytes
  Wire.endTransmission();                // stop transmitting
  #endif
  
  controllerStatus(errorFlag);
}

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
