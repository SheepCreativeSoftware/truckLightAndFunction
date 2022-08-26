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

#include "tools.h" // Get blink class from

bool parkLightOut = 0;
bool lowBeamLightOut = 0;
bool highBeamLightOut = 0;
bool highBeamLightFlashOut = 0;
bool fogLightOut = 0;
bool leftFlashLightOut = 0;
bool rightFlashLightOut = 0;
bool hazardLightOut = 0;
bool beaconLightOut = 0;
bool auxLightOut = 0;

void runLightFunctions();

Blink flasher[2];

#include "lightFunctions.cpp"
#endif