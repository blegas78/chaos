#include "controller.h"

#include <unistd.h>

bool ControllerRaw::applyHardware(const DeviceEvent* event) {
	//	State will already be stored, nothing to be done for raw
//	if (spooferFile) {
//		if (event->type == TYPE_AXIS &&
//			event->id > AXIS_DY) {
//			return true;
//		}
//		fwrite(event, sizeof(DeviceEvent), 1, spooferFile);
	//chaosHid.sendUpdate( mControllerState->getHackedState() );
//	}
	return true;
}

void ControllerRaw::doAction() {
	//printf("Queue length = %d\n", deviceEventQueue.size());
	
	while (!bufferQueue.empty()) {
		lock();
		std::array<unsigned char, 64> bufferFront = bufferQueue.front();
		bufferQueue.pop_front();
		unlock();
		
		//handleNewDeviceEvent( &event );
		// First convert the incoming buffer into a device event:
		// A buffer can contain multiple events
		std::vector<DeviceEvent> deviceEvents;
		mControllerState->getDeviceEvents(bufferFront.data(), 64, deviceEvents);
		
		for (std::vector<DeviceEvent>::iterator it=deviceEvents.begin();
			 it != deviceEvents.end();
			 it++) {
			DeviceEvent& event = *it;
			
			
			/*
			if (event.type == TYPE_AXIS) {
				bool doNotPush = false;
				for (std::deque<DeviceEvent>::iterator it = deviceEventQueue.begin();
					 it != deviceEventQueue.end();
					 it++) {
					if (event.id == (*it).id) {
						(*it) = event;
						doNotPush = true;
						printf("Replacing an event with id: %d\n", event.id);
					}
				}
				if (doNotPush) {
					//unlock();
					continue;
				}
			}
			 */
			//deviceEventQueue.push_back( event );
			handleNewDeviceEvent( &event );
		}

		
		
		
//		while (!deviceEventQueue.empty()) {
//			DeviceEvent& event = deviceEventQueue.front();
//			handleNewDeviceEvent( &event );
//			deviceEventQueue.pop_front();
//		}
		
		//printf(" - handled type = %d id = %d value = %d\n", event.type, event.id, event.value);
	}
	
	
	//unlock();
	
	
	pause();
}

void ControllerRaw::notification(unsigned char* buffer, int length) {
	
	//memcpy(copiedBuffer, buffer, length);
	lock();
	bufferQueue.push_back( *(std::array<unsigned char, 64>*) buffer);
	unlock();
	
	// For each event in a buffer, run the following:
	//lock();
//	unlock();
	
	//if (!deviceEventQueue.empty()) {
	resume();	// kick off the thread if paused
	//}
	
//	printf("Before: ");
//	for (int i = 0; i < length; i++) {
//		printf("%02X ", buffer[i]);
//	}
//	printf("\n");
	// This is our only chance to intercept the data.
	// Take the mControllerState and replace the provided buffer:
	mControllerState->applyHackedState(buffer, controllerState);
	
//	printf("After: ");
//	for (int i = 0; i < length; i++) {
//		printf("%02X ", buffer[i]);
//	}
//	printf("\n");
}

void ControllerRaw::initialize( ) {
	
	
	//copiedBuffer = malloc(sizeof(inputReport01_t));

	this->setEndpoint(0x84);	// Works for both dualshock4 and dualsense
	mRawGadgetPassthrough.addObserver(this);
	
	mRawGadgetPassthrough.initialize();
	mRawGadgetPassthrough.start();
	
	while (!mRawGadgetPassthrough.readyProductVendor()) {
		usleep(10000);
	}
	
	mControllerState = ControllerState::factory(mRawGadgetPassthrough.getVendor(), mRawGadgetPassthrough.getProduct());
	chaosHid = new ChaosUhid(mControllerState);
	chaosHid->start();
	
	if (mControllerState == NULL) {
		fprintf(stderr, "ERROR!  Could not build a ControllerState for vendor=0x%04x product=0x%04x\n", mRawGadgetPassthrough.getVendor(), mRawGadgetPassthrough.getProduct());
		exit(EXIT_FAILURE);
	}

}
