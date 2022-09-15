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

#ifndef _DEBBUGING_H_
#define _DEBBUGING_H_
#include "Arduino.h"
#include "readPPMdata.h"

#ifndef DEBUGLEVEL
#define DEBUGLEVEL 0
#endif

//Setup Serial and check if Board is UNO with one Serial or Leonardo/Micro with to Serials
#ifdef HAVE_HWSERIAL1							//if serial ports 1 exist then the arduino has more than one serial port
	#ifndef SerialUSB								//if not allready defined
		#define SerialUSB SERIAL_PORT_MONITOR		//then define monitor port
	#endif
#else
	#if (DEBUGLEVEL >1)								//if serial ports are the same debuging is not possible (for example on UNO)
		#define DEBUGLEVEL 1						//do not change!!!
	#endif
#endif



//Vars for debug
#if (DEBUGLEVEL >=1)
	bool serialIsSent = false;
	bool serialIsSent2 = false;
#endif

void debuggingInit();
bool controllerStatus(bool);			//function to signal errorstate
void debugInterrupt();					//function to debug the interrupt data
void debugChannelEvaluation();
void runDiagnostic();

#endif