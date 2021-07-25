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
