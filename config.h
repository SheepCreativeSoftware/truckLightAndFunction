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

#ifndef _CONFIG_H_
#define _CONFIG_H_

#define COUNTRY_OPTION "EU"					//Setup Region EU or US for Truck
#define HEADLIGHT_COMBINE false				//High => Low and High Beam on both Head Light output Pins | False => Seperate Pins for High and Low Beam 
#define SERIAL_COM false					//Activate Communication to other modules via Serial Port
#if (SERIAL_COM == true)
	#define TRUCK_ADDRESS 1					//Serial Adress for this Module: Truck
	#define BEACON_ADDRESS 2				//Serial Adress for Module: Beacon Lights Extension
	#define TRAILER_ADDRESS 3				//Serial Adress for Module: Trailer 
	#define EXT_FUNC_ADDRESS 4				//Serial Adress for Module: Special function for example Servos Steering
#endif
//Change this value for different debuging levels
#define DEBUGLEVEL 3						//1 = Status LED | >2 = Serial Monitor

#endif