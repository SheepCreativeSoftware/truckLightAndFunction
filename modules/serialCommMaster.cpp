/************************************ 
 * Simple SerialBus Slave Interface v0.0.1
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
#include "Arduino.h"
#include "serialCommMaster.h"
#include "HardwareSerial.h"

// state machine states
#define IDLE 1
#define WAITING_FOR_REPLY 2
#define WAITING_FOR_TURNAROUND 3
#define BUFFER_SIZE 64

#define FUNC_LIGHT_DATA 1
#define FUNC_SERVO 2

uint8_t state;
uint8_t sendState;
uint32_t timeout; // timeout interval
uint32_t polling; // turnaround delay interval
uint16_t frameDelay; // frame time out in microseconds
uint32_t delayStart; // init variable for turnaround and timeout delay

void serialConfigure(HardwareSerial *_SerialPort,	// Serial interface on arduino
					uint32_t baud,						// Baudrate
					uint8_t byteFormat,		// e.g. SERIAL_8N1 | start bit, data bit, stop bit
					long _timeout, 
					long _polling, 
					uint8_t _slaveID,			// Address of this device
					uint8_t _TxEnablePin,		// Pin to switch between Transmit and Receive
) {
	SerialPort = _SerialPort;						// Store on a global var
	(*SerialPort).begin(baud, byteFormat);			// Init communication port
	timeout = _timeout;
	polling = _polling;
	slaveID = _slaveID;								// Store on a global var for other functions
	TxEnablePin = _TxEnablePin;						// Store on a global var for other functions
	pinMode(TxEnablePin, OUTPUT);
	digitalWrite(TxEnablePin, LOW);					// Set to low at start to receive data
	errorCount = 0; 								// Initialize errorCount
} 

uint16_t serialUpdate() {
	switch (state) {
		case IDLE:
			idle();
		break;
		case WAITING_FOR_REPLY:
			waitingForReply();
		break;
		case WAITING_FOR_TURNAROUND:
			waitingForTurnaround();
		break;
	}
	
}

void idle() {
	switch (sendState) {
		case FUNC_LIGHT_DATA:
			constructPacket(FUNC_LIGHT_DATA, data);
		break;
		case FUNC_SERVO:
			constructPacket(FUNC_SERVO, data, data2);
		break;
	}
}

void waitingForTurnaround() {
	if ((millis() - delayStart) > polling) state = IDLE;
}

uint16_t calculateCRC(uint8_t bufferSize) {
	uint16_t crc16 = 0xFFFF;														// Load a 16–bit register with FFFF hex (all 1’s)
	for (uint8_t bufferPosition = 0; bufferPosition < bufferSize; bufferPosition++) {
		crc16 = crc16 ^ frame[bufferPosition];										// XOR byte into least significant byte of crc
		for (uint8_t bitPosition = 1; bitPosition <= BIT_COUNT; bitPosition++) {	// Loop over each bit
			if(crc16 & 0x0001) {													// If the LSB is set
				crc16 >>= 1;														// Shift right and XOR with polynomial 
				crc16 ^= POLYNOMIAL;
			} else {																// Shift only to the right if LSB is not set
				crc16 >>= 1;
			}
		}
	}
	return crc16; 																	// The final content of the CRC register is the CRC value
}

void sendPacket(unsigned char bufferSize) {
	digitalWrite(TxEnablePin, HIGH);
		
	for (unsigned char i = 0; i < bufferSize; i++) {
		(*SerialPort).write(frame[i]);
	}
	(*SerialPort).flush();
	
	//delayMicroseconds(frameDelay);
	
	digitalWrite(TxEnablePin, LOW);
		
	delayStart = millis(); // start the timeout delay	
}
