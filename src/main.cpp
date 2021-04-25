#include <cstdio>
#include <iostream>
#include <unistd.h>
#include <cmath>

#include "chaos.h"

#define JOYSTICK_MIN (-128)
#define JOYSTICK_MAX (127)

inline short joystickLimit( int input ) {
	return fmin( fmax( input, JOYSTICK_MIN), JOYSTICK_MAX);
}

class Inverted : public Modifier {
public:
	static void regist() { Modifier::factory["Inverted"] = [](){return new Inverted();}; };
	bool tweak( DeviceEvent* event ) {
		if ((event->id == AXIS_RY ) && event->type == TYPE_AXIS) {
			event->value = -((int)event->value+1) ;
		}
		return true;
	}
};

// Suggestion from protoxin:
class Moonwalk : public Modifier {
public:
	static void regist() { Modifier::factory["Moonwalk"] = [](){return new Moonwalk();}; };
	bool tweak( DeviceEvent* event ) {
		if ((event->id == AXIS_LY ) && event->type == TYPE_AXIS) {
			event->value = -((int)event->value+1) ;
		}
		return true;
	}
};

class MeleeOnly : public Modifier {
public:
	static void regist() { Modifier::factory["Melee Only"] = [](){return new MeleeOnly();}; };
	bool tweak( DeviceEvent* event ) {
		if (event->id == BUTTON_R2 && event->type == TYPE_BUTTON) {
			event->value = 0;
		}
		if (event->id == AXIS_R2 && event->type == TYPE_AXIS) {
			event->value = JOYSTICK_MIN;
		}
		return true;
	}
};

class NoMelee : public Modifier {
public:
	static void regist() { Modifier::factory["No Melee"] = [](){return new NoMelee();}; };
	bool tweak( DeviceEvent* event ) {
		if (event->id == BUTTON_SQUARE && event->type == TYPE_BUTTON) {
			event->value = 0;
		}
		return true;
	}
};

class Pacifist: public Modifier {
public:
	static void regist() { Modifier::factory["Pacifist%"] = [](){return new Pacifist();}; };
	bool tweak( DeviceEvent* event ) {
		if (event->type == TYPE_BUTTON &&
			(event->id == BUTTON_SQUARE ||	// disable Melee
			 (event->id == BUTTON_R2 && dualshock->getState(BUTTON_L2, TYPE_BUTTON) == 1) ) ) {	// Disable Shooting, but not reload
			event->value = 0;
		}
		return true;
	}
};

class NoTriangle : public Modifier {
public:
	static void regist() { Modifier::factory["No Triangle"] = [](){return new NoTriangle();}; };
	bool tweak( DeviceEvent* event ) {
		if (event->id == BUTTON_TRIANGLE && event->type == TYPE_BUTTON) {
			event->value = 0;
		}
		return true;
	}
};

class NoClimbJump : public Modifier {
public:
	static void regist() { Modifier::factory["No Climbing/Jumping"] = [](){return new NoClimbJump();}; };
	bool tweak( DeviceEvent* event ) {
		if (event->id == BUTTON_X && event->type == TYPE_BUTTON) {
			event->value = 0;
		}
		return true;
	}
};

class NoAiming : public Modifier {
public:
	static void regist() { Modifier::factory["No Aiming"] = [](){return new NoAiming();}; };
	bool tweak( DeviceEvent* event ) {
		if (event->id == BUTTON_L2 && event->type == TYPE_BUTTON) {
			event->value = 0;
		}
		return true;
	}
};

class NoScoping : public Modifier {
public:
	static void regist() { Modifier::factory["No Scoping"] = [](){return new NoScoping();}; };
	bool tweak( DeviceEvent* event ) {
		if (event->id == BUTTON_X && event->type == TYPE_BUTTON) {
			if (dualshock->getState(BUTTON_L2, TYPE_BUTTON)) {	// only disable X when aiming
				event->value = 0;
			}
		}
		return true;
	}
};

class NoShoulderSwap : public Modifier {
public:
	static void regist() { Modifier::factory["No Shoulder Swap"] = [](){return new NoShoulderSwap();}; };
	bool tweak( DeviceEvent* event ) {
		if (event->id == BUTTON_SQUARE && event->type == TYPE_BUTTON) {
			if (dualshock->getState(BUTTON_L2, TYPE_BUTTON)) {	// only disable X when aiming
				event->value = 0;
			}
		}
		return true;
	}
};

class NoReloading : public Modifier {
	bool dxLastPessed = true;
	bool aimHeld = false;
public:
	static void regist() { Modifier::factory["No Reloading"] = [](){return new NoReloading();}; };
	bool tweak( DeviceEvent* event ) {
		if (event->id == AXIS_DX && event->type == TYPE_AXIS && event->id != 0) {
			dxLastPessed = true;
		} else if (event->id == AXIS_DY && event->type == TYPE_AXIS && event->id != 0) {
			dxLastPessed = false;
		} else if (event->id == BUTTON_L2 && event->type == TYPE_BUTTON ) {
			aimHeld = event->value;
		} else if (!aimHeld && event->id == BUTTON_R2 && event->type == TYPE_BUTTON && dxLastPessed) {
			event->value = 0;
		}
		return true;
	}
};

class DisableCircle : public Modifier {
public:
	static void regist() { Modifier::factory["No Crouch/Prone"] = [](){return new DisableCircle();}; };
	void begin() {
		DeviceEvent event = {0,0,TYPE_BUTTON, BUTTON_CIRCLE};
		dualshock->applyEvent(&event);
	}
	bool tweak( DeviceEvent* event ) {
		if (event->id == BUTTON_CIRCLE && event->type == TYPE_BUTTON) {
			//return false;
			event->value = 0;
		}
		return true;
	}
};

class ForceProne : public Modifier {
public:
	static void regist() { Modifier::factory["Keep Proning"] = [](){return new ForceProne();}; };
	double pressTime;
	void begin() {
		DeviceEvent event = {0,1,TYPE_BUTTON, BUTTON_CIRCLE};
		dualshock->applyEvent(&event);
		pressTime = 0;
	}
	void update() {
		pressTime += timer.dTime();
		if ( pressTime > 2.0  && dualshock->getState(BUTTON_CIRCLE, TYPE_BUTTON) ) {
			DeviceEvent event = {0,0,TYPE_BUTTON, BUTTON_CIRCLE};
			dualshock->applyEvent(&event);
			pressTime = 0;
		} else if( pressTime > 2.0 && !dualshock->getState(BUTTON_CIRCLE, TYPE_BUTTON) ) {
			DeviceEvent event = {0,1,TYPE_BUTTON, BUTTON_CIRCLE};
			dualshock->applyEvent(&event);
			pressTime = 0;
		}
	}
	void finish() {
		DeviceEvent event = {0,0,TYPE_BUTTON, BUTTON_CIRCLE};
		dualshock->applyEvent(&event);
	}
	bool tweak( DeviceEvent* event ) {
		if (event->id == BUTTON_CIRCLE && event->type == TYPE_BUTTON) {
			//event->value = 1;
		}
		return true;
	}
};

class FactionsPro : public Modifier {
public:
	static void regist() { Modifier::factory["Factions Pro"] = [](){return new FactionsPro();}; };
	double pressTime;
	void begin() {
		DeviceEvent event = {0,1,TYPE_BUTTON, BUTTON_CIRCLE};
		dualshock->applyEvent(&event);
		pressTime = 0;
	}
	void update() {
		pressTime += timer.dTime();
		if ( pressTime > 0.1  && dualshock->getState(BUTTON_CIRCLE, TYPE_BUTTON) ) {
			DeviceEvent event = {0,0,TYPE_BUTTON, BUTTON_CIRCLE};
			dualshock->applyEvent(&event);
			pressTime = 0;
		} else if( pressTime > 0.7 && !dualshock->getState(BUTTON_CIRCLE, TYPE_BUTTON) ) {
			DeviceEvent event = {0,1,TYPE_BUTTON, BUTTON_CIRCLE};
			dualshock->applyEvent(&event);
			pressTime = 0;
		}
	}
	void finish() {
		DeviceEvent event = {0,0,TYPE_BUTTON, BUTTON_CIRCLE};
		dualshock->applyEvent(&event);
	}
	bool tweak( DeviceEvent* event ) {
		if (event->id == BUTTON_CIRCLE && event->type == TYPE_BUTTON) {
			//event->value = 1;
		}
		return true;
	}
};

class Disco : public Modifier {
public:
	static void regist() { Modifier::factory["Anthony Caliber"] = [](){return new Disco();}; };
	double pressTime;
	int toggleCount;
	void begin() {
		DeviceEvent event = {0,1,TYPE_BUTTON, BUTTON_R3};
		dualshock->applyEvent(&event);
		pressTime = 0;
		toggleCount = 0;
	}
	void update() {
		pressTime += timer.dTime();
		if (toggleCount < 8) {
			if ( pressTime > 0.2/3.0  && dualshock->getState(BUTTON_R3, TYPE_BUTTON) ) {
				DeviceEvent event = {0,0,TYPE_BUTTON, BUTTON_R3};
				dualshock->applyEvent(&event);
				pressTime = 0;
				toggleCount++;
			} else if( pressTime > 0.26875/3.0 && !dualshock->getState(BUTTON_R3, TYPE_BUTTON) ) {
				DeviceEvent event = {0,1,TYPE_BUTTON, BUTTON_R3};
				dualshock->applyEvent(&event);
				pressTime = 0;
			}
		} else if (pressTime > 4.0) {
			toggleCount = 0;
			pressTime = 0;
		}
	}
	void finish() {
		DeviceEvent event = {0,0,TYPE_BUTTON, BUTTON_R3};
		dualshock->applyEvent(&event);
	}
	bool tweak( DeviceEvent* event ) {
		return true;
	}
};

class KeepJumping : public Modifier {
public:
	static void regist() { Modifier::factory["Keep Jumping"] = [](){return new KeepJumping();}; };
	double pressTime;
	void begin() {
		DeviceEvent event = {0,1,TYPE_BUTTON, BUTTON_X};
		dualshock->applyEvent(&event);
		pressTime = 0;
	}
	void update() {
		pressTime += timer.dTime();
		if ( pressTime > 0.1  && dualshock->getState(BUTTON_X, TYPE_BUTTON) ) {
			DeviceEvent event = {0,0,TYPE_BUTTON, BUTTON_X};
			dualshock->applyEvent(&event);
			pressTime = 0;
		} else if( pressTime > 2.0 && !dualshock->getState(BUTTON_X, TYPE_BUTTON) ) {
			DeviceEvent event = {0,1,TYPE_BUTTON, BUTTON_X};
			dualshock->applyEvent(&event);
			pressTime = 0;
		}
	}
	void finish() {
		DeviceEvent event = {0,0,TYPE_BUTTON, BUTTON_X};
		dualshock->applyEvent(&event);
	}
	bool tweak( DeviceEvent* event ) {
		return true;
	}
};

class RachyFlail : public Modifier {
public:
	static void regist() { Modifier::factory["Rachy Flail"] = [](){return new RachyFlail();}; };
	double pressTime;
	void begin() {
		DeviceEvent event = {0,1,TYPE_BUTTON, BUTTON_SQUARE};
		dualshock->applyEvent(&event);
		pressTime = 0;
	}
	void update() {
		pressTime += timer.dTime();
		if ( pressTime > 0.1  && dualshock->getState(BUTTON_SQUARE, TYPE_BUTTON) ) {
			DeviceEvent event = {0,0,TYPE_BUTTON, BUTTON_SQUARE};
			dualshock->applyEvent(&event);
			pressTime = 0;
		} else if( pressTime > 1.1 && !dualshock->getState(BUTTON_SQUARE, TYPE_BUTTON) ) {
			DeviceEvent event = {0,1,TYPE_BUTTON, BUTTON_SQUARE};
			dualshock->applyEvent(&event);
			pressTime = 0;
		}
	}
	void finish() {
		DeviceEvent event = {0,0,TYPE_BUTTON, BUTTON_SQUARE};
		dualshock->applyEvent(&event);
	}
	bool tweak( DeviceEvent* event ) {
		return true;
	}
};

class KeepDodging : public Modifier {
public:
	static void regist() { Modifier::factory["Keep Dodging"] = [](){return new KeepDodging();}; };
	double pressTime;
	void begin() {
		DeviceEvent event = {0,1,TYPE_BUTTON, BUTTON_L1};
		dualshock->applyEvent(&event);
		pressTime = 0;
	}
	void update() {
		pressTime += timer.dTime();
		if ( pressTime > 0.1  && dualshock->getState(BUTTON_L1, TYPE_BUTTON) ) {
			DeviceEvent event = {0,0,TYPE_BUTTON, BUTTON_L1};
			dualshock->applyEvent(&event);
			pressTime = 0;
		} else if( pressTime > 2.0 && !dualshock->getState(BUTTON_L1, TYPE_BUTTON) ) {
			DeviceEvent event = {0,1,TYPE_BUTTON, BUTTON_L1};
			dualshock->applyEvent(&event);
			pressTime = 0;
		}
	}
	void finish() {
		DeviceEvent event = {0,0,TYPE_BUTTON, BUTTON_L1};
		dualshock->applyEvent(&event);
	}
	bool tweak( DeviceEvent* event ) {
		return true;
	}
};

class PeriodicListenMode : public Modifier {
public:
	static void regist() { Modifier::factory["Periodic Listen Mode"] = [](){return new PeriodicListenMode();}; };
	double pressTime;
	void begin() {
		DeviceEvent event = {0,1,TYPE_BUTTON, BUTTON_R1};
		dualshock->applyEvent(&event);
		pressTime = 0;
	}
	void update() {
		pressTime += timer.dTime();
		if ( pressTime > 3.0  && dualshock->getState(BUTTON_R1, TYPE_BUTTON) ) {
			DeviceEvent event = {0,0,TYPE_BUTTON, BUTTON_R1};
			dualshock->applyEvent(&event);
			pressTime = 0;
		} else if( pressTime > 8.0 && !dualshock->getState(BUTTON_R1, TYPE_BUTTON) ) {
			DeviceEvent event = {0,1,TYPE_BUTTON, BUTTON_R1};
			dualshock->applyEvent(&event);
			pressTime = 0;
		}
	}
	void finish() {
		DeviceEvent event = {0,0,TYPE_BUTTON, BUTTON_R1};
		dualshock->applyEvent(&event);
	}
	bool tweak( DeviceEvent* event ) {
		return true;
	}
};

class NoRun : public Modifier {
public:
	static void regist() { Modifier::factory["No Run/Dodge"] = [](){return new NoRun();}; };
	void begin() {
		DeviceEvent event = {0,0,TYPE_BUTTON, BUTTON_L1};
		dualshock->applyEvent(&event);
	}
	bool tweak( DeviceEvent* event ) {
		if (event->id == BUTTON_L1 && event->type == TYPE_BUTTON) {
			//return false;
			event->value = 0;
		}
		return true;
	}
};

class ForceRun : public Modifier {
public:
	static void regist() { Modifier::factory["Force Running"] = [](){return new ForceRun();}; };
	double pressTime;
	void begin() {
		DeviceEvent event = {0,1,TYPE_BUTTON, BUTTON_L1};
		dualshock->applyEvent(&event);
	}
	void update() {
		pressTime += timer.dTime();
		if (pressTime > 4 && dualshock->getState(BUTTON_L1, TYPE_BUTTON)) {
			DeviceEvent event = {0,0,TYPE_BUTTON, BUTTON_L1};
			dualshock->applyEvent(&event);
			pressTime = 0;
		} else if ( pressTime > 0.1 && !dualshock->getState(BUTTON_L1, TYPE_BUTTON)) {
			DeviceEvent event = {0,1,TYPE_BUTTON, BUTTON_L1};
			dualshock->applyEvent(&event);
			pressTime = 0;
		}
	}
	void finish() {
		DeviceEvent event = {0,0,TYPE_BUTTON, BUTTON_L1};
		dualshock->applyEvent(&event);
	}
	bool tweak( DeviceEvent* event ) {
		if (event->id == BUTTON_L1 && event->type == TYPE_BUTTON) {
			event->value = 1;
		}
		return true;
	}
};

class ForceAim : public Modifier {
	//Prototoxin187
public:
	static void regist() { Modifier::factory["Force Aiming"] = [](){return new ForceAim();}; };
	double pressTime;
	void begin() {
		DeviceEvent event = {0,1,TYPE_BUTTON, BUTTON_L2};
		dualshock->applyEvent(&event);
	}
	void update() {
		pressTime += timer.dTime();
		if (pressTime > 4 && dualshock->getState(BUTTON_L2, TYPE_BUTTON)) {
			DeviceEvent event = {0,0,TYPE_BUTTON, BUTTON_L2};
			dualshock->applyEvent(&event);
			pressTime = 0;
		} else if ( pressTime > 0.1 && !dualshock->getState(BUTTON_L2, TYPE_BUTTON)) {
			DeviceEvent event = {0,1,TYPE_BUTTON, BUTTON_L2};
			dualshock->applyEvent(&event);
			pressTime = 0;
		}
	}
	void finish() {
		DeviceEvent event = {0,0,TYPE_BUTTON, BUTTON_L2};
		dualshock->applyEvent(&event);
	}
	bool tweak( DeviceEvent* event ) {
		if (event->id == BUTTON_L2 && event->type == TYPE_BUTTON) {
			event->value = 1;
		}
		return true;
	}
};

class DisableJoystick : public Modifier {
public:
	static void regist() { Modifier::factory["Disable Right Joystick"] = [](){return new DisableJoystick();}; };
	bool tweak( DeviceEvent* event ) {
		if ( (event->id == AXIS_RX || event->id == AXIS_RY) && event->type == TYPE_AXIS) {
			event->value = 0;
		}
		return true;
	}
};

class StrafeOnly : public Modifier {
	//Prototoxin187
public:
	static void regist() { Modifier::factory["Only Strafe Movement"] = [](){return new StrafeOnly();}; };
	bool tweak( DeviceEvent* event ) {
		if ( event->id == AXIS_LY && event->type == TYPE_AXIS) {
			event->value = 0;
		}
		return true;
	}
};

class NoStrafe : public Modifier {
public:
	static void regist() { Modifier::factory["No Strafing"] = [](){return new NoStrafe();}; };
	bool tweak( DeviceEvent* event ) {
		if ( event->id == AXIS_LX && event->type == TYPE_AXIS) {
			event->value = 0;
		}
		return true;
	}
};

class SwapSticks : public Modifier {
public:
	static void regist() { Modifier::factory["Swap Joysticks"] = [](){return new SwapSticks();}; };
	bool tweak( DeviceEvent* event ) {
		if( event->type == TYPE_AXIS ) {
			switch (event->id) {
				case AXIS_RX: event->id = AXIS_LX; break;
				case AXIS_RY: event->id = AXIS_LY; break;
				case AXIS_LX: event->id = AXIS_RX; break;
				case AXIS_LY: event->id = AXIS_RY; break;
				default: break;
			}
		}
		return true;
	}
};

class SwapStickDpad : public Modifier {
public:
	static void regist() { Modifier::factory["Swap D-Pad/Left Joystick"] = [](){return new SwapStickDpad();}; };
	void begin() {
		DeviceEvent event = {0,0,TYPE_AXIS, AXIS_LY};
		dualshock->applyEvent(&event);
		event = {0,0,TYPE_AXIS, AXIS_LX};
		dualshock->applyEvent(&event);
	}
	bool tweak( DeviceEvent* event ) {
		if( event->type == TYPE_AXIS ) {
			switch (event->id) {
				case AXIS_DX: event->id = AXIS_LX; event->value = joystickLimit(JOYSTICK_MAX*event->value); break;
				case AXIS_DY: event->id = AXIS_LY; event->value = joystickLimit(JOYSTICK_MAX*event->value); break;
				case AXIS_LX:
					event->id = AXIS_DX;
					event->value = event->value > JOYSTICK_MAX/2 ? 1 : event->value < JOYSTICK_MIN/2 ? -1 : 0;
					break;
				case AXIS_LY:
					event->id = AXIS_DY;
					event->value = event->value > JOYSTICK_MAX/2 ? 1 : event->value < JOYSTICK_MIN/2 ? -1 : 0;
					break;
				default: break;
			}
		}
		return true;
	}
};

class MotionControls : public Modifier {
public:
	static void regist() { Modifier::factory["Motion Control Movement"] = [](){return new MotionControls();}; };
	bool tweak( DeviceEvent* event ) {
		if( event->type == TYPE_AXIS ) {
			DeviceEvent newEvent;
			switch (event->id) {
				case AXIS_ACCX:
					newEvent = *event;
					newEvent.id = AXIS_LX;
					newEvent.value = joystickLimit(-event->value/24);
					chaosEngine->fakePipelinedEvent(&newEvent, this);
					break;
				case AXIS_ACCZ:
					newEvent = *event;
					newEvent.id = AXIS_LY;
					newEvent.value = joystickLimit(-event->value/24);
					chaosEngine->fakePipelinedEvent(&newEvent, this);
					break;
				case AXIS_LX: return false;
				case AXIS_LY: return false;
				default: break;
			}
		}
		return true;
	}
};

class MotionControlAiming : public Modifier {
public:
	static void regist() { Modifier::factory["Motion Control Aiming"] = [](){return new MotionControlAiming();}; };
	bool tweak( DeviceEvent* event ) {
		if( event->type == TYPE_AXIS ) {
			DeviceEvent newEvent;
			switch (event->id) {
				case AXIS_ACCX:
					newEvent = *event;
					newEvent.id = AXIS_RX;
					newEvent.value = joystickLimit(-event->value/24);
					chaosEngine->fakePipelinedEvent(&newEvent, this);
					break;
				case AXIS_ACCZ:
					newEvent = *event;
					newEvent.id = AXIS_RY;
					newEvent.value = joystickLimit(event->value/24);	// This makes sense to be inverted
					chaosEngine->fakePipelinedEvent(&newEvent, this);
					break;
				case AXIS_RX: return false;
				case AXIS_RY: return false;
				default: break;
			}
		}
		return true;
	}
};

class TouchpadAiming : public Modifier {
public:
	static void regist() { Modifier::factory["Touchpad Aiming"] = [](){return new TouchpadAiming();}; };
	
	short priorX;
	short priorY;
	double timestampPriorX;
	double timestampPriorY;
	bool priorActiveX;
	bool priorActiveY;
	
	bool activeTouch;
	
	void begin() {
		priorActiveX = false;
		priorActiveY = false;
		activeTouch = false;
	}
	
	double derivativeX(short currentX, double timestampX) {
		double ret = 0;
		if (priorActiveX) {
			ret = ((double)(currentX - priorX))/(timestampX-timestampPriorX);
		} else {
			priorActiveX = true;
		}
		priorX = currentX;
		timestampPriorX = timestampX;
		return ret;
	}
	double derivativeY(short currentY, double timestampY) {
		double ret = 0;
		if (priorActiveY) {
			ret = ((double)(currentY - priorY))/(timestampY-timestampPriorY);
		} else {
			priorActiveY = true;
		}
		priorY = currentY;
		timestampPriorY = timestampY;
		return ret;
	}
	
	bool tweak( DeviceEvent* event ) {
		if ( event->type == TYPE_BUTTON && event->id == BUTTON_TOUCHPAD_ACTIVE) {
			
			//printf("Touchpad activity: %d\n", event->value);
			if (activeTouch == false && event->value == false) {	// rising edge
				priorActiveX = false;
				priorActiveY = false;
				//printf("Rising Edge TOuch\n");
			} else if (activeTouch == true && event->value == true) {	// falling edge
				DeviceEvent newEvent;
				newEvent.id = AXIS_RX;
				newEvent.type = TYPE_AXIS;
				newEvent.value = 0;
				chaosEngine->fakePipelinedEvent(&newEvent, this);
				newEvent.id = AXIS_RY;
				newEvent.type = TYPE_AXIS;
				newEvent.value = 0;
				chaosEngine->fakePipelinedEvent(&newEvent, this);
			//	printf("Falling Edge TOuch\n");
			}
			
			activeTouch = !event->value;
		}
		if(	activeTouch &&
		   event->type == TYPE_AXIS ) {
			DeviceEvent newEvent;
			switch (event->id) {
				case AXIS_TOUCHPAD_X:
					newEvent.id = AXIS_RX;
					newEvent.type = TYPE_AXIS;
					newEvent.value = joystickLimit( 0.07 * derivativeX(event->value, timer.runningTime()) );
					chaosEngine->fakePipelinedEvent(&newEvent, this);
					//printf("x derivative: %d\n", newEvent.value);
					break;
				case AXIS_TOUCHPAD_Y:
					DeviceEvent newEvent;
					newEvent.id = AXIS_RY;
					newEvent.type = TYPE_AXIS;
					newEvent.value = joystickLimit( 0.07 *  derivativeY(event->value, timer.runningTime()) );
					chaosEngine->fakePipelinedEvent(&newEvent, this);
					break;
				case AXIS_RX:
					return false;
				case AXIS_RY:
					return false;
				default: break;
			}
		}
		
		// This is a positional method, not very good:
		//		if( event->type == TYPE_AXIS ) {
		//			DeviceEvent newEvent;
		//			switch (event->id) {
		//				case AXIS_TOUCHPAD_X:
		//					newEvent = *event;
		//					newEvent.id = AXIS_RX;
		//					newEvent.value = joystickLimit(((double)event->value/1000 - 1) * JOYSTICK_MAX * 2 * 1.1 );
		//					chaosEngine->fakePipelinedEvent(&newEvent, this);
		//					break;
		//				case AXIS_TOUCHPAD_Y:
		//					newEvent = *event;
		//					newEvent.id = AXIS_RY;
		//					newEvent.value = joystickLimit(((double)event->value/500 - 1) * JOYSTICK_MAX * 1.1 );
		//					chaosEngine->fakePipelinedEvent(&newEvent, this);
		//					break;
		//				case AXIS_RX: return false;
		//				case AXIS_RY: return false;
		//				default: break;
		//			}
		//		}
		return true;
	}
};

class Nascar : public Modifier {
public:
	static void regist() { Modifier::factory["Nascar"] = [](){return new Nascar();}; };
	bool tweak( DeviceEvent* event ) {
		if( event->type == TYPE_AXIS ) {
			if (event->id == AXIS_RX || event->id == AXIS_LX) {
				event->value = event->value <= 0 ? event->value : 0;
			}
		}
		return true;
	}
};

class Drunk : public Modifier {
public:
	static void regist() { Modifier::factory["Drunk"] = [](){return new Drunk();}; };
	
	std::map<int,int> axisToValue;
	std::map<int,int> offsetValue;
	double buttonPressTime;
	
	void begin() {
		axisToValue[AXIS_RX] = dualshock->getState(AXIS_RX, TYPE_AXIS);
		axisToValue[AXIS_RY] = dualshock->getState(AXIS_RY, TYPE_AXIS);
		axisToValue[AXIS_LX] = dualshock->getState(AXIS_LX, TYPE_AXIS);
		axisToValue[AXIS_LY] = dualshock->getState(AXIS_LY, TYPE_AXIS);
		offsetValue[AXIS_RX] = 0;
		offsetValue[AXIS_RY] = 0;
		offsetValue[AXIS_LX] = 0;
		offsetValue[AXIS_LY] = 0;
	}
	
	void update() {
		DeviceEvent event;	// Event to inject
		event.type = TYPE_AXIS;
		
		double i = 0.0;
		double newValue;
		double t = timer.runningTime()*2.0;
		for (std::map<int, int>::iterator it = axisToValue.begin(); it != axisToValue.end(); it++) {
			event.id = it->first;
			//newValue = it->second + JOYSTICK_MAX/3.0 * cos((t+i)/4.0)*sin(t*i/4.0);
			//newValue = it->second + JOYSTICK_MAX/2.0 * (pow((cos(t+i) + cos(2*t)/2) * sin((t+i)*0.2), 3)/3);
			offsetValue[event.id] = 78 * ( (cos(t+i) + cos(2*t)/2) * sin((t+i)*0.2)/2.0 );
			
			event.value = joystickLimit(offsetValue[event.id] + it->second);
			if (event.value == JOYSTICK_MIN || event.value == JOYSTICK_MAX) {
				DeviceEvent fallDown = {0,1,TYPE_BUTTON,BUTTON_CIRCLE};
				dualshock->applyEvent(&fallDown);
				buttonPressTime = timer.runningTime();
			}
			i += 4;
			
//			dualshock->applyEvent(&event);
			chaosEngine->fakePipelinedEvent(&event, this);
			
		}
		if (timer.runningTime()-buttonPressTime > 1.0) {
			DeviceEvent fallDown = {0,0,TYPE_BUTTON,BUTTON_CIRCLE};
			   dualshock->applyEvent(&fallDown);
			buttonPressTime += 10000;
		}
	}
	
	void finish() {
		DeviceEvent event = {0,(short)axisToValue[AXIS_LX],TYPE_AXIS,AXIS_LX};
		chaosEngine->fakePipelinedEvent( &event, this);
		event = {0,(short)axisToValue[AXIS_LY],TYPE_AXIS,AXIS_LY};
		chaosEngine->fakePipelinedEvent( &event, this);
		event = {0,(short)axisToValue[AXIS_RX],TYPE_AXIS,AXIS_RX};
		chaosEngine->fakePipelinedEvent( &event, this);
		event = {0,(short)axisToValue[AXIS_RY],TYPE_AXIS,AXIS_RY};
		chaosEngine->fakePipelinedEvent( &event, this);
	}
	
	bool tweak( DeviceEvent* event ) {
		if( event->type == TYPE_AXIS ) {
			if (axisToValue.count(event->id) > 0) {
				axisToValue[event->id] = event->value; // save the value for update() to use
				event->value = joystickLimit( event->value + offsetValue[event->id] ); // apply the drunk offset
			}
		}
		return true;
	}
};

class SpeedrunGlitch : public Modifier {
public:
	static void regist() { Modifier::factory["TLOU1 Run Glitch"] = [](){return new SpeedrunGlitch();}; };
	
	void update() {
		DeviceEvent event;	// Event to inject
		event.type = TYPE_AXIS;
		
		double i = 0.0;
		double newValue;
		double t = timer.runningTime()*8.0*MOGI_PI;
		
		event.value = 1.5*cos(t);
		event.id = AXIS_DX;
		chaosEngine->fakePipelinedEvent( &event, this);
		
		event.value = 1.5*sin(t);
		event.id = AXIS_DY;
		chaosEngine->fakePipelinedEvent( &event, this);
		
	}
	
	void finish() {
		DeviceEvent event = {0,0,TYPE_AXIS,AXIS_DX};
		chaosEngine->fakePipelinedEvent( &event, this);
		event = {0,0,TYPE_AXIS,AXIS_DY};
		chaosEngine->fakePipelinedEvent( &event, this);
	}
	
	bool tweak( DeviceEvent* event ) {
		if( event->type == TYPE_AXIS &&
		   (event->id == AXIS_DY || event->id == AXIS_DX)) {
			return false;
		}
		return true;
	}
};

class MegaScopeSway : public Modifier {
public:
	static void regist() { Modifier::factory["Mega Scope Sway"] = [](){ return new MegaScopeSway();}; };
	
	std::map<int,int> axisToValue;
	std::map<int,int> offsetValue;
	
	void begin() {
		axisToValue[AXIS_RX] = dualshock->getState(AXIS_RX, TYPE_AXIS);
		axisToValue[AXIS_RY] = dualshock->getState(AXIS_RY, TYPE_AXIS);
		offsetValue[AXIS_RX] = 0;
		offsetValue[AXIS_RY] = 0;
	}
	
	void update() {
		DeviceEvent event;	// Event to inject
		event.type = TYPE_AXIS;
		
		bool applySway = dualshock->getState( BUTTON_L2, TYPE_BUTTON) != 0;
		
		double t = timer.runningTime()*3.0;
		double i = 1.0;
		
		for (std::map<int, int>::iterator it = axisToValue.begin(); it != axisToValue.end(); it++) {
			event.id = it->first;
			if (applySway) {
				offsetValue[event.id] = sin(((t+1.6)*i )*4.0) * JOYSTICK_MAX ;
				event.value = joystickLimit( it->second + offsetValue[event.id] );
				chaosEngine->fakePipelinedEvent(&event, this);
				i += 1;
			} else {
				offsetValue[event.id]  = 0;
			}
			
		}
		
	}
	
	void finish() {
		DeviceEvent event = {0,(short)axisToValue[AXIS_RX],TYPE_AXIS,AXIS_RX};
		chaosEngine->fakePipelinedEvent( &event, this);
		event = {0,(short)axisToValue[AXIS_RY],TYPE_AXIS,AXIS_RY};
		chaosEngine->fakePipelinedEvent( &event, this);
	}
	
	bool tweak( DeviceEvent* event ) {
		if( event->type == TYPE_AXIS ) {
			if ( axisToValue.count(event->id) > 0) {
				axisToValue[event->id] = event->value;
				event->value = joystickLimit( event->value + offsetValue[event->id] );
			}
		}
		return true;
	}
};

class ProneDive : public Modifier {
public:
	static void regist() { Modifier::factory["Only Prone Diving"] = [](){ return new ProneDive();}; };
	
	std::map<int,int> axisToValue;
	//std::map<int,int> offsetValue;
	double sequenceTime;
	bool inSequence;
	double magnitudeSquared;
	
	double activeThreshold;
	
	void begin() {
		axisToValue[AXIS_LX] = dualshock->getState(AXIS_LX, TYPE_AXIS);
		axisToValue[AXIS_LY] = dualshock->getState(AXIS_LY, TYPE_AXIS);
		
		sequenceTime = 0;
		inSequence = false;
		
		activeThreshold = pow( JOYSTICK_MAX*0.2, 2);
	}
	
	void update() {
		DeviceEvent event;	// Event to inject
		//event.type = TYPE_AXIS;
		if (!inSequence) {
			magnitudeSquared = pow(dualshock->getState(AXIS_LX, TYPE_AXIS), 2) + pow(dualshock->getState(AXIS_LY, TYPE_AXIS), 2);
			//printf("magnitude = %02f\n", sqrt(magnitudeSquared));
			if (magnitudeSquared > activeThreshold) {	// magnitude of 9 & max 127
				inSequence = true;
			}
		}
		
		if (inSequence) {
			sequenceTime += timer.dTime();
			if (sequenceTime >= 0  &&
				sequenceTime <= 0.1 &&
				dualshock->getState(BUTTON_L1, TYPE_BUTTON) == 0) {
				event.id = BUTTON_L1;
				event.type = TYPE_BUTTON;
				event.value = 1;
				dualshock->applyEvent(&event);
				//printf("Dodge!\n");
			} else if (sequenceTime > 0.1 &&
					   sequenceTime <= 0.9 &&
					   dualshock->getState(BUTTON_CIRCLE, TYPE_BUTTON) == 0) {
				event.id = BUTTON_CIRCLE;
				event.type = TYPE_BUTTON;
				event.value = 1;
				dualshock->applyEvent(&event);
				//printf("Dive!\n");
			} else if (sequenceTime > 0.9 &&
					   sequenceTime < 1.4 &&
					   dualshock->getState(BUTTON_CIRCLE, TYPE_BUTTON) == 1) {
				event.id = BUTTON_L1;
				event.type = TYPE_BUTTON;
				event.value = 0;
				dualshock->applyEvent(&event);
				event.id = BUTTON_CIRCLE;
				event.type = TYPE_BUTTON;
				event.value = 0;
				dualshock->applyEvent(&event);
				//printf("Release!\n");
			} else if (sequenceTime > 1.4) {
				inSequence = false;
				sequenceTime = 0;
				//printf("Done.\n");
			}
		}
		
		//bool applySway = dualshock->getState( BUTTON_L2, TYPE_BUTTON) != 0;
		
		//double t = timer.runningTime()*3.0;
		
		
	}
	
	void finish() {
		DeviceEvent event = {0,0,TYPE_BUTTON,BUTTON_CIRCLE};
		dualshock->applyEvent(&event);
		event = {0,0,TYPE_BUTTON,BUTTON_L1};
		dualshock->applyEvent(&event);
//		chaosEngine->fakePipelinedEvent( &event, this);
//		event = {0,(short)axisToValue[AXIS_RY],TYPE_AXIS,AXIS_RY};
//		chaosEngine->fakePipelinedEvent( &event, this);
	}
	
	bool tweak( DeviceEvent* event ) {
//		if( event->type == TYPE_AXIS ) {
//			if ( axisToValue.count(event->id) > 0) {
//				axisToValue[event->id] = event->value;	// We only sample the left stick
//			}
//		}
		
		if ( event->type == TYPE_BUTTON) {
			if (inSequence) {
				return false;
			}
			if ( event->id == BUTTON_L1 ) {
				return false;
			}
			if (event->id == BUTTON_CIRCLE &&
				magnitudeSquared > activeThreshold) {
				return false;
			}
		}
		
		return true;
	}
};

class NoGuns : public Modifier {
public:
	static void regist() { Modifier::factory["No Guns"] = [](){ return new NoGuns();}; };
	
	void begin() {
		DeviceEvent event = {0,0,TYPE_AXIS, AXIS_DX};
		dualshock->applyEvent(&event);
		
		Sequence sequence;
		sequence.addAxisPress( AXIS_DY, 1);	// select a non-gun
		sequence.send(dualshock);
	}
	
	bool tweak( DeviceEvent* event ) {
		if (event->type == TYPE_AXIS && event->id == AXIS_DX) {
			return false;
		}
		return true;
	}
};

class NoThrows : public Modifier {
public:
	static void regist() { Modifier::factory["No Throwables"] = [](){return new NoThrows();}; };
	
	void begin() {
		DeviceEvent event = {0,0,TYPE_AXIS, AXIS_DY};
		dualshock->applyEvent(&event);
		
		Sequence sequence;
		sequence.addAxisPress( AXIS_DX, 1);	// select a non-gun
		sequence.send(dualshock);
	}
	
	bool tweak( DeviceEvent* event ) {
		if (event->type == TYPE_AXIS &&
			event->id == AXIS_DY) {
			return false;
		}
		return true;
	}
};

class MaxSensitivity : public Modifier {
    // gabemusic
public:
    static void regist() { Modifier::factory["Max Sensitivity"] = [](){return new MaxSensitivity();}; };
    
    bool tweak( DeviceEvent* event ) {
        if (event->type == TYPE_AXIS) {
            switch (event->id) {
                case AXIS_LX:
                case AXIS_LY:
                case AXIS_RX:
                case AXIS_RY:
                    event->value = joystickLimit( (int)event->value * 5 );
                    
                default:
                    break;
            }
        }
        return true;
    }
};

class ControllerMirror : public Modifier {
    // Princess, cliverfieldmel, DJ_Squall_808
public:
    static void regist() { Modifier::factory["Controller Mirror"] = [](){return new ControllerMirror();}; };
    
    bool tweak( DeviceEvent* event ) {
        if(event->type == TYPE_AXIS) {
            switch (event->id) {
                case AXIS_LX: event->id = AXIS_RX; break;
                case AXIS_LY: event->id = AXIS_RY; break;
                case AXIS_RX: event->id = AXIS_LX; break;
                case AXIS_RY: event->id = AXIS_LY; break;
				case AXIS_L2: event->id = AXIS_R2; break;
				case AXIS_R2: event->id = AXIS_L2; break;
                case AXIS_DX:
                    event->type = TYPE_BUTTON;
                    if (event->value > 0) {
                        event->id = BUTTON_SQUARE;
                        event->value = 1;
                    } else if(event->value < 0) {
                        event->id = BUTTON_CIRCLE;
                        event->value = 1;
                    } else {
                        event->id = BUTTON_CIRCLE;
                        event->value = 0;
                        
                        
                        DeviceEvent event2;
                        event2.id = BUTTON_SQUARE;
                        event2.value = 0;
                        event2.type = TYPE_BUTTON;
                        dualshock->applyEvent(&event2);
                    }
                    break;
                case AXIS_DY:
                    event->type = TYPE_BUTTON;
                    if (event->value > 0) {
                        event->id = BUTTON_X;
                        event->value = 1;
                    } else if(event->value < 0) {
                        event->id = BUTTON_TRIANGLE;
                        event->value = 1;
                    } else {
                        event->id = BUTTON_TRIANGLE;
                        event->value = 0;
                        
                        
                        DeviceEvent event2;
                        event2.id = BUTTON_X;
                        event2.value = 0;
                        event2.type = TYPE_BUTTON;
                        dualshock->applyEvent(&event2);
                    }
                    break;
                default:
                    break;
            }
            return true;
        }
        if(event->type == TYPE_BUTTON) {
            switch (event->id) {
                case BUTTON_R1: event->id = BUTTON_L1; break;
                case BUTTON_L1: event->id = BUTTON_R1; break;
                case BUTTON_R2: event->id = BUTTON_L2; break;
                case BUTTON_L2: event->id = BUTTON_R2; break;
                
                case BUTTON_TRIANGLE:
                    event->type = TYPE_AXIS;
                    event->value *= -1;
                    event->id = AXIS_DY;
                    break;
                case BUTTON_X:
                    event->type = TYPE_AXIS;
                    event->value *= 1;
                    event->id = AXIS_DY;
                    break;
                case BUTTON_SQUARE:
                    event->type = TYPE_AXIS;
                    event->value *= 1;
                    event->id = AXIS_DX;
                    break;
                case BUTTON_CIRCLE:
                    event->type = TYPE_AXIS;
                    event->value *= -1;
                    event->id = AXIS_DX;
                    break;
            default:
                break;
            }
        }
        
        return true;
    }
};

/*
 Main Menu Operation:
 */
class RestartCheckpoint : public Modifier {
public:
	static void regist() { Modifier::factory["Restart Checkpoint"] = [](){return new RestartCheckpoint();}; };

	bool busy;
	
	void begin() {
		busy = true;
		
		Sequence sequence;
		sequence.disablejoysticks();
		sequence.addButtonPress( BUTTON_OPTIONS);
		sequence.addAxisPress( AXIS_DY, -1);
		sequence.addAxisPress( AXIS_DY, -1);
		sequence.addButtonPress( BUTTON_X);
		sequence.addAxisPress( AXIS_DX, -1);
		sequence.addButtonPress( BUTTON_X);
		sequence.addTimeDelay(2000);
		sequence.send(dualshock);
		
		busy = false;
	}
	
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};

/*
 HUD settings:
 */
class NoReticle : public Modifier {
public:
	static void regist() { Modifier::factory["No Reticle"] = [](){return new NoReticle();}; };

	bool busy;
	
	void begin() {
		busy = true;
		Menuing::getInstance()->selectHudMode(HUD_RETICLE, RETICLE_OFF, dualshock);
		busy = false;
	}
	
	void finish() {
		busy = true;
		Menuing::getInstance()->selectHudMode(HUD_RETICLE, RETICLE_SIMPLE, dualshock);
		busy = false;
	}
	
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};

/*
Render Modes:
 */
class Graphic : public Modifier {
    bool busy;
public:
    static void regist() { Modifier::factory["Graphic"] = [](){return new Graphic();}; };
    
    void begin() {
        busy = true;
        Menuing::getInstance()->selectRenderMode(RENDER_GRAPHIC, dualshock);
        busy = false;
    }
    void finish() {
        busy = true;
        Menuing::getInstance()->teardownRenderMode(dualshock);
        busy = false;
    }
    bool tweak( DeviceEvent* event ) {
        return !busy;
    }
};
class Headache : public Modifier {
    bool busy;
public:
    static void regist() { Modifier::factory["Headache"] = [](){return new Headache();}; };
    
    void begin() {
        busy = true;
        Menuing::getInstance()->selectRenderMode(RENDER_HEADHACHE, dualshock);
        busy = false;
    }
    void finish() {
        busy = true;
        Menuing::getInstance()->teardownRenderMode(dualshock);
        busy = false;
    }
    bool tweak( DeviceEvent* event ) {
        return !busy;
    }
};

class Afterlife : public Modifier {
    bool busy;
public:
    static void regist() { Modifier::factory["Afterlife"] = [](){return new Afterlife();}; };
    
    void begin() {
        busy = true;
        Menuing::getInstance()->selectRenderMode(RENDER_AFTERLIFE, dualshock);
        busy = false;
    }
    void finish() {
        busy = true;
        Menuing::getInstance()->teardownRenderMode(dualshock);
        busy = false;
    }
    bool tweak( DeviceEvent* event ) {
        return !busy;
    }
};

class EightBit : public Modifier {
    bool busy;
public:
    static void regist() { Modifier::factory["8-Bit"] = [](){return new EightBit();}; };
    
    void begin() {
        busy = true;
        Menuing::getInstance()->selectRenderMode(RENDER_EIGHTBIT, dualshock);
        busy = false;
    }
    void finish() {
        busy = true;
        Menuing::getInstance()->teardownRenderMode(dualshock);
        busy = false;
    }
    bool tweak( DeviceEvent* event ) {
        return !busy;
    }
};

class WaterColor : public Modifier {
    bool busy;
public:
    static void regist() { Modifier::factory["Water Color"] = [](){return new WaterColor();}; };
    
    void begin() {
        busy = true;
        Menuing::getInstance()->selectRenderMode(RENDER_WATERCOLOR, dualshock);
        busy = false;
    }
    void finish() {
        busy = true;
        Menuing::getInstance()->teardownRenderMode(dualshock);
        busy = false;
    }
    bool tweak( DeviceEvent* event ) {
        return !busy;
    }
};

class Dungeon : public Modifier {
    bool busy;
public:
    static void regist() { Modifier::factory["Dungeon"] = [](){return new Dungeon();}; };
    
    void begin() {
        busy = true;
        Menuing::getInstance()->selectRenderMode(RENDER_DUNGEON, dualshock);
        busy = false;
    }
    void finish() {
        busy = true;
        Menuing::getInstance()->teardownRenderMode(dualshock);
        busy = false;
    }
    bool tweak( DeviceEvent* event ) {
        return !busy;
    }
};

class Void : public Modifier {
    bool busy;
public:
    static void regist() { Modifier::factory["Void"] = [](){return new Void();}; };
    
    void begin() {
        busy = true;
        Menuing::getInstance()->selectRenderMode(RENDER_VOID, dualshock);
        busy = false;
    }
    void finish() {
        busy = true;
        Menuing::getInstance()->teardownRenderMode(dualshock);
        busy = false;
    }
    bool tweak( DeviceEvent* event ) {
        return !busy;
    }
};

class PopPoster : public Modifier {
    bool busy;
public:
    static void regist() { Modifier::factory["Pop Poster"] = [](){return new PopPoster();}; };
    
    void begin() {
        busy = true;
        Menuing::getInstance()->selectRenderMode(RENDER_POPPOSTER, dualshock);
        busy = false;
    }
    void finish() {
        busy = true;
        Menuing::getInstance()->teardownRenderMode(dualshock);
        busy = false;
    }
    bool tweak( DeviceEvent* event ) {
        return !busy;
    }
};

class NineteenSixty : public Modifier {
    bool busy;
public:
    static void regist() { Modifier::factory["1960"] = [](){return new NineteenSixty();}; };
    
    void begin() {
        busy = true;
        Menuing::getInstance()->selectRenderMode(RENDER_NINETEENSIXTY, dualshock);
        busy = false;
    }
    void finish() {
        busy = true;
        Menuing::getInstance()->teardownRenderMode(dualshock);
        busy = false;
    }
    bool tweak( DeviceEvent* event ) {
        return !busy;
    }
};

class Cool : public Modifier {
	bool busy;
public:
	static void regist() { Modifier::factory["Cool"] = [](){return new Cool();}; };
	
	void begin() {
		busy = true;
		Menuing::getInstance()->selectRenderMode(RENDER_COOL, dualshock);
		busy = false;
	}
	void finish() {
		busy = true;
		Menuing::getInstance()->teardownRenderMode(dualshock);
		busy = false;
	}
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};

class Warm : public Modifier {
	bool busy;
public:
	static void regist() { Modifier::factory["Warm"] = [](){return new Warm();}; };
	
	void begin() {
		busy = true;
		Menuing::getInstance()->selectRenderMode(RENDER_WARM, dualshock);
		busy = false;
	}
	void finish() {
		busy = true;
		Menuing::getInstance()->teardownRenderMode(dualshock);
		busy = false;
	}
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};

class Vibrant : public Modifier {
	bool busy;
public:
	static void regist() { Modifier::factory["Vibrant"] = [](){return new Vibrant();}; };
	
	void begin() {
		busy = true;
		Menuing::getInstance()->selectRenderMode(RENDER_VIBRANT, dualshock);
		busy = false;
	}
	void finish() {
		busy = true;
		Menuing::getInstance()->teardownRenderMode(dualshock);
		busy = false;
	}
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};

class Noir : public Modifier {
	bool busy;
public:
	static void regist() { Modifier::factory["Noir"] = [](){return new Noir();}; };
	
	void begin() {
		busy = true;
		Menuing::getInstance()->selectRenderMode(RENDER_NOIR, dualshock);
		busy = false;
	}
	void finish() {
		busy = true;
		Menuing::getInstance()->teardownRenderMode(dualshock);
		busy = false;
	}
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};

class Blorange : public Modifier {
	bool busy;
public:
	static void regist() { Modifier::factory["Blorange"] = [](){return new Blorange();}; };
	
	void begin() {
		busy = true;
		Menuing::getInstance()->selectRenderMode(RENDER_BLORANGE, dualshock);
		busy = false;
	}
	void finish() {
		busy = true;
		Menuing::getInstance()->teardownRenderMode(dualshock);
		busy = false;
	}
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};

class Sepia : public Modifier {
	bool busy;
public:
	static void regist() { Modifier::factory["Sepia"] = [](){return new Sepia();}; };
	
	void begin() {
		busy = true;
		Menuing::getInstance()->selectRenderMode(RENDER_SEPIA, dualshock);
		busy = false;
	}
	void finish() {
		busy = true;
		Menuing::getInstance()->teardownRenderMode(dualshock);
		busy = false;
	}
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};

class Vintage : public Modifier {
	bool busy;
public:
	static void regist() { Modifier::factory["Vintage"] = [](){return new Vintage();}; };
	
	void begin() {
		busy = true;
		Menuing::getInstance()->selectRenderMode(RENDER_VINTAGE, dualshock);
		busy = false;
	}
	void finish() {
		busy = true;
		Menuing::getInstance()->teardownRenderMode(dualshock);
		busy = false;
	}
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};

class Beasts : public Modifier {
	bool busy;
public:
	static void regist() { Modifier::factory["Beasts"] = [](){return new Beasts();}; };
	
	void begin() {
		busy = true;
		Menuing::getInstance()->selectRenderMode(RENDER_BEASTS, dualshock);
		busy = false;
	}
	void finish() {
		busy = true;
		Menuing::getInstance()->teardownRenderMode(dualshock);
		busy = false;
	}
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};

class Terminated : public Modifier {
	bool busy;
public:
	static void regist() { Modifier::factory["Terminated"] = [](){return new Terminated();}; };
	
	void begin() {
		busy = true;
		Menuing::getInstance()->selectRenderMode(RENDER_TERMINATED, dualshock);
		busy = false;
	}
	void finish() {
		busy = true;
		Menuing::getInstance()->teardownRenderMode(dualshock);
		busy = false;
	}
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};
class GridLocked : public Modifier {
	bool busy;
public:
	static void regist() { Modifier::factory["Gridlocked"] = [](){return new GridLocked();}; };
	
	void begin() {
		busy = true;
		Menuing::getInstance()->selectRenderMode(RENDER_GRIDLOCKED, dualshock);
		busy = false;
	}
	void finish() {
		busy = true;
		Menuing::getInstance()->teardownRenderMode(dualshock);
		busy = false;
	}
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};

class Blacklight : public Modifier {
	bool busy;
public:
	static void regist() { Modifier::factory["Blacklight"] = [](){return new Blacklight();}; };
	
	void begin() {
		busy = true;
		Menuing::getInstance()->selectRenderMode(RENDER_BLACKLIGHT, dualshock);
		busy = false;
	}
	void finish() {
		busy = true;
		Menuing::getInstance()->teardownRenderMode(dualshock);
		busy = false;
	}
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};

class DesertFog : public Modifier {
	bool busy;
public:
	static void regist() { Modifier::factory["Desert Fog"] = [](){return new DesertFog();}; };
	
	void begin() {
		busy = true;
		Menuing::getInstance()->selectRenderMode(RENDER_DESERT_FOG, dualshock);
		busy = false;
	}
	void finish() {
		busy = true;
		Menuing::getInstance()->teardownRenderMode(dualshock);
		busy = false;
	}
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};

class Blood : public Modifier {
	bool busy;
public:
	static void regist() { Modifier::factory["Blood"] = [](){return new Blood();}; };
	
	void begin() {
		busy = true;
		Menuing::getInstance()->selectRenderMode(RENDER_BLOOD, dualshock);
		busy = false;
	}
	void finish() {
		busy = true;
		Menuing::getInstance()->teardownRenderMode(dualshock);
		busy = false;
	}
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};

class Inferno : public Modifier {	// Working
	bool busy;
public:
	static void regist() { Modifier::factory["Inferno"] = [](){return new Inferno();}; };
	
	void begin() {
		busy = true;
		Menuing::getInstance()->selectRenderMode(RENDER_INFERNO, dualshock);
		busy = false;
	}
	void finish() {
		busy = true;
		Menuing::getInstance()->teardownRenderMode(dualshock);
		busy = false;
	}
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};

class Fire : public Modifier {
	bool busy;
public:
	static void regist() { Modifier::factory["Fire"] = [](){return new Fire();}; };
	
	void begin() {
		busy = true;
		Menuing::getInstance()->selectRenderMode(RENDER_FIRE, dualshock);
		busy = false;
	}
	void finish() {
		busy = true;
		Menuing::getInstance()->teardownRenderMode(dualshock);
		busy = false;
	}
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};

class Trinity : public Modifier {
	bool busy;
public:
	static void regist() { Modifier::factory["Trinity"] = [](){return new Trinity();}; };
	
	void begin() {
		busy = true;
		Menuing::getInstance()->selectRenderMode(RENDER_TRINITY, dualshock);
		busy = false;
	}
	void finish() {
		busy = true;
		Menuing::getInstance()->teardownRenderMode(dualshock);
		busy = false;
	}
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};

class Pusher : public Modifier {
	bool busy;
public:
	static void regist() { Modifier::factory["Pusher"] = [](){return new Pusher();}; };
	
	void begin() {
		busy = true;
		Menuing::getInstance()->selectRenderMode(RENDER_PUSHER, dualshock);
		busy = false;
	}
	void finish() {
		busy = true;
		Menuing::getInstance()->teardownRenderMode(dualshock);
		busy = false;
	}
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};

class Demons : public Modifier {
	bool busy;
public:
	static void regist() { Modifier::factory["Demons"] = [](){return new Demons();}; };
	
	void begin() {
		busy = true;
		Menuing::getInstance()->selectRenderMode(RENDER_DEMONS, dualshock);
		busy = false;
	}
	void finish() {
		busy = true;
		Menuing::getInstance()->teardownRenderMode(dualshock);
		busy = false;
	}
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};

class DrugZone : public Modifier {
	bool busy;
public:
	static void regist() { Modifier::factory["Drug Zone"] = [](){return new DrugZone();}; };
	
	void begin() {
		busy = true;
		Menuing::getInstance()->selectRenderMode(RENDER_DRUGZONE, dualshock);
		busy = false;
	}
	void finish() {
		busy = true;
		Menuing::getInstance()->teardownRenderMode(dualshock);
		busy = false;
	}
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};
class Moonlight : public Modifier {
	bool busy;
public:
	static void regist() { Modifier::factory["Moonlight"] = [](){return new Moonlight();}; };
	
	void begin() {
		busy = true;
		Menuing::getInstance()->selectRenderMode(RENDER_MOONLIGHT, dualshock);
		busy = false;
	}
	void finish() {
		busy = true;
		Menuing::getInstance()->teardownRenderMode(dualshock);
		busy = false;
	}
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};

/*
 Gameplay Modes:
 */
class Mirror : public Modifier {
    bool busy;
public:
    static void regist() { Modifier::factory["Mirror World"] = [](){return new Mirror();}; };
    
    void begin() {
        busy = true;
        Menuing::getInstance()->selectGameplayMode(GAMEPLAY_MIRROR, dualshock);
        busy = false;
    }
    void finish() {
        begin();
    }
    bool tweak( DeviceEvent* event ) {
        return !busy;
    }
};

class MirrorOnDeath : public Modifier {
	bool busy;
public:
	static void regist() { Modifier::factory["Mirror On Death"] = [](){return new MirrorOnDeath();}; };
	
	void begin() {
		busy = true;
		Menuing::getInstance()->selectGameplayMode(GAMEPLAY_MIRROR_ON_DEATH, dualshock);
		busy = false;
	}
	void finish() {
		begin();
	}
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};


class SlowMotion : public Modifier {
	bool busy;
public:
	static void regist() { Modifier::factory["Slow Motion"] = [](){return new SlowMotion();}; };
	
	void begin() {
		busy = true;
		Menuing::getInstance()->selectGameplayMode(GAMEPLAY_SLOW_MOTION, dualshock);
		busy = false;
	}
	void finish() {
		begin();
	}
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};

class BulletSpeed : public Modifier {
	bool busy;
public:
	static void regist() { Modifier::factory["Bullet Speed"] = [](){return new BulletSpeed();}; };
	
	void begin() {
		busy = true;
		Menuing::getInstance()->selectGameplayMode(GAMEPLAY_BULLET_SPEED, dualshock);
		busy = false;
	}
	void finish() {
		begin();
	}
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};

class InfiniteAmmo : public Modifier {
    bool busy;
public:
    static void regist() { Modifier::factory["Infinite Ammo"] = [](){return new InfiniteAmmo();}; };
    
    void begin() {
        busy = true;
        Menuing::getInstance()->selectGameplayMode(GAMEPLAY_INFINITE_AMMO, dualshock);
        busy = false;
    }
    void finish() {
        begin();
    }
    bool tweak( DeviceEvent* event ) {
        return !busy;
    }
};

class InfiniteCrafting : public Modifier {
    bool busy;
public:
    static void regist() { Modifier::factory["Infinite Crafting"] = [](){return new InfiniteCrafting();}; };
    
    void begin() {
        busy = true;
        Menuing::getInstance()->selectGameplayMode(GAMEPLAY_INFINITE_CRAFT, dualshock);
        busy = false;
    }
    void finish() {
        begin();
    }
    bool tweak( DeviceEvent* event ) {
        return !busy;
    }
};

class InfiniteMeleeDurability : public Modifier {
	bool busy;
public:
	static void regist() { Modifier::factory["Infinite Melee Durability"] = [](){return new InfiniteMeleeDurability();}; };
	
	void begin() {
		busy = true;
		Menuing::getInstance()->selectGameplayMode(GAMEPLAY_INFINITE_MELEE_DURABILITY, dualshock);
		busy = false;
	}
	void finish() {
		begin();
	}
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};

class InfiniteListenRange : public Modifier {	// working
	bool busy;
public:
	static void regist() { Modifier::factory["Infinite Listen Range"] = [](){return new InfiniteListenRange();}; };
	
	void begin() {
		busy = true;
		Menuing::getInstance()->selectGameplayMode(GAMEPLAY_INFINITE_LISTEN_RANGE, dualshock);
		busy = false;
	}
	void finish() {
		begin();
	}
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};

class OneShot : public Modifier {
	bool busy;
public:
	static void regist() { Modifier::factory["One Shot"] = [](){return new OneShot();}; };
	
	void begin() {
		busy = true;
		Menuing::getInstance()->selectGameplayMode(GAMEPLAY_ONE_SHOT, dualshock);
		busy = false;
	}
	void finish() {
		begin();
	}
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};

class TouchOfDeath : public Modifier {
	bool busy;
public:
	static void regist() { Modifier::factory["Touch Of Death"] = [](){return new TouchOfDeath();}; };
	
	void begin() {
		busy = true;
		Menuing::getInstance()->selectGameplayMode(GAMEPLAY_TOUCH_OF_DEATH, dualshock);
		busy = false;
	}
	void finish() {
		begin();
	}
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};

class EightBitAudio : public Modifier {
	bool busy;
public:
	static void regist() { Modifier::factory["8-Bit Audio"] = [](){return new EightBitAudio();}; };
	
	void begin() {
		busy = true;
		Menuing::getInstance()->selectGameplayMode(GAMEPLAY_AUDIO_EIGHT_BIT, dualshock);
		busy = false;
	}
	void finish() {
		busy = true;
		Menuing::getInstance()->teardownAudio(GAMEPLAY_AUDIO_EIGHT_BIT, dualshock);
		busy = false;
	}
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};

class FourBitAudio : public Modifier {
	bool busy;
public:
	static void regist() { Modifier::factory["4-Bit Audio"] = [](){return new FourBitAudio();}; };
	
	void begin() {
		busy = true;
		Menuing::getInstance()->selectGameplayMode(GAMEPLAY_AUDIO_FOUR_BIT, dualshock);
		busy = false;
	}
	void finish() {
		busy = true;
		Menuing::getInstance()->teardownAudio(GAMEPLAY_AUDIO_FOUR_BIT, dualshock);
		busy = false;
	}
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};

class HeliumAudio : public Modifier {
	bool busy;
public:
	static void regist() { Modifier::factory["Helium Audio"] = [](){return new HeliumAudio();}; };
	
	void begin() {
		busy = true;
		Menuing::getInstance()->selectGameplayMode(GAMEPLAY_AUDIO_HELIUM, dualshock);
		busy = false;
	}
	void finish() {
		busy = true;
		Menuing::getInstance()->teardownAudio(GAMEPLAY_AUDIO_HELIUM, dualshock);
		busy = false;
	}
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};

class XenonAudio : public Modifier {	// Working!
	bool busy;
public:
	static void regist() { Modifier::factory["Xenon Audio"] = [](){return new XenonAudio();}; };
	
	void begin() {
		busy = true;
		Menuing::getInstance()->selectGameplayMode(GAMEPLAY_AUDIO_XENON, dualshock);
		busy = false;
	}
	void finish() {
		busy = true;
		Menuing::getInstance()->teardownAudio(GAMEPLAY_AUDIO_XENON, dualshock);
		busy = false;
	}
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};

class DeskPop : public Modifier {
public:
	static void regist() { Modifier::factory["Desk Pop"] = [](){return new DeskPop();}; };

	bool busy;
	
	void begin() {
		busy = true;
		
		Sequence sequence;
		
		sequence.addAxisPress(AXIS_DX, 1);
		sequence.addTimeDelay(1000);
		sequence.addButtonHold(BUTTON_L2);
		sequence.addTimeDelay(1000);
		sequence.addButtonHold(BUTTON_R2);
		sequence.addTimeDelay(200);
		sequence.addButtonRelease(BUTTON_L2);
		sequence.addButtonRelease(BUTTON_R2);
		
		sequence.send(dualshock);
		
		busy = false;
	}
	
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};

class TossMolly : public Modifier {
public:
	static void regist() { Modifier::factory["Toss A Molly"] = [](){return new TossMolly();}; };

	bool busy;
	
	void begin() {
		busy = true;
		
		Sequence sequence;
		
		sequence.disablejoysticks();
		sequence.addAxisPress(AXIS_DY, 1);
		sequence.addAxisPress(AXIS_DY, 1);
//		sequence.addTimeDelay(1000);
//		sequence.addButtonHold(BUTTON_L2);
		sequence.addTimeDelay(2500);
		sequence.addButtonHold(BUTTON_R2);
		sequence.addTimeDelay(500);
//		sequence.addButtonRelease(BUTTON_L2);
		sequence.addButtonRelease(BUTTON_R2);
		
		sequence.send(dualshock);
		
		busy = false;
	}
	
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};

class CtgStrat : public Modifier {
public:
	static void regist() { Modifier::factory["CTG Strat"] = [](){return new CtgStrat();}; };

	bool busy;
	
	void begin() {
		busy = true;
		
		Sequence sequence;
		
		sequence.disablejoysticks();
		sequence.addAxisHold(AXIS_RY, JOYSTICK_MAX);
		sequence.addAxisHold(AXIS_RX, 0);
		sequence.addAxisPress(AXIS_DY, 1);
		sequence.addAxisPress(AXIS_DY, 1);
//		sequence.addTimeDelay(1000);
//		sequence.addButtonHold(BUTTON_L2);
		sequence.addTimeDelay(2500);
		sequence.addButtonHold(BUTTON_R2);
		sequence.addTimeDelay(500);
//		sequence.addButtonRelease(BUTTON_L2);
		sequence.addButtonRelease(BUTTON_R2);
		
		sequence.addAxisHold(AXIS_RY, 0);
		sequence.addAxisHold(AXIS_RX, 0);
		
		sequence.send(dualshock);
		
		busy = false;
	}
	
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};

class PdubIt : public Modifier {
public:
	static void regist() { Modifier::factory["Pdub It"] = [](){return new PdubIt();}; };

	bool busy;
	
	void begin() {
		busy = true;
		
		Sequence sequence;
		
		sequence.disablejoysticks();
		sequence.addAxisPress(AXIS_DY, 1);
		sequence.addTimeDelay(2500);
//		sequence.addButtonHold(BUTTON_L2);
//		sequence.addTimeDelay(1000);
		sequence.addButtonHold(BUTTON_R2);
		sequence.addTimeDelay(200);
//		sequence.addButtonRelease(BUTTON_L2);
		sequence.addButtonRelease(BUTTON_R2);
		
		sequence.send(dualshock);
		
		busy = false;
	}
	
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};

class ChuckStun : public Modifier {
public:
	static void regist() { Modifier::factory["Chuck Smoke Bomb"] = [](){return new ChuckStun();}; };

	bool busy;
	
	void begin() {
		busy = true;
		
		Sequence sequence;
		
		sequence.disablejoysticks();
		sequence.addAxisPress(AXIS_DY, -1);
		sequence.addAxisPress(AXIS_DY, -1);
		sequence.addTimeDelay(2000);
//		sequence.addButtonHold(BUTTON_L2);
//		sequence.addTimeDelay(1000);
		sequence.addButtonHold(BUTTON_R2);
		sequence.addTimeDelay(200);
//		sequence.addButtonRelease(BUTTON_L2);
		sequence.addButtonRelease(BUTTON_R2);
		
		sequence.send(dualshock);
		
		busy = false;
	}
	
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};


class HostagesDontEscape : public Modifier {
	bool busy;
public:
	static void regist() { Modifier::factory["Hostage Don\'t Escape"] = [](){return new HostagesDontEscape();}; };
	
	void begin() {
		busy = true;
		Menuing::getInstance()->setCombatAccessibility(COMBAT_HOSTAGES_DONT_ESCAPE, 1, dualshock);
		busy = false;
	}
	void finish() {
		busy = true;
		Menuing::getInstance()->setCombatAccessibility(COMBAT_HOSTAGES_DONT_ESCAPE, 0, dualshock);
		busy = false;
	}
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};

class EnemiesDontFlank : public Modifier {
	bool busy;
public:
	static void regist() { Modifier::factory["Enemies Don\'t Flank"] = [](){return new EnemiesDontFlank();}; };
	
	void begin() {
		busy = true;
		Menuing::getInstance()->setCombatAccessibility(COMBAT_ENEMIES_DONT_FLANK, 1, dualshock);
		busy = false;
	}
	void finish() {
		busy = true;
		Menuing::getInstance()->setCombatAccessibility(COMBAT_ENEMIES_DONT_FLANK, 0, dualshock);
		busy = false;
	}
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};

class ReducedEnemyPerception : public Modifier {
	bool busy;
public:
	static void regist() { Modifier::factory["Reduced Enemy Perception"] = [](){return new ReducedEnemyPerception();}; };
	
	void begin() {
		busy = true;
		Menuing::getInstance()->setCombatAccessibility(COMBAT_REDUCED_ENEMY_PERCEPTION, 1, dualshock);
		busy = false;
	}
	void finish() {
		busy = true;
		Menuing::getInstance()->setCombatAccessibility(COMBAT_REDUCED_ENEMY_PERCEPTION, 0, dualshock);
		busy = false;
	}
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};

class ReducedEnemyAccuracy : public Modifier {
	bool busy;
public:
	static void regist() { Modifier::factory["Reduced Enemy Accuracy"] = [](){return new ReducedEnemyAccuracy();}; };
	
	void begin() {
		busy = true;
		Menuing::getInstance()->setCombatAccessibility(COMBAT_REDUCED_ENEMY_ACCURACY, 1, dualshock);
		busy = false;
	}
	void finish() {
		busy = true;
		Menuing::getInstance()->setCombatAccessibility(COMBAT_REDUCED_ENEMY_ACCURACY, 0, dualshock);
		busy = false;
	}
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};

class EnhancedDodge : public Modifier {
	bool busy;
public:
	static void regist() { Modifier::factory["Enhanced Dodge"] = [](){return new EnhancedDodge();}; };
	
	void begin() {
		busy = true;
		Menuing::getInstance()->setCombatAccessibility(COMBAT_ENHANCED_DODGE, 1, dualshock);
		busy = false;
	}
	void finish() {
		busy = true;
		Menuing::getInstance()->setCombatAccessibility(COMBAT_ENHANCED_DODGE, 0, dualshock);
		busy = false;
	}
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};

class InvisibleWhileProne : public Modifier {
	bool busy;
public:
	static void regist() { Modifier::factory["Invisible While Prone"] = [](){return new InvisibleWhileProne();}; };
	
	void begin() {
		busy = true;
		Menuing::getInstance()->setCombatAccessibility(COMBAT_INVISBLE_WHILE_PRONE, 1, dualshock);
		busy = false;
	}
	void finish() {
		busy = true;
		Menuing::getInstance()->setCombatAccessibility(COMBAT_INVISBLE_WHILE_PRONE, 0, dualshock);
		busy = false;
	}
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};

class LockOnAim : public Modifier {
	bool busy;
public:
	static void regist() { Modifier::factory["Lock On Aim"] = [](){return new LockOnAim();}; };
	
	void begin() {
		busy = true;
		Menuing::getInstance()->setAlternateControls(ALTERNATE_LOCK_ON_AIM, 1, dualshock);
		busy = false;
	}
	void finish() {
		busy = true;
		Menuing::getInstance()->setAlternateControls(ALTERNATE_LOCK_ON_AIM, 0, dualshock);
		busy = false;
	}
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};
class AutoPickUp : public Modifier {
	bool busy;
public:
	static void regist() { Modifier::factory["Auto Pick-Up"] = [](){return new AutoPickUp();}; };
	
	void begin() {
		busy = true;
		Menuing::getInstance()->setAlternateControls(ALTERNATE_AUTO_PICK_UP, 1, dualshock);
		busy = false;
	}
	void finish() {
		busy = true;
		Menuing::getInstance()->setAlternateControls(ALTERNATE_AUTO_PICK_UP, 0, dualshock);
		busy = false;
	}
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};

class LargeHud : public Modifier {
	bool busy;
public:
	static void regist() { Modifier::factory["Large HUD"] = [](){return new LargeHud();}; };
	
	void begin() {
		busy = true;
		Menuing::getInstance()->setMagnificationAndAids(MAGNIFICATION_HUD_SCALE, 1, dualshock);
		busy = false;
	}
	void finish() {
		busy = true;
		Menuing::getInstance()->setMagnificationAndAids(MAGNIFICATION_HUD_SCALE, 0, dualshock);
		busy = false;
	}
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};

class HighContrastDisplay : public Modifier {
	bool busy;
public:
	static void regist() { Modifier::factory["High Contrast Display"] = [](){return new HighContrastDisplay();}; };
	
	void begin() {
		busy = true;
		Menuing::getInstance()->setMagnificationAndAids(MAGNIFICATION_HIGH_CONTRAST, 1, dualshock);
		busy = false;
	}
	void finish() {
		busy = true;
		Menuing::getInstance()->setMagnificationAndAids(MAGNIFICATION_HIGH_CONTRAST, 0, dualshock);
		busy = false;
	}
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};

class TraversalAudioCues : public Modifier {
	bool busy;
public:
	static void regist() { Modifier::factory["Traversal Audio Cues"] = [](){return new TraversalAudioCues();}; };
	
	void begin() {
		busy = true;
		Menuing::getInstance()->setTtsAndAudioCues(TTS_TRAVERSAL_CUES, 1, dualshock);
		busy = false;
	}
	void finish() {
		busy = true;
		Menuing::getInstance()->setTtsAndAudioCues(TTS_TRAVERSAL_CUES, 0, dualshock);
		busy = false;
	}
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};

class CombatAudioCues : public Modifier {
	bool busy;
public:
	static void regist() { Modifier::factory["Combat Audio Cues"] = [](){return new CombatAudioCues();}; };
	
	void begin() {
		busy = true;
		Menuing::getInstance()->setTtsAndAudioCues(TTS_COMBAT_CUES, 1, dualshock);
		busy = false;
	}
	void finish() {
		busy = true;
		Menuing::getInstance()->setTtsAndAudioCues(TTS_COMBAT_CUES, 0, dualshock);
		busy = false;
	}
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};

class TraversalAssistance : public Modifier {
	bool busy;
public:
	static void regist() { Modifier::factory["Traversal Assistance"] = [](){return new TraversalAssistance();}; };
	
	void begin() {
		busy = true;
		Menuing::getInstance()->setNavigationAndTraversal(NAVIGATION_TRAVERSAL_ASSISTANCE, 1, dualshock);
		busy = false;
	}
	void finish() {
		busy = true;
		Menuing::getInstance()->setNavigationAndTraversal(NAVIGATION_TRAVERSAL_ASSISTANCE, 0, dualshock);
		busy = false;
	}
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};

class EnhancedListenMode : public Modifier {
	bool busy;
public:
	static void regist() { Modifier::factory["Enhanced Listen Mode"] = [](){return new EnhancedListenMode();}; };
	
	void begin() {
		busy = true;
		Menuing::getInstance()->setNavigationAndTraversal(NAVIGATION_ENHANCED_LISTEN_MODE, 1, dualshock);
		busy = false;
	}
	void finish() {
		busy = true;
		Menuing::getInstance()->setNavigationAndTraversal(NAVIGATION_ENHANCED_LISTEN_MODE, 0, dualshock);
		busy = false;
	}
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};

class InfiniteBreath : public Modifier {
	bool busy;
public:
	static void regist() { Modifier::factory["Infinite Breath"] = [](){return new InfiniteBreath();}; };
	
	void begin() {
		busy = true;
		Menuing::getInstance()->setNavigationAndTraversal(NAVIGATION_INFINITE_BREATH, 1, dualshock);
		busy = false;
	}
	void finish() {
		busy = true;
		Menuing::getInstance()->setNavigationAndTraversal(NAVIGATION_INFINITE_BREATH, 0, dualshock);
		busy = false;
	}
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};

class Mystery : public Modifier {
	// Prototoxin
	bool dummy;
public:
	static void regist() {
		Modifier::factory["Mystery"] = []() {
			Mogi::Math::Random rng;
			int selection = rng.uniform(0, Modifier::factory.size()-1);
			printf("Selection = %d out of %d\n", selection, Modifier::factory.size());
			int count = 0;
			std::map<std::string, std::function<Modifier*()>>:: iterator it;
			for (it = Modifier::factory.begin();
				 it != Modifier::factory.end();
				 it++) {
				count++;
				if (count >= selection) {
					printf("Mystery: %s\n", it->first.c_str());
					if (it->first.compare("Mystery") == 0) {
						continue;
					}
					return it->second();
				}
			}
			return (--it)->second();
			//return new InfiniteBreath();
			//return new Mystery();	// will do nothing
		};
		
	};
};



int main(int argc, char** argv) {
	std::cout << "Welcome to Chaos" << std::endl;
	
	//ControllerGpio dualshock;
	//dualshock.initialize( "/dev/input/js0" );
	

	ControllerRaw dualshock;
	dualshock.initialize();
	dualshock.start();
	
	// Custom:
    Inverted::regist();
	Moonwalk::regist();
	MeleeOnly::regist();
	NoMelee::regist();
	NoTriangle::regist();
	NoClimbJump::regist();
	NoAiming::regist();
	NoScoping::regist();
	NoShoulderSwap::regist();
	NoReloading::regist();
    DisableCircle::regist();
    ForceProne::regist();
	FactionsPro::regist();
	Disco::regist();
	KeepJumping::regist();
	RachyFlail::regist();
	KeepDodging::regist();
	
	PeriodicListenMode::regist();
    NoRun::regist();
    ForceRun::regist();
    DisableJoystick::regist();
    SwapSticks::regist();
	SwapStickDpad::regist();
	MotionControls::regist();
	MotionControlAiming::regist();
	TouchpadAiming::regist();
	
    Nascar::regist();
    Drunk::regist();
	MegaScopeSway::regist();
	ProneDive::regist();
	
    NoGuns::regist();
    NoThrows::regist();
    MaxSensitivity::regist();
    ControllerMirror::regist();

	DeskPop::regist();
	TossMolly::regist();
	CtgStrat::regist();
	PdubIt::regist();
	ChuckStun::regist();


	// Main menu:
	RestartCheckpoint::regist();

	//  HUD settings:
	NoReticle::regist();

	// Render modes:
    Graphic::regist();
    Headache::regist();
    Afterlife::regist();
    EightBit::regist();
    WaterColor::regist();
    Dungeon::regist();
    Void::regist();
    PopPoster::regist();
	NineteenSixty::regist();
	Cool::regist();
	Warm::regist();
	Vibrant::regist();
	Noir::regist();
	Blorange::regist();
	Sepia::regist();
	Vintage::regist();
	Beasts::regist();
	Terminated::regist();
	GridLocked::regist();	// Working
	Blacklight::regist();
	DesertFog::regist();
	Blood::regist();
	Inferno::regist();		// Working
	Fire::regist();
	Trinity::regist();
	Pusher::regist();
	Demons::regist();
	DrugZone::regist();
	Moonlight::regist();

	// Gameplay Modes:
    Mirror::regist();		// Render mode
	MirrorOnDeath::regist();
	SlowMotion::regist();
	BulletSpeed::regist();
	InfiniteAmmo::regist();
    InfiniteCrafting::regist();
	InfiniteMeleeDurability::regist();
	InfiniteListenRange::regist();
	OneShot::regist();
	TouchOfDeath::regist();

	// Gameplay audio:
	EightBitAudio::regist();
	FourBitAudio::regist();
	HeliumAudio::regist();
	XenonAudio::regist();

	// Alternative Controls:
	LockOnAim::regist();
	AutoPickUp::regist();

	// Magnification:
	LargeHud::regist();
	HighContrastDisplay::regist();	// Render mode

	// Navigation And Traversal
	TraversalAssistance::regist();
	EnhancedListenMode::regist();
	InfiniteBreath::regist();

	// TTS:
	TraversalAudioCues::regist();	// audio
	CombatAudioCues::regist();		//audio

	// Combat Accessibilities:
	HostagesDontEscape::regist();
	EnemiesDontFlank::regist();
	ReducedEnemyPerception::regist();
	ReducedEnemyAccuracy::regist();
	EnhancedDodge::regist();
	InvisibleWhileProne::regist();
	
	StrafeOnly::regist();
	NoStrafe::regist();
	ForceAim::regist();
	SpeedrunGlitch::regist();
	
	Mystery::regist();
	
	// Custom: 44
	// Audio: 6
	// Render: 31
	// Gameplay: 23
	
	int i = 1;
	printf("Registered Modfiers: \n");
	for (auto const& it : Modifier::factory) {
		//std::cout << "Registered Mod: " << it.first << std::endl;
		printf("%3d: %s\n", i++, it.first.c_str());
	}
	
	
	
	ChaosEngine chaosEngine(&dualshock);
	chaosEngine.start();
	
	double timePerModifier = 30.0;
	chaosEngine.setTimePerModifier(timePerModifier);
	
	

	while(1) {
		chaosEngine.setInterfaceReply( Modifier::getModList(timePerModifier) );
		usleep(10000000);
	}

	return 0;
}
