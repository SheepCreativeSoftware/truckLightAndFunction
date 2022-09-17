/************************************ 
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

#ifndef _IO_MAPPING_H_
#define _IO_MAPPING_H_

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
#define inSoundPPM 7               			//Soundmodul Servo Signal from Receiver  | Interrupt Needed
// Pin 0+1+4 reserved for RS485!
#define inBrakeSignal A4                   //Brake Signal from Servonaut Speed Controller
#define inReverseSignal A5                 //Reverse Signal from Servonaut Speed Controller

//Outputs
#define outStatusLed 13					//Arduino status LED output Pin
#define outTxEnablePin 4
#define outParkingLight A3                 //Parking light output pin
#define outLowBeamLight 10                 //Head light low beam output pin | PWM
#define outHighBeamLight 11               //Head light high beam output pin
#define outFogLight A2                    //Fog light output pin
#define outFrontLeftFlashLight 8         //Front left flashing light output pin
#define outFrontRightFlashLight 9        //Front right flashing light output pin
#define outRearLeftFlashLight 5           //Rear left flashing light output pin | PWM Needed for US
#define outRearRightFlashLight 6          //Rear right flashing light output pin | PWM Needed for US
#define outReverseLight A1                //Reverse light output pin
#define outBrakeLight 12                  //Brake light output pin | PWM for Parking Light
#define outAuxLight A0                    //Reserved for Special Auxiliary Light
//Free IOs 18, 19

#endif