#ifndef DUALSCHOCK_H
#define DUALSCHOCK_H

#include <map>	// mapping Events to Functions
#include <cstdio>
#include <deque>
#include <array>

#include "device.h"	// Joystick, Mouse
#include "raw-gadget.hpp"
#include "controller-state.h"

#define PWM_RANGE (11)

#ifdef __APPLE__
	// Dummy functions just so I can compile crap on a non-pi
	void wiringPiSetup() {}
	void digitalWrite(int,int) {};
	void pinMode(int, int) {};
	void softPwmCreate(int,int,int) {};
	void softPwmWrite (int,int) {};
	#define HIGH (1)
	#define LOW (0)
	#define OUTPUT (1)
#else
//	#include <wiringPi.h>
//	#include <softPwm.h>
#endif


// Functions that take an input and aply it tot eh raspberry pi pins
void functionAxis(int* pins, int value, double calibration);
void functionButton(int* pins, int value, double calibration);
void functionDpad(int* pins, int value, double calibration);

// Holds data for function calling for DeviceEvents
//typedef struct FunctionInfo {
//	void (*function)(int*, int, double) = NULL;
//	int pins[2] = {-1,-1};
//	double calibration = 1.0;
//	unsigned char outputType;
//} FunctionInfo;

typedef struct FunctionInfo {
	void (*function)(int*, int, double);
	int pins[2];
	double calibration;
	unsigned char outputType;
} FunctionInfo;


class ControllerInjector  {
public:
	// sniff the input and pass/modify it on way to the output
	virtual bool sniffify(const DeviceEvent* input, DeviceEvent* output) = 0;
};




class Controller {
protected:
	std::deque<DeviceEvent> deviceEventQueue;
	//virtual void writeToSpoofFile( std::map<int,short>& controllerState ) = 0;
	
	virtual bool applyHardware(const DeviceEvent* event) = 0;
	
	void storeState(const DeviceEvent* event);
	
	void handleNewDeviceEvent(const DeviceEvent* event);
	
	// Database for tracking current button states:
	// Input: ((int)event->type<<8) + (int)event->id ] = event->value;
	// Output: state of that type/id combo
	//std::map<int,short> controllerState;
	short controllerState[1024];
	
	ControllerInjector* dualShockInjector = NULL;
	
	
	
	
public:
	Controller();
	int getState(int id, ButtonType type);
	
	void applyEvent(const DeviceEvent* event);
	void addInjector(ControllerInjector* injector);
	
};





class ControllerGpio : public Controller,  public DeviceObserver {
	
private:
//	FILE* spooferFile = NULL;
	char    *buf;
	//int spooferFd = 0;
	//fd_set rfds;
	//void writeToSpoofFile( std::map<int,short>& controllerState );
	
	bool applyHardware(const DeviceEvent* event);
	
	// Give me an event, and I'll tell you what to do with it:
	std::map<DeviceEvent, FunctionInfo> eventToFunction;
	
	
	
	Joystick joystick;
	Mouse mouse;
	
	
	
protected:
	void newDeviceEvent(const DeviceEvent*);	// Override from DeviceObserver
	
public:
	ControllerGpio();
	~ControllerGpio();
	
	void initialize( const char* joystickFileName );
//	void setSpoofFile( const char* filename );
	
//	void applyEvent(const DeviceEvent* event);
//	void addInjector(ControllerInjector* injector);
	
	
	
};


class ControllerRaw : public Controller, public EndpointObserver, public Mogi::Thread {
private:
	ControllerState* mControllerState;
	//unsigned char copiedBuffer[64];	// based on DS4 and Dualsense sizes
	std::deque<std::array<unsigned char,64>> bufferQueue;
	
	bool applyHardware(const DeviceEvent* event);
	
	// Main purpose of this Mogi::Thread is to handle DeviceEvent queue
	void doAction();
	
	void notification(unsigned char* buffer, int length); // overloaded from EndpointObserver

public:
	RawGadgetPassthrough mRawGadgetPassthrough;
	
	void initialize( );
	
};




#endif
