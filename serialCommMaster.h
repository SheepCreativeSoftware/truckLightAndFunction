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

#ifndef _SERIAL_COMM_MASTER_H_
#define _SERIAL_COMM_MASTER_H_

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

enum AdditionalDataIdentifier {
	LEFT_TURN_INDICATOR = 0,
	RIGHT_TURN_INDICATOR = 1,
	HAZARD_STATE = 2,
	SERVO_POSITION_DOWN = 3,
	SERVO_POSITION_UP = 4
};

enum ServoDataIdentifier {
	SERVO_CHANNEL_1 = 0,
	SERVO_CHANNEL_2 = 1
};

class SerialCommMaster {
public:
	void begin(HardwareSerial* serialPort, uint32_t baud, uint8_t byteFormat, long timeout, long polling,
			   uint8_t txEnablePin, uint8_t protocolVersion = 1);

	uint16_t update();
	void setLightData(LightIdentifier lightOption, bool lightState);
	void setAdditionalData(AdditionalDataIdentifier additionalOption, bool additionalState);
	void setServoData(ServoDataIdentifier servoOption, uint16_t servoValue);

private:
	// Constants
	static constexpr uint8_t BUFFER_SIZE = 64;
	static constexpr uint8_t MIN_BUFFER_SIZE = 4;
	static constexpr uint8_t BUFFER_EMPTY = 0;
	static constexpr uint8_t BROADCAST_ADDRESS = 0;
	static constexpr uint8_t FUNC_LIGHT_DATA = 1;
	static constexpr uint8_t FUNC_LIGHT_SERVO = 2;
	static constexpr uint8_t BIT_COUNT = 8;
	static constexpr uint16_t POLYNOMIAL = 0xA001;

	// State machine states
	enum State { IDLE, WAITING_FOR_TURNAROUND };

	// Member variables
	HardwareSerial* _serialPort;
	uint8_t _txEnablePin;
	uint16_t _errorCount;
	uint8_t _protocolVersion;
	uint32_t _timeout;
	uint32_t _polling;
	uint16_t _frameDelay;
	uint32_t _delayStart;

	uint8_t _frame[BUFFER_SIZE];
	State _state;

	uint8_t _lightDataFromSerial;
	uint8_t _additionalDataFromSerial;
	uint16_t _servoMicrosFromSerial[2];

	// Private methods
	void idle();
	void waitingForTurnaround();
	void constructPacket(
		uint8_t function,
		uint16_t lightData,
		uint16_t additionalData = 0,
		uint16_t servoData1 = 0,
		uint16_t servoData2 = 0);
	uint16_t calculateCRC(uint8_t bufferSize);
	void sendPacket(uint8_t bufferSize);
};

#endif