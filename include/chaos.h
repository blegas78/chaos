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
	
	void newCommand(const std::string& command);	// override from CommandListenerObserver
	bool sniffify(const DeviceEvent* input, DeviceEvent* output); // override from DualShockInjector
	void doAction(); // override from Mogi::Thread
	
public:
	Engine(Controller* dualshock);
	
	void setInterfaceReply(const std::string& reply);
	void setTimePerModifier(double time);
	void fakePipelinedEvent(DeviceEvent* fakeEvent, Modifier* modifierThatSentTheFakeEvent);
	
	bool isPaused();
};

};

#endif
