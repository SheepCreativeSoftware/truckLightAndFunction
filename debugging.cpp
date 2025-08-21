/************************************s
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

#include "debugging.h"

bool pulseStatus = false;
uint32_t StatusPreviousMillis = 0;
uint32_t blinkOnTime = 0;
bool serialIsSent[3] = { false };


void debuggingInit(uint8_t debugLevel, uint8_t outputLED) {
	if(debugLevel >=1) pinMode(outputLED, OUTPUT);
	if(debugLevel >=2) SerialUSB.begin(9600);  // start Serial for Monitoring
}

void controllerStatus(bool errorFlag, uint8_t outputLED) {
	if(errorFlag) {
		digitalWrite(outputLED, true);
	} else {
		uint32_t currentMillis = millis();
		if (currentMillis - StatusPreviousMillis >= 1000) { 	//Zeitverzoegerte Abfrage
			StatusPreviousMillis = currentMillis;
			pulseStatus = !pulseStatus;
		} else if (currentMillis < StatusPreviousMillis) {  	//Reset
			StatusPreviousMillis = currentMillis; 
		}
		digitalWrite(outputLED, pulseStatus);					//Flash if everything is OK
	}
}

void debugChannelEvaluation(
	uint16_t channelNumber,
	uint16_t channel1,
	uint16_t channel2,
	uint16_t channel3,
	uint16_t channel4,
	uint16_t channel5,
	uint16_t channel6,
	uint16_t channel7,
	uint16_t channel8
) {
	if((millis()%1000 >= 500) && (serialIsSent[0] == false)) {
		SerialUSB.print(F("--Multiswitch "));
		SerialUSB.print(channelNumber);
		SerialUSB.println(F(" --"));
		SerialUSB.print(F("channel 1: "));
		SerialUSB.println(channel1);
		SerialUSB.print(F("channel 2: "));
		SerialUSB.println(channel2);
		SerialUSB.print(F("channel 3: "));
		SerialUSB.println(channel3);
		SerialUSB.print(F("channel 4: "));
		SerialUSB.println(channel4);
		SerialUSB.print(F("channel 5: "));
		SerialUSB.println(channel5);
		SerialUSB.print(F("channel 6: "));
		SerialUSB.println(channel6);
		SerialUSB.print(F("channel 7: "));
		SerialUSB.println(channel7);
		SerialUSB.print(F("channel 8: "));
		SerialUSB.println(channel8);
		SerialUSB.println(F("-------End-------"));
		serialIsSent[0] = true;
	} else if((millis()%1000 < 500) && (serialIsSent[0] == true)) {
		serialIsSent[0] = false;
	}
}

void debugFunctionState(bool parkLight,
						bool lowBeamLight,
						bool highBeamLight,
						bool highBeamLightFlash,
						bool fogLight,
						bool beaconLight,
						bool auxLight,
						bool hazardLight,
						bool leftIndicatorLight,
						bool rightIndicatorLight,
						bool reverseLight,
						bool brakeLight) {
	if((millis()%1000 >= 500) && (serialIsSent[1] == false)) {
		SerialUSB.println(F("-- Light State --"));
		SerialUSB.print(F("parkLight: "));
		SerialUSB.println(parkLight);
		SerialUSB.print(F("lowBeamLight: "));
		SerialUSB.println(lowBeamLight);
		SerialUSB.print(F("highBeamLight: "));
		SerialUSB.println(highBeamLight);
		SerialUSB.print(F("highBeamLightFlash: "));
		SerialUSB.println(highBeamLightFlash);
		SerialUSB.print(F("fogLight: "));
		SerialUSB.println(fogLight);
		SerialUSB.print(F("beaconLight: "));
		SerialUSB.println(beaconLight);
		SerialUSB.print(F("auxLight: "));
		SerialUSB.println(auxLight);
		SerialUSB.print(F("hazardLight: "));
		SerialUSB.println(hazardLight);
		SerialUSB.print(F("leftIndicatorLight: "));
		SerialUSB.println(leftIndicatorLight);
		SerialUSB.print(F("rightIndicatorLight: "));
		SerialUSB.println(rightIndicatorLight);
		SerialUSB.print(F("reverseLight: "));
		SerialUSB.println(reverseLight);
		SerialUSB.print(F("brakeLight: "));
		SerialUSB.println(brakeLight);
		SerialUSB.println(F("-------End-------"));
		serialIsSent[1] = true;
	} else if((millis()%1000 < 500) && (serialIsSent[1] == true)) {
		serialIsSent[1] = false;
	}
}

void debugFunctionOut(bool parkLight,
						bool lowBeamLight,
						bool highBeamLight,
						bool highBeamLightFlash,
						bool fogLight,
						bool beaconLight,
						bool auxLight,
						bool hazardLight,
						bool leftIndicatorLight,
						bool rightIndicatorLight,
						bool reverseLight,
						bool brakeLight) {
	if((millis()%1000 >= 500) && (serialIsSent[2] == false)) {
		SerialUSB.println(F("-- Light Out --"));
		SerialUSB.print(F("parkLight: "));
		SerialUSB.println(parkLight);
		SerialUSB.print(F("lowBeamLight: "));
		SerialUSB.println(lowBeamLight);
		SerialUSB.print(F("highBeamLight: "));
		SerialUSB.println(highBeamLight);
		SerialUSB.print(F("highBeamLightFlash: "));
		SerialUSB.println(highBeamLightFlash);
		SerialUSB.print(F("fogLight: "));
		SerialUSB.println(fogLight);
		SerialUSB.print(F("beaconLight: "));
		SerialUSB.println(beaconLight);
		SerialUSB.print(F("auxLight: "));
		SerialUSB.println(auxLight);
		SerialUSB.print(F("hazardLight: "));
		SerialUSB.println(hazardLight);
		SerialUSB.print(F("leftIndicatorLight: "));
		SerialUSB.println(leftIndicatorLight);
		SerialUSB.print(F("rightIndicatorLight: "));
		SerialUSB.println(rightIndicatorLight);
		SerialUSB.print(F("reverseLight: "));
		SerialUSB.println(reverseLight);
		SerialUSB.print(F("brakeLight: "));
		SerialUSB.println(brakeLight);
		SerialUSB.println(F("-------End-------"));
		serialIsSent[2] = true;
	} else if((millis()%1000 < 500) && (serialIsSent[2] == true)) {
		serialIsSent[2] = false;
	}
}