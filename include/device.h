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
#ifndef DEVICE_H
#define DEVICE_H

#include <cstdio>		// for FILE
#include <mogi/thread.h>

#include "deviceTypes.h"


// Classes

namespace Chaos {



class DeviceObserver {
public:
	virtual void newDeviceEvent( const DeviceEvent* event ) = 0;
};


// Handles the input device through file reading (from /dev/input/* )
class Device : public Mogi::Thread {
private:
	FILE* file = NULL;
	
	DeviceObserver* observer = NULL;
	
	int bytesPerFrame = 8;
	char* buffer = NULL;
	
	void doAction();	//override
	
protected:
	void init(const char* filename, int bytesPerFrame);
	
public:
	virtual ~Device() = 0;
	
	
	void addObserver(DeviceObserver* observer);
	
	virtual void interpret( char* buffer, DeviceEvent* event) = 0;
	
};

class DeviceRaw : public Device {
	
};

class DeviceHID : public Device {	// This implements devices found in /dev/input/
	
};


class Joystick : public DeviceHID {
private:
	void interpret( char* buffer, DeviceEvent* event);	//override
	
public:
	~Joystick() {};
	void open(const char* filename);
};


class Mouse : public DeviceHID {
private:
	void interpret( char* buffer, DeviceEvent* event);	//override
	
public:
	~Mouse() {};
	void open(const char* filename);
};

};

#endif
