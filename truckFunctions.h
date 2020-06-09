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
 
#ifndef _TRUCKFUNCTIONS_H_
#define _TRUCKFUNCTIONS_H_
//Definition
#define DIRECTION_UP 1
#define DIRECTION_MID 2
#define DIRECTION_DOWN 3
#define PPM_INVERT 1

//Functions
uint8_t ppmToSwitchStages(uint16_t signal, bool invertDirection = 0);	//function to evaluate the ppm signal of a switch
uint8_t ppm2ToSwitch3Stages(uint16_t signal1, uint16_t signal2);
int32_t ppmServoToRange(int32_t signal, int32_t inMin = 1060, int32_t inMax = 1980, int32_t outMin = 0, int32_t outMax = 1023);

//Classes
class EdgeEvaluation {
		bool lastEdge;
    public:
		bool readEdge(bool input);
};

class Filter {
		int16_t lastValue;
		bool doneFilter;
    public:
		int16_t filterValue(int16_t input, int16_t filterFactor = 20, uint16_t filterTime = 100);
};
/*
class outputDefine {
		unsigned int outPinModus;
    public:
		
		void outputMode(int outPin, unsigned char modus);
		void outputMode(int outPin, unsigned char modus, int fadeUpTime, int fadeDownTime);		
};
//Modus pwmHW softPWM softFade digital
*/

#endif