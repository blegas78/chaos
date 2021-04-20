
#include "controller.h"

#include <cstring>

Controller::Controller() {
	memset(controllerState, 0, sizeof(controllerState));
}

int Controller::getState(int id, ButtonType type) {
	return controllerState[ ((int)type<<8) + (int)id ];
}

void Controller::storeState(const DeviceEvent* event) {
	int location = ((int)event->type<<8) + (int)event->id;
	if (location < 1024) {
		controllerState[ location ] = event->value;
	}
	
	// In GPIO, maybe nothing needs to be done.
	// In RAW, the above state storage should get passed through
	//writeToSpoofFile( controllerState );
	//printf("State stored!\n");
	
}

void Controller::handleNewDeviceEvent(const DeviceEvent* event) {
	
	// We have an event!  Let's send it right away, but let's let chaos engine play around with the values
	DeviceEvent updatedEvent;
	bool validEvent = false;
	if (dualShockInjector != NULL) {
		validEvent = dualShockInjector->sniffify(event, &updatedEvent);
	} else {
		validEvent = true;
		updatedEvent = *event;
	}
	
	// Is our event valid?  If so, send the chaos modified data:
	if (validEvent) {
		applyEvent(&updatedEvent);
	} else {
		//printf("Event with id %d was NOT applied\n", event->id);
	}
	
}

void Controller::applyEvent(const DeviceEvent* event) {
	if (!applyHardware(event)) {
		return;
	}
	
	storeState(event);
}


void Controller::addInjector(ControllerInjector* injector) {
	this->dualShockInjector = injector;
}

