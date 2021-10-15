#ifndef CHAOS_H
#define CHAOS_H

//#include <stdio.h>
#include <mogi/thread.h>
//#include <mogi/math/systems.h>
#include <list>
#include <string>
//#include <functional>
//#include <map>
//#include <vector>
#include <queue>

#include "interface.h"	// for CommandListener
#include "controller.h"

#include "modifier.h"

namespace Chaos {

class Engine : public CommandListenerObserver, public ControllerInjector, public Mogi::Thread {
private:
	ChaosInterface chaosInterface;
	Controller* dualshock;
	
	Mogi::Math::Time time;
	double timePerModifier;
	DeviceEvent event;
	
	std::list<Modifier*> modifiers;
	std::queue<Modifier*> modifiersThatNeedToStart;
	
	bool pause;
	bool pausePrimer = false;
	bool pausedPrior = false;
	
	//pthread_mutex_t chaosMutex;
	
	
	bool sniffify(const DeviceEvent* input, DeviceEvent* output); // override from DualShockInjector
	void doAction(); // override from Mogi::Thread
	
public:
	Engine(Controller* dualshock);
	
	void setInterfaceReply(const std::string& reply);
	void setTimePerModifier(double time);
	void fakePipelinedEvent(DeviceEvent* fakeEvent, Modifier* modifierThatSentTheFakeEvent);
	
	void newCommand(const std::string& command);	// override from CommandListenerObserver.  Can also be called manually
	
	bool isPaused();
};

};

#endif
