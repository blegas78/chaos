
#include "chaos.h"

#include <iostream>
#include <unistd.h>
#include <cmath>
#include <algorithm>
#include <json/json.h>

ChaosEngine::ChaosEngine(Controller* dualshock)
: dualshock(dualshock), timePerModifier(30.0), pause(true)
{
	dualshock->addInjector(this);
	time.initialize();
	chaosInterface.addObserver(this);
	//commandListener.start();
	
	//pthread_mutex_init(&chaosMutex, NULL);
}

void ChaosEngine::newCommand(const std::string& command) {
	std::cout << "ChaosEngine::newCommand() received: " << command << std::endl;
	
	Json::Value root;
	Json::CharReaderBuilder builder;
	Json::CharReader* reader = builder.newCharReader();
	std::string errs;
	bool parsingSuccessful = reader->parse(command.c_str(), command.c_str() + command.length(), &root, &errs);
	
	if (!parsingSuccessful) {
		std::cerr << " - Unable to parse the above message!" << std::endl;
	}
	
	if (root.isMember("winner")) {
		//printf("No winner reported\n");
		//return;
		
		Modifier* mod = Modifier::build(root["winner"].asString());
		if (mod != NULL) {
			//std::cout << "Adding Modifier: " << typeid(*mod).name() << std::endl;
			
			mod->setDualshock(dualshock);
			mod->setChaosEngine(this);
			lock();
			//pthread_mutex_lock(&chaosMutex);
			modifiers.push_back(mod);
			modifiersThatNeedToStart.push(mod);
			//        if( !pause ){
			//			mod->begin();
			//        }
			unlock();
			//pthread_mutex_unlock(&chaosMutex);
		} else {
			std::cerr << "ERROR: Unable to build Modifier for key: " << command << std::endl;
		}
	}
	
	if (root.isMember("timePerModifier")) {
		int newModifierTime = root["timePerModifier"].asFloat();
		printf("New Modifier Time: %d\n", newModifierTime);
		setTimePerModifier(newModifierTime);
	}
	
}

void ChaosEngine::doAction() {
	usleep(500);	// 200Hz
	
	// Update timers/states of modifiers
    if(pause) {
        //std::cout << "Paused" << std::endl;
        return;
        
    }
	
	lock();
	//pthread_mutex_lock(&chaosMutex);
	while(!modifiersThatNeedToStart.empty()) {
		modifiersThatNeedToStart.front()->begin();
		modifiersThatNeedToStart.pop();
	}
	unlock();
	//pthread_mutex_unlock(&chaosMutex);
	
	lock();
	//pthread_mutex_lock(&chaosMutex);
	for (std::list<Modifier*>::iterator it = modifiers.begin(); it != modifiers.end(); it++) {
		(*it)->_update();
	}
	unlock();
	//pthread_mutex_unlock(&chaosMutex);
	
	// Check front element.  If timer ran out, remove it.
	if (modifiers.size() > 0) {
		Modifier* front = modifiers.front();
		if (front->lifetime() > timePerModifier) {
//			std::cout << "Killing Modifier: " << typeid(*front).name() << std::endl;
			lock();
			//pthread_mutex_lock(&chaosMutex);
				front->finish();	// Do any cleanup, if necessary
				modifiers.pop_front();
				delete front;
			unlock();
			//pthread_mutex_unlock(&chaosMutex);
		}
		
	}

}

bool ChaosEngine::sniffify(const DeviceEvent* input, DeviceEvent* output) {
	*output = *input;
	
	bool valid = true;
	lock();
	//pthread_mutex_lock(&chaosMutex);
	//int lockCount = 0;
	//bool gotALock = false;
//	while (lockCount < 1000) {	// 1/100th of a second
//		if(pthread_mutex_trylock(&chaosMutex)) {
//			usleep(10);
//			//return false;
//		} else {
//			gotALock = true;
//			break;
//		}
//	}
//
//	if (!gotALock) {
//		fprintf(stderr, "Could not get a lock!  May be missing events\n");
//		return false;
//	}
	
	
	// Tweak the event based on modifiers
	if (input->type == TYPE_BUTTON && input->id == BUTTON_OPTIONS) {
		if(input->value == 1 && pause == false)	{ // on rising edge
			pause = true;
			chaosInterface.sendMessage("{\"pause\":1}");
			pausePrimer = false;
			std::cout << "Paused" << std::endl;
		}
//		else if(input->value == 1 && pause == true)	{ // on rising edge
//			pausePrimer = true;
//		} else if(input->value == 0 && pausePrimer == true) {	// falling edge
//			pause = false;
//		}
	}
	
	if (input->type == TYPE_BUTTON && input->id == BUTTON_SHARE) {
		if(input->value == 1 && pause == true)	{ // on rising edge
			pausePrimer = true;
		} else if(input->value == 0 && pausePrimer == true) {	// falling edge
			pause = false;
			chaosInterface.sendMessage("{\"pause\":0}");
			std::cout << "Resumed" << std::endl;
		}
		output->value = 0;
	}
	
	unlock();
	//pthread_mutex_unlock(&chaosMutex);
	
	if (!pause) {
		lock();
		//pthread_mutex_lock(&chaosMutex);
		for (std::list<Modifier*>::iterator it = modifiers.begin(); it != modifiers.end(); it++) {
			valid &= (*it)->tweak(output);
		}
		unlock();
		//pthread_mutex_unlock(&chaosMutex);
	}
	
	
	return valid;
}

void ChaosEngine::fakePipelinedEvent(DeviceEvent* fakeEvent, Modifier* modifierThatSentTheFakeEvent) {
	bool valid = true;
	
	if (!pause) {
		//lock();
		// Since we are faking a pipelined event, find the location of the modifier
		// and feed the fake event through the rest of the modifier, in order:
		std::list<Modifier*>::iterator it = std::find(modifiers.begin(), modifiers.end(), modifierThatSentTheFakeEvent);
		
		for ( it++; it != modifiers.end(); it++) {	// iterate from the next element till the end
			valid &= (*it)->tweak(fakeEvent);
		}
		//unlock();
	}
	
	if (valid) {
		dualshock->applyEvent(fakeEvent);
	}
}

void ChaosEngine::setInterfaceReply(const std::string& reply) {
//	commandListener.setReply(reply);
	chaosInterface.sendMessage(reply);
}

void ChaosEngine::setTimePerModifier(double time) {
	timePerModifier = time;
}
