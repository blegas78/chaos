
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
