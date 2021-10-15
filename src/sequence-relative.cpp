
#include "sequence.h"
#include <unistd.h>
#include <sys/time.h>

#include <iostream>

using namespace Chaos;

SequenceRelative::SequenceRelative() {
	tickTime = 200;
}

void SequenceRelative::send(Controller* dualshock) {
	struct timeval startTime;
	gettimeofday(&startTime, NULL);
	
	long long lastTime = 0;
	
	struct timeval currentTime = startTime;
	long long runningTimeInMicroseconds = 0;
	
	bool done;
	
	for (std::vector<DeviceEvent>::iterator it = events.begin(); it != events.end(); it++) {
		DeviceEvent& event = (*it);
		
		done = false;
		
		while(!done) {
			// Send ASAP then check next event:
			if (((long long)event.time + lastTime) <= runningTimeInMicroseconds) {
				lastTime += event.time;
				
				dualshock->applyEvent( &event );
				
				done = true;
				continue;
			}
			
			// Event not ready, wait then check new time:
			usleep(tickTime);
			
			gettimeofday(&currentTime, NULL);
			runningTimeInMicroseconds = (long long)(currentTime.tv_sec - startTime.tv_sec)*1000000 + (long long)(currentTime.tv_usec - startTime.tv_usec);
			
		}
	}
}
