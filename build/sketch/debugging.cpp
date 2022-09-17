#line 1 "/home/magraina/projects/truckLightAndFunction/debugging.cpp"
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

void debugChannelEvaluation(uint8_t channel1Poti1,
							uint8_t channel1Poti2,
							uint8_t channel1Switch1,
							uint8_t channel1Switch2,
							uint8_t channel1Switch3,
							uint8_t channel1Switch4,
							uint8_t channel1Switch5,
							uint8_t channel1Switch6,
							uint8_t channel2Switch1,
							uint8_t channel2Switch2,
							uint8_t channel2Switch3,
							uint8_t channel2Switch4,
							uint8_t channel2Switch5,
							uint8_t channel2Switch6) {
	if((millis()%1000 >= 500) && (serialIsSent[0] == false)) {
		SerialUSB.println(F("--Channelswitch 1--"));
		SerialUSB.print(F("poti 0: "));
		SerialUSB.println(channel1Poti1);
		SerialUSB.print(F("poti 1: "));
		SerialUSB.println(channel1Poti2);
		SerialUSB.print(F("lowerSwitch 0: "));
		SerialUSB.println(channel1Switch1);
		SerialUSB.print(F("lowerSwitch 1: "));
		SerialUSB.println(channel1Switch2);
		SerialUSB.print(F("upperSwitch 0: "));
		SerialUSB.println(channel1Switch3);
		SerialUSB.print(F("upperSwitch 1: "));
		SerialUSB.println(channel1Switch4);
		SerialUSB.print(F("upperSwitch 2: "));
		SerialUSB.println(channel1Switch5);
		SerialUSB.print(F("upperSwitch 3: "));
		SerialUSB.println(channel1Switch6);
		SerialUSB.println("--Channelswitch 2--");
		SerialUSB.print(F("lowerSwitch 0: "));
		SerialUSB.println(channel2Switch1);
		SerialUSB.print(F("lowerSwitch 1: "));
		SerialUSB.println(channel2Switch2);
		SerialUSB.print(F("upperSwitch 0: "));
		SerialUSB.println(channel2Switch3);
		SerialUSB.print(F("upperSwitch 1: "));
		SerialUSB.println(channel2Switch4);
		SerialUSB.print(F("upperSwitch 2: "));
		SerialUSB.println(channel2Switch5);
		SerialUSB.print(F("upperSwitch 3: "));
		SerialUSB.println(channel2Switch6);
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
						bool leftFlashLight,
						bool rightFlashLight,
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
		SerialUSB.print(F("leftFlashLight: "));
		SerialUSB.println(leftFlashLight);
		SerialUSB.print(F("rightFlashLight: "));
		SerialUSB.println(rightFlashLight);
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
						bool leftFlashLight,
						bool rightFlashLight,
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
		SerialUSB.print(F("leftFlashLight: "));
		SerialUSB.println(leftFlashLight);
		SerialUSB.print(F("rightFlashLight: "));
		SerialUSB.println(rightFlashLight);
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