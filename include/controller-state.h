/*----------------------------------------------------------------------------
* This file is part of Twitch Controls Chaos (TCC).
* Copyright 2021 blegas78
*
* TCC is free software: you can redistribute it and/or modify it under the
* terms of the GNU General Public License as published by the Free Software
* Foundation, either version 3 of the License, or (at your option) any later
* version.
*
* TCC is distributed in the hope that it will be useful, but WITHOUT ANY
* WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
* FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
* details.
*
* You should have received a copy of the GNU General Public License along
* with TCC.  If not, see <https://www.gnu.org/licenses/>.
*---------------------------------------------------------------------------*/
#ifndef CONTROLLER_STATE_H
#define CONTROLLER_STATE_H

#include <vector>
#include <map>

#include "deviceTypes.h"

namespace Chaos {



// Helper functions for raw interpretation:
short int unpackJoystick(uint8_t& input);
uint8_t packJoystick(short int& input);

short int fixShort(short int input);
short int unfixShort(short int& input);

short int positionDY( const uint8_t& input );
short int  positionDX( const uint8_t& input );
uint8_t packDpad( const short int& dx, const short int& dy );

/*
 This class is responsible for interpreting and tracking stated of of raw-usb controllers
 The first controllers to be implemented will be the Dualsense followed by the DS4.
 
 Due to the unknown
 */
class ControllerState {
public:
	static ControllerState* factory(int vendor, int product);
	
	virtual void getDeviceEvents(unsigned char* buffer, int length, std::vector<DeviceEvent>& events) = 0;
	
	// This has to be virtual since we don't modify all values in a report structure:
//	virtual void applyHackedState(unsigned char* buffer, std::map<int,short>& chaosState) = 0;
	virtual void applyHackedState(unsigned char* buffer, short* chaosState) = 0;
	
	virtual ~ControllerState() = 0;
	
	void* getHackedState();
protected:
	int stateLength;
	
	void* trueState;
	void* hackedState;
	
};

class ControllerStateDualsense : public ControllerState {
friend ControllerState;
private:
	void getDeviceEvents(unsigned char* buffer, int length, std::vector<DeviceEvent>& events);
	
protected:
	ControllerStateDualsense();
	
public:
//	void applyHackedState(unsigned char* buffer, std::map<int,short>& chaosState);
	void applyHackedState(unsigned char* buffer, short* chaosState);
	
	~ControllerStateDualsense();
};



class ControllerStateDualshock : public ControllerState {
friend ControllerState;
private:
	void getDeviceEvents(unsigned char* buffer, int length, std::vector<DeviceEvent>& events);
	
protected:
	ControllerStateDualshock();
	
public:
//	void applyHackedState(unsigned char* buffer, std::map<int,short>& chaosState);
	void applyHackedState(unsigned char* buffer, short* chaosState);
	
	~ControllerStateDualshock();
};

};

#endif
