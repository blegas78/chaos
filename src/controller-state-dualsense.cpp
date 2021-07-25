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
#include "controller-state.h"

#include "ps5-generated.h"

using namespace Chaos;

short int Chaos::unpackJoystick(uint8_t& input) {
	return ((short int)input)-128;
}
uint8_t Chaos::packJoystick(short int& input) {
	return input+128;
}

short int Chaos::fixShort(short int input) {
	return input;
//	return ((input & 0x00ff) << 8) | ((input & 0xff00) >> 8);
}
//short int unfixShort(short int& input) {
//	return ((input & 0x00ff) << 8) | ((input & 0xff00) >> 8);
//}


short int Chaos::positionDY( const uint8_t& input ) {
	if (input == 0x08) {
		return 0;
	} if (input <= 1 || input == 7) {
		return -1;
	} else if ( input >= 3 && input <= 5) {
		return 1;
	}
	return 0;
}
short int Chaos::positionDX( const uint8_t& input ) {
	if (input == 0x08) {
		return 0;
	} if (input >= 1 && input <= 3) {
		return 1;
	} else if ( input >= 5 && input <= 7) {
		return -1;
	}
	return 0;
}

uint8_t Chaos::packDpad( const short int& dx, const short int& dy ) {
	switch (dx) {
		case -1:
			return 6-dy;
			break;
		case 1:
			return 2+dy;
			break;
		case 0:
			if (dy == 0) {
				return 0x08;
			}
			return 2*(1+dy);
		default:
			break;
	}
	return 0x08;
}



ControllerStateDualsense::ControllerStateDualsense() {
	stateLength = sizeof(inputReport01_t);
	trueState = (void*)new inputReport01_t;
	hackedState = (void*)new inputReport01_t;
}

ControllerStateDualsense::~ControllerStateDualsense() {
	delete (inputReport01_t*)trueState;
	delete (inputReport01_t*)hackedState;
}

//void ControllerStateDualsense::applyHackedState(unsigned char* buffer, std::map<int,short>& chaosState) {
void ControllerStateDualsense::applyHackedState(unsigned char* buffer, short* chaosState) {
	inputReport01_t* report = (inputReport01_t*)buffer;
	
	report->BTN_GamePadButton1 = chaosState[((int)TYPE_BUTTON<<8) + (int)BUTTON_SQUARE];
	report->BTN_GamePadButton2 = chaosState[((int)TYPE_BUTTON<<8) + (int)BUTTON_X];
	report->BTN_GamePadButton3 = chaosState[((int)TYPE_BUTTON<<8) + (int)BUTTON_CIRCLE];
	report->BTN_GamePadButton4 = chaosState[((int)TYPE_BUTTON<<8) + (int)BUTTON_TRIANGLE];
	report->BTN_GamePadButton5 = chaosState[((int)TYPE_BUTTON<<8) + (int)BUTTON_L1];
	report->BTN_GamePadButton6 = chaosState[((int)TYPE_BUTTON<<8) + (int)BUTTON_R1];
	report->BTN_GamePadButton7 = chaosState[((int)TYPE_BUTTON<<8) + (int)BUTTON_L2];
	report->BTN_GamePadButton8 = chaosState[((int)TYPE_BUTTON<<8) + (int)BUTTON_R2];
	report->BTN_GamePadButton9 = chaosState[((int)TYPE_BUTTON<<8) + (int)BUTTON_SHARE];
	report->BTN_GamePadButton10 = chaosState[((int)TYPE_BUTTON<<8) + (int)BUTTON_OPTIONS];
	report->BTN_GamePadButton11 = chaosState[((int)TYPE_BUTTON<<8) + (int)BUTTON_L3];
	report->BTN_GamePadButton12 = chaosState[((int)TYPE_BUTTON<<8) + (int)BUTTON_R3];
	report->BTN_GamePadButton13 = chaosState[((int)TYPE_BUTTON<<8) + (int)BUTTON_PS];
	report->BTN_GamePadButton14 = chaosState[((int)TYPE_BUTTON<<8) + (int)BUTTON_TOUCHPAD];
	
	report->GD_GamePadX = packJoystick(chaosState[((int)TYPE_AXIS<<8) + (int)AXIS_LX]);
	report->GD_GamePadY = packJoystick(chaosState[((int)TYPE_AXIS<<8) + (int)AXIS_LY]);
	report->GD_GamePadZ = packJoystick(chaosState[((int)TYPE_AXIS<<8) + (int)AXIS_RX]);
	report->GD_GamePadRz = packJoystick(chaosState[((int)TYPE_AXIS<<8) + (int)AXIS_RY]);
	
	report->GD_GamePadHatSwitch = packDpad(chaosState[((int)TYPE_AXIS<<8) + (int)AXIS_DX],
										   chaosState[((int)TYPE_AXIS<<8) + (int)AXIS_DY]);
	
}

/*
 square
 x
 circle
 triangle
 l1
 r1
 l2
 r2
 sgare
 options
 L3
 R3
 PS
 touchpad
 
 */



void  ControllerStateDualsense::getDeviceEvents(unsigned char* buffer, int length, std::vector<DeviceEvent>& events)  {
	//std::vector<DeviceEvent> events;
	
	inputReport01_t* currentState = (inputReport01_t*)buffer;
	inputReport01_t* myState = (inputReport01_t*)trueState;
	
	if (currentState->BTN_GamePadButton1 != myState->BTN_GamePadButton1 ) {
		events.push_back({0, currentState->BTN_GamePadButton1, TYPE_BUTTON, BUTTON_SQUARE}); }
	if (currentState->BTN_GamePadButton2 != myState->BTN_GamePadButton2 ) {
		events.push_back({0, currentState->BTN_GamePadButton2, TYPE_BUTTON, BUTTON_X}); }
	if (currentState->BTN_GamePadButton3 != myState->BTN_GamePadButton3 ) {
		events.push_back({0, currentState->BTN_GamePadButton3, TYPE_BUTTON, BUTTON_CIRCLE}); }
	if (currentState->BTN_GamePadButton4 != myState->BTN_GamePadButton4 ) {
		events.push_back({0, currentState->BTN_GamePadButton4, TYPE_BUTTON, BUTTON_TRIANGLE}); }
	if (currentState->BTN_GamePadButton5 != myState->BTN_GamePadButton5 ) {
		events.push_back({0, currentState->BTN_GamePadButton5, TYPE_BUTTON, BUTTON_L1}); }
	if (currentState->BTN_GamePadButton6 != myState->BTN_GamePadButton6 ) {
		events.push_back({0, currentState->BTN_GamePadButton6, TYPE_BUTTON, BUTTON_R1}); }
	if (currentState->BTN_GamePadButton7 != myState->BTN_GamePadButton7 ) {
		events.push_back({0, currentState->BTN_GamePadButton7, TYPE_BUTTON, BUTTON_L2}); }
	if (currentState->BTN_GamePadButton8 != myState->BTN_GamePadButton8 ) {
		events.push_back({0, currentState->BTN_GamePadButton8, TYPE_BUTTON, BUTTON_R2}); }
	if (currentState->BTN_GamePadButton9 != myState->BTN_GamePadButton9 ) {
		events.push_back({0, currentState->BTN_GamePadButton9, TYPE_BUTTON, BUTTON_SHARE}); }
	if (currentState->BTN_GamePadButton10 != myState->BTN_GamePadButton10 ) {
		events.push_back({0, currentState->BTN_GamePadButton10, TYPE_BUTTON, BUTTON_OPTIONS}); }
	if (currentState->BTN_GamePadButton11 != myState->BTN_GamePadButton11 ) {
		events.push_back({0, currentState->BTN_GamePadButton11, TYPE_BUTTON, BUTTON_L3}); }
	if (currentState->BTN_GamePadButton12 != myState->BTN_GamePadButton12 ) {
		events.push_back({0, currentState->BTN_GamePadButton12, TYPE_BUTTON, BUTTON_R3}); }
	if (currentState->BTN_GamePadButton13 != myState->BTN_GamePadButton13 ) {
		events.push_back({0, currentState->BTN_GamePadButton13, TYPE_BUTTON, BUTTON_PS}); }
	if (currentState->BTN_GamePadButton14 != myState->BTN_GamePadButton14 ) {
		events.push_back({0, currentState->BTN_GamePadButton14, TYPE_BUTTON, BUTTON_TOUCHPAD}); }
	
	
	if (currentState->GD_GamePadX != myState->GD_GamePadX ) {
		events.push_back({0, unpackJoystick(currentState->GD_GamePadX), TYPE_AXIS, AXIS_LX}); }
	if (currentState->GD_GamePadY != myState->GD_GamePadY ) {
		events.push_back({0, unpackJoystick(currentState->GD_GamePadY), TYPE_AXIS, AXIS_LY}); }
	if (currentState->GD_GamePadZ != myState->GD_GamePadZ ) {
		events.push_back({0, unpackJoystick(currentState->GD_GamePadZ), TYPE_AXIS, AXIS_RX}); }
	if (currentState->GD_GamePadRz != myState->GD_GamePadRz ) {
		events.push_back({0, unpackJoystick(currentState->GD_GamePadRz), TYPE_AXIS, AXIS_RY}); }
	
	
	if (currentState->GD_GamePadHatSwitch != myState->GD_GamePadHatSwitch ) {
		short int currentX = positionDX( currentState->GD_GamePadHatSwitch );
		short int currentY = positionDY( currentState->GD_GamePadHatSwitch );
		
		if (positionDY(myState->GD_GamePadHatSwitch) != currentY) {
			events.push_back({0, currentY, TYPE_AXIS, AXIS_DY});
		}
		if (positionDX(myState->GD_GamePadHatSwitch) != currentX) {
			events.push_back({0, currentX, TYPE_AXIS, AXIS_DX});
		}
		
	}
	
	// Need to compare for next time:
	*(inputReport01_t*)trueState = *currentState;
	
	//return events;
}
