
#include "chaos.h"

#include <iostream>
#include <unistd.h>
#include <sstream>
#include <json/json.h>

Modifier::Modifier() {
	this->timer.initialize();
	pauseTimeAccumulator = 0;
}

void Modifier::setDualshock(Controller* dualshock) {
	this->dualshock = dualshock;
}

void Modifier::setChaosEngine(ChaosEngine* chaosEngine) {
	this->chaosEngine = chaosEngine;
}

void Modifier::_update(bool isPaused) {
	timer.update();
	if (isPaused) {
		pauseTimeAccumulator += timer.dTime();
	}
	this->update();
}
 
void Modifier::begin() {	// virtual override
}

void Modifier::update() {	// virtual override
}

void Modifier::finish() {	// virtual override
}

const char* Modifier::description() {	// virtual override
	return "";
}

std::map<std::string, std::function<Modifier*()>> Modifier::factory;

Modifier* Modifier::build( const std::string& name ) {
	Modifier* mod = NULL;
	
	if (factory.count(name) > 0) {
		mod = factory[name]();
	}
	
	return mod;
}

//std::string modifierToJsonString(Modifier* modifier) {
//	std::stringstream result("");
//	result <<
//	return result;
//}

Json::Value modifierToJsonObject( std::string name, Modifier* mod ) {
	Json::Value result;
	result["name"] = name;
	result["desc"] = mod->description();
	return result;
}

std::string Modifier::getModList(double timePerModifier) {
	Json::Value root;
	Json::Value & data = root["mods"];
//	std::stringstream result;
	
//	result << "{\"mods\":[";
//	std::map<std::string,std::function<Modifier*()>>::iterator it = factory.begin();
//	if (it != factory.end()) {
//		result << "{\"name\":\"" << it->first << "\", ";
//		it++;
//	}
//	for (; it != factory.end(); it++) {
//		result << "\",\"" << it->first;
//	}
//	result << "\"],\"voteTime\":" << (timePerModifier/3.0 - 0.5) << "}";
	int i = 0;
	for (std::map<std::string,std::function<Modifier*()>>::iterator it = factory.begin();
		 it != factory.end();
		 it++) {
		Modifier* tempMod = it->second();
		data[i++] = modifierToJsonObject( it->first, tempMod);
		delete tempMod;
	}
//	root["mods"] = data;
	
	Json::StreamWriterBuilder builder;
//	const std::string json_file = Json::writeString(builder, root);
//	std::cout << json_file << std::endl;
	
	return Json::writeString(builder, root);
}

double Modifier::lifetime() {
	return timer.runningTime() - pauseTimeAccumulator;
}

bool Modifier::tweak( DeviceEvent* event ) {
	return true;	// by default it is valid and we do nothing
}

