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

void SerialCommMaster::begin(
	HardwareSerial* serialPort,
	uint32_t baud,
	uint8_t byteFormat,
	long timeout,
	long polling,
	uint8_t txEnablePin,
	uint8_t protocolVersion
) {
	_serialPort = serialPort;
	_timeout = timeout;
	_polling = polling;
	_txEnablePin = txEnablePin;
	_protocolVersion = protocolVersion;

	(*_serialPort).begin(baud, byteFormat);
	pinMode(_txEnablePin, OUTPUT);
	digitalWrite(_txEnablePin, LOW);

	_errorCount = 0;
	_state = WAITING_FOR_TURNAROUND;
	_frameDelay = 10;
	_delayStart = 0;
	_lightDataFromSerial = 0;
	_additionalDataFromSerial = 0;
	_servoMicrosFromSerial[0] = 0;
	_servoMicrosFromSerial[1] = 0;
}

uint16_t SerialCommMaster::update() {
	switch (_state) {
		case IDLE:
			idle();
			break;
		case WAITING_FOR_TURNAROUND:
			waitingForTurnaround();
			break;
	}
	return _errorCount;
}

void SerialCommMaster::idle() {
	switch (_protocolVersion) {
		case FUNC_LIGHT_DATA:
			constructPacket(FUNC_LIGHT_DATA, _lightDataFromSerial);
			break;
		case FUNC_LIGHT_SERVO:
			constructPacket(FUNC_LIGHT_SERVO, _lightDataFromSerial, _additionalDataFromSerial, _servoMicrosFromSerial[0],
							_servoMicrosFromSerial[1]);
			break;
	}
}

void SerialCommMaster::constructPacket(
	uint8_t function,
	uint16_t lightData,
	uint16_t additionalData,
	uint16_t servoData1,
	uint16_t servoData2
) {
	_frame[0] = function;
	uint8_t frameSize;
	switch (function) {
		case FUNC_LIGHT_DATA:
			frameSize = 4; // 1 byte function + 1 byte lightData + 2 bytes CRC
			_frame[1] = lightData & 0x00FF;
			break;
		case FUNC_LIGHT_SERVO:
			frameSize = 9; // 1 byte function + 1 byte lightData + 1 byte additionalData + 2 bytes servo1 + 2 bytes
						   // servo2 + 2 bytes CRC
			_frame[1] = lightData & 0x00FF;
			_frame[2] = additionalData & 0x00FF;
			_frame[3] = servoData1 >> 8;
			_frame[4] = servoData1 & 0xFF;
			_frame[5] = servoData2 >> 8;
			_frame[6] = servoData2 & 0xFF;
			break;
		default:
			return; // Invalid function, do not send anything
	}

	uint16_t crc16 = calculateCRC(frameSize - 2);
	_frame[frameSize - 2] = crc16 >> 8; // Split crc into two bytes
	_frame[frameSize - 1] = crc16 & 0xFF;

	sendPacket(frameSize);
	_state = WAITING_FOR_TURNAROUND;
}

void SerialCommMaster::waitingForTurnaround() {
	if ((millis() - _delayStart) > _polling)
		_state = IDLE;
}

uint16_t SerialCommMaster::calculateCRC(uint8_t bufferSize) {
	uint16_t crc16 = 0xFFFF; // Load a 16–bit register with FFFF hex (all 1’s)
	for (uint8_t bufferPosition = 0; bufferPosition < bufferSize; bufferPosition++) {
		crc16 = crc16 ^ _frame[bufferPosition]; // XOR byte into least significant byte of crc
		for (uint8_t bitPosition = 1; bitPosition <= BIT_COUNT; bitPosition++) { // Loop over each bit
			if (crc16 & 0x0001) { // If the LSB is set
				crc16 >>= 1; // Shift right and XOR with polynomial
				crc16 ^= POLYNOMIAL;
			} else { // Shift only to the right if LSB is not set
				crc16 >>= 1;
			}
		}
	}
	return crc16; // The final content of the CRC register is the CRC value
}

void SerialCommMaster::sendPacket(uint8_t bufferSize) {
	digitalWrite(_txEnablePin, HIGH);

	for (unsigned char i = 0; i < bufferSize; i++) {
		(*_serialPort).write(_frame[i]);
	}
	(*_serialPort).flush();

	delayMicroseconds(_frameDelay);

	digitalWrite(_txEnablePin, LOW);

	_delayStart = millis(); // start the timeout delay
}

void SerialCommMaster::setLightData(LightIdentifier lightOption, bool lightState) {
	if (lightState) {
		uint8_t bitmask = 0x1 << lightOption;
		_lightDataFromSerial |= bitmask;
	} else {
		uint8_t bitmask = ~(0x1 << lightOption);
		_lightDataFromSerial &= bitmask;
	}
}

void SerialCommMaster::setAdditionalData(AdditionalDataIdentifier additionalOption, bool additionalState) {
	if (additionalState) {
		uint8_t bitmask = 0x1 << additionalOption;
		_additionalDataFromSerial |= bitmask;
	} else {
		uint8_t bitmask = ~(0x1 << additionalOption);
		_additionalDataFromSerial &= bitmask;
	}
}

void SerialCommMaster::setServoData(ServoDataIdentifier servoOption, uint16_t servoValue) {
	_servoMicrosFromSerial[servoOption] = servoValue;
}