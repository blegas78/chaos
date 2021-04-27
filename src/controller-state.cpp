#include "controller-state.h"

#include <cstddef>

ControllerState::~ControllerState() {
	
}

void* ControllerState::getHackedState() {
	return hackedState;
}

ControllerState* ControllerState::factory(int vendor, int product) {
	if (vendor == 0x054c && product == 0x09cc) {
		return new ControllerStateDualshock;
	} else if (vendor == 0x054c && product == 0x0ce6) {
		return new ControllerStateDualsense;
	} else if (vendor==0x2f24  && product==0x00f8) {	// Magic-S Pro
		return new ControllerStateDualshock;
	}
	
	return NULL;
}
