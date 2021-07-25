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
#include "interface.h"

#include <iostream>
#include <unistd.h>
#include <string>
#include <json/json.h>

using namespace Chaos;

ChaosInterface::ChaosInterface() {
	listener.start();
	start();
}

void ChaosInterface::doAction() {
	while (!outgoingQueue.empty()) {
		lock();
		std::string message = outgoingQueue.front();
		outgoingQueue.pop();
		unlock();
		
		sender.sendMessage(message);
	}
	
	pause();
}
	
bool ChaosInterface::sendMessage(std::string message) {
	lock();
	outgoingQueue.push(message);
	unlock();
	
	resume();
	return true;
}

void ChaosInterface::addObserver( CommandListenerObserver* observer ) {
	listener.addObserver(observer);
}
