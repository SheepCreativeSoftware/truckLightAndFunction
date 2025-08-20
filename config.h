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

#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "vehicleConfig.h"

VehicleConfig vehicleConfig = {
	.generalConfig = {
		.countryOption = CountryOption::US,
		.debugLevel = DebugLevel::PPM_CHANNEL1,
		.statusLightPin = 13
	},
	.ppmConfig = {
		.pinChannel1 = 2,
		.pinChannel2 = 3,
		.pinSoundChannel = 7
	},
	.generalLightConfig = {
		.fadeOnTime = 200,
		.fadeOffTime = 200,
		.starterDimmingFactor = 5,
		.starterDimmingMultiplier = 2
	},
	.serialConfig = {
		.isEnabled = true,
		.outTxEnablePin = 4,
		.baudRate = 19200,
		.byteFormat = SERIAL_8N1,
		.timeout = 1000,
		.pollingInterval = 20
	},
	.lightInputChannel = {
		.reverseSignal = A4,
		.brakeSignal = A5
	},
	.rearLeftTurnLight = {
		.outputPin = 5,
		.primaryOnBrightness = 255,
		.secondaryOnBrightness = 0,
		.tertiaryOnBrightness = 0,
		.offBrightness = 0,
		.fadeOnTime = 200,
		.fadeOffTime = 200,
	},
	.rearRightTurnLight = {
		.outputPin = 6,
		.primaryOnBrightness = 255,
		.secondaryOnBrightness = 0,
		.tertiaryOnBrightness = 0,
		.offBrightness = 0,
		.fadeOnTime = 200,
		.fadeOffTime = 200
	},
	.frontLeftTurnLight = {
		.outputPin = 8,
		.primaryOnBrightness = 255,
		.secondaryOnBrightness = 0,
		.tertiaryOnBrightness = 0,
		.offBrightness = 0,
		.fadeOnTime = 200,
		.fadeOffTime = 200
	},
	.frontRightTurnLight = {
		.outputPin = 9,
		.primaryOnBrightness = 255,
		.secondaryOnBrightness = 0,
		.tertiaryOnBrightness = 0,
		.offBrightness = 0,
		.fadeOnTime = 200,
		.fadeOffTime = 200
	},
	.lowBeamLight = {
		.outputPin = 10,
		// Low Beam or High beam when combined
		.primaryOnBrightness = 255,
		// Low Beam when combined with High Beam
		.secondaryOnBrightness = 100,
		// Parking when combined with Low Beam
		.tertiaryOnBrightness = 5,
		.offBrightness = 0,
		.fadeOnTime = 200,
		.fadeOffTime = 200
	},
	.highBeamLight = {
		.outputPin = 11,
		.primaryOnBrightness = 255,
		.secondaryOnBrightness = 0,
		.tertiaryOnBrightness = 0,
		.offBrightness = 0,
		.fadeOnTime = 200,
		.fadeOffTime = 200
	},
	.parkingLight = {
		.outputPin = A3,
		.primaryOnBrightness = 255,
		.secondaryOnBrightness = 0,
		.tertiaryOnBrightness = 0,
		.offBrightness = 0,
		.fadeOnTime = 200,
		.fadeOffTime = 200
	},
	.fogLight = {
		.outputPin = A2,
		.primaryOnBrightness = 255,
		.secondaryOnBrightness = 0,
		.tertiaryOnBrightness = 0,
		.offBrightness = 0,
		.fadeOnTime = 200,
		.fadeOffTime = 200
	},
	.reverseLight = {
		.outputPin = A1,
		.primaryOnBrightness = 255,
		.secondaryOnBrightness = 0,
		.tertiaryOnBrightness = 0,
		.offBrightness = 0,
		.fadeOnTime = 200,
		.fadeOffTime = 200
	},
	.brakeLight = {
		.outputPin = 12,
		.primaryOnBrightness = 255,
		// Parking when combined
		.secondaryOnBrightness = 15,
		.tertiaryOnBrightness = 0,
		.offBrightness = 0,
		.fadeOnTime = 200,
		.fadeOffTime = 200
	},
	.auxLight = {
		.outputPin = A0,
		.primaryOnBrightness = 255,
		.secondaryOnBrightness = 0,
		.tertiaryOnBrightness = 0,
		.offBrightness = 0,
		.fadeOnTime = 200,
		.fadeOffTime = 200
	},
	.lowBeamConfig = {
		.isParkingLight = true,
		.isHighBeam = true
	},
	.highBeamConfig = {
		.flashFrequency = 800
	},
	.turnSignalConfig = {
		.flashFrequency = 1000
	}
};

#define DEBUGLEVEL vehicleConfig.generalConfig.debugLevel

// Old config - remove after refactoring
/* #define COUNTRY_OPTION US			  // Setup Region EU or US for Truck
#define HEADLIGHT_IS_PARKING true	  // Head light is also parking light (dimmed)
#define HEADLIGHT_IS_HIGHBEAM true	  // High => Low and High Beam on both Head Light output Pins | False => Seperate Pins for High and Low Beam
#define HEADLIGHT_PARKING_VALUE 5	  // 0-255 Value for combined operation of Headlight
#define HEADLIGHT_LOWBEAM_VALUE 100	  // 0-255 Value for combined operation of Headlight
#define HEADLIGHT_HIGHBEAM_VALUE 255  // 0-255 Value for combined operation of Headlight
#define HIGH_BEAM_FLASH_FREQUENCY 800 // Time frequency for head beam to flash
#define BLINKER_FREQUENCY 1000		  // Time frequency for Flasher to flash
#define LIGHT_FADE_ON_TIME 200		  // 200ms Fade on time for the Light
#define LIGHT_FADE_OFF_TIME 200		  // 200ms Fade off time for the Light
#define PARKING_DIMM 50				  // 0-255 Value for dimming the parking light
#define PARKING_DIMM_REAR 15		  // 0-255 Value for dimming the parking light
#define NORMAL_LIGHT_DIMMING 255	  // 0-255 Value for all light when active
#define STARTER_DIMM_DIVISOR 5		  // Divisor for Dimming function
#define STARTER_DIMM_MULTI1 2		  // 0-255 MAX Value for all light when active starter is activ
#define SERIAL_COM true				  // Activate Communication to other modules via Serial Port
#if (SERIAL_COM == true)
#define SERIAL_TURNAROUND 20
#endif */

#endif