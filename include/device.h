#ifndef DEVICE_H
#define DEVICE_H

#include <cstdio>		// for FILE
#include <mogi/thread.h>

#include "deviceTypes.h"


// Classes

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

#endif
