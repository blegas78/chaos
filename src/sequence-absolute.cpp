
#include "sequence.h"
#include <unistd.h>
#include <sys/time.h>


using namespace Chaos;

SequenceAbsolute::SequenceAbsolute() {
	tickTime = 200;
}

void SequenceAbsolute::send(Controller* dualshock) {
	struct timeval startTime;
	gettimeofday(&startTime, NULL);
	
	struct timeval currentTime = startTime;
	long long runningTimeInMicroseconds = 0;
	
	bool done;
	
	for (std::vector<DeviceEvent>::iterator it = events.begin(); it != events.end(); it++) {
		DeviceEvent& event = (*it);
		
		done = false;
		
		while(!done) {
			// Send ASAP then check next event:
			if (event.time <= runningTimeInMicroseconds) {
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
