/************************************ 
 * Simple SerialBus Slave Interface v0.0.1
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
#include "serialCommMaster.h"


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
uint8_t TxEnablePin;
uint16_t errorCount;
HardwareSerial* SerialPort;

// state machine states
#define IDLE 1
#define WAITING_FOR_TURNAROUND 2
#define BUFFER_SIZE 64

#define FUNC_LIGHT_DATA 1
#define FUNC_SERVO 2

uint8_t state = 2;
uint8_t sendState = 1;
uint32_t timeout; // timeout interval
uint32_t polling; // turnaround delay interval
uint16_t frameDelay = 10; // frame time out in microseconds
uint32_t delayStart; // init variable for turnaround and timeout delay

uint8_t lightDataFromSerial;
uint16_t servoMicrosFromSerial[2];

void serialConfigure(HardwareSerial *_SerialPort,	// Serial interface on arduino
					uint32_t baud,						// Baudrate
					uint8_t byteFormat,		// e.g. SERIAL_8N1 | start bit, data bit, stop bit
					long _timeout, 
					long _polling, 
					uint8_t _TxEnablePin		// Pin to switch between Transmit and Receive
) {
	SerialPort = _SerialPort;						// Store on a global var
	(*SerialPort).begin(baud, byteFormat);			// Init communication port
	timeout = _timeout;
	polling = _polling;
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
		case WAITING_FOR_TURNAROUND:
			waitingForTurnaround();
		break;
	}
	return errorCount;
}


void idle() {
	switch (sendState) {
		case FUNC_LIGHT_DATA:
			constructPacket(FUNC_LIGHT_DATA, lightDataFromSerial, 0);
		break;
		case FUNC_SERVO:
			constructPacket(FUNC_SERVO, servoMicrosFromSerial[0], servoMicrosFromSerial[1]);
		break;
	}
}

void constructPacket(uint8_t function, uint16_t data, uint16_t data2) {
	
	frame[0] = function;
	uint8_t frameSize;
	if(function == FUNC_LIGHT_DATA) {
		frameSize = 4;
		frame[1] = data & 0x00FF;
	}
	if(function == FUNC_SERVO) {
		frameSize = 7;
		frame[1] = data >> 8;
		frame[2] = data & 0xFF;
		frame[3] = data2 >> 8;
		frame[4] = data2 & 0xFF;
	}
	uint16_t crc16 = calculateCRC(frameSize -2);
	frame[frameSize - 2] = crc16 >> 8; // Split crc into two bytes
	frame[frameSize - 1] = crc16 & 0xFF;

	sendPacket(frameSize);
	state = WAITING_FOR_TURNAROUND;
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
	
	delayMicroseconds(frameDelay);
	
	digitalWrite(TxEnablePin, LOW);
		
	delayStart = millis(); // start the timeout delay	
}

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
void setLightData(uint8_t lightOption, bool lightState) {
	if(lightState) {
		uint8_t bitmask = 0x1 << lightOption;
		lightDataFromSerial |= bitmask;
	} else {
		uint8_t bitmask = ~(0x1 << lightOption);
		lightDataFromSerial &= bitmask;
	}

}

void setServoData(uint8_t servoOption, uint16_t servoValue) {
	servoMicrosFromSerial[servoOption] = servoValue;
}