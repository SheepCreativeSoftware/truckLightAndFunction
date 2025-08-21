/************************************ 
 * Simple SerialBus Master Interface v0.0.1
 * Date: 30.08.2022 | 21:52
 * <Truck Light and function module>
 * Copyright (C) 2020-2025 Marina Egner <hello@sheepcs.de>
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

#ifndef _SERIAL_COMM_SLAVE_H_
#define _SERIAL_COMM_SLAVE_H_

#include "Arduino.h"
#include "HardwareSerial.h"

enum LightIdentifier {
	PARK_LIGHT = 0,
	BRAKE_LIGHT = 1,
	REVERSE_LIGHT = 2,
	RIGHT_BLINK = 3,
	LEFT_BLINK = 4,
	AUX_LIGHT = 5,
	BEACON_LIGHT = 6,
	DIMM_LIGHTS = 7
};

#define PARKLIGHT 0
#define BRAKELIGHT 1
#define REVERSELIGHT 2
#define RIGHTBLINK 3
#define LEFTBLINK 4
#define AUXLIGHT 5
#define BEACONLIGHT 6
#define DIMMLIGHTS 7

void serialConfigure(HardwareSerial *_SerialPort,	// Serial interface on arduino
					uint32_t baud,						// Baudrate
					uint8_t byteFormat,		// e.g. SERIAL_8N1 | start bit, data bit, stop bit
					long _timeout, 
					long _polling, 
					uint8_t _TxEnablePin,		// Pin to switch between Transmit and Receive
					uint8_t protocolVersion = 1	// Protocol version to use
) ;

uint16_t serialUpdate();
void idle();
void waitingForTurnaround();
void constructPacket(uint8_t function, uint16_t lightData, uint16_t additionalData = 0, uint16_t servoData1 = 0, uint16_t ServoData2 = 0);
uint16_t calculateCRC(uint8_t bufferSize);
void sendPacket(unsigned char bufferSize);
void setLightData(uint8_t lightOption, bool lightState);
void setAdditionalData(uint8_t additionalOption, bool additionalState);
void setServoData(uint8_t servoOption, uint16_t servoValue);

#endif