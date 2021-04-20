#include "controller-state.h"

#include "ps4-generated.h"


ControllerStateDualshock::ControllerStateDualshock() {
	stateLength = sizeof(inputReport01_t);
	trueState = (void*)new inputReport01_t;
	hackedState = (void*)new inputReport01_t;
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
}

void ControllerStateDualshock::getDeviceEvents(unsigned char* buffer, int length, std::vector<DeviceEvent>& events)  {
	//std::vector<DeviceEvent> events;
	
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
	
	//printf("Touch x = %d\ty=%d\n", currentState.TOUCH_EVENTS[0].finger1.x, currentState.TOUCH_EVENTS[0].finger1.y);
	//printf("sizeof(inputReport01_t) = %d\n", (int)sizeof(inputReport01_t));
	if (currentState.TOUCH_EVENTS[0].finger1.x != ((inputReport01_t*)trueState)->TOUCH_EVENTS[0].finger1.x) {
		//printf("Acc y = %d\n", fixShort(currentState.GD_ACC_Y));
		//printf("Touch x = %d\ty=%d\n", currentState.TOUCH_EVENTS[0].finger1.x, currentState.TOUCH_EVENTS[0].finger1.y);
		events.push_back({0, currentState.TOUCH_EVENTS[0].finger1.x, TYPE_AXIS, AXIS_TOUCHPAD_X}); }
	if (currentState.TOUCH_EVENTS[0].finger1.y != ((inputReport01_t*)trueState)->TOUCH_EVENTS[0].finger1.y) {
		//printf("Acc y = %d\n", fixShort(currentState.GD_ACC_Y));
		//printf("Touch x = %d\ty=%d\n", currentState.TOUCH_EVENTS[0].finger1.x, currentState.TOUCH_EVENTS[0].finger1.y);
		events.push_back({0, currentState.TOUCH_EVENTS[0].finger1.y, TYPE_AXIS, AXIS_TOUCHPAD_Y}); }
	
	
	// Need to compare for next time:
	*(inputReport01_t*)trueState = currentState;
	
	//return events;
}
