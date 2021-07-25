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

#include <iostream>
#include <cstring>

using namespace Chaos;

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

void Device::addObserver(DeviceObserver* observer) {
	this->observer = observer;
}

void Device::doAction() {
//	printf("Read: " );
	for (int i = 0; i < bytesPerFrame; i++) {
		buffer[i] = fgetc( file );
//		printf("%d ", (int)buffer[i]);
	}
//	printf("\n");
	
	DeviceEvent event;
	
	interpret( buffer, &event );
	
	if (observer != NULL &&
		event.time != -1) {
		observer->newDeviceEvent( &event );
	}
}
