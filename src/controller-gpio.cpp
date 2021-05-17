
#include "controller.h"

#include <iostream>
#include <cstring>
#include <unistd.h>

#include <fcntl.h>

using namespace Chaos;

void Chaos::functionAxis(int* pins, int value, double calibration) {
	//softPwmWrite (pins[0], PWM_RANGE * (calibration*(double)value/65535.0 + 0.5) ); // deprecated
}

void Chaos::functionButton(int* pins, int value, double calibration) {
	//digitalWrite( pins[0], calibration > 0 ? value : !value ); // deprecated
}

void Chaos::functionDpad(int* pins, int value, double calibration) {
	value *= calibration;
	
	/*	// deprecated
	if(value < 0) {
		digitalWrite(pins[1], HIGH);
		digitalWrite(pins[0], LOW);
	} else if(value > 0) {
		digitalWrite(pins[0], HIGH);
		digitalWrite(pins[1], LOW);
	} else {
		digitalWrite(pins[0], HIGH);
		digitalWrite(pins[1], HIGH);
	}
	 */
}
/*
typedef struct
{
  uint8_t  reportId = 0x01;                                 // Report ID = 0x01 (1)
													 // Collection: CA:GamePad
  uint8_t  GD_GamePadX;                              // Usage 0x00010030: X, Value = 0 to 255
  uint8_t  GD_GamePadY;                              // Usage 0x00010031: Y, Value = 0 to 255
  uint8_t  GD_GamePadZ;                              // Usage 0x00010032: Z, Value = 0 to 255
  uint8_t  GD_GamePadRz;                             // Usage 0x00010035: Rz, Value = 0 to 255
  uint8_t  GD_GamePadRx;                             // Usage 0x00010033: Rx, Value = 0 to 255
  uint8_t  GD_GamePadRy;                             // Usage 0x00010034: Ry, Value = 0 to 255
  uint8_t  VEN_GamePad0020;                          // Usage 0xFF000020: , Value = 0 to 255
  uint8_t  GD_GamePadHatSwitch : 4;                  // Usage 0x00010039: Hat switch, Value = 0 to 7, Physical = Value x 45 in degrees
  uint8_t  BTN_GamePadButton1 : 1;                   // Usage 0x00090001: Button 1 Primary/trigger, Value = 0 to 1, Physical = Value x 315
  uint8_t  BTN_GamePadButton2 : 1;                   // Usage 0x00090002: Button 2 Secondary, Value = 0 to 1, Physical = Value x 315
  uint8_t  BTN_GamePadButton3 : 1;                   // Usage 0x00090003: Button 3 Tertiary, Value = 0 to 1, Physical = Value x 315
  uint8_t  BTN_GamePadButton4 : 1;                   // Usage 0x00090004: Button 4, Value = 0 to 1, Physical = Value x 315
  uint8_t  BTN_GamePadButton5 : 1;                   // Usage 0x00090005: Button 5, Value = 0 to 1, Physical = Value x 315
  uint8_t  BTN_GamePadButton6 : 1;                   // Usage 0x00090006: Button 6, Value = 0 to 1, Physical = Value x 315
  uint8_t  BTN_GamePadButton7 : 1;                   // Usage 0x00090007: Button 7, Value = 0 to 1, Physical = Value x 315
  uint8_t  BTN_GamePadButton8 : 1;                   // Usage 0x00090008: Button 8, Value = 0 to 1, Physical = Value x 315
  uint8_t  BTN_GamePadButton9 : 1;                   // Usage 0x00090009: Button 9, Value = 0 to 1, Physical = Value x 315
  uint8_t  BTN_GamePadButton10 : 1;                  // Usage 0x0009000A: Button 10, Value = 0 to 1, Physical = Value x 315
  uint8_t  BTN_GamePadButton11 : 1;                  // Usage 0x0009000B: Button 11, Value = 0 to 1, Physical = Value x 315
  uint8_t  BTN_GamePadButton12 : 1;                  // Usage 0x0009000C: Button 12, Value = 0 to 1, Physical = Value x 315
  uint8_t  BTN_GamePadButton13 : 1;                  // Usage 0x0009000D: Button 13, Value = 0 to 1, Physical = Value x 315
  uint8_t  BTN_GamePadButton14 : 1;                  // Usage 0x0009000E: Button 14, Value = 0 to 1, Physical = Value x 315
  uint8_t  BTN_GamePadButton15 : 1;                  // Usage 0x0009000F: Button 15, Value = 0 to 1, Physical = Value x 315
  uint8_t  VEN_GamePad0021 : 1;                      // Usage 0xFF000021: , Value = 0 to 1, Physical = Value x 315
  uint8_t  VEN_GamePad00211 : 1;                     // Usage 0xFF000021: , Value = 0 to 1, Physical = Value x 315
  uint8_t  VEN_GamePad00212 : 1;                     // Usage 0xFF000021: , Value = 0 to 1, Physical = Value x 315
  uint8_t  VEN_GamePad00213 : 1;                     // Usage 0xFF000021: , Value = 0 to 1, Physical = Value x 315
  uint8_t  VEN_GamePad00214 : 1;                     // Usage 0xFF000021: , Value = 0 to 1, Physical = Value x 315
  uint8_t  VEN_GamePad00215 : 1;                     // Usage 0xFF000021: , Value = 0 to 1, Physical = Value x 315
  uint8_t  VEN_GamePad00216 : 1;                     // Usage 0xFF000021: , Value = 0 to 1, Physical = Value x 315
  uint8_t  VEN_GamePad00217 : 1;                     // Usage 0xFF000021: , Value = 0 to 1, Physical = Value x 315
  uint8_t  VEN_GamePad00218 : 1;                     // Usage 0xFF000021: , Value = 0 to 1, Physical = Value x 315
  uint8_t  VEN_GamePad00219 : 1;                     // Usage 0xFF000021: , Value = 0 to 1, Physical = Value x 315
  uint8_t  VEN_GamePad002110 : 1;                    // Usage 0xFF000021: , Value = 0 to 1, Physical = Value x 315
  uint8_t  VEN_GamePad002111 : 1;                    // Usage 0xFF000021: , Value = 0 to 1, Physical = Value x 315
  uint8_t  VEN_GamePad002112 : 1;                    // Usage 0xFF000021: , Value = 0 to 1, Physical = Value x 315
  uint8_t  VEN_GamePad0022[52];                      // Usage 0xFF000022: , Value = 0 to 255, Physical = Value x 21 / 17
} inputReport01_t;
*/
//--------------------------------------------------------------------------------
// Generic Desktop Page inputReport 01 (Device --> Host)
//--------------------------------------------------------------------------------

//typedef struct
//{
//  uint8_t  reportId = 0x01;                                 // Report ID = 0x01 (1)
//													 // Collection: CA:GamePad
//  uint8_t  GD_GamePadX;                              // Usage 0x00010030: X, Value = 0 to 255
//  uint8_t  GD_GamePadY;                              // Usage 0x00010031: Y, Value = 0 to 255
//  uint8_t  GD_GamePadZ;                              // Usage 0x00010032: Z, Value = 0 to 255
//  uint8_t  GD_GamePadRz;                             // Usage 0x00010035: Rz, Value = 0 to 255
//  uint8_t  GD_GamePadHatSwitch : 4;                  // Usage 0x00010039: Hat switch, Value = 0 to 7, Physical = Value x 45 in degrees
//  uint8_t  BTN_GamePadButton1 : 1;                   // Usage 0x00090001: Button 1 Primary/trigger, Value = 0 to 1, Physical = Value x 315
//  uint8_t  BTN_GamePadButton2 : 1;                   // Usage 0x00090002: Button 2 Secondary, Value = 0 to 1, Physical = Value x 315
//  uint8_t  BTN_GamePadButton3 : 1;                   // Usage 0x00090003: Button 3 Tertiary, Value = 0 to 1, Physical = Value x 315
//  uint8_t  BTN_GamePadButton4 : 1;                   // Usage 0x00090004: Button 4, Value = 0 to 1, Physical = Value x 315
//  uint8_t  BTN_GamePadButton5 : 1;                   // Usage 0x00090005: Button 5, Value = 0 to 1, Physical = Value x 315
//  uint8_t  BTN_GamePadButton6 : 1;                   // Usage 0x00090006: Button 6, Value = 0 to 1, Physical = Value x 315
//  uint8_t  BTN_GamePadButton7 : 1;                   // Usage 0x00090007: Button 7, Value = 0 to 1, Physical = Value x 315
//  uint8_t  BTN_GamePadButton8 : 1;                   // Usage 0x00090008: Button 8, Value = 0 to 1, Physical = Value x 315
//  uint8_t  BTN_GamePadButton9 : 1;                   // Usage 0x00090009: Button 9, Value = 0 to 1, Physical = Value x 315
//  uint8_t  BTN_GamePadButton10 : 1;                  // Usage 0x0009000A: Button 10, Value = 0 to 1, Physical = Value x 315
//  uint8_t  BTN_GamePadButton11 : 1;                  // Usage 0x0009000B: Button 11, Value = 0 to 1, Physical = Value x 315
//  uint8_t  BTN_GamePadButton12 : 1;                  // Usage 0x0009000C: Button 12, Value = 0 to 1, Physical = Value x 315
//  uint8_t  BTN_GamePadButton13 : 1;                  // Usage 0x0009000D: Button 13, Value = 0 to 1, Physical = Value x 315
//  uint8_t  BTN_GamePadButton14 : 1;                  // Usage 0x0009000E: Button 14, Value = 0 to 1, Physical = Value x 315
//  uint8_t  VEN_GamePad0020 : 6;                      // Usage 0xFF000020: , Value = 0 to 127, Physical = Value x 315 / 127
//  uint8_t  GD_GamePadRx;                             // Usage 0x00010033: Rx, Value = 0 to 255, Physical = Value x 21 / 17
//  uint8_t  GD_GamePadRy;                             // Usage 0x00010034: Ry, Value = 0 to 255, Physical = Value x 21 / 17
//  uint8_t  VEN_GamePad0021[54];                      // Usage 0xFF000021: , Value = 0 to 255, Physical = Value x 21 / 17
//} inputReport01_t;

typedef struct SpoofState {
	//unsigned short buttons = 0;
	uint8_t  BTN_GamePadButton1 : 1;                   // Usage 0x00090001: Button 1 Primary/trigger, Value = 0 to 1, Physical = Value x 315
	uint8_t  BTN_GamePadButton2 : 1;                   // Usage 0x00090002: Button 2 Secondary, Value = 0 to 1, Physical = Value x 315
	uint8_t  BTN_GamePadButton3 : 1;                   // Usage 0x00090003: Button 3 Tertiary, Value = 0 to 1, Physical = Value x 315
	uint8_t  BTN_GamePadButton4 : 1;                   // Usage 0x00090004: Button 4, Value = 0 to 1, Physical = Value x 315
	uint8_t  BTN_GamePadButton5 : 1;                   // Usage 0x00090005: Button 5, Value = 0 to 1, Physical = Value x 315
	uint8_t  BTN_GamePadButton6 : 1;                   // Usage 0x00090006: Button 6, Value = 0 to 1, Physical = Value x 315
	uint8_t  BTN_GamePadButton7 : 1;                   // Usage 0x00090007: Button 7, Value = 0 to 1, Physical = Value x 315
	uint8_t  BTN_GamePadButton8 : 1;                   // Usage 0x00090008: Button 8, Value = 0 to 1, Physical = Value x 315
	uint8_t  BTN_GamePadButton9 : 1;                   // Usage 0x00090009: Button 9, Value = 0 to 1, Physical = Value x 315
	uint8_t  BTN_GamePadButton10 : 1;                  // Usage 0x0009000A: Button 10, Value = 0 to 1, Physical = Value x 315
	uint8_t  BTN_GamePadButton11 : 1;                  // Usage 0x0009000B: Button 11, Value = 0 to 1, Physical = Value x 315
	uint8_t  BTN_GamePadButton12 : 1;                  // Usage 0x0009000C: Button 12, Value = 0 to 1, Physical = Value x 315
	uint8_t  BTN_GamePadButton13 : 1;                  // Usage 0x0009000D: Button 13, Value = 0 to 1, Physical = Value x 315
	uint8_t  BTN_GamePadButton14 : 1;                  // Usage 0x0009000E: Button 14, Value = 0 to 1, Physical = Value x 315
	uint8_t  BTN_GamePadButton15 : 1;	// dummy
	uint8_t  BTN_GamePadButton16 : 1;	// dummy
//	unsigned char dpad = 0;
	uint8_t  GD_GamePadHatSwitch;
	uint8_t  GD_GamePadX;
	uint8_t  GD_GamePadY;
	uint8_t  GD_GamePadZ;
	uint8_t  GD_GamePadRz;
	unsigned char setMeToZero = 0;
} inputReport01_t;

// \x05\x03\x00\x00\x00\x00\xff\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00 red
// \x05\x03\x00\x00\x00\x00\x00\xff\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00 green
// \x05\x03\x00\x00\x00\x00\x00\x00\xff\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00 blue

//void ControllerGpio::writeToSpoofFile( std::map<int,short>& controllerState) {
////	if (spooferFile == NULL) {
////		return;
////	}
////	if (spooferFile >= 0) {
////		return;
////	}
//
//	static inputReport01_t state;
//	memset(&state, 0, sizeof(state));
//	//state.reportId = 0x01;
//
//
//	int id = 0;
//	int type = ((int)TYPE_BUTTON<<8);
////	for ( id = 0; id < 14; id++) {
////		if (controllerState[ type + (int)id ]) {
////			state.buttons |= 1 << id;
////		}
////	}
//
//	state.BTN_GamePadButton1 = controllerState[ type + 0 ];
//	state.BTN_GamePadButton2 = controllerState[ type + 1 ];
//	state.BTN_GamePadButton3 = controllerState[ type + 2 ];
//	state.BTN_GamePadButton4 = controllerState[ type + 3 ];
//	state.BTN_GamePadButton5 = controllerState[ type + 4 ];
//	state.BTN_GamePadButton6 = controllerState[ type + 5 ];
//	state.BTN_GamePadButton7 = controllerState[ type + 6 ];
//	state.BTN_GamePadButton8 = controllerState[ type + 7 ];
//	state.BTN_GamePadButton9 = controllerState[ type + 8 ];
//	state.BTN_GamePadButton10 = controllerState[ type + 9 ];
//	state.BTN_GamePadButton11 = controllerState[ type + 10 ];
//	state.BTN_GamePadButton12 = controllerState[ type + 11 ];
//	state.BTN_GamePadButton13 = controllerState[ type + 12 ];
//	state.BTN_GamePadButton14 = controllerState[ type + 13 ];
//
//	type = ((int)TYPE_AXIS<<8);
//
//	state.GD_GamePadX = (controllerState[ type + AXIS_LX ] + 32768) / 256;
//	state.GD_GamePadY = (controllerState[ type + AXIS_LY ] + 32768) / 256;
//	state.GD_GamePadZ = (controllerState[ type + AXIS_RX ] + 32768) / 256;
//	state.GD_GamePadRz = (controllerState[ type + AXIS_RY ] + 32768) / 256;
//
//	int dPadX = controllerState[ type + AXIS_DX ];
//	int dPadY = controllerState[ type + AXIS_DY ];
//
//	if ( dPadX == 0 ) {
//		if (dPadY == 0) {
//			state.GD_GamePadHatSwitch = 0xf;
//		} else if ( dPadY < 0 ) {
//			state.GD_GamePadHatSwitch = 0;
//		} else {
//			state.GD_GamePadHatSwitch = 4;
//		}
//	} else if ( dPadX < 0) {
//		if (dPadY == 0) {
//			state.GD_GamePadHatSwitch = 6;
//		} else if ( dPadY < 0 ) {
//			state.GD_GamePadHatSwitch = 7;
//		} else {
//			state.GD_GamePadHatSwitch = 5;
//		}
//	} else {
//		if (dPadY == 0) {
//			state.GD_GamePadHatSwitch = 2;
//		} else if ( dPadY < 0 ) {
//			state.GD_GamePadHatSwitch = 1;
//		} else {
//			state.GD_GamePadHatSwitch = 3;
//		}
//
//	}
//
//
//
//
////	std::cout << "sizeof(state) = " << (int)sizeof(state) << std::endl;
//	//fwrite( &state, 1, sizeof(state), file);
////	fwrite( &state, sizeof(state), 1, spooferFile );
//	//fflush( file );	// send immediately
//
//	//if (FD_ISSET(STDIN_FILENO, &rfds)) {
//
////		std::cout << "trying..." << std::endl;
////		if (write(spooferFd, &state, sizeof(state)) != sizeof(state)) {
////						perror("Oh no...");
////						//return 5;
////		} else {
////			std::cout << "wrote all data" << std::endl;
////		}
//	//}
//
////	unsigned char* ptr = (unsigned char*) &state;
////	printf("Sent: ");
////	for (int i = 0; i < (int)sizeof(state); i++) {
////		printf("%02x ", ptr[i]);
////	}
////	printf("\n");
//}




ControllerGpio::ControllerGpio() {
	
}

void ControllerGpio::initialize(const char* joystickFileName ) {
	//chaosEngine = new ChaosEngine(this);
	//chaosEngine->start();
	
//	if (spooferFilename != NULL) {
////		this->spooferFile = fopen( spooferFilename, "w" );
////		if( spooferFile == NULL ) {
////			std::cerr << "Couldn't open speefer: " << spooferFilename << std::endl;
////		}
//		//spooferFd
//		if ((spooferFd = open(spooferFilename, O_RDWR | O_NONBLOCK)) == -1) {
//				//perror(filename);
//				//return 3;
//			std::cerr << "Couldn't open speefer: " << spooferFilename << std::endl;
//			}
//
//	//	FD_ZERO(&rfds);
//		//FD_SET(STDIN_FILENO, &rfds);
//		//FD_SET(spooferFd, &rfds);
//		//int retval = select(spooferFd + 1, &rfds, NULL, NULL, NULL);
////		if (retval == -1 && errno == EINTR)
////			continue;
////		if (retval < 0) {
////			perror("select()");
////			return 4;
////		}
//
//
////		buf = (char*)malloc(sizeof(inputReport01_t));
////		setvbuf(spooferFile, buf, _IOFBF, sizeof(inputReport01_t));
//	}
	
	
	
	joystick.open(joystickFileName);
	mouse.open("/dev/input/mouse0");
	
	joystick.addObserver(this);
	mouse.addObserver(this);
	
	joystick.start();
	mouse.start();
	
	/*
	 Pins on the Ds4 pressure connector:
	 GND
	 square		15
	 R2			16
	 ?
	 R1			1
	 ?
	 Triangle	4
	 Circle		5
	 X			6
	 PS			10
	 ?
	 Dpad Y 2	11
	 Dpad X 1	31
	 Dpad Y 1	26
	 Dpad X 2	27
	 L2			28
	 ?
	 L1			29
	 ?
	 
	 R3 yellow		21
	 L3 blue		14
	 Options green	12
	 Touchpad white 3
	 Share orange	2
	 */
	
		// Pin resource for wiringPi pins: https://pinout.xyz/pinout/wiringpi
		
		eventToFunction[{0, 0, TYPE_BUTTON, BUTTON_SQUARE}]		= {functionButton, {15,-1}, -1.0, TYPE_BUTTON};
		eventToFunction[{0, 0, TYPE_BUTTON, BUTTON_R2}]			= {functionButton, {16,-1}, -1.0, TYPE_BUTTON};
		
		eventToFunction[{0, 0, TYPE_BUTTON, BUTTON_R1}]			= {functionButton, { 1,-1},  1.0, TYPE_BUTTON};
		
		eventToFunction[{0, 0, TYPE_BUTTON, BUTTON_TRIANGLE}]	= {functionButton, { 4,-1}, -1.0, TYPE_BUTTON};
		eventToFunction[{0, 0, TYPE_BUTTON, BUTTON_CIRCLE}]		= {functionButton, { 5,-1}, -1.0, TYPE_BUTTON};
		eventToFunction[{0, 0, TYPE_BUTTON, BUTTON_X}]			= {functionButton, { 6,-1}, -1.0, TYPE_BUTTON};
		eventToFunction[{0, 0, TYPE_BUTTON, BUTTON_PS}]			= {functionButton, {10,-1}, -1.0, TYPE_BUTTON};
		
		eventToFunction[{0, 0, TYPE_AXIS, AXIS_DX}]				= {functionDpad, {31,27},  1.0, TYPE_BUTTON};	// 2 pins
		eventToFunction[{0, 0, TYPE_AXIS, AXIS_DY}]				= {functionDpad, {26,11},  1.0, TYPE_BUTTON};	// 2 pins
		
		eventToFunction[{0, 0, TYPE_BUTTON, BUTTON_L2}]			= {functionButton, {28,-1}, -1.0, TYPE_BUTTON};
		eventToFunction[{0, 0, TYPE_BUTTON, BUTTON_L1}]			= {functionButton, {29,-1},  1.0, TYPE_BUTTON};
		
		// TODO:
		eventToFunction[{0, 0, TYPE_BUTTON, BUTTON_R3}]			= {functionButton, {21,-1},  1.0, TYPE_BUTTON};
		eventToFunction[{0, 0, TYPE_BUTTON, BUTTON_L3}]			= {functionButton, {14,-1},  1.0, TYPE_BUTTON};
		eventToFunction[{0, 0, TYPE_BUTTON, BUTTON_OPTIONS}]	= {functionButton, {12,-1}, -1.0, TYPE_BUTTON};
		eventToFunction[{0, 0, TYPE_BUTTON, BUTTON_TOUCHPAD}]	= {functionButton, { 3,-1}, -1.0, TYPE_BUTTON};
		eventToFunction[{0, 0, TYPE_BUTTON, BUTTON_SHARE}]		= {functionButton, { 2,-1}, -1.0, TYPE_BUTTON};
		
		eventToFunction[{0, 0, TYPE_AXIS, AXIS_RX}]				= {functionAxis, {22,-1},  1.0, TYPE_AXIS};
		eventToFunction[{0, 0, TYPE_AXIS, AXIS_RY}]				= {functionAxis, {23,-1},  1.0, TYPE_AXIS};
		eventToFunction[{0, 0, TYPE_AXIS, AXIS_LX}]				= {functionAxis, {24,-1},  1.0, TYPE_AXIS};
		eventToFunction[{0, 0, TYPE_AXIS, AXIS_LY}]				= {functionAxis, {25,-1},  1.0, TYPE_AXIS};
		//eventToFunction[{0, 0, TYPE_AXIS, AXIS_L2}]			= {functionAxis, { 9,-1},  1.0, TYPE_AXIS};	// we are using digital interfaces, not analog
		//eventToFunction[{0, 0, TYPE_AXIS, AXIS_R2}]			= {functionAxis, { 8,-1},  1.0, TYPE_AXIS};
		
//	if( spooferFd < 0 ) {
		//wiringPiSetup();	// deprecated
		for (std::map<DeviceEvent,FunctionInfo>::iterator it = eventToFunction.begin(); it != eventToFunction.end(); it++) {
			if (it->second.outputType == TYPE_BUTTON) {
				//pinMode(it->second.pins[0], OUTPUT); // deprecated
				if (it->second.pins[1] >= 0) {
					//pinMode(it->second.pins[1], OUTPUT); // deprecated
				}
			} else if (it->second.outputType == TYPE_AXIS) {
				//softPwmCreate(it->second.pins[0], PWM_RANGE/2, PWM_RANGE); // deprecated
				if (it->second.pins[1] >= 0) {
					//softPwmCreate(it->second.pins[1], PWM_RANGE/2, PWM_RANGE); // deprecated
				}
			}
			it->second.function(it->second.pins, 0, it->second.calibration);	// let's turn everythong off
		}
//	}
}

ControllerGpio::~ControllerGpio() {
//	delete chaosEngine;
}


void ControllerGpio::newDeviceEvent(const DeviceEvent* event) {
	//if (event->id == BUTTON_L3 && event->type == TYPE_BUTTON) {
//		printf("Time: %.02f\tType: %d\tButton: %d\tValue: %d\n", (double)event->time/1000.0, event->type, event->id, event->value );
	//}
	//
	
	if (eventToFunction.count(*event) == 0) {
		//std::cout << "Untracked Event" << std::endl;
		return;
	}
	
	handleNewDeviceEvent( event );
}


bool ControllerGpio::applyHardware(const DeviceEvent* event) {
	std::map<DeviceEvent, FunctionInfo>::iterator it = eventToFunction.find(*event);
	if ( it == eventToFunction.end()) {
		if (event->id != 255) {
			std::cerr << "Error: Controller::applyEvent(): event " << (int)event->type << ":" << (int)event->id << " doesn't exist" << std::endl;
		}
		return false;
	}
	
//	if (spooferFd < 0) {
		FunctionInfo& info = it->second;
		info.function(info.pins, event->value, info.calibration);
//	}
	
	return true;
}
