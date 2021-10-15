#include <cstdio>
#include <iostream>
#include <unistd.h>
#include <cmath>

#include <queue>

#include "chaos.h"
#include "sequence.h"


#define JOYSTICK_MIN (-128)
#define JOYSTICK_MAX (127)

#define JOYSTICK_UP (JOYSTICK_MIN/1.0)
#define JOYSTICK_DOWN (JOYSTICK_MAX/1.0)
#define JOYSTICK_LEFT (JOYSTICK_MIN/1.0)
#define JOYSTICK_RIGHT (JOYSTICK_MAX/1.0)

#define JOYSTICK_HOLD_TIME (1.0)

#define CRAFT_TIME (3.0)
#define HEAL_TIME (4.25)

using namespace Chaos;

class Action : public Chaos::Modifier {
private:
	bool tweak( DeviceEvent* event ) {
		return false;
	}
};

inline short joystickLimit( int input ) {
	return fmin( fmax( input, JOYSTICK_MIN), JOYSTICK_MAX);
}

class Forward : public Action {
public:
	static void regist() { Chaos::Modifier::factory["w"] = [](){ return new Forward();}; };
	const char* description() { return "Left joystick up"; }
	
	//	std::map<int,int> axisToValue;
	//std::map<int,int> offsetValue;
//	double sequenceTime;
//	bool inSequence;
//	double magnitudeSquared;
	
	//	double activeThreshold;
	
//	void begin() {
//		//		axisToValue[AXIS_LX] = dualshock->getState(AXIS_LX, TYPE_AXIS);
//		//		axisToValue[AXIS_LY] = dualshock->getState(AXIS_LY, TYPE_AXIS);
//
////		sequenceTime = 0;
////		inSequence = false;
//
//		//		activeThreshold = pow( JOYSTICK_MAX*0.2, 2);
//	}
	
	Forward() {
		this->totalLifespan = JOYSTICK_HOLD_TIME*2.0;
	}
	
	void update() {
		DeviceEvent event;	// Event to inject
		//event.type = TYPE_AXIS;
//		sequenceTime += timer.dTime();
//		if (!inSequence) {
//			if (sequenceTime > 6) {	// magnitude of 9 & max 127
//				inSequence = true;
//			}
//		}
		
		event.id = AXIS_LY;
		event.type = TYPE_AXIS;
		event.value = JOYSTICK_UP;
		dualshock->applyEvent(&event);
		
//		if (inSequence) {
//			if (sequenceTime >= 6  &&
//				sequenceTime <= 6.1 ) {
////				event.id = BUTTON_X;
////				event.type = TYPE_BUTTON;
////				event.value = 0;
////				dualshock->applyEvent(&event);
////				event.id = BUTTON_L2;	// stop aiming
////				dualshock->applyEvent(&event);
//				event.id = AXIS_LY;
//				event.type = TYPE_AXIS;
//				event.value = JOYSTICK_MAX;
//				dualshock->applyEvent(&event);
////				event.id = AXIS_LX;
////				event.value = 0;
////				dualshock->applyEvent(&event);
////				event.id = AXIS_L2;
////				event.value = JOYSTICK_MIN;
//				dualshock->applyEvent(&event);
//			} else if (sequenceTime > 6.1 &&
//					   sequenceTime <= 6.2 ) {
//				event.id = BUTTON_X;
//				event.type = TYPE_BUTTON;
//				event.value = 1;
//				dualshock->applyEvent(&event);
//				//printf("Dive!\n");
//			} else if (sequenceTime > 6.2) {
//				event.id = BUTTON_X;
//				event.type = TYPE_BUTTON;
//				event.value = 0;
//				dualshock->applyEvent(&event);
//				inSequence = false;
//				sequenceTime = 0;
//				//printf("Release!\n");
//			}
//		}
	}
	
	void finish() {
//		if (inSequence) {
//			DeviceEvent event = {0,0,TYPE_BUTTON,BUTTON_X};
//			dualshock->applyEvent(&event);
//			event = {0,0,TYPE_BUTTON,BUTTON_L2};
//			dualshock->applyEvent(&event);
		DeviceEvent event = { 0, 0, TYPE_AXIS, AXIS_LY};
		dualshock->applyEvent(&event);
//		}
		//		chaosEngine->fakePipelinedEvent( &event, me);
		//		event = {0,(short)axisToValue[AXIS_RY],TYPE_AXIS,AXIS_RY};
		//		chaosEngine->fakePipelinedEvent( &event, me);
	}
	
//	bool tweak( DeviceEvent* event ) {
//		return true;
//	}
};

class Backward : public Action {
public:
	static void regist() { Chaos::Modifier::factory["s"] = [](){ return new Backward();}; };
	const char* description() { return "Left joystick down"; }
	
	Backward() {
		this->totalLifespan = JOYSTICK_HOLD_TIME;
	}
	
	void update() {
		DeviceEvent event;	// Event to inject
		
		event.id = AXIS_LY;
		event.type = TYPE_AXIS;
		event.value = JOYSTICK_DOWN;
		dualshock->applyEvent(&event);
	
	}
	
	void finish() {
		DeviceEvent event = { 0, 0, TYPE_AXIS, AXIS_LY};
		dualshock->applyEvent(&event);
	}
};

class Right : public Action {
public:
	static void regist() { Chaos::Modifier::factory["d"] = [](){ return new Right();}; };
	const char* description() { return "Left joystick right"; }
	
	Right() {
		this->totalLifespan = JOYSTICK_HOLD_TIME;
	}
	
	void update() {
		DeviceEvent event;	// Event to inject
		
		event.id = AXIS_LX;
		event.type = TYPE_AXIS;
		event.value = JOYSTICK_RIGHT;
		dualshock->applyEvent(&event);
	}
	
	void finish() {
		DeviceEvent event = { 0, 0, TYPE_AXIS, AXIS_LX};
		dualshock->applyEvent(&event);
	}
};

class Left : public Action {
public:
	static void regist() { Chaos::Modifier::factory["a"] = [](){ return new Left();}; };
	const char* description() { return "Left joystick left"; }
	
	Left() {
		this->totalLifespan = JOYSTICK_HOLD_TIME;
	}
	
	void update() {
		DeviceEvent event;	// Event to inject
		
		event.id = AXIS_LX;
		event.type = TYPE_AXIS;
		event.value = JOYSTICK_LEFT;
		dualshock->applyEvent(&event);
	}
	
	void finish() {
		DeviceEvent event = { 0, 0, TYPE_AXIS, AXIS_LX};
		dualshock->applyEvent(&event);
	}
};



class Jump : public Action {
public:
	static void regist() { Chaos::Modifier::factory["x"] = [](){ return new Jump();}; };
	const char* description() { return "Jump/Climb.  X button"; }
	
	double sequenceTime;
	
	Jump() {
		this->totalLifespan = 0.5;
		sequenceTime = 0;
	}
	
	void update() {
		DeviceEvent event;	// Event to inject
		
		event.id = BUTTON_X;
		event.type = TYPE_BUTTON;
		if (sequenceTime < 0.25) {
			event.value = 1;
		} else if ( sequenceTime >= 0.25) {
			event.value = 0;
		}
		dualshock->applyEvent(&event);
		
		sequenceTime += timer.dTime();
	}
	
	void finish() {
		DeviceEvent event = { 0, 0, TYPE_BUTTON, BUTTON_X};
		dualshock->applyEvent(&event);
	}
};

class TriangleHold : public Action {
public:
	static void regist() { Chaos::Modifier::factory["holdt"] = [](){ return new TriangleHold();}; };
	const char* description() { return "Holds Triangle for 6 seconds"; }
	
	TriangleHold() {
		this->totalLifespan = 6.0;
	}
	
	void update() {
		DeviceEvent event;	// Event to inject
		
		event.id = BUTTON_TRIANGLE;
		event.type = TYPE_BUTTON;
		event.value = 1;
		dualshock->applyEvent(&event);
	}
	
	void finish() {
		DeviceEvent event = { 0, 0, TYPE_BUTTON, BUTTON_TRIANGLE};
		dualshock->applyEvent(&event);
	}
};

class ListenMode : public Action {
public:
	static void regist() { Chaos::Modifier::factory["listen"] = [](){ return new ListenMode();}; };
	const char* description() { return "Holds R1 for 4 seconds"; }
	
	ListenMode() {
		this->totalLifespan = 4.0;
	}
	
	void update() {
		DeviceEvent event;	// Event to inject
		
		event.id = BUTTON_R1;
		event.type = TYPE_BUTTON;
		event.value = 1;
		dualshock->applyEvent(&event);
	}
	
	void finish() {
		DeviceEvent event = { 0, 0, TYPE_BUTTON, BUTTON_R1};
		dualshock->applyEvent(&event);
	}
};

class Triangle : public Action {
public:
	static void regist() { Chaos::Modifier::factory["t"] = [](){ return new Triangle();}; };
	const char* description() { return "Triangle button press"; }
	
	double sequenceTime;
	
	enum States {
		STATE_IDLE,
		STATE_PRESS,
		STATE_RELEASE
	} state;
	
	Triangle() {
		this->totalLifespan = 1.5;
		sequenceTime = 0;
		state = STATE_IDLE;
	}
	
	void update() {
		DeviceEvent event;	// Event to inject
		event.id = BUTTON_TRIANGLE;
		event.type = TYPE_BUTTON;
		
		switch (state) {
			case STATE_IDLE:
				event.value = 1;
				dualshock->applyEvent(&event);
				state = STATE_PRESS;
				break;
			case STATE_PRESS:
				if (sequenceTime > 0.15) {
					event.value = 0;
					dualshock->applyEvent(&event);
					state = STATE_RELEASE;
				}
				break;
				
			default:
				break;
		}
		
		sequenceTime += timer.dTime();
	}
};

class Prone : public Action {
public:
	static void regist() { Chaos::Modifier::factory["p"] = [](){ return new Prone();}; };
	const char* description() { return "Hold Circle button"; }
	
	double sequenceTime;
	
	Prone() {
		this->totalLifespan = 1.0;
		sequenceTime = 0;
	}
	void update() {
		DeviceEvent event;	// Event to inject
		
		event.id = BUTTON_CIRCLE;
		event.type = TYPE_BUTTON;
		if (sequenceTime < 0.85) {
			event.value = 1;
		} else if ( sequenceTime >= 0.85) {
			event.value = 0;
		}
		dualshock->applyEvent(&event);
		
		sequenceTime += timer.dTime();
	}
	
	void finish() {
		DeviceEvent event = { 0, 0, TYPE_BUTTON, BUTTON_CIRCLE};
		dualshock->applyEvent(&event);
	}
};

class Melee : public Action {
public:
	static void regist() { Chaos::Modifier::factory["e"] = [](){ return new Melee();}; };
	const char* description() { return "Square button press"; }
	
	double sequenceTime;
	
	enum MeleeStates {
		MELEE_IDLE,
		MELEE_PRESS_1,
		MELEE_RELEASE_1,
		MELEE_PRESS_2,
		MELEE_RELEASE_2,
		MELEE_DONE
	} state;
	
	Melee() {
		this->totalLifespan = 0.36;
		sequenceTime = 0;
		state = MELEE_IDLE;
	}
	
	void update() {
		DeviceEvent event;	// Event to inject
		event.id = BUTTON_SQUARE;
		event.type = TYPE_BUTTON;
		
		
		if ( state == MELEE_IDLE ) {
			event.value = 1;
			dualshock->applyEvent(&event);
			state = MELEE_PRESS_1;
		} else if ( sequenceTime > 0.09 && state == MELEE_PRESS_1 ) {
			event.value = 0;
			dualshock->applyEvent(&event);
			state = MELEE_RELEASE_1;
		} else if ( sequenceTime > 0.18 && state == MELEE_RELEASE_1 ) {
			event.value = 1;
			dualshock->applyEvent(&event);
			state = MELEE_PRESS_2;
		} else if ( sequenceTime > 0.27 && state == MELEE_PRESS_2 ) {
			event.value = 0;
			dualshock->applyEvent(&event);
			state = MELEE_DONE;
		}
		
		sequenceTime += timer.dTime();
	}
	
	void finish() {
		DeviceEvent event = { 0, 0, TYPE_BUTTON, BUTTON_SQUARE};
		dualshock->applyEvent(&event);
	}
};

class Dodge : public Action {
public:
	static void regist() { Chaos::Modifier::factory["f"] = [](){ return new Dodge();}; };
	const char* description() { return "Presses L1"; }
	
	double sequenceTime;
	
	Dodge() {
		this->totalLifespan = 0.4;
		sequenceTime = 0;
	}
	
	void update() {
		DeviceEvent event;	// Event to inject
		
		event.id = BUTTON_L1;
		event.type = TYPE_BUTTON;
		
		if (sequenceTime < 0.15) {
			event.value = 1;
		} else if ( sequenceTime >= 0.15) {
			event.value = 0;
			
		}
		dualshock->applyEvent(&event);
		
		sequenceTime += timer.dTime();
	}
	
	void finish() {
		DeviceEvent event = { 0, 0, TYPE_BUTTON, BUTTON_L1};
		dualshock->applyEvent(&event);
	}
};

class NavigationAssistant : public Action {
public:
	static void regist() { Chaos::Modifier::factory["c"] = [](){ return new NavigationAssistant();}; };
	const char* description() { return "Activate Navigation Assistant (points camera forward)"; }
	
	double sequenceTime;
	
	NavigationAssistant() {
		this->totalLifespan = 1.0;
		sequenceTime = 0;
	}
	
	void update() {
		DeviceEvent event;	// Event to inject
		
		event.id = BUTTON_L3;
		event.type = TYPE_BUTTON;
		
		if (sequenceTime < 0.15) {
			event.value = 1;
		} else if ( sequenceTime >= 0.15) {
			event.value = 0;
			
		}
		dualshock->applyEvent(&event);
		
		sequenceTime += timer.dTime();
	}
};

class Swap : public Action {
public:
	static void regist() { Chaos::Modifier::factory["swap"] = [](){ return new Swap();}; };
	const char* description() { return "Swap Weapons (press R1)"; }
	
	double sequenceTime;
	
	Swap() {
		this->totalLifespan = 0.5;
		sequenceTime = 0;
	}
	
	void update() {
		DeviceEvent event;	// Event to inject
		
		event.id = BUTTON_R1;
		event.type = TYPE_BUTTON;
		
		if (sequenceTime < 0.15) {
			event.value = 1;
		} else if ( sequenceTime >= 0.15) {
			event.value = 0;
			
		}
		dualshock->applyEvent(&event);
		
		sequenceTime += timer.dTime();
	}
};

class Reload : public Action {
public:
	static void regist() { Chaos::Modifier::factory["r"] = [](){ return new Reload();}; };
	const char* description() { return "Presses R2 to reload/throw something"; }
	
	double sequenceTime;
	
	Reload() {
		this->totalLifespan = 0.4;
		sequenceTime = 0;
	}
	
	void update() {
		DeviceEvent event;	// Event to inject
		
		event.id = BUTTON_R2;
		event.type = TYPE_BUTTON;
		
		if (sequenceTime < 0.15) {
			event.value = 1;
			dualshock->applyEvent(&event);
			
			event.value = JOYSTICK_MAX;
			event.id = AXIS_R2;
			event.type = TYPE_AXIS;
			dualshock->applyEvent(&event);
			
		} else if ( sequenceTime >= 0.15) {
			event.value = 0;
			dualshock->applyEvent(&event);
			
			event.value = JOYSTICK_MIN;
			event.id = AXIS_R2;
			event.type = TYPE_AXIS;
			dualshock->applyEvent(&event);
		}
		
		sequenceTime += timer.dTime();
	}

};

class Shoot : public Action {
public:
	static void regist() { Chaos::Modifier::factory["shoot"] = [](){ return new Shoot();}; };
	const char* description() { return "Holds L2 then presses R2"; }
	
	double sequenceTime;
	
	Shoot() {
		this->totalLifespan = 1.5;
		sequenceTime = 0;
	}
	
	
	
	void update() {
		DeviceEvent event;	// Event to inject
		
		if (sequenceTime < 0.5) {
			event.value = 1;
			event.id = BUTTON_L2;
			event.type = TYPE_BUTTON;
			dualshock->applyEvent(&event);
			
			event.value = JOYSTICK_MAX;
			event.id = AXIS_L2;
			event.type = TYPE_AXIS;
			dualshock->applyEvent(&event);
		} else if ( sequenceTime >= 0.5 && sequenceTime < 1.25 && dualshock->getState(BUTTON_R2, TYPE_BUTTON) != 1) {
			event.value = 1;
			event.id = BUTTON_R2;
			event.type = TYPE_BUTTON;
			dualshock->applyEvent(&event);
			
			event.value = JOYSTICK_MAX;
			event.id = AXIS_R2;
			event.type = TYPE_AXIS;
			dualshock->applyEvent(&event);
		} else if ( sequenceTime >= 1.25 && sequenceTime < 1.35 && dualshock->getState(BUTTON_R2, TYPE_BUTTON) != 0) {
			event.value = 0;
			event.id = BUTTON_R2;
			event.type = TYPE_BUTTON;
			dualshock->applyEvent(&event);
			
			event.value = JOYSTICK_MIN;
			event.id = AXIS_R2;
			event.type = TYPE_AXIS;
			dualshock->applyEvent(&event);
			
		} else if (sequenceTime >= 1.35) {
			event.value = 0;
			event.id = BUTTON_L2;
			event.type = TYPE_BUTTON;
			dualshock->applyEvent(&event);
			
			event.value = JOYSTICK_MIN;
			event.id = AXIS_L2;
			event.type = TYPE_AXIS;
			dualshock->applyEvent(&event);
		}
		
		sequenceTime += timer.dTime();
	}
	
};

class Scopeshot : public Action {
public:
	static void regist() { Chaos::Modifier::factory["scopeshot"] = [](){ return new Scopeshot();}; };
	const char* description() { return "Holds L2 then presses x, followed by R2"; }
	
	double sequenceTime;
	
	Scopeshot() {
		this->totalLifespan = 2.0;
		sequenceTime = 0;
	}
	
	
	
	void update() {
		DeviceEvent event;	// Event to inject
		
		if (sequenceTime < 0.15) {
			event.value = 1;
			event.id = BUTTON_L2;
			event.type = TYPE_BUTTON;
			dualshock->applyEvent(&event);
			
			event.value = JOYSTICK_MAX;
			event.id = AXIS_L2;
			event.type = TYPE_AXIS;
			dualshock->applyEvent(&event);
		} else if ( sequenceTime >= 0.4 && sequenceTime < 1.35 && dualshock->getState(BUTTON_X, TYPE_BUTTON) == 0) {
			event.value = 1;
			event.id = BUTTON_X;
			event.type = TYPE_BUTTON;
			dualshock->applyEvent(&event);
		} else if ( sequenceTime >= 1.5 && sequenceTime < 1.65 && dualshock->getState(BUTTON_R2, TYPE_BUTTON) == 0) {
			event.value = 0;
			event.id = BUTTON_X;
			event.type = TYPE_BUTTON;
			dualshock->applyEvent(&event);
			
			event.value = 1;
			event.id = BUTTON_R2;
			event.type = TYPE_BUTTON;
			dualshock->applyEvent(&event);
			
			event.value = JOYSTICK_MAX;
			event.id = AXIS_R2;
			event.type = TYPE_AXIS;
			dualshock->applyEvent(&event);
		} else if ( sequenceTime >= 1.65 && sequenceTime < 1.9 && dualshock->getState(BUTTON_R2, TYPE_BUTTON) == 1) {
			event.value = 0;
			event.id = BUTTON_R2;
			event.type = TYPE_BUTTON;
			dualshock->applyEvent(&event);
			
			event.value = JOYSTICK_MIN;
			event.id = AXIS_R2;
			event.type = TYPE_AXIS;
			dualshock->applyEvent(&event);
			
		} else if (sequenceTime >= 1.9) {
			event.value = 0;
			event.id = BUTTON_L2;
			event.type = TYPE_BUTTON;
			dualshock->applyEvent(&event);
			
			event.value = JOYSTICK_MIN;
			event.id = AXIS_L2;
			event.type = TYPE_AXIS;
			dualshock->applyEvent(&event);
		}
		
		sequenceTime += timer.dTime();
	}
	
};


class Molly : public Action {
public:
	static void regist() { Chaos::Modifier::factory["molly"] = [](){ return new Molly();}; };
	const char* description() { return "Presses down on d-pad twice"; }
	
	double sequenceTime;
	int count;
	
	Molly() {
		this->totalLifespan = 0.55;
		sequenceTime = 0;
		count = 0;
	}
	
	void update() {
		DeviceEvent event;	// Event to inject
		
		event.id = AXIS_DY;
		event.type = TYPE_AXIS;
		if (sequenceTime < 0.15 && dualshock->getState(AXIS_DY, TYPE_AXIS) == 0) {
			event.value = 1;
			dualshock->applyEvent(&event);
		} else if ( sequenceTime >= 0.15 && dualshock->getState(AXIS_DY, TYPE_AXIS) == 1) {
			event.value = 0;
			dualshock->applyEvent(&event);
		} else if ( count < 1 && sequenceTime > 0.25 ) {
			count = 1;
			sequenceTime = 0;
		}
		
		sequenceTime += timer.dTime();
	}
};



class Smoke : public Action {
public:
	static void regist() { Chaos::Modifier::factory["smoke"] = [](){ return new Smoke();}; };
	const char* description() { return "Presses up on d-pad twice"; }
	
	double sequenceTime;
	int count;
	
	Smoke() {
		this->totalLifespan = 0.6;
		sequenceTime = 0;
		count = 0;
	}
	
	void update() {
		DeviceEvent event;	// Event to inject
		
		event.id = AXIS_DY;
		event.type = TYPE_AXIS;
		if (sequenceTime < 0.15 && dualshock->getState(AXIS_DY, TYPE_AXIS) != -1) {
			event.value = -1;
			dualshock->applyEvent(&event);
		} else if ( sequenceTime >= 0.15 && dualshock->getState(AXIS_DY, TYPE_AXIS) != 0) {
			event.value = 0;
			dualshock->applyEvent(&event);
		} else if ( count < 1 && sequenceTime > 0.25 ) {
			count = 1;
			sequenceTime = 0;
		}
		
		sequenceTime += timer.dTime();
	}
	
	void finish() {
//		DeviceEvent event = { 0, 0, TYPE_BUTTON, BUTTON_L1};
//		dualshock->applyEvent(&event);
	}
};

class Gun : public Action {
public:
	static void regist() { Chaos::Modifier::factory["gun"] = [](){ return new Gun();}; };
	const char* description() { return "Presses left on d-pad once"; }
	
	double sequenceTime;
	int count;
	
	Gun() {
		this->totalLifespan = 0.6;
		sequenceTime = 0;
		count = 0;
	}
	
	void update() {
		DeviceEvent event;	// Event to inject
		
		event.id = AXIS_DX;
		event.type = TYPE_AXIS;
		if (sequenceTime < 0.15 && dualshock->getState(AXIS_DY, TYPE_AXIS) == 0) {
			event.value = -1;
			dualshock->applyEvent(&event);
		} else if ( sequenceTime >= 0.15 && dualshock->getState(AXIS_DY, TYPE_AXIS) == 1) {
			event.value = 0;
			dualshock->applyEvent(&event);
		} else if ( count < 0 && sequenceTime > 0.3 ) {
			count = 1;
			sequenceTime = 0;
		}
		
		sequenceTime += timer.dTime();
	}
	
	void finish() {
//		DeviceEvent event = { 0, 0, TYPE_BUTTON, BUTTON_L1};
//		dualshock->applyEvent(&event);
	}
};

class Brick : public Action {
public:
	static void regist() { Chaos::Modifier::factory["brick"] = [](){ return new Brick();}; };
	const char* description() { return "Presses down on d-pad once"; }
	
	double sequenceTime;
	int count;
	
	Brick() {
		this->totalLifespan = 0.6;
		sequenceTime = 0;
		count = 0;
	}
	
	void update() {
		DeviceEvent event;	// Event to inject
		
		event.id = AXIS_DY;
		event.type = TYPE_AXIS;
		if (sequenceTime < 0.15 && dualshock->getState(AXIS_DY, TYPE_AXIS) == 0) {
			event.value = 1;
			dualshock->applyEvent(&event);
		} else if ( sequenceTime >= 0.15 && dualshock->getState(AXIS_DY, TYPE_AXIS) == 1) {
			event.value = 0;
			dualshock->applyEvent(&event);
		} else if ( count < 0 && sequenceTime > 0.3 ) {
			count = 1;
			sequenceTime = 0;
		}
		
		sequenceTime += timer.dTime();
	}
	
	void finish() {
//		DeviceEvent event = { 0, 0, TYPE_BUTTON, BUTTON_L1};
//		dualshock->applyEvent(&event);
	}
};

class Run : public Action {
public:
	static void regist() { Chaos::Modifier::factory["run"] = [](){ return new Run();}; };
	const char* description() { return "Presses and does not release L1"; }
	
	double sequenceTime;
	
	Run() {
		this->totalLifespan = 1.0;
		sequenceTime = 0;
	}
	
	
	void update() {
		DeviceEvent event;	// Event to inject
		
		event.id = BUTTON_L1;
		event.type = TYPE_BUTTON;
		
		if(sequenceTime < 0.1)
			event.value = 0;
		else
			event.value = 1;
		
		dualshock->applyEvent(&event);
		
		sequenceTime += timer.dTime();
	}
	
};

class Walk : public Action {
public:
	static void regist() { Chaos::Modifier::factory["walk"] = [](){ return new Walk();}; };
	const char* description() { return "Releases L1"; }
	
	double sequenceTime;
	
	Walk() {
		this->totalLifespan = 1.0;
		sequenceTime = 0;
	}
	
	void update() {
		DeviceEvent event;	// Event to inject
		
		event.value = 0;
		event.id = BUTTON_L1;
		event.type = TYPE_BUTTON;
		dualshock->applyEvent(&event);
	}
	
};


class CraftMolly : public Action {
public:
	static void regist() { Chaos::Modifier::factory["cmolly"] = [](){ return new CraftMolly();}; };
	const char* description() { return "Crafts a molly"; }
	
	double sequenceTime;
	
	enum CraftStates {
		CRAFT_IDLE,
		CRAFT_DY_PRESS_1,
		CRAFT_DY_RELEASE_1,
		CRAFT_DY_PRESS_2,
		CRAFT_DY_RELEASE_2,
		CRAFT_X_PRESS,
		CRAFT_X_RELEASE,
		CRAFT_CIRCLE_PRESS,
		CRAFT_DONE
	} state;
	
	CraftMolly() {
		this->totalLifespan = CRAFT_TIME + 0.95;
		sequenceTime = 0;
		state = CRAFT_IDLE;
	}
	
	void update() {
		DeviceEvent event;	// Event to inject
		
		event.id = AXIS_DY;
		event.type = TYPE_AXIS;
		
		switch (state) {
			case CRAFT_IDLE:
				event.value = 1;
				dualshock->applyEvent(&event);
				state = CRAFT_DY_PRESS_1;
				break;
			case CRAFT_DY_PRESS_1:
				if (sequenceTime > 0.15) {
					event.value = 0;
					dualshock->applyEvent(&event);
					state = CRAFT_DY_RELEASE_1;
				}
				break;
			case CRAFT_DY_RELEASE_1:
				if (sequenceTime > 0.3) {
					event.value = 1;
					dualshock->applyEvent(&event);
					state = CRAFT_DY_PRESS_2;
				}
				break;
			case CRAFT_DY_PRESS_2:
				if (sequenceTime > 0.45) {
					event.value = 0;
					dualshock->applyEvent(&event);
					state = CRAFT_DY_RELEASE_2;
				}
				break;
			case CRAFT_DY_RELEASE_2:
				if (sequenceTime > 0.6) {
					event.value = 1;
					event.type = TYPE_BUTTON;
					event.id = BUTTON_X;
					dualshock->applyEvent(&event);
					state = CRAFT_X_PRESS;
				}
				break;
			case CRAFT_X_PRESS:
				if (sequenceTime > CRAFT_TIME + 0.6) {
					event.value = 0;
					event.type = TYPE_BUTTON;
					event.id = BUTTON_X;
					dualshock->applyEvent(&event);
					state = CRAFT_X_RELEASE;
				}
				break;
			case CRAFT_X_RELEASE:
				if (sequenceTime > CRAFT_TIME + 0.75) {
					event.value = 1;
					event.type = TYPE_BUTTON;
					event.id = BUTTON_CIRCLE;
					dualshock->applyEvent(&event);
					state = CRAFT_CIRCLE_PRESS;
				}
				break;
			case CRAFT_CIRCLE_PRESS:
				if (sequenceTime > CRAFT_TIME + 0.9) {
					event.value = 0;
					event.type = TYPE_BUTTON;
					event.id = BUTTON_CIRCLE;
					dualshock->applyEvent(&event);
					state = CRAFT_DONE;
				}
				break;
				
			default:
				break;
		}
		
		sequenceTime += timer.dTime();
	}
	
	void finish() {
		DeviceEvent event = { 0, 0, TYPE_BUTTON, BUTTON_CIRCLE};
		dualshock->applyEvent(&event);
	}
};

class CraftSmoke : public Action {
public:
	static void regist() { Chaos::Modifier::factory["csmoke"] = [](){ return new CraftSmoke();}; };
	const char* description() { return "Crafts smoke bombs"; }
	
	double sequenceTime;
	
	enum CraftStates {
		CRAFT_IDLE,
		CRAFT_DY_PRESS_1,
		CRAFT_DY_RELEASE_1,
		CRAFT_DY_PRESS_2,
		CRAFT_DY_RELEASE_2,
		CRAFT_X_PRESS,
		CRAFT_X_RELEASE,
		CRAFT_CIRCLE_PRESS,
		CRAFT_DONE
	} state;
	
	CraftSmoke() {
		this->totalLifespan = CRAFT_TIME + 0.95;
		sequenceTime = 0;
		state = CRAFT_IDLE;
	}
	
	void update() {
		DeviceEvent event;	// Event to inject
		
		event.id = AXIS_DY;
		event.type = TYPE_AXIS;
		
		switch (state) {
			case CRAFT_IDLE:
				event.value = -1;
				dualshock->applyEvent(&event);
				state = CRAFT_DY_PRESS_1;
				break;
			case CRAFT_DY_PRESS_1:
				if (sequenceTime > 0.15) {
					event.value = 0;
					dualshock->applyEvent(&event);
					state = CRAFT_DY_RELEASE_1;
				}
				break;
			case CRAFT_DY_RELEASE_1:
				if (sequenceTime > 0.3) {
					event.value = -1;
					dualshock->applyEvent(&event);
					state = CRAFT_DY_PRESS_2;
				}
				break;
			case CRAFT_DY_PRESS_2:
				if (sequenceTime > 0.45) {
					event.value = 0;
					dualshock->applyEvent(&event);
					state = CRAFT_DY_RELEASE_2;
				}
				break;
			case CRAFT_DY_RELEASE_2:
				if (sequenceTime > 0.6) {
					event.value = -1;
					event.type = TYPE_BUTTON;
					event.id = BUTTON_X;
					dualshock->applyEvent(&event);
					state = CRAFT_X_PRESS;
				}
				break;
			case CRAFT_X_PRESS:
				if (sequenceTime > CRAFT_TIME + 0.6) {
					event.value = 0;
					event.type = TYPE_BUTTON;
					event.id = BUTTON_X;
					dualshock->applyEvent(&event);
					state = CRAFT_X_RELEASE;
				}
				break;
			case CRAFT_X_RELEASE:
				if (sequenceTime > CRAFT_TIME + 0.75) {
					event.value = 1;
					event.type = TYPE_BUTTON;
					event.id = BUTTON_CIRCLE;
					dualshock->applyEvent(&event);
					state = CRAFT_CIRCLE_PRESS;
				}
				break;
			case CRAFT_CIRCLE_PRESS:
				if (sequenceTime > CRAFT_TIME + 0.9) {
					event.value = 0;
					event.type = TYPE_BUTTON;
					event.id = BUTTON_CIRCLE;
					dualshock->applyEvent(&event);
					state = CRAFT_DONE;
				}
				break;
				
			default:
				break;
		}
		
		sequenceTime += timer.dTime();
	}
	
	void finish() {
		DeviceEvent event = { 0, 0, TYPE_BUTTON, BUTTON_CIRCLE};
		dualshock->applyEvent(&event);
	}
};

class CraftTrap : public Action {
public:
	static void regist() { Chaos::Modifier::factory["ctrap"] = [](){ return new CraftTrap();}; };
	const char* description() { return "Crafts traps"; }
	
	double sequenceTime;
	
	enum CraftStates {
		CRAFT_IDLE,
		CRAFT_DY_PRESS_1,
		CRAFT_DY_RELEASE_1,
		CRAFT_DY_PRESS_2,
		CRAFT_DY_RELEASE_2,
		CRAFT_DY_PRESS_3,
		CRAFT_DY_RELEASE_3,
		CRAFT_X_PRESS,
		CRAFT_X_RELEASE,
		CRAFT_CIRCLE_PRESS,
		CRAFT_DONE
	} state;
	
	CraftTrap() {
		this->totalLifespan = CRAFT_TIME + 1.25;
		sequenceTime = 0;
		state = CRAFT_IDLE;
	}
	
	void update() {
		DeviceEvent event;	// Event to inject
		
		event.id = AXIS_DY;
		event.type = TYPE_AXIS;
		
		switch (state) {
			case CRAFT_IDLE:
				event.value = -1;
				dualshock->applyEvent(&event);
				state = CRAFT_DY_PRESS_1;
				break;
			case CRAFT_DY_PRESS_1:
				if (sequenceTime > 0.15) {
					event.value = 0;
					dualshock->applyEvent(&event);
					state = CRAFT_DY_RELEASE_1;
				}
				break;
			case CRAFT_DY_RELEASE_1:
				if (sequenceTime > 0.3) {
					event.value = -1;
					dualshock->applyEvent(&event);
					state = CRAFT_DY_PRESS_2;
				}
				break;
			case CRAFT_DY_PRESS_2:
				if (sequenceTime > 0.45) {
					event.value = 0;
					dualshock->applyEvent(&event);
					state = CRAFT_DY_RELEASE_2;
				}
				break;
			case CRAFT_DY_RELEASE_2:
				if (sequenceTime > 0.6) {
					event.value = -1;
					dualshock->applyEvent(&event);
					state = CRAFT_DY_PRESS_3;
				}
				break;
			case CRAFT_DY_PRESS_3:
				if (sequenceTime > 0.75) {
					event.value = 0;
					dualshock->applyEvent(&event);
					state = CRAFT_DY_RELEASE_3;
				}
				break;
			case CRAFT_DY_RELEASE_3:
				if (sequenceTime > 0.9) {
					event.value = -1;
					event.type = TYPE_BUTTON;
					event.id = BUTTON_X;
					dualshock->applyEvent(&event);
					state = CRAFT_X_PRESS;
				}
				break;
			case CRAFT_X_PRESS:
				if (sequenceTime > CRAFT_TIME + 0.9) {
					event.value = 0;
					event.type = TYPE_BUTTON;
					event.id = BUTTON_X;
					dualshock->applyEvent(&event);
					state = CRAFT_X_RELEASE;
				}
				break;
			case CRAFT_X_RELEASE:
				if (sequenceTime > CRAFT_TIME + 1.05) {
					event.value = 1;
					event.type = TYPE_BUTTON;
					event.id = BUTTON_CIRCLE;
					dualshock->applyEvent(&event);
					state = CRAFT_CIRCLE_PRESS;
				}
				break;
			case CRAFT_CIRCLE_PRESS:
				if (sequenceTime > CRAFT_TIME + 1.2) {
					event.value = 0;
					event.type = TYPE_BUTTON;
					event.id = BUTTON_CIRCLE;
					dualshock->applyEvent(&event);
					state = CRAFT_DONE;
				}
				break;
				
			default:
				break;
		}
		
		sequenceTime += timer.dTime();
	}
	
	void finish() {
		DeviceEvent event = { 0, 0, TYPE_BUTTON, BUTTON_CIRCLE};
		dualshock->applyEvent(&event);
	}
};

class Trap : public Action {
public:
	static void regist() { Chaos::Modifier::factory["trap"] = [](){ return new Trap();}; };
	const char* description() { return "Selects traps from the inventory"; }
	
	double sequenceTime;
	
	enum CraftStates {
		CRAFT_IDLE,
		CRAFT_DY_PRESS_1,
		CRAFT_DY_RELEASE_1,
		CRAFT_DY_PRESS_2,
		CRAFT_DY_RELEASE_2,
		CRAFT_DY_PRESS_3,
		CRAFT_DY_RELEASE_3,
		CRAFT_DONE
	} state;
	
	Trap() {
		this->totalLifespan = 2.3;
		sequenceTime = 0;
		state = CRAFT_IDLE;
	}
	
	void update() {
		DeviceEvent event;	// Event to inject
		
		event.id = AXIS_DY;
		event.type = TYPE_AXIS;
		
		switch (state) {
			case CRAFT_IDLE:
				event.value = -1;
				dualshock->applyEvent(&event);
				state = CRAFT_DY_PRESS_1;
				break;
			case CRAFT_DY_PRESS_1:
				if (sequenceTime > 0.15) {
					event.value = 0;
					dualshock->applyEvent(&event);
					state = CRAFT_DY_RELEASE_1;
				}
				break;
			case CRAFT_DY_RELEASE_1:
				if (sequenceTime > 0.3) {
					event.value = -1;
					dualshock->applyEvent(&event);
					state = CRAFT_DY_PRESS_2;
				}
				break;
			case CRAFT_DY_PRESS_2:
				if (sequenceTime > 0.45) {
					event.value = 0;
					dualshock->applyEvent(&event);
					state = CRAFT_DY_RELEASE_2;
				}
				break;
			case CRAFT_DY_RELEASE_2:
				if (sequenceTime > 0.6) {
					event.value = -1;
					dualshock->applyEvent(&event);
					state = CRAFT_DY_PRESS_3;
				}
				break;
			case CRAFT_DY_PRESS_3:
				if (sequenceTime > 0.75) {
					event.value = 0;
					dualshock->applyEvent(&event);
					state = CRAFT_DONE;
				}
				break;
				
			default:
				break;
		}
		
		sequenceTime += timer.dTime();
	}
	
	void finish() {
		DeviceEvent event = { 0, 0, TYPE_BUTTON, BUTTON_X};
		dualshock->applyEvent(&event);
	}
};

class CraftHeal : public Action {
public:
	static void regist() { Chaos::Modifier::factory["cheal"] = [](){ return new CraftHeal();}; };
	const char* description() { return "Crafts a health kit"; }
	
	double sequenceTime;
	
	enum CraftStates {
		CRAFT_IDLE,
		CRAFT_DY_PRESS_1,
		CRAFT_DY_RELEASE_1,
		CRAFT_X_PRESS,
		CRAFT_X_RELEASE,
		CRAFT_CIRCLE_PRESS,
		CRAFT_DONE
	} state;
	
	CraftHeal() {
		this->totalLifespan = CRAFT_TIME + 0.75;
		sequenceTime = 0;
		state = CRAFT_IDLE;
	}
	
	void update() {
		DeviceEvent event;	// Event to inject
		
		event.id = AXIS_DY;
		event.type = TYPE_AXIS;
		
		switch (state) {
			case CRAFT_IDLE:
				event.value = -1;
				dualshock->applyEvent(&event);
				state = CRAFT_DY_PRESS_1;
				break;
			case CRAFT_DY_PRESS_1:
				if (sequenceTime > 0.15) {
					event.value = 0;
					dualshock->applyEvent(&event);
					state = CRAFT_DY_RELEASE_1;
				}
				break;
			case CRAFT_DY_RELEASE_1:
				if (sequenceTime > 0.3) {
					event.value = 1;
					event.type = TYPE_BUTTON;
					event.id = BUTTON_X;
					dualshock->applyEvent(&event);
					state = CRAFT_X_PRESS;
				}
				break;
			case CRAFT_X_PRESS:
				if (sequenceTime > CRAFT_TIME + 0.3) {
					event.value = 0;
					event.type = TYPE_BUTTON;
					event.id = BUTTON_X;
					dualshock->applyEvent(&event);
					state = CRAFT_X_RELEASE;
				}
				break;
			case CRAFT_X_RELEASE:
				if (sequenceTime > CRAFT_TIME + 0.45) {
					event.value = 1;
					event.type = TYPE_BUTTON;
					event.id = BUTTON_CIRCLE;
					dualshock->applyEvent(&event);
					state = CRAFT_CIRCLE_PRESS;
				}
				break;
			case CRAFT_CIRCLE_PRESS:
				if (sequenceTime > CRAFT_TIME + 0.6) {
					event.value = 0;
					event.type = TYPE_BUTTON;
					event.id = BUTTON_CIRCLE;
					dualshock->applyEvent(&event);
					state = CRAFT_DONE;
				}
				break;
				
			default:
				break;
		}
		
		sequenceTime += timer.dTime();
	}
	
	void finish() {
		DeviceEvent event = { 0, 0, TYPE_BUTTON, BUTTON_CIRCLE};
		dualshock->applyEvent(&event);
	}
};

class Heal : public Action {
public:
	static void regist() { Chaos::Modifier::factory["heal"] = [](){ return new Heal();}; };
	const char* description() { return "Use a medkit"; }
	
	double sequenceTime;
	
	enum CraftStates {
		CRAFT_IDLE,
		CRAFT_DY_PRESS_1,
		CRAFT_DY_RELEASE_1,
		CRAFT_X_PRESS,
		CRAFT_X_RELEASE,
		CRAFT_DONE
	} state;
	
	Heal() {
		this->totalLifespan = HEAL_TIME + 0.35;
		sequenceTime = 0;
		state = CRAFT_IDLE;
	}
	
	void update() {
		DeviceEvent event;	// Event to inject
		
		event.id = AXIS_DY;
		event.type = TYPE_AXIS;
		
		switch (state) {
			case CRAFT_IDLE:
				event.value = -1;
				dualshock->applyEvent(&event);
				state = CRAFT_DY_PRESS_1;
				break;
			case CRAFT_DY_PRESS_1:
				if (sequenceTime > 0.15) {
					event.value = 0;
					dualshock->applyEvent(&event);
					state = CRAFT_DY_RELEASE_1;
				}
				break;
			case CRAFT_DY_RELEASE_1:
				if (sequenceTime > 0.3) {
					event.value = 1;
					event.type = TYPE_BUTTON;
					event.id = BUTTON_R2;
					dualshock->applyEvent(&event);
					event.value = JOYSTICK_MAX;
					event.type = TYPE_AXIS;
					event.id = AXIS_R2;
					dualshock->applyEvent(&event);
					state = CRAFT_X_PRESS;
				}
				break;
			case CRAFT_X_PRESS:
				if (sequenceTime > HEAL_TIME + 0.3) {
					event.value = 0;
					event.type = TYPE_BUTTON;
					event.id = BUTTON_R2;
					dualshock->applyEvent(&event);
					event.value = JOYSTICK_MIN;
					event.type = TYPE_AXIS;
					event.id = AXIS_R2;
					dualshock->applyEvent(&event);
					state = CRAFT_DONE;
				}
				break;
				
			default:
				break;
		}
		
		sequenceTime += timer.dTime();
	}
	
	void finish() {
		DeviceEvent event = { 0, 0, TYPE_BUTTON, BUTTON_R2};
		dualshock->applyEvent(&event);
		event = { 0, JOYSTICK_MIN, TYPE_AXIS, AXIS_R2};
		dualshock->applyEvent(&event);
	}
};


class SkipCut : public Chaos::Modifier {
public:
	static void regist() { Chaos::Modifier::factory["skip"] = [](){return new SkipCut();}; };
	const char* description() { return "Performs a Cutscene Skip (if available)"; };
	
	void begin() {

		Chaos::Sequence sequence;
		sequence.disablejoysticks();
		sequence.addButtonPress( BUTTON_OPTIONS );
//		sequence.addAxisPress( AXIS_DY, -1);
//		sequence.addAxisPress( AXIS_DY, -1);
		sequence.addButtonPress( BUTTON_X);
		sequence.addAxisPress( AXIS_DX, -1);
		sequence.addButtonPress( BUTTON_X);
		sequence.addButtonPress( BUTTON_CIRCLE);
		sequence.addButtonPress( BUTTON_CIRCLE);
		sequence.addButtonPress( BUTTON_CIRCLE);
//		sequence.addTimeDelay(3000);
		sequence.send(dualshock);
		
	}

};

int main(int argc, char** argv) {
	std::cout << "Welcome to Twitch Plays" << std::endl;
	
	ControllerRaw dualshock;
	dualshock.initialize();
	dualshock.start();
	
	// Register the mods:
	Forward::regist();
	Backward::regist();
	Right::regist();
	Left::regist();
	
	Jump::regist();
	TriangleHold::regist();
	ListenMode::regist();
	Triangle::regist();
	Prone::regist();
	Melee::regist();
	Dodge::regist();
	
	NavigationAssistant::regist();
	
	Swap::regist();
	Reload::regist();
	Shoot::regist();
	Scopeshot::regist();
	
	Molly::regist();
	Smoke::regist();
	Gun::regist();
	Brick::regist();
	
	Run::regist();
	Walk::regist();
	
	CraftMolly::regist();
	CraftSmoke::regist();
	CraftHeal::regist();
	CraftTrap::regist();
	
	Trap::regist();
	Heal::regist();
	
	SkipCut::regist();
	
	int i = 1;
	printf("Registered Modifiers: \n");
	for (auto const& it : Chaos::Modifier::factory) {
		//std::cout << "Registered Mod: " << it.first << std::endl;
		Chaos::Modifier* tempMod = it.second();
		printf("%3d: %s -- %s\n", i++, it.first.c_str(), tempMod->description());
		delete  tempMod;
	}
	
	Chaos::Engine chaosEngine(&dualshock);
	chaosEngine.start();
	
	double timePerModifier = 30.0;
	chaosEngine.setTimePerModifier(timePerModifier);
	
	std::string reply = Chaos::Modifier::getModList(timePerModifier);
	while(1) {
		chaosEngine.setInterfaceReply( reply );
		usleep(10000000);
	}
	
	return 0;
}
