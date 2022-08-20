#line 1 "/home/magraina/projects/truckLightAndFunction/modules/lightFunctions.cpp"
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
#include "lightFunctions.h"

bool setIfCondition(uint8_t state, uint8_t condition, uint8_t comparator){
	switch (comparator) {
	case CON_NOT_EQUAL:
		if(state != condition) return true;
	case CON_EQUAL:
		if(state == condition) return true;
	case CON_LOWER:
		if(state < condition) return true;
	case CON_GREATHER:
		if(state > condition) return true;
	case CON_EQUAL_LOWER:
		if(state <= condition) return true;
	case CON_EQUAL_GREATHER:
		if(state >= condition) return true;
	default:
		return false;
	}
}

void setParkLight(uint8_t state, uint8_t condition){
	parkLightState = setIfCondition(state, condition, CON_EQUAL_GREATHER);
	return;
}

void setLowBeamLight(uint8_t state, uint8_t condition){
	lowBeamLightState = setIfCondition(state, condition, CON_EQUAL);
	return;
}

void setHighBeamLight(uint8_t state, uint8_t condition){
	highBeamLightState = setIfCondition(state, condition, CON_EQUAL);
	return;
}

void setFogLight(uint8_t state, uint8_t condition){
	fogLightState = setIfCondition(state, condition, CON_EQUAL);
	return;
}

void setLeftFlashLight(uint8_t state, uint8_t condition){
	leftFlashLightState = setIfCondition(state, condition, CON_EQUAL);
	return;
}


void setRightFlashLight(uint8_t state, uint8_t condition){
	RightFlashLightState = setIfCondition(state, condition, CON_EQUAL);
	return;
}

void setHazardLight(uint8_t state, uint8_t condition){
	hazardLightState = setIfCondition(state, condition, CON_EQUAL);
	return;
}

void setAux1Light(uint8_t state, uint8_t condition){
	auxLightState = setIfCondition(state, condition, CON_EQUAL);
	return;
}

void runLightFunctions() {
	int vartest = 5;
	vartest++;
	bool sometest = (1 <= vartest);
// 	#define DIRECTION_UP 1
// #define DIRECTION_MID 2
// #define DIRECTION_DOWN 3
	// channel1Poti[0] = ppmServoToRange(filter[0].filterValue(interrupt1Buffer[0]));
	// channel1Poti[1] = ppmServoToRange(filter[1].filterValue(interrupt1Buffer[1]));
	// channel1Switch[0] = ppmToSwitchStages(interrupt1Buffer[2]);
	// channel1Switch[1] = ppmToSwitchStages(interrupt1Buffer[3]);
	// channel1Switch[2] = ppmToSwitchStages(interrupt1Buffer[4]);
	// channel1Switch[3] = ppmToSwitchStages(interrupt1Buffer[5]);
	// channel1Switch[4] = ppmToSwitchStages(interrupt1Buffer[6]);
	// channel1Switch[5] = ppmToSwitchStages(interrupt1Buffer[7]);
	
	// channel2Switch[0] = ppmToSwitchStages(interrupt2Buffer[0], PPM_INVERT);
	// channel2Switch[1] = ppmToSwitchStages(interrupt2Buffer[1], PPM_INVERT);
	// channel2Switch[2] = ppmToSwitchStages(interrupt2Buffer[2], PPM_INVERT);
	// channel2Switch[3] = ppmToSwitchStages(interrupt2Buffer[3], PPM_INVERT);
	// channel2Switch[4] = ppm2ToSwitch3Stages(interrupt2Buffer[4], interrupt2Buffer[5]);
	// channel2Switch[5] = ppm2ToSwitch3Stages(interrupt2Buffer[6], interrupt2Buffer[7]);

}