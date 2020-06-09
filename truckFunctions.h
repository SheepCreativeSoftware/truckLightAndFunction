/************************************ 
 * truckLightAndFunction v0.0.10
 * Date: 09.06.2020 | 19:27
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
uint16_t ppmServoToRange(int16_t signal, int16_t inMin = 1000, int16_t inMax = 2000, int16_t outMin = 0, int16_t outMax = 1023);

//Classes
class EdgeEvaluation {
		bool lastEdge;
    public:
		bool readEdge(bool input);
};

class Filter {
		uint16_t lastValue;
		bool doneFilter;
    public:
		uint16_t filterValue(uint16_t input, uint16_t filterFactor = 10, uint16_t filterTime = 1000);
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