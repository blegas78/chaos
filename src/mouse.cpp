
#include "device.h"

using namespace Chaos;

void Mouse::open(const char* filename) {
	init(filename, 3);
}

void Mouse::interpret( char* buffer, DeviceEvent* event) {
	if(	(buffer[0] == 8 || buffer[0] == 9) &&
		buffer[1] == 0 &&
		buffer[2] == 0 ) {
		event->time = 0;
		event->type = TYPE_BUTTON;
		event->id = BUTTON_TOUCHPAD;
		event->value = buffer[0] == 9 ? 1 : 0;
	} else {
		event->time = -1;
		event->type = -1;
	}
}
