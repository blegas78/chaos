#ifndef MODIFIER_H
#define MODIFIER_H

#include <mogi/math/systems.h>

#include <string>
#include <functional>

#include "controller.h"

namespace Chaos {


class Engine;

class Modifier {
friend Engine;
//friend Modifier;
protected:
	Modifier();
	
	Mogi::Math::Time timer;
	Controller* dualshock;
	Engine* chaosEngine;
	
	Modifier* me; // represents "this" except in cases where there is a parent modifier
	
	double pauseTimeAccumulator;
	
	double totalLifespan; // Designates a custom lifespan if necesary
	
	//void _update(bool isPaused);	// ChaosEngine call this, which then calls virtual update();
	
public:
	
	static std::map<std::string, std::function<Modifier*()>> factory;
	static Modifier* build( const std::string& name );
	static std::string getModList( double timePerModifier );
	
	void setDualshock(Controller* dualshock);
	void setChaosEngine(Engine* chaosEngine);
	
	void _update(bool isPaused);	// ChaosEngine call this, which then calls virtual update();
	
	virtual void begin();	// called when first instantiated
	virtual void update();	// called regularly
	virtual void finish();	// called just
	virtual const char* description(); // A short description of this mod, for Twitch bot response
	
	double lifetime();
	double lifespan();
	
	virtual bool tweak( DeviceEvent* event );
	
	void setParentModifier(Modifier* parent);
};

};

#endif
