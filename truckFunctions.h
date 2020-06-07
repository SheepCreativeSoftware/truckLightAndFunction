/************************************ 
 * truckLightAndFunction v0.0.6
 * Date: 03.06.2020 | 22:16
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
#define OUT_HARD_PWM 1
#define OUT_SOFT_PWM 2
#define OUT_SOFT_FADE 3
#define OUT_DIGITAL 4
//Functions
int ppmToSwitchStages(unsigned int);	//function to evaluate the ppm signal of a switch
int ppmServoToRange(int, int, int, int, int);

//Classes
class edgeEvaluation {
		bool lastEdge;
    public:
		bool readEdge(int input);
};


class outputDefine {
		unsigned int outPinModus;
    public:
		
		void outputMode(int outPin, unsigned char modus);
		void outputMode(int outPin, unsigned char modus, int fadeUpTime, int fadeDownTime);		
};
//Modus pwmHW softPWM softFade digital


#endif