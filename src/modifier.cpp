
#include "chaos.h"

#include <iostream>
#include <unistd.h>
#include <sstream>

Modifier::Modifier() {
	this->timer.initialize();
}

void Modifier::setDualshock(Controller* dualshock) {
	this->dualshock = dualshock;
}

void Modifier::setChaosEngine(ChaosEngine* chaosEngine) {
	this->chaosEngine = chaosEngine;
}

void Modifier::_update() {
	timer.update();
	this->update();
}
 
void Modifier::begin() {	// virtual override
}

void Modifier::update() {	// virtual override
}

void Modifier::finish() {	// virtual override
}

std::map<std::string, std::function<Modifier*()>> Modifier::factory;

Modifier* Modifier::build( const std::string& name ) {
	Modifier* mod = NULL;
	
	if (factory.count(name) > 0) {
		mod = factory[name]();
	}
	
	return mod;
}

std::string Modifier::getModList(double timePerModifier) {
	std::stringstream result;
	
	result << "{\"mods\":[";
	std::map<std::string,std::function<Modifier*()>>::iterator it = factory.begin();
	if (it != factory.end()) {
		result << "\"" << it->first;
		it++;
	}
	for (; it != factory.end(); it++) {
		result << "\",\"" << it->first;
	}
	result << "\"],\"voteTime\":" << (timePerModifier/3.0 - 0.5) << "}";
	
	return result.str();
}

double Modifier::lifetime() {
	return timer.runningTime();
}

bool Modifier::tweak( DeviceEvent* event ) {
	return true;	// by default it is valid and we do nothing
}

