/************************************ 
 * Simple SerialBus Master Interface v0.0.1
 * Date: 30.08.2022 | 21:52
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

#ifndef _SERIAL_COMM_SLAVE_H_
#define _SERIAL_COMM_SLAVE_H_

#include "HardwareSerial.h"

#define BUFFER_SIZE 64
#define MIN_BUFFER_SIZE 4
#define BUFFER_EMPTY 0
#define BROADCAST_ADDRESS 0
#define FUNC_LIGHT_DATA 1
#define FUNC_SERVO 2

// For CRC calculation
#define BIT_COUNT 8
#define POLYNOMIAL 0xA001

uint8_t frame[BUFFER_SIZE];
uint8_t slaveID;
uint8_t TxEnablePin;
uint16_t errorCount;
HardwareSerial* SerialPort;

uint8_t lightDataFromSerial;
uint16_t servoMicrosFromSerial[2];

uint16_t serialUpdate();
void serialConfigure(HardwareSerial *_SerialPort,	// Serial interface on arduino
					uint32_t baud,						// Baudrate
					uint8_t byteFormat,		// e.g. SERIAL_8N1 | start bit, data bit, stop bit
					uint8_t _slaveID,			// Address of this device
					uint8_t _TxEnablePin,		// Pin to switch between Transmit and Receive
);

#include "serialCommMaster.cpp"
#endif