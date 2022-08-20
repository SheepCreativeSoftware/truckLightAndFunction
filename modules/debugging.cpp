/************************************ 
 * truckLightAndFunction v0.0.11
 * Date: 10.06.2020 | 00:25
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

#if (debugLevel >=1)
bool controllerStatus(bool errorFlag) {
	if(errorFlag) {
		return true;
	} else {
		uint32_t currentMillis = millis();
    if (currentMillis - StatusPreviousMillis >= 1000) { 	//Zeitverzoegerte Abfrage
		StatusPreviousMillis = currentMillis;
		pulseStatus = !pulseStatus;
    } else if (currentMillis < StatusPreviousMillis) {  	//Reset
		StatusPreviousMillis = currentMillis; 
    }
    return pulseStatus;                 					//Flash if everything is OK
	}
}
#endif

#if (debugLevel >=6)
void debugInterrupt() {
	if((millis()%1000 >= 500) && (serialIsSent == false)) {
		SerialUSB.println("--Multiswitch 1--");
		SerialUSB.println(int1Value[0]);
		SerialUSB.println(int1Value[1]);
		SerialUSB.println(int1Value[2]);
		SerialUSB.println(int1Value[3]);
		SerialUSB.println(int1Value[4]);
		SerialUSB.println(int1Value[5]);
		SerialUSB.println(int1Value[6]);
		SerialUSB.println(int1Value[7]);
		SerialUSB.println("-------End-------");
		SerialUSB.println("--Multiswitch 2--");
		SerialUSB.println(int2Value[0]);
		SerialUSB.println(int2Value[1]);
		SerialUSB.println(int2Value[2]);
		SerialUSB.println(int2Value[3]);
		SerialUSB.println(int2Value[4]);
		SerialUSB.println(int2Value[5]);
		SerialUSB.println(int2Value[6]);
		SerialUSB.println(int2Value[7]);
		SerialUSB.println("-------End-------");
		serialIsSent = true;
	} else if((millis()%1000 < 500) && (serialIsSent == true)) {
		serialIsSent = false;
	}
}
#endif
#if (debugLevel >=3)
void debugChannelEvaluation() {
	if((millis()%1000 >= 500) && (serialIsSent2 == false)) {
		SerialUSB.println("--Multiswitch 1--");
		SerialUSB.println(channel1Poti[0]);
		SerialUSB.println(channel1Poti[1]);
		SerialUSB.println(channel1Switch[0]);
		SerialUSB.println(channel1Switch[1]);
		SerialUSB.println(channel1Switch[2]);
		SerialUSB.println(channel1Switch[3]);
		SerialUSB.println(channel1Switch[4]);
		SerialUSB.println(channel1Switch[5]);
		SerialUSB.println("-------End-------");
		SerialUSB.println("--Multiswitch 2--");
		SerialUSB.println(channel2Switch[0]);
		SerialUSB.println(channel2Switch[1]);
		SerialUSB.println(channel2Switch[2]);
		SerialUSB.println(channel2Switch[3]);
		SerialUSB.println(channel2Switch[4]);
		SerialUSB.println(channel2Switch[5]);
		SerialUSB.println("-------End-------");
		serialIsSent2 = true;
	} else if((millis()%1000 < 500) && (serialIsSent2 == true)) {
		serialIsSent2 = false;
	}
}
	#endif