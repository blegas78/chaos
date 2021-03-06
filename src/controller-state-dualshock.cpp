#include "controller-state.h"

#include "ps4-generated.h"

using namespace Chaos;

ControllerStateDualshock::ControllerStateDualshock() {
	stateLength = sizeof(inputReport01_t);
	trueState = (void*)new inputReport01_t;
	hackedState = (void*)new inputReport01_t;
	
	touchCounter = 0;
	touchTimeStamp = 0;
	priorFingerActive[0] = 0;
	priorFingerActive[1] = 0;
}

ControllerStateDualshock::~ControllerStateDualshock() {
	delete (inputReport01_t*)trueState;
	delete (inputReport01_t*)hackedState;
}

//void ControllerStateDualshock::applyHackedState(unsigned char* buffer, std::map<int,short>& chaosState) {
void ControllerStateDualshock::applyHackedState(unsigned char* buffer, short* chaosState) {
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
	
	report->GD_ACC_X = fixShort(chaosState[((int)TYPE_AXIS<<8) + (int)AXIS_ACCX]);
	report->GD_ACC_Y = fixShort(chaosState[((int)TYPE_AXIS<<8) + (int)AXIS_ACCY]);
	report->GD_ACC_Z = fixShort(chaosState[((int)TYPE_AXIS<<8) + (int)AXIS_ACCZ]);
	
	report->GD_GamePadRx = packJoystick(chaosState[((int)TYPE_AXIS<<8) + (int)AXIS_L2]);
	report->GD_GamePadRy = packJoystick(chaosState[((int)TYPE_AXIS<<8) + (int)AXIS_R2]);
	
	report->GD_GamePadHatSwitch = packDpad(chaosState[((int)TYPE_AXIS<<8) + (int)AXIS_DX],
										   chaosState[((int)TYPE_AXIS<<8) + (int)AXIS_DY]);
	
	if (shouldClearTouchpadCount) {
		shouldClearTouchpadCount = false;
		
	}
	
	// Touchpad hanlding.  Yup it's a lot and shouldn't be handled here:
	touchTimeStamp += 7;	// sometimes this also increments by 8
	
	if ((
		 chaosState[((int)TYPE_BUTTON<<8) + (int)BUTTON_TOUCHPAD_ACTIVE] &&
		 (
		  lastX[0] != chaosState[((int)TYPE_AXIS<<8) + (int)AXIS_TOUCHPAD_X] ||
		  lastX[1] != chaosState[((int)TYPE_AXIS<<8) + (int)AXIS_TOUCHPAD_X_2])
		 )
		||
		(
		 chaosState[((int)TYPE_BUTTON<<8) + (int)BUTTON_TOUCHPAD_ACTIVE] &&
		 (
		  lastY[0] != chaosState[((int)TYPE_AXIS<<8) + (int)AXIS_TOUCHPAD_Y] ||
		  lastY[1] != chaosState[((int)TYPE_AXIS<<8) + (int)AXIS_TOUCHPAD_Y_2]
		  )
		 )
		) {
		touchTimeStampToReport = touchTimeStamp;
		report->TOUCH_EVENTS[0].timestamp = touchTimeStampToReport;
		
		lastX[0] = chaosState[((int)TYPE_AXIS<<8) + (int)AXIS_TOUCHPAD_X];
		lastX[1] = chaosState[((int)TYPE_AXIS<<8) + (int)AXIS_TOUCHPAD_X_2];
		lastY[0] = chaosState[((int)TYPE_AXIS<<8) + (int)AXIS_TOUCHPAD_Y];
		lastY[1] = chaosState[((int)TYPE_AXIS<<8) + (int)AXIS_TOUCHPAD_Y_2];
	}
	
	if (chaosState[((int)TYPE_BUTTON<<8) + (int)BUTTON_TOUCHPAD_ACTIVE]) {
		report->TOUCH_COUNT = 1;
		report->TOUCH_EVENTS[0].finger[0].active = 0;
		report->TOUCH_EVENTS[0].finger[0].x = chaosState[((int)TYPE_AXIS<<8) + (int)AXIS_TOUCHPAD_X];
		report->TOUCH_EVENTS[0].finger[0].y = chaosState[((int)TYPE_AXIS<<8) + (int)AXIS_TOUCHPAD_Y];
		
		if (priorFingerActive[0] == 0) {
			priorFingerActive[0] = 1;
			currentTouchpadCount++;
			currentTouchpadCount &= 0x7f;
			touchCounterSaved[0] = currentTouchpadCount;
		}
		
		report->TOUCH_EVENTS[0].finger[0].counter = touchCounterSaved[0];
		
	} else {
		priorFingerActive[0] = 0;
		report->TOUCH_EVENTS[0].finger[0].active = 1;
	}
	
	if (chaosState[((int)TYPE_BUTTON<<8) + (int)BUTTON_TOUCHPAD_ACTIVE_2]) {
		report->TOUCH_COUNT = 1;
		report->TOUCH_EVENTS[0].finger[1].active = 0;
		report->TOUCH_EVENTS[0].finger[1].x = chaosState[((int)TYPE_AXIS<<8) + (int)AXIS_TOUCHPAD_X_2];
		report->TOUCH_EVENTS[0].finger[1].y = chaosState[((int)TYPE_AXIS<<8) + (int)AXIS_TOUCHPAD_Y_2];
		
		if (priorFingerActive[1] == 0) {
			priorFingerActive[1] = 1;
			currentTouchpadCount++;
			currentTouchpadCount &= 0x7f;
			touchCounterSaved[1] = currentTouchpadCount;
		}
		
		report->TOUCH_EVENTS[0].finger[1].counter = touchCounterSaved[1];
		
	} else {
		priorFingerActive[1] = 0;
		report->TOUCH_EVENTS[0].finger[1].active = 1;
	}
	
	
	
	
	*(inputReport01_t*)hackedState = *report;
}

void ControllerStateDualshock::getDeviceEvents(unsigned char* buffer, int length, std::vector<DeviceEvent>& events)  {
	//std::vector<DeviceEvent> events;
	
//	for (int i = 0; i < length; i++) {
//		printf("%02X ", buffer[i]);
//	}
//	printf("\n");
	
	inputReport01_t currentState = *(inputReport01_t*)buffer;
	
	if (currentState.BTN_GamePadButton1 != ((inputReport01_t*)trueState)->BTN_GamePadButton1 ) {
		events.push_back({0, currentState.BTN_GamePadButton1, TYPE_BUTTON, BUTTON_SQUARE}); }
	if (currentState.BTN_GamePadButton2 != ((inputReport01_t*)trueState)->BTN_GamePadButton2 ) {
		events.push_back({0, currentState.BTN_GamePadButton2, TYPE_BUTTON, BUTTON_X}); }
	if (currentState.BTN_GamePadButton3 != ((inputReport01_t*)trueState)->BTN_GamePadButton3 ) {
		events.push_back({0, currentState.BTN_GamePadButton3, TYPE_BUTTON, BUTTON_CIRCLE}); }
	if (currentState.BTN_GamePadButton4 != ((inputReport01_t*)trueState)->BTN_GamePadButton4 ) {
		events.push_back({0, currentState.BTN_GamePadButton4, TYPE_BUTTON, BUTTON_TRIANGLE}); }
	if (currentState.BTN_GamePadButton5 != ((inputReport01_t*)trueState)->BTN_GamePadButton5 ) {
		events.push_back({0, currentState.BTN_GamePadButton5, TYPE_BUTTON, BUTTON_L1}); }
	if (currentState.BTN_GamePadButton6 != ((inputReport01_t*)trueState)->BTN_GamePadButton6 ) {
		events.push_back({0, currentState.BTN_GamePadButton6, TYPE_BUTTON, BUTTON_R1}); }
	if (currentState.BTN_GamePadButton7 != ((inputReport01_t*)trueState)->BTN_GamePadButton7 ) {
		events.push_back({0, currentState.BTN_GamePadButton7, TYPE_BUTTON, BUTTON_L2}); }
	if (currentState.BTN_GamePadButton8 != ((inputReport01_t*)trueState)->BTN_GamePadButton8 ) {
		events.push_back({0, currentState.BTN_GamePadButton8, TYPE_BUTTON, BUTTON_R2}); }
	if (currentState.BTN_GamePadButton9 != ((inputReport01_t*)trueState)->BTN_GamePadButton9 ) {
		events.push_back({0, currentState.BTN_GamePadButton9, TYPE_BUTTON, BUTTON_SHARE}); }
	if (currentState.BTN_GamePadButton10 != ((inputReport01_t*)trueState)->BTN_GamePadButton10 ) {
		events.push_back({0, currentState.BTN_GamePadButton10, TYPE_BUTTON, BUTTON_OPTIONS}); }
	if (currentState.BTN_GamePadButton11 != ((inputReport01_t*)trueState)->BTN_GamePadButton11 ) {
		events.push_back({0, currentState.BTN_GamePadButton11, TYPE_BUTTON, BUTTON_L3}); }
	if (currentState.BTN_GamePadButton12 != ((inputReport01_t*)trueState)->BTN_GamePadButton12 ) {
		events.push_back({0, currentState.BTN_GamePadButton12, TYPE_BUTTON, BUTTON_R3}); }
	if (currentState.BTN_GamePadButton13 != ((inputReport01_t*)trueState)->BTN_GamePadButton13 ) {
		events.push_back({0, currentState.BTN_GamePadButton13, TYPE_BUTTON, BUTTON_PS}); }
	if (currentState.BTN_GamePadButton14 != ((inputReport01_t*)trueState)->BTN_GamePadButton14 ) {
		events.push_back({0, currentState.BTN_GamePadButton14, TYPE_BUTTON, BUTTON_TOUCHPAD}); }
	
	
	if (currentState.GD_GamePadX != ((inputReport01_t*)trueState)->GD_GamePadX ) {
		events.push_back({0, unpackJoystick(currentState.GD_GamePadX), TYPE_AXIS, AXIS_LX}); }
	if (currentState.GD_GamePadY != ((inputReport01_t*)trueState)->GD_GamePadY ) {
		events.push_back({0, unpackJoystick(currentState.GD_GamePadY), TYPE_AXIS, AXIS_LY}); }
	if (currentState.GD_GamePadZ != ((inputReport01_t*)trueState)->GD_GamePadZ ) {
		events.push_back({0, unpackJoystick(currentState.GD_GamePadZ), TYPE_AXIS, AXIS_RX}); }
	if (currentState.GD_GamePadRz != ((inputReport01_t*)trueState)->GD_GamePadRz ) {
		events.push_back({0, unpackJoystick(currentState.GD_GamePadRz), TYPE_AXIS, AXIS_RY}); }
	if (currentState.GD_GamePadRx != ((inputReport01_t*)trueState)->GD_GamePadRx ) {
		events.push_back({0, unpackJoystick(currentState.GD_GamePadRx), TYPE_AXIS, AXIS_L2}); }
	if (currentState.GD_GamePadRy != ((inputReport01_t*)trueState)->GD_GamePadRy ) {
		events.push_back({0, unpackJoystick(currentState.GD_GamePadRy), TYPE_AXIS, AXIS_R2}); }
	if (currentState.GD_ACC_X != ((inputReport01_t*)trueState)->GD_ACC_X ) {
//		printf("VEN_GamePadunknown1 = %d\n", currentState.VEN_GamePadunknown1);
//		printf("Gy x = %d\n", fixShort( currentState.GD_GYRO_X));
//		printf("Acc x = %d\n", fixShort( currentState.GD_ACC_X));
		events.push_back({0, fixShort(currentState.GD_ACC_X), TYPE_AXIS, AXIS_ACCX}); }
	if (currentState.GD_ACC_Y != ((inputReport01_t*)trueState)->GD_ACC_Y ) {
		//printf("Acc y = %d\n", fixShort(currentState.GD_ACC_Y));
		events.push_back({0, fixShort(currentState.GD_ACC_Y), TYPE_AXIS, AXIS_ACCY}); }
	if (currentState.GD_ACC_Z != ((inputReport01_t*)trueState)->GD_ACC_Z ) {
		//printf("Acc z = %d\n", fixShort(currentState.GD_ACC_Z));
		events.push_back({0, fixShort(currentState.GD_ACC_Z), TYPE_AXIS, AXIS_ACCZ}); }
	
	
	if (currentState.GD_GamePadHatSwitch != ((inputReport01_t*)trueState)->GD_GamePadHatSwitch ) {
		short int currentX = positionDX( currentState.GD_GamePadHatSwitch );
		short int currentY = positionDY( currentState.GD_GamePadHatSwitch );

		if (positionDY(((inputReport01_t*)trueState)->GD_GamePadHatSwitch) != currentY) {
			events.push_back({0, currentY, TYPE_AXIS, AXIS_DY});
		}
		if (positionDX(((inputReport01_t*)trueState)->GD_GamePadHatSwitch) != currentX) {
			events.push_back({0, currentX, TYPE_AXIS, AXIS_DX});
		}
	}
	
	// Touchpad events:
	for (int e = 0; e < ((inputReport01_t*)trueState)->TOUCH_COUNT; e++) {
		/*
		for (int i = 0; i < e; i++) {
			printf("- ");
		}
		printf("e:%d", e);
		printf(" t:%5d", ((inputReport01_t*)trueState)->TOUCH_EVENTS[e].timestamp);
		*/
		for (int f = 0; f < 2; f++) {
			
			if (currentState.TOUCH_EVENTS[e].finger[f].active != ((inputReport01_t*)trueState)->TOUCH_EVENTS[e].finger[f].active ) {
				events.push_back({0, !currentState.TOUCH_EVENTS[e].finger[f].active, TYPE_BUTTON, BUTTON_TOUCHPAD_ACTIVE}); }
			//printf("Touch finger1 active: %d\n", currentState.TOUCH_EVENTS[0].finger1.active);
			//printf("Touch x = %d\ty=%d\n", currentState.TOUCH_EVENTS[0].finger1.x, currentState.TOUCH_EVENTS[0].finger1.y);
			//printf("sizeof(inputReport01_t) = %d\n", (int)sizeof(inputReport01_t));
			if (currentState.TOUCH_EVENTS[e].finger[f].x != ((inputReport01_t*)trueState)->TOUCH_EVENTS[e].finger[f].x) {
				//printf("Acc y = %d\n", fixShort(currentState.GD_ACC_Y));
				//printf("Touch x = %d\ty=%d\n", currentState.TOUCH_EVENTS[0].finger1.x, currentState.TOUCH_EVENTS[0].finger1.y);
				events.push_back({0, currentState.TOUCH_EVENTS[e].finger[f].x, TYPE_AXIS, AXIS_TOUCHPAD_X}); }
			if (currentState.TOUCH_EVENTS[e].finger[f].y != ((inputReport01_t*)trueState)->TOUCH_EVENTS[e].finger[f].y) {
				//printf("Acc y = %d\n", fixShort(currentState.GD_ACC_Y));
				//printf("Touch x = %d\ty=%d\n", currentState.TOUCH_EVENTS[0].finger1.x, currentState.TOUCH_EVENTS[0].finger1.y);
				events.push_back({0, currentState.TOUCH_EVENTS[e].finger[f].y, TYPE_AXIS, AXIS_TOUCHPAD_Y}); }
			
//			if (currentState.TOUCH_EVENTS[e].finger[f].active == 0) {
//				printf("Touch event[%d].finger[%d].counter = %d\n", e, f, currentState.TOUCH_EVENTS[e].finger[f].counter);
//				printf(" - x = %d\ty=%d\n", currentState.TOUCH_EVENTS[e].finger[f].x, currentState.TOUCH_EVENTS[e].finger[f].y);
//			}
			/*
			printf(" f:%d", f);
			printf(" a:%d", ((inputReport01_t*)trueState)->TOUCH_EVENTS[e].finger[f].active);
			printf(" c:%4d", ((inputReport01_t*)trueState)->TOUCH_EVENTS[e].finger[f].counter);
//			printf(" a:%d", ((inputReport01_t*)trueState)->TOUCH_EVENTS[e].finger[f].active);
			printf(" x,y:(%4d,%4d)",
				   ((inputReport01_t*)trueState)->TOUCH_EVENTS[e].finger[f].x,
				   ((inputReport01_t*)trueState)->TOUCH_EVENTS[e].finger[f].y);
			 */
		}
		
//		printf("\n");
	}
	
	// Need to compare for next time:
	*(inputReport01_t*)trueState = currentState;
	
	//return events;
}
