/************************************
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

#ifndef _VEHICLE_CONFIG_H_
#define _VEHICLE_CONFIG_H_

#include "Arduino.h"

enum CountryOption {
	US,
	EU
};

enum DebugLevel {
	NONE,
	STATUS_ONLY,
	PPM_CHANNEL1,
	PPM_CHANNEL2,
	FUNCTION_STATE,
	FUNCTION_OUT,
	STARTER_DIMMING
};

struct GeneralConfig {
	// Setup Region EU or US for Truck - Use `CountryOption` enum
	CountryOption countryOption;
	// Level for debugging - Use `DebugLevel` enum
	DebugLevel debugLevel;
	// Pin for status LED on the Arduino
	uint8_t statusLightPin;
};

struct PPMConfig {
	// First PPM Multiswitch Signal from Remote Control
	uint8_t pinChannel1;
	// Second PPM Multiswitch Signal from Remote Control
	uint8_t pinChannel2;
	// PPM Servo Signal from Sound Module
	uint8_t pinSoundChannel;
};

struct GeneralLightConfig {
	// Fade in time for all lights
	uint16_t fadeOnTime;
	// Fade out time for all lights
	uint16_t fadeOffTime;
	// Starter Brightness decrease factor
	uint8_t starterDimmingFactor;
	// Starter Brightness decrease multiplier
	uint8_t starterDimmingMultiplier;
};

struct SerialConfig {
	// Enable Serial Communication
	bool isEnabled;
	// Pin for TX Enable on MAX485
	uint8_t outTxEnablePin;
	// Baud rate for Serial Communication (e.g. 19200)
	uint32_t baudRate;
	// e.g. SERIAL_8N1 | start bit, data bit, stop bit
	uint8_t byteFormat;
	// Time to wait for a response
	long timeout;
	// Time between polling requests
	long pollingInterval;
};

struct LightInputChannel {
	// Pin for Reverse Signal from External Controller
	uint8_t reverseSignal;
	// Pin for Brake Signal from External Controller
	uint8_t brakeSignal;
};

struct LightOutputChannel {
	// Hardware Pin on the Arduino for the Light Output Channel
	uint8_t outputPin;
	// Brightness level for Primary Light Function
	uint8_t primaryOnBrightness;
	// Brightness level for Secondary Light Function
	uint8_t secondaryOnBrightness;
	// Brightness level for Tertiary Light Function
	uint8_t tertiaryOnBrightness;
	// Brightness level for Off Light Function
	uint8_t offBrightness;
	// Fade in time for all lights
	uint16_t fadeOnTime;
	// Fade out time for all lights
	uint16_t fadeOffTime;
};

struct LowBeamConfig {
	// Low Beam also functions as Parking Light (tertiary <brightness>)
	bool isParkingLight;
	// Low Beam also functions as High Beam (primary <brightness>)
	bool isHighBeam;
};

struct HighBeamConfig {
	// Frequency for High Beam Flashing functionality
	uint16_t flashFrequency;
};

struct TurnSignalConfig {
	// Frequency for Turn Signal
	uint16_t flashFrequency;
};


struct VehicleConfig {
	GeneralConfig generalConfig;
	PPMConfig ppmConfig;
	GeneralLightConfig generalLightConfig;
	SerialConfig serialConfig;
	LightInputChannel lightInputChannel;
	LightOutputChannel rearLeftTurnLight;
	LightOutputChannel rearRightTurnLight;
	LightOutputChannel frontLeftTurnLight;
	LightOutputChannel frontRightTurnLight;
	LightOutputChannel lowBeamLight;
	LightOutputChannel highBeamLight;
	LightOutputChannel parkingLight;
	LightOutputChannel fogLight;
	LightOutputChannel reverseLight;
	LightOutputChannel brakeLight;
	LightOutputChannel auxLight;
	LowBeamConfig lowBeamConfig;
	HighBeamConfig highBeamConfig;
	TurnSignalConfig turnSignalConfig;
};

#endif