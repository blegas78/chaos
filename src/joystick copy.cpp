
#include "joystick.h"

#include <iostream>
#include <cstring>

void functionAxis(int* pins, int value, double calibration) {
	softPwmWrite (pins[0], PWM_RANGE * (calibration*(double)value/65535.0 + 0.5) );
}

void functionButton(int* pins, int value, double calibration) {
	digitalWrite( pins[0], calibration > 0 ? value : !value );
}

void functionDpad(int* pins, int value, double calibration) {
	value *= calibration;
	
	if(value < 0) {
		digitalWrite(pins[0], LOW);
	} else if(value > 0) {
		digitalWrite(pins[1], LOW);
	} else {
		digitalWrite(pins[0], HIGH);
		digitalWrite(pins[1], HIGH);
	}
}

void Device::init(const char* filename, int bytesPerFrame) {
	file = fopen( filename, "r" );

	if( file == NULL ) {
		std::cerr << "Couldn't open " << filename << std::endl;
	}
	
	this->bytesPerFrame = bytesPerFrame;
	this->buffer = new char[bytesPerFrame];
}

Device::~Device() {
	stop();
	WaitForInternalThreadToExit();
	fclose( file );
	
	if( buffer != NULL) {
		delete [] buffer;
	}
}

void Device::addObserver(Dualshock* observer) {
	this->observer = observer;
}

void Device::doAction() {
	//printf("Read: " );
	for (int i = 0; i < bytesPerFrame; i++) {
		buffer[i] = fgetc( file );
		//printf("%d ", (int)buffer[i]);
	}
	//printf("\n");
	
	DeviceEvent event;
	
	interpret( buffer, &event);
	
	if (observer != NULL &&
		event.time != -1) {
		observer->notify(&event);
	}
}

Dualshock::Dualshock() {
	chaosEngine = new ChaosEngine(this);
	chaosEngine->start();
	
	
	joystick.init("/dev/input/js0", 8);
	mouse.init("/dev/input/mouse0", 3);
	
	joystick.addObserver(this);
	mouse.addObserver(this);
	
	joystick.start();
	mouse.start();
	
	
	FunctionInfo info;
	DeviceEvent event;
	
	event.type = TYPE_BUTTON; info.outputType = TYPE_BUTTON; event.id = BUTTON_X;			info.calibration = -1; info.pins[0] =  4; info.function = functionButton; eventToFunction[event] = info;
	event.type = TYPE_BUTTON; info.outputType = TYPE_BUTTON; event.id = BUTTON_CIRCLE;		info.calibration = -1; info.pins[0] =  1; info.function = functionButton; eventToFunction[event] = info;
	event.type = TYPE_BUTTON; info.outputType = TYPE_BUTTON; event.id = BUTTON_TRIANGLE;	info.calibration = -1; info.pins[0] = 16; info.function = functionButton; eventToFunction[event] = info;
	event.type = TYPE_BUTTON; info.outputType = TYPE_BUTTON; event.id = BUTTON_SQUARE;		info.calibration = -1; info.pins[0] = 15; info.function = functionButton; eventToFunction[event] = info;
	event.type = TYPE_BUTTON; info.outputType = TYPE_BUTTON; event.id = BUTTON_L1;			info.calibration =  1; info.pins[0] = 27; info.function = functionButton; eventToFunction[event] = info;
	event.type = TYPE_BUTTON; info.outputType = TYPE_BUTTON; event.id = BUTTON_R1;			info.calibration =  1; info.pins[0] = 26; info.function = functionButton; eventToFunction[event] = info;
	event.type = TYPE_BUTTON; info.outputType = TYPE_BUTTON; event.id = BUTTON_L2;		info.calibration = -1; info.pins[0] =  9; info.function = functionButton; eventToFunction[event] = info;
	event.type = TYPE_BUTTON; info.outputType = TYPE_BUTTON; event.id = BUTTON_R2;		info.calibration = -1; info.pins[0] =  8; info.function = functionButton; eventToFunction[event] = info;
	event.type = TYPE_BUTTON; info.outputType = TYPE_BUTTON; event.id = BUTTON_PS;			info.calibration = -1; info.pins[0] =  5; info.function = functionButton; eventToFunction[event] = info;
	
	// TODO:
//	event.type = TYPE_BUTTON; event.id = BUTTON_SHARE;		info.calibration = 1; info.pins[0] = 4; info.function = functionButton; eventToFunction[event] = info;
//	event.type = TYPE_BUTTON; event.id = BUTTON_OPTIONS;	info.calibration = 1; info.pins[0] = 4; info.function = functionButton; eventToFunction[event] = info;
//	event.type = TYPE_BUTTON; event.id = BUTTON_L3;			info.calibration = 1; info.pins[0] = 4; info.function = functionButton; eventToFunction[event] = info;
//	event.type = TYPE_BUTTON; event.id = BUTTON_R3;			info.calibration = 1; info.pins[0] = 4; info.function = functionButton; eventToFunction[event] = info;
//	event.type = TYPE_BUTTON; event.id = BUTTON_TOUCHPAD;	info.calibration = 1; info.pins[0] = 4; info.function = functionButton; eventToFunction[event] = info;

	event.type = TYPE_AXIS; info.outputType = TYPE_AXIS; event.id = AXIS_LX;	info.calibration =  1; info.pins[0] = 24; info.function = functionAxis; eventToFunction[event] = info;
	event.type = TYPE_AXIS; info.outputType = TYPE_AXIS; event.id = AXIS_LY;	info.calibration =  1; info.pins[0] = 25; info.function = functionAxis; eventToFunction[event] = info;
	//event.type = TYPE_AXIS; info.outputType = TYPE_AXIS; event.id = AXIS_L2;	info.calibration = -1; info.pins[0] =  9; info.function = functionAxis; eventToFunction[event] = info;
	event.type = TYPE_AXIS; info.outputType = TYPE_AXIS; event.id = AXIS_RX;	info.calibration =  1; info.pins[0] = 22; info.function = functionAxis; eventToFunction[event] = info;
	event.type = TYPE_AXIS; info.outputType = TYPE_AXIS; event.id = AXIS_RY;	info.calibration =  1; info.pins[0] = 23; info.function = functionAxis; eventToFunction[event] = info;
	//event.type = TYPE_AXIS; info.outputType = TYPE_AXIS; event.id = AXIS_R2;	info.calibration = -1; info.pins[0] =  8; info.function = functionAxis; eventToFunction[event] = info;
	event.type = TYPE_AXIS; info.outputType = TYPE_BUTTON; event.id = AXIS_DX;	info.calibration =  1; info.pins[0] = 10; info.pins[1] = 31; info.function = functionDpad; eventToFunction[event] = info;
	event.type = TYPE_AXIS; info.outputType = TYPE_BUTTON; event.id = AXIS_DY;	info.calibration = -1; info.pins[0] =  6; info.pins[1] = 11; info.function = functionDpad; eventToFunction[event] = info;

	wiringPiSetup();
	for (std::map<DeviceEvent,FunctionInfo>::iterator it = eventToFunction.begin(); it != eventToFunction.end(); it++) {
		if (it->second.outputType == TYPE_BUTTON) {
			pinMode(it->second.pins[0], OUTPUT);
			digitalWrite(it->second.pins[0], HIGH);
			if (it->second.pins[1] >= 0) {
				pinMode(it->second.pins[1], OUTPUT);
				digitalWrite(it->second.pins[1], HIGH);
			}
		} else if (it->second.outputType == TYPE_AXIS) {
			softPwmCreate(it->second.pins[0], PWM_RANGE/2, PWM_RANGE);
			if (it->second.pins[1] >= 0) {
				softPwmCreate(it->second.pins[1], PWM_RANGE/2, PWM_RANGE);
			}
		}
	}
	
}

Dualshock::~Dualshock() {
	delete chaosEngine;
}

void Dualshock::notify(const DeviceEvent* event) {
	//printf("Time: %.02f\tType: %d\tButton: %d\tValue: %d\n", (double)event->time/1000.0, event->type, event->id, event->value );
	
	if (eventToFunction.count(*event) == 0) {
		return;
	}
	
	// We have an event!  Let's send it right away, but let's let chaos engine play around with the values
	DeviceEvent updatedEvent;
	bool validEvent = chaosEngine->sprinkleSomeChaos(event, &updatedEvent);
	
	// Is our event valid?  If so, send the chaos modified data:
	if (validEvent) {
		applyEvent(&updatedEvent);
	}
}

void Dualshock::applyEvent(const DeviceEvent* event) {
	FunctionInfo& info = eventToFunction[*event];
	info.function(info.pins, event->value, info.calibration);
}


void Joystick::interpret( char* buffer, DeviceEvent* event) {
	memcpy(event, buffer, 8);
}

