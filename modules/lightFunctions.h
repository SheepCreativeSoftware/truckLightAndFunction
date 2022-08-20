/************************************ 
 * truckLightAndFunction v0.1.0
 * Date: 2022-08-18T19:48:28Z
 * <Truck Light and function module>
 * Copyright (C) 2020 Marina Egner <info@sheepCreativeStudios.de>
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

#ifndef _LIGHT_FUNCTIONS_H_
#define _LIGHT_FUNCTIONS_H_

#include "ppmToSwitches.h"

#ifndef DIRECTION_UP
	#define DIRECTION_UP 1
#endif
#ifndef DIRECTION_MID
	#define DIRECTION_MID 2
#endif
#ifndef DIRECTION_DOWN
	#define DIRECTION_DOWN 3
#endif


bool parkLightState = 0;
bool lowBeamLightState = 0;
bool highBeamLightState = 0;
bool fogLightState = 0;
bool leftFlashLightState = 0;
bool RightFlashLightState = 0;
bool hazardLightState = 0;
bool auxLightState = 0;

bool setIfCondition(uint8_t state, uint8_t condition, uint8_t comparator);
void setParkLight(uint8_t state, uint8_t condition);
void setLowBeamLight(uint8_t state, uint8_t condition);
void setHighBeamLight(uint8_t state, uint8_t condition);
void setFogLight(uint8_t state, uint8_t condition);
void setLeftFlashLight(uint8_t state, uint8_t condition);
void setRightFlashLight(uint8_t state, uint8_t condition);
void setHazardLight(uint8_t state, uint8_t condition);
void setAux1Light(uint8_t state, uint8_t condition);

void runLightFunctions();

EdgeEvaluation edgeEval[2];


#include "lightFunctions.cpp"
#endif