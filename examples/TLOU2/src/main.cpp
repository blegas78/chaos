#include <cstdio>
#include <iostream>
#include <unistd.h>
#include <cmath>

#include <queue>

#include "chaos.h"
#include "menuing.h"

#define JOYSTICK_MIN (-128)
#define JOYSTICK_MAX (127)

using namespace Chaos;

inline short joystickLimit( int input ) {
	return fmin( fmax( input, JOYSTICK_MIN), JOYSTICK_MAX);
}

class Inverted : public Chaos::Modifier {
public:
	static void regist() { Chaos::Modifier::factory["Inverted"] = [](){return new Inverted();}; };
	const char* description() { return "Inverts the Y-axis on the right joystick (aim inversion)"; };
	
	bool tweak( DeviceEvent* event ) {
		if ((event->id == AXIS_RY ) && event->type == TYPE_AXIS) {
			event->value = -((int)event->value+1) ;
		}
		return true;
	}
};

// Suggestion from protoxin:
class Moonwalk : public Chaos::Modifier {
public:
	static void regist() { Chaos::Modifier::factory["Moonwalk"] = [](){return new Moonwalk();}; };
	const char* description() { return "Be like Michael Jackson!  Trying to walk forward will actually make you go backward"; };
	
	bool tweak( DeviceEvent* event ) {
		if ((event->id == AXIS_LY ) && event->type == TYPE_AXIS) {
			event->value = -((int)event->value+1) ;
		}
		return true;
	}
};

class SidewaysMoonwalk : public Chaos::Modifier {
public:
	static void regist() { Chaos::Modifier::factory["Sideways Moonwalk"] = [](){return new SidewaysMoonwalk();}; };
	const char* description() { return "Go left to go right and go right to go left"; };
	
	bool tweak( DeviceEvent* event ) {
		if ((event->id == AXIS_LX ) && event->type == TYPE_AXIS) {
			event->value = -((int)event->value+1) ;
		}
		return true;
	}
};

class MeleeOnly : public Chaos::Modifier {
public:
	static void regist() { Chaos::Modifier::factory["Melee Only"] = [](){return new MeleeOnly();}; };
	const char* description() { return "No shooting and no throwables"; };
	
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

class NoMelee : public Chaos::Modifier {
public:
	static void regist() { Chaos::Modifier::factory["No Melee"] = [](){return new NoMelee();}; };
	const char* description() { return "Square presses are feeble"; };
	
	bool tweak( DeviceEvent* event ) {
		if (event->id == BUTTON_SQUARE && event->type == TYPE_BUTTON) {
			event->value = 0;
		}
		return true;
	}
};

class Pacifist: public Chaos::Modifier {
public:
	static void regist() { Chaos::Modifier::factory["Pacifist%"] = [](){return new Pacifist();}; };
	const char* description() { return "No shooting, throwing, nor melee. Good luck NPC friends!"; };
	bool tweak( DeviceEvent* event ) {
		if (event->type == TYPE_BUTTON &&
			(event->id == BUTTON_SQUARE ||	// disable Melee
			 event->id == BUTTON_R2) ) {	// Disable Shooting, blind fire
			event->value = 0;
		} else if(event->type == TYPE_AXIS &&
				  event->id == AXIS_R2) {
			event->value = JOYSTICK_MIN;
		}
		return true;
	}
};

class NoTriangle : public Chaos::Modifier {
public:
	static void regist() { Chaos::Modifier::factory["No Triangle"] = [](){return new NoTriangle();}; };
	const char* description() { return "No looting, stealth grabbing, opening doors. Say goodbye to the everything button"; };
	bool tweak( DeviceEvent* event ) {
		if (event->id == BUTTON_TRIANGLE && event->type == TYPE_BUTTON) {
			event->value = 0;
		}
		return true;
	}
};

class NoClimbJump : public Chaos::Modifier {
public:
	static void regist() { Chaos::Modifier::factory["No Climbing/Jumping"] = [](){return new NoClimbJump();}; };
	const char* description() { return "No X presses"; };
	bool tweak( DeviceEvent* event ) {
		if (event->id == BUTTON_X && event->type == TYPE_BUTTON) {
			event->value = 0;
		}
		return true;
	}
};

class NoAiming : public Chaos::Modifier {
public:
	static void regist() { Chaos::Modifier::factory["No Aiming"] = [](){return new NoAiming();}; };
	const char* description() { return "At least reloading is still possible, along with throwables and melee"; };
	bool tweak( DeviceEvent* event ) {
		if (event->id == BUTTON_L2 && event->type == TYPE_BUTTON) {
			event->value = 0;
		}
		return true;
	}
};

class NoScoping : public Chaos::Modifier {
public:
	static void regist() { Chaos::Modifier::factory["No Scoping"] = [](){return new NoScoping();}; };
	const char* description() { return "No scopes! Aiming is still allowed"; };
	bool tweak( DeviceEvent* event ) {
		if (event->id == BUTTON_X && event->type == TYPE_BUTTON) {
			if (dualshock->getState(BUTTON_L2, TYPE_BUTTON)) {	// only disable X when aiming
				event->value = 0;
			}
		}
		return true;
	}
};

class NoShoulderSwap : public Chaos::Modifier {
public:
	static void regist() { Chaos::Modifier::factory["No Shoulder Swap"] = [](){return new NoShoulderSwap();}; };
	const char* description() { return "While aiming, Square is disabled"; };
	bool tweak( DeviceEvent* event ) {
		if (event->id == BUTTON_SQUARE && event->type == TYPE_BUTTON) {
			if (dualshock->getState(BUTTON_L2, TYPE_BUTTON)) {	// only disable X when aiming
				event->value = 0;
			}
		}
		return true;
	}
};

class NoReloading : public Chaos::Modifier {
	bool dxLastPessed = true;
	bool aimHeld = false;
public:
	static void regist() { Chaos::Modifier::factory["No Reloading"] = [](){return new NoReloading();}; };
	const char* description() { return "If guns were last selected, R2 is disabled without L2"; };
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

class DisableCircle : public Chaos::Modifier {
public:
	static void regist() { Chaos::Modifier::factory["No Crouch/Prone"] = [](){return new DisableCircle();}; };
	const char* description() { return "Circle is disabled"; };
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

class ForceProne : public Chaos::Modifier {
public:
	static void regist() { Chaos::Modifier::factory["Keep Proning"] = [](){return new ForceProne();}; };
	const char* description() { return "Proning at inconvenient times"; };
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

class FactionsPro : public Chaos::Modifier {
public:
	static void regist() { Chaos::Modifier::factory["Factions Pro"] = [](){return new FactionsPro();}; };
	const char* description() { return "Keeping things toxic with teabagging"; };
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
		} else if( pressTime > 0.6 && !dualshock->getState(BUTTON_CIRCLE, TYPE_BUTTON) ) {
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

class Disco : public Chaos::Modifier {
	// Rename/tuning from HeHathYought
public:
	static void regist() { Chaos::Modifier::factory["Anthony Caliber"] = [](){return new Disco();}; };
	const char* description() { return "Flashlight flickering"; };
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

class KeepJumping : public Chaos::Modifier {
public:
	static void regist() { Chaos::Modifier::factory["Keep Jumping"] = [](){return new KeepJumping();}; };
	const char* description() { return "Repeated presses of X"; };
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

class RachyFlail : public Chaos::Modifier {
public:
	static void regist() { Chaos::Modifier::factory["Rachy Flail"] = [](){return new RachyFlail();}; };
	const char* description() { return "The only way to melee is to follow the master techniques of RachyMonster"; };
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

class KeepDodging : public Chaos::Modifier {
public:
	static void regist() { Chaos::Modifier::factory["Keep Dodging"] = [](){return new KeepDodging();}; };
	const char* description() { return "Repeated presses of Circle"; };
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

class PeriodicListenMode : public Chaos::Modifier {
public:
	static void regist() { Chaos::Modifier::factory["Periodic Listen Mode"] = [](){return new PeriodicListenMode();}; };
	const char* description() { return "Repeated holds of R1"; };
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

class NoRun : public Chaos::Modifier {
public:
	static void regist() { Chaos::Modifier::factory["No Run/Dodge"] = [](){return new NoRun();}; };
	const char* description() { return "L1 is disabled"; };
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

class ForceRun : public Chaos::Modifier {
public:
	static void regist() { Chaos::Modifier::factory["Force Running"] = [](){return new ForceRun();}; };
	const char* description() { return "L1 is held down (mostly)"; };
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

class BadStamina : public Chaos::Modifier {
public:
	static void regist() { Chaos::Modifier::factory["Bad Stamina"] = [](){return new BadStamina();}; };
	const char* description() { return "Running gets disabled after 2 seconds and takes 4 seconds to recharge."; };
	double stateTime;
	bool pressedState;
	double stamina;
	
	enum {
		STAMINA_GOOD,
		STAMINA_BAD
	} state;
	
	void begin() {
		pressedState = dualshock->getState(BUTTON_L1, TYPE_BUTTON);
		stamina = 0.0;
		state = STAMINA_GOOD;
		stateTime = 0.0;
	}
	void update() {
		stateTime = timer.dTime();
		
		if (state == STAMINA_GOOD) {
			if (dualshock->getState(BUTTON_L1, TYPE_BUTTON)) {
				stamina += stateTime;
				if (stamina > 2.0) {
					stamina = 2.0;
					state = STAMINA_BAD;
					DeviceEvent event = {0, 0, TYPE_BUTTON, BUTTON_L1};
					chaosEngine->fakePipelinedEvent(&event, me);
				}
			} else {
				stamina -= stateTime/2.0;
				if (stamina < 0.0) {
					stamina = 0;
				}
			}
		} else if (state == STAMINA_BAD) {
			stamina -= stateTime/2.0;
			if (stamina < 0.0) {
				stamina = 0;
				state = STAMINA_GOOD;
			}
		}
	}
	bool tweak( DeviceEvent* event ) {
		if (event->id == BUTTON_L1 && event->type == TYPE_BUTTON) {
			return state == STAMINA_GOOD;
		}
		return true;
	}
};

class LeeroyJenkins : public Chaos::Modifier {
public:
	static void regist() { Chaos::Modifier::factory["Leeroy Jenkins"] = [](){return new LeeroyJenkins();}; };
	const char* description() { return "Alright let's do this! LEEEEROOOOOY NNNJEEEENNKIINNS!!! (Holds forward and sprint)"; };
	double pressTime;
	void begin() {
		DeviceEvent event = {0,1,TYPE_BUTTON, BUTTON_L1};
		dualshock->applyEvent(&event);
		event = {0,JOYSTICK_MIN,TYPE_AXIS, AXIS_LY};
		dualshock->applyEvent(&event);
	}
	void update() {
		pressTime += timer.dTime();
		DeviceEvent event = {0,JOYSTICK_MIN,TYPE_AXIS, AXIS_LY};
		dualshock->applyEvent(&event);
		if (pressTime > 4 && dualshock->getState(BUTTON_L1, TYPE_BUTTON)) {
			event = {0,0,TYPE_BUTTON, BUTTON_L1};
			dualshock->applyEvent(&event);
			pressTime = 0;
		} else if ( pressTime > 0.1 && !dualshock->getState(BUTTON_L1, TYPE_BUTTON)) {
			event = {0,1,TYPE_BUTTON, BUTTON_L1};
			dualshock->applyEvent(&event);
			pressTime = 0;
		}
	}
	void finish() {
		DeviceEvent event = {0,0,TYPE_BUTTON, BUTTON_L1};
		dualshock->applyEvent(&event);
		event = {0,0,TYPE_AXIS, AXIS_LY};
		dualshock->applyEvent(&event);
	}
	bool tweak( DeviceEvent* event ) {
		if (event->id == BUTTON_L1 && event->type == TYPE_BUTTON) {
			event->value = 1;
		}
		if (event->id == AXIS_LY && event->type == TYPE_AXIS) {
//			event->value = JOYSTICK_MIN;
			return false;
		}
		return true;
	}
};

class ForceAim : public Chaos::Modifier {
	//Prototoxin187
public:
	static void regist() { Chaos::Modifier::factory["Force Aiming"] = [](){return new ForceAim();}; };
	const char* description() { return "L2 is held"; };
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
			event = {0,JOYSTICK_MIN,TYPE_AXIS, AXIS_L2};
			dualshock->applyEvent(&event);
			pressTime = 0;
		} else if ( pressTime > 0.1 && !dualshock->getState(BUTTON_L2, TYPE_BUTTON)) {
			DeviceEvent event = {0,1,TYPE_BUTTON, BUTTON_L2};
			dualshock->applyEvent(&event);
			event = {0,JOYSTICK_MAX,TYPE_AXIS, AXIS_L2};
			dualshock->applyEvent(&event);
			pressTime = 0;
		}
	}
	void finish() {
		DeviceEvent event = {0,0,TYPE_BUTTON, BUTTON_L2};
		dualshock->applyEvent(&event);
		event = {0,JOYSTICK_MIN,TYPE_AXIS, AXIS_L2};
		dualshock->applyEvent(&event);
	}
	bool tweak( DeviceEvent* event ) {
		if (event->id == BUTTON_L2 && event->type == TYPE_BUTTON) {
			event->value = 1;
		} else if (event->id == AXIS_L2 && event->type == TYPE_AXIS) {
			event->value = JOYSTICK_MAX;
		}
		return true;
	}
};

class DisableJoystick : public Chaos::Modifier {
public:
	static void regist() { Chaos::Modifier::factory["Disable Right Joystick"] = [](){return new DisableJoystick();}; };
	const char* description() { return "Good luck with aiming and looking around!"; };
	bool tweak( DeviceEvent* event ) {
		if ( (event->id == AXIS_RX || event->id == AXIS_RY) && event->type == TYPE_AXIS) {
			event->value = 0;
		}
		return true;
	}
};

class DisableJoystickHorizontal : public Chaos::Modifier {
public:
	static void regist() { Chaos::Modifier::factory["No Horizontal Aim"] = [](){return new DisableJoystickHorizontal();}; };
	const char* description() { return "The right joystick right/left is disabled"; };
	bool tweak( DeviceEvent* event ) {
		if ( (event->id == AXIS_RX) && event->type == TYPE_AXIS) {
			event->value = 0;
		}
		return true;
	}
};

class DisableDpad : public Chaos::Modifier {
	// JustSaft
public:
	static void regist() { Chaos::Modifier::factory["Disable D-pad"] = [](){return new DisableDpad();}; };
	const char* description() { return "Inventory selection is disabled"; };
	bool tweak( DeviceEvent* event ) {
		if ( (event->id == AXIS_DX || event->id == AXIS_DY) && event->type == TYPE_AXIS) {
			event->value = 0;
		}
		return true;
	}
};

class DpadRotate : public Chaos::Modifier {
public:
	static void regist() { Chaos::Modifier::factory["D-pad Rotate"] = [](){return new DpadRotate();}; };
	const char* description() { return "Rotates the D-pad inputs clockwise"; };
	bool tweak( DeviceEvent* event ) {
		if (event->type == TYPE_AXIS) {
			if (event->id == AXIS_DX) {
				event->id = AXIS_DY;
			} else if (event->id == AXIS_DY) {
				event->id = AXIS_DX;
				event->value = joystickLimit( -event->value );
			}
		}
		
		return true;
	}
};

class Moose : public Chaos::Modifier {
public:
	static void regist() { Chaos::Modifier::factory["Moose"] = [](){return new Moose();}; };
	const char* description() { return "The moose is dead.  The moose does not move.  (Disables Left Joystick)"; };
	bool tweak( DeviceEvent* event ) {
		if ( (event->id == AXIS_LX || event->id == AXIS_LY) && event->type == TYPE_AXIS) {
			event->value = 0;
		}
		return true;
	}
};

class StrafeOnly : public Chaos::Modifier {
	//Prototoxin187
public:
	static void regist() { Chaos::Modifier::factory["Only Strafe Movement"] = [](){return new StrafeOnly();}; };
	const char* description() { return "X-axis on the left joystick is disabled.  Only forward/back motion is allowed."; };
	bool tweak( DeviceEvent* event ) {
		if ( event->id == AXIS_LY && event->type == TYPE_AXIS) {
			event->value = 0;
		}
		return true;
	}
};

class NoStrafe : public Chaos::Modifier {
public:
	static void regist() { Chaos::Modifier::factory["No Strafing"] = [](){return new NoStrafe();}; };
	const char* description() { return "Y-axis on the left joystick is disabled.  Only left/right motion is allowed."; };
	bool tweak( DeviceEvent* event ) {
		if ( event->id == AXIS_LX && event->type == TYPE_AXIS) {
			event->value = 0;
		}
		return true;
	}
};

class AimMovement : public Chaos::Modifier {
public:
	static void regist() { Chaos::Modifier::factory["Only Aim Movement"] = [](){return new AimMovement();}; };
	const char* description() { return "Left joystick is disabled if not aiming"; };
	
	bool tweak( DeviceEvent* event ) {
		if ((event->id == AXIS_LX || event->id == AXIS_LY) &&
			event->type == TYPE_AXIS &&
			!dualshock->getState(BUTTON_L2, TYPE_BUTTON) ) {
			event->value = 0;
		}
		return true;
	}
};

class SwapSticks : public Chaos::Modifier {
public:
	static void regist() { Chaos::Modifier::factory["Swap Joysticks"] = [](){return new SwapSticks();}; };
	const char* description() { return "You may want to cross your thumbs to work with your muscle memory"; };
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

class SwapStickDpad : public Chaos::Modifier {
	//JustForSaft, joshuatimes7
public:
	static void regist() { Chaos::Modifier::factory["Swap D-Pad/Left Joystick"] = [](){return new SwapStickDpad();}; };
	const char* description() { return "Retro style motion, and an odd analog inventory selector"; };
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

class SwapStickShapes : public Chaos::Modifier {
public:
	DeviceEvent fakeEvent;
	static void regist() { Chaos::Modifier::factory["Swap Shapes/Right Joystick"] = [](){return new SwapStickShapes();}; };
	const char* description() { return "Analog actions, and digital camera movement"; };
	void begin() {
		DeviceEvent event = {0,0,TYPE_AXIS, AXIS_RY};
		dualshock->applyEvent(&event);
		event = {0,0,TYPE_AXIS, AXIS_RX};
		dualshock->applyEvent(&event);
	}
	bool tweak( DeviceEvent* event ) {
		if( event->type == TYPE_AXIS ) {
			switch (event->id) {
//				case AXIS_DX: event->id = AXIS_LX; event->value = joystickLimit(JOYSTICK_MAX*event->value); break;
//				case AXIS_DY: event->id = AXIS_LY; event->value = joystickLimit(JOYSTICK_MAX*event->value); break;
				case AXIS_RX:
					event->type = TYPE_BUTTON;// AXIS_DX;
					if (event->value < JOYSTICK_MIN/2) {
						event->id = BUTTON_SQUARE;
						event->value = 1;
					} else if (event->value > JOYSTICK_MAX/2) {
						event->id = BUTTON_CIRCLE;
						event->value = 1;
					} else {
						event->id = BUTTON_CIRCLE;
						event->value = 0;
						
						fakeEvent.id = BUTTON_SQUARE;
						fakeEvent.value = 0;
						chaosEngine->fakePipelinedEvent(&fakeEvent, me);
					}
					//event->value = event->value > JOYSTICK_MAX/2 ? 1 : event->value < JOYSTICK_MIN/2 ? -1 : 0;
					break;
				case AXIS_RY:
						event->type = TYPE_BUTTON;// AXIS_DX;
						if (event->value < JOYSTICK_MIN/2) {
							event->id = BUTTON_TRIANGLE;
							event->value = 1;
						} else if (event->value > JOYSTICK_MAX/2) {
							event->id = BUTTON_X;
							event->value = 1;
						} else {
							event->id = BUTTON_TRIANGLE;
							event->value = 0;
							
							fakeEvent.id = BUTTON_X;
							fakeEvent.value = 0;
							chaosEngine->fakePipelinedEvent(&fakeEvent, me);
						}
					break;
				default: break;
			}
		} else if (event->type == TYPE_BUTTON) {
			switch (event->id) {
				case BUTTON_SQUARE:
					event->type = TYPE_AXIS;
					event->id = AXIS_RX;
					event->value = event->value ? JOYSTICK_MIN : 0;
					break;
				case BUTTON_CIRCLE:
					event->type = TYPE_AXIS;
					event->id = AXIS_RX;
					event->value = event->value ? JOYSTICK_MAX : 0;
					break;
				case BUTTON_TRIANGLE:
					event->type = TYPE_AXIS;
					event->id = AXIS_RY;
					event->value = event->value ? JOYSTICK_MIN : 0;
					break;
				case BUTTON_X:
					event->type = TYPE_AXIS;
					event->id = AXIS_RY;
					event->value = event->value ? JOYSTICK_MAX : 0;
					break;
					
				default:
					break;
			}
		}
		
		return true;
	}
};

class MotionControls : public Chaos::Modifier {
public:
	static void regist() { Chaos::Modifier::factory["Motion Control Movement"] = [](){return new MotionControls();}; };
	const char* description() { return "No more left joystick, tilt the controller to move instead"; };
	bool tweak( DeviceEvent* event ) {
		if( event->type == TYPE_AXIS ) {
			DeviceEvent newEvent;
			switch (event->id) {
				case AXIS_ACCX:
					newEvent = *event;
					newEvent.id = AXIS_LX;
					newEvent.value = joystickLimit(-event->value/20);
					chaosEngine->fakePipelinedEvent(&newEvent, me);
					break;
				case AXIS_ACCZ:
					newEvent = *event;
					newEvent.id = AXIS_LY;
					newEvent.value = joystickLimit(-event->value/20);
					chaosEngine->fakePipelinedEvent(&newEvent, me);
					break;
				case AXIS_LX: return false;
				case AXIS_LY: return false;
				default: break;
			}
		}
		return true;
	}
};

class MotionControlAiming : public Chaos::Modifier {
public:
	static void regist() { Chaos::Modifier::factory["Motion Control Aiming"] = [](){return new MotionControlAiming();}; };
	const char* description() { return "No more right joystick, tilt the controller to aim/look instead"; };
	bool tweak( DeviceEvent* event ) {
		if( event->type == TYPE_AXIS ) {
			DeviceEvent newEvent;
			switch (event->id) {
				case AXIS_ACCX:
					newEvent = *event;
					newEvent.id = AXIS_RX;
					newEvent.value = joystickLimit(-event->value/24);
					chaosEngine->fakePipelinedEvent(&newEvent, me);
					break;
				case AXIS_ACCZ:
					newEvent = *event;
					newEvent.id = AXIS_RY;
					newEvent.value = joystickLimit(event->value/24);	// This makes sense to be inverted
					chaosEngine->fakePipelinedEvent(&newEvent, me);
					break;
				case AXIS_RX: return false;
				case AXIS_RY: return false;
				default: break;
			}
		}
		return true;
	}
};

class TouchpadAiming : public Chaos::Modifier {
public:
	static void regist() { Chaos::Modifier::factory["Touchpad Aiming"] = [](){return new TouchpadAiming();}; };
	const char* description() { return "No more right joystick, finally making use of the touchpad!"; };
	
	short priorX[5];
	short priorY[5];
	double timestampPriorX[5];
	double timestampPriorY[5];
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
			if(timestampX != timestampPriorX[0]) {
				ret = ((double)(currentX - priorX[0]))/(timestampX-timestampPriorX[0]);
			}
		} else {
			priorActiveX = true;
			priorX[1] = priorX[2] = priorX[3] = priorX[4] = currentX;
			timestampPriorX[1] = timestampPriorX[2] =  timestampPriorX[3] =  timestampPriorX[4] = timestampX;
		}
		priorX[0] = priorX[1];
		priorX[1] = priorX[2];
		priorX[2] = priorX[3];
		priorX[3] = priorX[4];
		priorX[4] = currentX;
		timestampPriorX[0] = timestampPriorX[1];
		timestampPriorX[1] = timestampPriorX[2];
		timestampPriorX[2] = timestampPriorX[3];
		timestampPriorX[3] = timestampPriorX[4];
		timestampPriorX[4] = timestampX;
		return ret;
	}
	double derivativeY(short currentY, double timestampY) {
		double ret = 0;
		if (priorActiveY) {
			if(timestampY != timestampPriorY[0]) {
				ret = ((double)(currentY - priorY[0]))/(timestampY-timestampPriorY[0]);
			}
		} else {
			priorActiveY = true;
			priorY[1] = priorY[2] = priorY[3] = priorY[4] = currentY;
			timestampPriorY[1] = timestampPriorY[2] = timestampPriorY[3] = timestampPriorY[4] = timestampY;
		}
		priorY[0] = priorY[1];
		priorY[1] = priorY[2];
		priorY[2] = priorY[3];
		priorY[3] = priorY[4];
		priorY[4] = currentY;
		timestampPriorY[0] = timestampPriorY[1];
		timestampPriorY[1] = timestampPriorY[2];
		timestampPriorY[2] = timestampPriorY[3];
		timestampPriorY[3] = timestampPriorY[4];
		timestampPriorY[4] = timestampY;
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
				chaosEngine->fakePipelinedEvent(&newEvent, me);
				newEvent.id = AXIS_RY;
				newEvent.type = TYPE_AXIS;
				newEvent.value = 0;
				chaosEngine->fakePipelinedEvent(&newEvent, me);
				//	printf("Falling Edge TOuch\n");
			}
			
			activeTouch = !event->value;
		}
		if( event->type == TYPE_AXIS ) {
			DeviceEvent newEvent;
			double derivativeValue;
			switch (event->id) {
				case AXIS_TOUCHPAD_X:
					if(activeTouch ) {
						newEvent.id = AXIS_RX;
						newEvent.type = TYPE_AXIS;
						derivativeValue = (dualshock->getState(BUTTON_L2, TYPE_BUTTON) ? 0.04 : 0.12) * derivativeX(event->value, timer.runningTime());
						if (derivativeValue > 0) {
							newEvent.value = joystickLimit(derivativeValue + 30);
						} else if(derivativeValue < 0) {
							newEvent.value = joystickLimit(derivativeValue - 30);
						} else  {
							newEvent.value = 0;
						}
						chaosEngine->fakePipelinedEvent(&newEvent, me);
					}
					//printf("x derivative: %d\n", newEvent.value);
					break;
				case AXIS_TOUCHPAD_Y:
					if(activeTouch ) {
						newEvent.id = AXIS_RY;
						newEvent.type = TYPE_AXIS;
						derivativeValue = (dualshock->getState(BUTTON_L2, TYPE_BUTTON) ? 0.04 : 0.12) * derivativeY(event->value, timer.runningTime());
						if (derivativeValue > 0) {
							newEvent.value = joystickLimit(derivativeValue + 30);
						} else if(derivativeValue < 0 ) {
							newEvent.value = joystickLimit(derivativeValue - 30);
						} else  {
							newEvent.value = 0;
						}
						chaosEngine->fakePipelinedEvent(&newEvent, me);
					}
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
		//					chaosEngine->fakePipelinedEvent(&newEvent, me);
		//					break;
		//				case AXIS_TOUCHPAD_Y:
		//					newEvent = *event;
		//					newEvent.id = AXIS_RY;
		//					newEvent.value = joystickLimit(((double)event->value/500 - 1) * JOYSTICK_MAX * 1.1 );
		//					chaosEngine->fakePipelinedEvent(&newEvent, me);
		//					break;
		//				case AXIS_RX: return false;
		//				case AXIS_RY: return false;
		//				default: break;
		//			}
		//		}
		return true;
	}
};

class Nascar : public Chaos::Modifier {
public:
	static void regist() { Chaos::Modifier::factory["Nascar"] = [](){return new Nascar();}; };
	const char* description() { return "There is no going right in Nascar.  Right movement/camera disabled"; };
	bool tweak( DeviceEvent* event ) {
		if( event->type == TYPE_AXIS ) {
			if (event->id == AXIS_RX || event->id == AXIS_LX) {
				event->value = event->value <= 0 ? event->value : 0;
			}
		}
		return true;
	}
};


class Zoolander : public Chaos::Modifier {
	// carnalgasyeah
public:
	static void regist() { Chaos::Modifier::factory["Zoolander"] = [](){return new Zoolander();}; };
	const char* description() { return "We are not an ambiturner, but at least we can look right.  Left movement/camera disabled."; };
	bool tweak( DeviceEvent* event ) {
		if( event->type == TYPE_AXIS ) {
			if (event->id == AXIS_RX || event->id == AXIS_LX) {
				event->value = event->value >= 0 ? event->value : 0;
			}
		}
		return true;
	}
};

class NoBackward : public Chaos::Modifier {
public:
	static void regist() { Chaos::Modifier::factory["No Backward Movement"] = [](){return new NoBackward();}; };
	const char* description() { return "Moving backwards is not allowed"; };
	bool tweak( DeviceEvent* event ) {
		if( event->type == TYPE_AXIS ) {
			if (event->id == AXIS_LY) {
				event->value = event->value <= 0 ? event->value : 0;
			}
		}
		return true;
	}
};

class NoForward : public Chaos::Modifier {
public:
	static void regist() { Chaos::Modifier::factory["No Forward Movement"] = [](){return new NoForward();}; };
	const char* description() { return "Moving forward is not allowed"; };
	bool tweak( DeviceEvent* event ) {
		if( event->type == TYPE_AXIS ) {
			if (event->id == AXIS_LY) {
				event->value = event->value >= 0 ? event->value : 0;
			}
		}
		return true;
	}
};

class Drunk : public Chaos::Modifier {
public:
	static void regist() { Chaos::Modifier::factory["Drunk"] = [](){return new Drunk();}; };
	const char* description() { return "Random joystick motion.  Also don't push things too far, you may stumble (go prone)"; };
	
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
			chaosEngine->fakePipelinedEvent(&event, me);
			
		}
		if (timer.runningTime()-buttonPressTime > 1.0) {
			DeviceEvent fallDown = {0,0,TYPE_BUTTON,BUTTON_CIRCLE};
			dualshock->applyEvent(&fallDown);
			buttonPressTime += 10000;
		}
	}
	
	void finish() {
		DeviceEvent event = {0,(short)axisToValue[AXIS_LX],TYPE_AXIS,AXIS_LX};
		chaosEngine->fakePipelinedEvent( &event, me);
		event = {0,(short)axisToValue[AXIS_LY],TYPE_AXIS,AXIS_LY};
		chaosEngine->fakePipelinedEvent( &event, me);
		event = {0,(short)axisToValue[AXIS_RX],TYPE_AXIS,AXIS_RX};
		chaosEngine->fakePipelinedEvent( &event, me);
		event = {0,(short)axisToValue[AXIS_RY],TYPE_AXIS,AXIS_RY};
		chaosEngine->fakePipelinedEvent( &event, me);
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

class SpeedrunGlitch : public Chaos::Modifier {
	// HeHathYought
public:
	static void regist() { Chaos::Modifier::factory["TLOU1 Run Glitch"] = [](){return new SpeedrunGlitch();}; };
	const char* description() { return "Is this an Any% speedrun of TLOU1? Rapid D-pad circular presses"; };
	
	void update() {
		DeviceEvent event;	// Event to inject
		event.type = TYPE_AXIS;
		
		double i = 0.0;
		double newValue;
		double t = timer.runningTime()*8.0*MOGI_PI;
		
		event.value = 1.5*cos(t);
		event.id = AXIS_DX;
		chaosEngine->fakePipelinedEvent( &event, me);
		
		event.value = 1.5*sin(t);
		event.id = AXIS_DY;
		chaosEngine->fakePipelinedEvent( &event, me);
		
	}
	
	void finish() {
		DeviceEvent event = {0,0,TYPE_AXIS,AXIS_DX};
		chaosEngine->fakePipelinedEvent( &event, me);
		event = {0,0,TYPE_AXIS,AXIS_DY};
		chaosEngine->fakePipelinedEvent( &event, me);
	}
	
	bool tweak( DeviceEvent* event ) {
		if( event->type == TYPE_AXIS &&
		   (event->id == AXIS_DY || event->id == AXIS_DX)) {
			return false;
		}
		return true;
	}
};

class MegaScopeSway : public Chaos::Modifier {
public:
	static void regist() { Chaos::Modifier::factory["Mega Scope Sway"] = [](){ return new MegaScopeSway();}; };
	const char* description() { return "Good luck landing shots"; };
	
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
				chaosEngine->fakePipelinedEvent(&event, me);
				i += 1;
			} else {
				offsetValue[event.id]  = 0;
			}
			
		}
		
	}
	
	void finish() {
		DeviceEvent event = {0,(short)axisToValue[AXIS_RX],TYPE_AXIS,AXIS_RX};
		chaosEngine->fakePipelinedEvent( &event, me);
		event = {0,(short)axisToValue[AXIS_RY],TYPE_AXIS,AXIS_RY};
		chaosEngine->fakePipelinedEvent( &event, me);
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

class ProneDive : public Chaos::Modifier {
public:
	static void regist() { Chaos::Modifier::factory["Only Prone Diving"] = [](){ return new ProneDive();}; };
	const char* description() { return "RIP elbows and knees. Any movement results in a prone dive"; }
	
	//	std::map<int,int> axisToValue;
	//std::map<int,int> offsetValue;
	double sequenceTime;
	bool inSequence;
	double magnitudeSquared;
	
	double activeThreshold;
	
	void begin() {
		//		axisToValue[AXIS_LX] = dualshock->getState(AXIS_LX, TYPE_AXIS);
		//		axisToValue[AXIS_LY] = dualshock->getState(AXIS_LY, TYPE_AXIS);
		
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

class Rubbernecking : public Chaos::Modifier {
public:
	static void regist() { Chaos::Modifier::factory["Rubbernecking"] = [](){ return new Rubbernecking();}; };
	const char* description() { return "Woah!  What's behind you?  Invokes periodic 180 quick turn"; }
	
	//	std::map<int,int> axisToValue;
	//std::map<int,int> offsetValue;
	double sequenceTime;
	bool inSequence;
	double magnitudeSquared;
	
	//	double activeThreshold;
	
	void begin() {
		//		axisToValue[AXIS_LX] = dualshock->getState(AXIS_LX, TYPE_AXIS);
		//		axisToValue[AXIS_LY] = dualshock->getState(AXIS_LY, TYPE_AXIS);
		
		sequenceTime = 0;
		inSequence = false;
		
		//		activeThreshold = pow( JOYSTICK_MAX*0.2, 2);
	}
	
	void update() {
		DeviceEvent event;	// Event to inject
		//event.type = TYPE_AXIS;
		sequenceTime += timer.dTime();
		if (!inSequence) {
			if (sequenceTime > 6) {	// magnitude of 9 & max 127
				inSequence = true;
			}
		}
		
		if (inSequence) {
			if (sequenceTime >= 6  &&
				sequenceTime <= 6.1 ) {
				event.id = BUTTON_X;
				event.type = TYPE_BUTTON;
				event.value = 0;
				dualshock->applyEvent(&event);
				event.id = BUTTON_L2;	// stop aiming
				dualshock->applyEvent(&event);
				event.id = AXIS_LY;
				event.type = TYPE_AXIS;
				event.value = JOYSTICK_MAX;
				dualshock->applyEvent(&event);
				event.id = AXIS_LX;
				event.value = 0;
				dualshock->applyEvent(&event);
				event.id = AXIS_L2;
				event.value = JOYSTICK_MIN;
				dualshock->applyEvent(&event);
			} else if (sequenceTime > 6.1 &&
					   sequenceTime <= 6.2 ) {
				event.id = BUTTON_X;
				event.type = TYPE_BUTTON;
				event.value = 1;
				dualshock->applyEvent(&event);
				//printf("Dive!\n");
			} else if (sequenceTime > 6.2) {
				event.id = BUTTON_X;
				event.type = TYPE_BUTTON;
				event.value = 0;
				dualshock->applyEvent(&event);
				inSequence = false;
				sequenceTime = 0;
				//printf("Release!\n");
			}
		}
	}
	
	void finish() {
		if (inSequence) {
			DeviceEvent event = {0,0,TYPE_BUTTON,BUTTON_X};
			dualshock->applyEvent(&event);
			event = {0,0,TYPE_BUTTON,BUTTON_L2};
			dualshock->applyEvent(&event);
			event = {0,0,TYPE_AXIS,AXIS_LY};
			dualshock->applyEvent(&event);
		}
		//		chaosEngine->fakePipelinedEvent( &event, me);
		//		event = {0,(short)axisToValue[AXIS_RY],TYPE_AXIS,AXIS_RY};
		//		chaosEngine->fakePipelinedEvent( &event, me);
	}
	
	bool tweak( DeviceEvent* event ) {
		
		if (inSequence) {
			if ( event->type == TYPE_BUTTON &&
				(event->id == BUTTON_X ||
				 event->id == BUTTON_L2) ){
				return false;
			}
			if ( event->type == TYPE_AXIS &&
				(event->id == AXIS_LY ||
				 event->id == AXIS_LX ||
				 event->id == AXIS_L2) ){
				return false;
			}
		}
		
		return true;
	}
};

class NoGuns : public Chaos::Modifier {
public:
	static void regist() { Chaos::Modifier::factory["No Gun Selection"] = [](){ return new NoGuns();}; };
	const char* description() { return "D-Pad Left/Right Disabled"; };
	
	void begin() {
		DeviceEvent event = {0,0,TYPE_AXIS, AXIS_DX};
		dualshock->applyEvent(&event);
		
		Chaos::Sequence sequence;
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

class NoShortGuns : public Chaos::Modifier {
public:
	static void regist() { Chaos::Modifier::factory["No Short Guns"] = [](){ return new NoShortGuns();}; };
	const char* description() { return "D-Pad Right Disabled"; };
	
	void begin() {
		DeviceEvent event = {0,0,TYPE_AXIS, AXIS_DX};
		dualshock->applyEvent(&event);
		
		Chaos::Sequence sequence;
		sequence.addAxisPress( AXIS_DY, 1);	// select a non-gun
		sequence.send(dualshock);
	}
	
	bool tweak( DeviceEvent* event ) {
		if (event->type == TYPE_AXIS && event->id == AXIS_DX && event->value > 0) {
			return false;
		}
		return true;
	}
};

class NoLongGuns : public Chaos::Modifier {
public:
	static void regist() { Chaos::Modifier::factory["No Long Guns"] = [](){ return new NoLongGuns();}; };
	const char* description() { return "D-Pad Left Disabled"; };
	
	void begin() {
		DeviceEvent event = {0,0,TYPE_AXIS, AXIS_DX};
		dualshock->applyEvent(&event);
		
		Chaos::Sequence sequence;
		sequence.addAxisPress( AXIS_DY, 1);	// select a non-gun
		sequence.send(dualshock);
	}
	
	bool tweak( DeviceEvent* event ) {
		if (event->type == TYPE_AXIS && event->id == AXIS_DX && event->value < 0) {
			return false;
		}
		return true;
	}
};

class NoThrows : public Chaos::Modifier {
public:
	static void regist() { Chaos::Modifier::factory["No Throwables"] = [](){return new NoThrows();}; };
	const char* description() { return "D-Pad Up/Down Disabled"; };
	
	void begin() {
		DeviceEvent event = {0,0,TYPE_AXIS, AXIS_DY};
		dualshock->applyEvent(&event);
		
		Chaos::Sequence sequence;
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

class MaxSensitivity : public Chaos::Modifier {
	// gabemusic
public:
	static void regist() { Chaos::Modifier::factory["Max Sensitivity"] = [](){return new MaxSensitivity();}; };
	const char* description() { return "Goodbye precision aiming.  Joystick postions multiplied by 5"; };
	
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

class MinSensitivity : public Chaos::Modifier {
	// prototoxin
public:
	static void regist() { Chaos::Modifier::factory["Min Sensitivity"] = [](){return new MinSensitivity();}; };
	const char* description() { return "It is like slomo, but not for the enemies.  Joystick positions divided by 2.5"; };
	
	bool tweak( DeviceEvent* event ) {
		if (event->type == TYPE_AXIS) {
			switch (event->id) {
				case AXIS_LX:
				case AXIS_LY:
				case AXIS_RX:
				case AXIS_RY:
					event->value = joystickLimit( ((double)event->value) / 2.5 );
					
				default:
					break;
			}
		}
		return true;
	}
};

class ControllerMirror : public Chaos::Modifier {
	// PrincessDiodes, cloverfieldmel, DJ_Squall_808
public:
	static void regist() { Chaos::Modifier::factory["Controller Mirror"] = [](){return new ControllerMirror();}; };
	const char* description() { return "Left Joystick swapped with Right Joystick, R2 is now L2. Circle is D-Pad Left, etc."; };
	
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

class ControllerFlip : public Chaos::Modifier {
	// Hipsterobot
public:
	static void regist() { Chaos::Modifier::factory["Controller Flip"] = [](){return new ControllerFlip();}; };
	const char* description() { return "Double joystick inverted, L1 swapped with L2, Triangle swapped with X, etc."; };
	
	bool tweak( DeviceEvent* event ) {
		DeviceEvent newEvent;
		if(event->type == TYPE_AXIS) {
			switch (event->id) {
				case AXIS_DY:
				case AXIS_LY:
				case AXIS_RY:
					event->value = joystickLimit(-event->value);
					break;
				case AXIS_L2:
				case AXIS_R2:
					return false;
				default:
					break;
			}
			return true;
		}
		if(event->type == TYPE_BUTTON) {
			switch (event->id) {
				case BUTTON_R1:
					event->id = BUTTON_R2;
					newEvent.id	= AXIS_R2;
					newEvent.type = TYPE_AXIS;
					newEvent.value = event->value ? JOYSTICK_MAX : JOYSTICK_MIN;
					chaosEngine->fakePipelinedEvent(&newEvent, me);
					break;
				case BUTTON_L1:
					event->id = BUTTON_L2;
					newEvent.id	= AXIS_L2;
					newEvent.type = TYPE_AXIS;
					newEvent.value = event->value ? JOYSTICK_MAX : JOYSTICK_MIN;
					chaosEngine->fakePipelinedEvent(&newEvent, me);
					break;
				case BUTTON_R2: event->id = BUTTON_R1; break;
				case BUTTON_L2: event->id = BUTTON_L1; break;
					
				case BUTTON_TRIANGLE:
					event->id = BUTTON_X;
					break;
				case BUTTON_X:
					event->id = BUTTON_TRIANGLE;
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
class RestartCheckpoint : public Chaos::Modifier {
public:
	static void regist() { Chaos::Modifier::factory["Restart Checkpoint"] = [](){return new RestartCheckpoint();}; };
	const char* description() { return "Best served prior to the end of a long encounter"; };
	
	bool busy;
	
	void begin() {
		busy = true;
		
		Chaos::Sequence sequence;
		sequence.disablejoysticks();
		sequence.addButtonPress( BUTTON_OPTIONS);
		sequence.addAxisPress( AXIS_DY, -1);
		sequence.addAxisPress( AXIS_DY, -1);
		sequence.addButtonPress( BUTTON_X);
		sequence.addAxisPress( AXIS_DX, -1);
		sequence.addButtonPress( BUTTON_X);
		sequence.addTimeDelay(3000);
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
class NoReticle : public Chaos::Modifier {
public:
	static void regist() { Chaos::Modifier::factory["No Reticle"] = [](){return new NoReticle();}; };
	const char* description() { return "Headshots just got trickier"; };
	
	bool busy;
	
	void begin() {
		busy = true;
		Menuing::getInstance()->selectHudMode(HUD_RETICLE, RETICLE_OFF, dualshock);
		busy = false;
	}
	
	void finish() {
		busy = true;
//		Menuing::getInstance()->selectHudMode(HUD_RETICLE, RETICLE_SIMPLE, dualshock);
		Menuing::getInstance()->selectHudMode(HUD_RETICLE, RETICLE_DEFAULT, dualshock);
		busy = false;
	}
	
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};

class NoInventoryHud : public Chaos::Modifier {
public:
	static void regist() { Chaos::Modifier::factory["No Inventory HUD"] = [](){return new NoInventoryHud();}; };
	const char* description() { return "Blind weapon and throwable selection"; };
	
	bool busy;
	
	void begin() {
		busy = true;
		Menuing::getInstance()->selectHudMode(HUD_WEAPON_CROSS, -1, dualshock);
		busy = false;
	}
	
	void finish() {
		busy = true;
		Menuing::getInstance()->selectHudMode(HUD_WEAPON_CROSS, 1, dualshock);
		busy = false;
	}
	
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};

class NoDamageIndicators : public Chaos::Modifier {
public:
	static void regist() { Chaos::Modifier::factory["No Damage Indicators"] = [](){return new NoDamageIndicators();}; };
	const char* description() { return "Locating the source of damage will be harder"; };
	
	bool busy;
	
	void begin() {
		busy = true;
		Menuing::getInstance()->selectHudMode(HUD_DAMAGE_INDICATORS, -1, dualshock);
		busy = false;
	}
	
	void finish() {
		busy = true;
		Menuing::getInstance()->selectHudMode(HUD_DAMAGE_INDICATORS, 1, dualshock);
		busy = false;
	}
	
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};

class NoAwarenessIndicators : public Chaos::Modifier {
public:
	static void regist() { Chaos::Modifier::factory["No Awareness Indicators"] = [](){return new NoAwarenessIndicators();}; };
	const char* description() { return "Stealth strats have no visual feedback"; };
	
	bool busy;
	
	void begin() {
		busy = true;
		Menuing::getInstance()->selectHudMode(HUD_AWARENESS_INDICATORS, -2, dualshock);
		busy = false;
	}
	
	void finish() {
		busy = true;
		Menuing::getInstance()->selectHudMode(HUD_AWARENESS_INDICATORS, 1, dualshock);
		busy = false;
	}
	
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};

class NoHitMarkers : public Chaos::Modifier {
public:
	static void regist() { Chaos::Modifier::factory["No Hit Markers"] = [](){return new NoHitMarkers();}; };
	const char* description() { return "No feedback on connected shots"; };
	
	bool busy;
	
	void begin() {
		busy = true;
		Menuing::getInstance()->selectHudMode(HUD_HIT_MARKERS, -2, dualshock);
		busy = false;
	}
	
	void finish() {
		busy = true;
		Menuing::getInstance()->selectHudMode(HUD_HIT_MARKERS, 2, dualshock);
		busy = false;
	}
	
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};

class NoArcs : public Chaos::Modifier {
public:
	static void regist() { Chaos::Modifier::factory["No Arc Throw Paths"] = [](){return new NoArcs();}; };
	const char* description() { return "The trajectory of your throwable will now be based on feel"; };
	
	bool busy;
	
	void begin() {
		busy = true;
		Menuing::getInstance()->selectHudMode(HUD_ARC_THROW_PATH, -1, dualshock);
		busy = false;
	}
	
	void finish() {
		busy = true;
		Menuing::getInstance()->selectHudMode(HUD_ARC_THROW_PATH, 1, dualshock);
		busy = false;
	}
	
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};

class NoHealthHud : public Chaos::Modifier {
public:
	static void regist() { Chaos::Modifier::factory["No Health HUD"] = [](){return new NoHealthHud();}; };
	const char* description() { return "The health and weapon HUD int he lower right is disabled"; };
	
	bool busy;
	
	void begin() {
		busy = true;
		Menuing::getInstance()->selectHudMode(HUD_HEALTH_AND_WEAPON, -1, dualshock);
		busy = false;
	}
	
	void finish() {
		busy = true;
		Menuing::getInstance()->selectHudMode(HUD_HEALTH_AND_WEAPON, 1, dualshock);
		busy = false;
	}
	
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};

class PickupNotifications : public Chaos::Modifier {
public:
	static void regist() { Chaos::Modifier::factory["Show Pickups"] = [](){return new PickupNotifications();}; };
	const char* description() { return "Enables the HUD that shows notifications on ammo, crafting, and other pickups"; };
	
	bool busy;
	
	void begin() {
		busy = true;
		Menuing::getInstance()->selectHudMode(HUD_PICK_UP_NOTIFICATIONS, 1, dualshock);
		busy = false;
	}
	
	void finish() {
		busy = true;
		Menuing::getInstance()->selectHudMode(HUD_PICK_UP_NOTIFICATIONS, -1, dualshock);
		busy = false;
	}
	
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};

class LargeSubtitles : public Chaos::Modifier {
public:
	static void regist() { Chaos::Modifier::factory["Large Subtitles"] = [](){return new LargeSubtitles();}; };
	const char* description() { return "The ability to read subtitles even at 160p"; };
	
	bool busy;
	
	void begin() {
		busy = true;
		Menuing::getInstance()->selectSubtitleMode(SUBTITLES_SIZE, 2, dualshock);
		busy = false;
	}
	
	void finish() {
		busy = true;
		Menuing::getInstance()->selectSubtitleMode(SUBTITLES_SIZE, -1, dualshock);
		busy = false;
	}
	
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};


/*
 Render Modes:
 */
class Graphic : public Chaos::Modifier {
	bool busy;
public:
	static void regist() { Chaos::Modifier::factory["Graphic"] = [](){return new Graphic();}; };
	const char* description() { return "Render Mode"; };
	
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
class Headache : public Chaos::Modifier {
	bool busy;
public:
	static void regist() { Chaos::Modifier::factory["Headache"] = [](){return new Headache();}; };
	const char* description() { return "Render Mode"; };
	
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

class Afterlife : public Chaos::Modifier {
	bool busy;
public:
	static void regist() { Chaos::Modifier::factory["Afterlife"] = [](){return new Afterlife();}; };
	const char* description() { return "Render Mode"; };
	
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

class EightBit : public Chaos::Modifier {
	bool busy;
public:
	static void regist() { Chaos::Modifier::factory["8-Bit"] = [](){return new EightBit();}; };
	const char* description() { return "Render Mode"; };
	
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

class WaterColor : public Chaos::Modifier {
	bool busy;
public:
	static void regist() { Chaos::Modifier::factory["Water Color"] = [](){return new WaterColor();}; };
	const char* description() { return "Render Mode"; };
	
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

class Dungeon : public Chaos::Modifier {
	bool busy;
public:
	static void regist() { Chaos::Modifier::factory["Dungeon"] = [](){return new Dungeon();}; };
	const char* description() { return "Render Mode"; };
	
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

class Void : public Chaos::Modifier {
	bool busy;
public:
	static void regist() { Chaos::Modifier::factory["Void"] = [](){return new Void();}; };
	const char* description() { return "Render Mode"; };
	
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

class PopPoster : public Chaos::Modifier {
	bool busy;
public:
	static void regist() { Chaos::Modifier::factory["Pop Poster"] = [](){return new PopPoster();}; };
	const char* description() { return "Stream Destroyer.  This kills the bitrate"; };
	
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

class NineteenSixty : public Chaos::Modifier {
	bool busy;
public:
	static void regist() { Chaos::Modifier::factory["1960"] = [](){return new NineteenSixty();}; };
	const char* description() { return "Render Mode"; };
	
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

class Cool : public Chaos::Modifier {
	bool busy;
public:
	static void regist() { Chaos::Modifier::factory["Cool"] = [](){return new Cool();}; };
	const char* description() { return "Render Mode"; };
	
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

class Warm : public Chaos::Modifier {
	bool busy;
public:
	static void regist() { Chaos::Modifier::factory["Warm"] = [](){return new Warm();}; };
	const char* description() { return "Render Mode"; };
	
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

class Vibrant : public Chaos::Modifier {
	bool busy;
public:
	static void regist() { Chaos::Modifier::factory["Vibrant"] = [](){return new Vibrant();}; };
	const char* description() { return "Render Mode"; };
	
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

class Noir : public Chaos::Modifier {
	bool busy;
public:
	static void regist() { Chaos::Modifier::factory["Noir"] = [](){return new Noir();}; };
	const char* description() { return "Render Mode"; };
	
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

class Blorange : public Chaos::Modifier {
	bool busy;
public:
	static void regist() { Chaos::Modifier::factory["Blorange"] = [](){return new Blorange();}; };
	const char* description() { return "Render Mode"; };
	
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

class Sepia : public Chaos::Modifier {
	bool busy;
public:
	static void regist() { Chaos::Modifier::factory["Sepia"] = [](){return new Sepia();}; };
	const char* description() { return "Render Mode"; };
	
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

class Vintage : public Chaos::Modifier {
	bool busy;
public:
	static void regist() { Chaos::Modifier::factory["Vintage"] = [](){return new Vintage();}; };
	const char* description() { return "Render Mode"; };
	
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

class Beasts : public Chaos::Modifier {
	bool busy;
public:
	static void regist() { Chaos::Modifier::factory["Beasts"] = [](){return new Beasts();}; };
	const char* description() { return "Render Mode"; };
	
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

class Terminated : public Chaos::Modifier {
	bool busy;
public:
	static void regist() { Chaos::Modifier::factory["Terminated"] = [](){return new Terminated();}; };
	const char* description() { return "Render Mode"; };
	
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
class GridLocked : public Chaos::Modifier {
	bool busy;
public:
	static void regist() { Chaos::Modifier::factory["Gridlocked"] = [](){return new GridLocked();}; };
	const char* description() { return "Render Mode"; };
	
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

class Blacklight : public Chaos::Modifier {
	bool busy;
public:
	static void regist() { Chaos::Modifier::factory["Blacklight"] = [](){return new Blacklight();}; };
	const char* description() { return "Render Mode"; };
	
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

class DesertFog : public Chaos::Modifier {
	bool busy;
public:
	static void regist() { Chaos::Modifier::factory["Desert Fog"] = [](){return new DesertFog();}; };
	const char* description() { return "Render Mode"; };
	
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

class Blood : public Chaos::Modifier {
	bool busy;
public:
	static void regist() { Chaos::Modifier::factory["Blood"] = [](){return new Blood();}; };
	const char* description() { return "Render Mode"; };
	
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

class Inferno : public Chaos::Modifier {	// Working
	bool busy;
public:
	static void regist() { Chaos::Modifier::factory["Inferno"] = [](){return new Inferno();}; };
	const char* description() { return "Render Mode"; };
	
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

class Fire : public Chaos::Modifier {
	bool busy;
public:
	static void regist() { Chaos::Modifier::factory["Fire"] = [](){return new Fire();}; };
	const char* description() { return "Render Mode"; };
	
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

class Trinity : public Chaos::Modifier {
	bool busy;
public:
	static void regist() { Chaos::Modifier::factory["Trinity"] = [](){return new Trinity();}; };
	const char* description() { return "Render Mode"; };
	
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

class Pusher : public Chaos::Modifier {
	bool busy;
public:
	static void regist() { Chaos::Modifier::factory["Pusher"] = [](){return new Pusher();}; };
	const char* description() { return "Render Mode"; };
	
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

class Demons : public Chaos::Modifier {
	bool busy;
public:
	static void regist() { Chaos::Modifier::factory["Demons"] = [](){return new Demons();}; };
	const char* description() { return "Render Mode"; };
	
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

class DrugZone : public Chaos::Modifier {
	bool busy;
public:
	static void regist() { Chaos::Modifier::factory["Drug Zone"] = [](){return new DrugZone();}; };
	const char* description() { return "Render Mode"; };
	
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
class Moonlight : public Chaos::Modifier {
	bool busy;
public:
	static void regist() { Chaos::Modifier::factory["Moonlight"] = [](){return new Moonlight();}; };
	const char* description() { return "Render Mode"; };
	
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
class Mirror : public Chaos::Modifier {
	bool busy;
public:
	static void regist() { Chaos::Modifier::factory["Mirror World"] = [](){return new Mirror();}; };
	const char* description() { return "Wait, where do we go?"; };
	
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

class MirrorOnDeath : public Chaos::Modifier {
	bool busy;
public:
	static void regist() { Chaos::Modifier::factory["Mirror On Death"] = [](){return new MirrorOnDeath();}; };
	const char* description() { return "Mirror World on Death.  Can result in permanent Mirror World."; };
	
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


class SlowMotion : public Chaos::Modifier {
	bool busy;
public:
	static void regist() { Chaos::Modifier::factory["Slow Motion"] = [](){return new SlowMotion();}; };
	const char* description() { return "The speed run killer"; };
	
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

class BulletSpeed : public Chaos::Modifier {
	bool busy;
public:
	static void regist() { Chaos::Modifier::factory["Bullet Speed"] = [](){return new BulletSpeed();}; };
	const char* description() { return "Slow motion when aiming"; };
	
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

class InfiniteAmmo : public Chaos::Modifier {
	bool busy;
public:
	static void regist() { Chaos::Modifier::factory["Infinite Ammo"] = [](){return new InfiniteAmmo();}; };
	const char* description() { return "The strimmer responds with <3"; };
	
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

class InfiniteCrafting : public Chaos::Modifier {
	bool busy;
public:
	static void regist() { Chaos::Modifier::factory["Infinite Crafting"] = [](){return new InfiniteCrafting();}; };
	const char* description() { return "Molotovs for everyone!"; };
	
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

class InfiniteMeleeDurability : public Chaos::Modifier {
	bool busy;
public:
	static void regist() { Chaos::Modifier::factory["Infinite Melee Durability"] = [](){return new InfiniteMeleeDurability();}; };
	const char* description() { return "Finally realistic durability"; };
	
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

class InfiniteListenRange : public Chaos::Modifier {	// working
	bool busy;
public:
	static void regist() { Chaos::Modifier::factory["Infinite Listen Range"] = [](){return new InfiniteListenRange();}; };
	const char* description() { return "There used to be a limit?"; };
	
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

class OneShot : public Chaos::Modifier {
	bool busy;
public:
	static void regist() { Chaos::Modifier::factory["One Shot"] = [](){return new OneShot();}; };
	const char* description() { return "Golden guns activated"; };
	
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

class TouchOfDeath : public Chaos::Modifier {
	bool busy;
public:
	static void regist() { Chaos::Modifier::factory["Touch Of Death"] = [](){return new TouchOfDeath();}; };
	const char* description() { return "Golden fists activated.  One hit enemy deaths"; };
	
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

class EightBitAudio : public Chaos::Modifier {
	bool busy;
public:
	static void regist() { Chaos::Modifier::factory["8-Bit Audio"] = [](){return new EightBitAudio();}; };
	const char* description() { return "Audio mode.  Incompatible with 4-bit"; };
	
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

class FourBitAudio : public Chaos::Modifier {
	bool busy;
public:
	static void regist() { Chaos::Modifier::factory["4-Bit Audio"] = [](){return new FourBitAudio();}; };
	const char* description() { return "Audio mode.  Incompatible with 8-bit"; };
	
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

class HeliumAudio : public Chaos::Modifier {
	bool busy;
public:
	static void regist() { Chaos::Modifier::factory["Helium Audio"] = [](){return new HeliumAudio();}; };
	const char* description() { return "Audio mode.  Incompatible with Xenon"; };
	
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

class XenonAudio : public Chaos::Modifier {	// Working!
	bool busy;
public:
	static void regist() { Chaos::Modifier::factory["Xenon Audio"] = [](){return new XenonAudio();}; };
	const char* description() { return "Audio mode.  Incompatible with Helium"; };
	
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

class DeskPop : public Chaos::Modifier {
public:
	static void regist() { Chaos::Modifier::factory["Desk Pop"] = [](){return new DeskPop();}; };
	const char* description() { return "We honor the flag, and you crap on it when you don't shoot your gun https://www.youtube.com/watch?v=wWZTTtE5_zQ"; };
	
	bool busy;
	
	void begin() {
		busy = true;
		
		Chaos::Sequence sequence;
		
		sequence.addAxisPress(AXIS_DX, 1);
		sequence.addTimeDelay(1000);
		sequence.addButtonHold(BUTTON_L2);
		sequence.addTimeDelay(1000);
		sequence.addButtonHold(BUTTON_R2);
		sequence.addAxisHold(AXIS_R2, JOYSTICK_MAX);
		sequence.addTimeDelay(200);
		sequence.addButtonRelease(BUTTON_L2);
		sequence.addButtonRelease(BUTTON_R2);
		sequence.addAxisHold(AXIS_R2, JOYSTICK_MIN);
		
		sequence.send(dualshock);
		
		busy = false;
	}
	
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};

class DoubleTap : public Chaos::Modifier {
public:
	static void regist() { Chaos::Modifier::factory["Double Tap"] = [](){return new DoubleTap();}; };
	const char* description() { return "It's Rule #2. Everytime a shot is fired, another occurs in quick succession.  For Rule #1, see Leeroy Jenkins"; };
	
	//bool busy;
	//bool priorR2;
	bool shotFired;
	double timeBetweenShots;
	double timeTracker;
	
	enum DoubleTapStates {
		DOUBLE_TAP_IDLE,
		DOUBLE_TAP_FIRST_TAP,
		DOUBLE_TAP_FIRST_RELEASE,
		DOUBLE_TAP_SECOND_TAP,
		DOUBLE_TAP_FINAL_RELEASE
	} state;
	
	
	void begin() {
		timeBetweenShots = 1.0;
		timeTracker = 0;
		//busy = false;
		shotFired = false;
		state = DOUBLE_TAP_IDLE;
		//priorR2 = dualshock->getState(BUTTON_R2, TYPE_BUTTON);
	}
	
	void update() {
		if (state == DOUBLE_TAP_IDLE) {
			return;
		}
		timeTracker += timer.dTime();
		
		if (timeTracker > 0.2 && state == DOUBLE_TAP_FIRST_TAP) { // invoke a release
			DeviceEvent fakeEvent = {0, 0, TYPE_BUTTON, BUTTON_R2};
			dualshock->applyEvent(&fakeEvent);
			fakeEvent = {0, JOYSTICK_MIN, TYPE_AXIS, AXIS_R2};
			dualshock->applyEvent(&fakeEvent);
			state = DOUBLE_TAP_FIRST_RELEASE;
		} else if (timeTracker > timeBetweenShots && state == DOUBLE_TAP_FIRST_RELEASE) {
			DeviceEvent fakeEvent = {0, 1, TYPE_BUTTON, BUTTON_R2};
			dualshock->applyEvent(&fakeEvent);
			fakeEvent = {0, JOYSTICK_MAX, TYPE_AXIS, AXIS_R2};
			dualshock->applyEvent(&fakeEvent);
			state = DOUBLE_TAP_SECOND_TAP;
		} else if (timeTracker > (timeBetweenShots+0.2) && state == DOUBLE_TAP_SECOND_TAP) {
			DeviceEvent fakeEvent = {0, 0, TYPE_BUTTON, BUTTON_R2};
			dualshock->applyEvent(&fakeEvent);
			fakeEvent = {0, JOYSTICK_MIN, TYPE_AXIS, AXIS_R2};
			dualshock->applyEvent(&fakeEvent);
			state = DOUBLE_TAP_FINAL_RELEASE;
		}
		
		// end of sequence
		if (state == DOUBLE_TAP_FINAL_RELEASE) {
			//shotFired = false;
			timeTracker = 0.0;
			state = DOUBLE_TAP_IDLE;
		}
	}
	
	bool tweak( DeviceEvent* event ) {
		
		if (event->type == TYPE_BUTTON &&
			event->id == BUTTON_R2 &&
			event->value == 1 &&
			state == DOUBLE_TAP_IDLE) {
			state = DOUBLE_TAP_FIRST_TAP;
			DeviceEvent fakeEvent = {0, JOYSTICK_MAX, TYPE_AXIS, AXIS_R2};
			dualshock->applyEvent(&fakeEvent);
			return true;
		}
		
		if ((event->type == TYPE_BUTTON && (event->id == BUTTON_L2 || event->id == BUTTON_R2)) ||
			(event->type == TYPE_AXIS && (event->id == AXIS_L2 || event->id == AXIS_R2)) ) {
			return state == DOUBLE_TAP_IDLE;
		}
		
		return true;
	}
};

class EmptyGun : public Chaos::Modifier {
public:
	static void regist() { Chaos::Modifier::factory["Use Items"] = [](){return new EmptyGun();}; };
	const char* description() { return "Shoots or throws 6 of whatever item is currently equipped (will not work on medkits)"; };
	
	bool busy;
	
	void begin() {
		busy = true;
		
		Chaos::Sequence sequence;
		
		//sequence.addAxisPress(AXIS_DX, 1);
		//sequence.addTimeDelay(1000);
		sequence.addButtonHold(BUTTON_L2);
		sequence.addTimeDelay(1000);
		for (int i = 0; i < 6; i++) {
//			sequence.addAxisPress(AXIS_R2, JOYSTICK_MAX);
			sequence.addAxisHold(AXIS_R2, JOYSTICK_MAX);
			//sequence.addButtonPress(BUTTON_R2);
			sequence.addButtonHold(BUTTON_R2);
			sequence.addTimeDelay(250);
			sequence.addAxisHold(AXIS_R2, JOYSTICK_MIN);
			sequence.addButtonRelease(BUTTON_R2);
			sequence.addTimeDelay(250);
		}
//		sequence.addButtonHold(BUTTON_R2);
		sequence.addButtonRelease(BUTTON_L2);
//		sequence.addButtonRelease(BUTTON_R2);
//		sequence.addAxisHold(AXIS_R2, JOYSTICK_MIN);
		
		sequence.send(dualshock);
		
		busy = false;
	}
	
	bool tweak( DeviceEvent* event ) {
		if (event->type == TYPE_BUTTON) {
			if (event->id == BUTTON_L2 ||
				event->id == BUTTON_R2) {
				return !busy;
			}
		} else if (event->type == TYPE_AXIS) {
			if (event->id == AXIS_R2 ||
				event->id == AXIS_DX ||
				event->id == AXIS_DY) {
				return !busy;
			}
		}
		
		return true;
	}
};

class TossMolly : public Chaos::Modifier {
public:
	static void regist() { Chaos::Modifier::factory["Toss A Molly/Bomb"] = [](){return new TossMolly();}; };
	const char* description() { return "Time to riot, chucks a molly (bomb if Abby)"; };
	
	bool busy;
	
	void begin() {
		busy = true;
		
		Chaos::Sequence sequence;
		
		sequence.disablejoysticks();
		sequence.addAxisPress(AXIS_DY, 1);
		sequence.addAxisPress(AXIS_DY, 1);
		//		sequence.addTimeDelay(1000);
		//		sequence.addButtonHold(BUTTON_L2);
		sequence.addTimeDelay(2500);
		sequence.addButtonHold(BUTTON_R2);
		sequence.addAxisHold(AXIS_R2, JOYSTICK_MAX);
		sequence.addTimeDelay(500);
		//		sequence.addButtonRelease(BUTTON_L2);
		sequence.addButtonRelease(BUTTON_R2);
		sequence.addAxisHold(AXIS_R2, JOYSTICK_MIN);
		
		sequence.send(dualshock);
		
		busy = false;
	}
	
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};

class CtgStrat : public Chaos::Modifier {
public:
	static void regist() { Chaos::Modifier::factory["CTG Strat"] = [](){return new CtgStrat();}; };
	const char* description() { return "Self immolation resulting in death, at least 7/11 times"; };
	
	bool busy;
	
	void begin() {
		busy = true;
		
		Chaos::Sequence sequence;
		
		sequence.disablejoysticks();
		sequence.addAxisHold(AXIS_RY, JOYSTICK_MAX);
		sequence.addAxisHold(AXIS_RX, 0);
		sequence.addAxisPress(AXIS_DY, 1);
		sequence.addAxisPress(AXIS_DY, 1);
		//		sequence.addTimeDelay(1000);
		//		sequence.addButtonHold(BUTTON_L2);
		sequence.addTimeDelay(3000);
		sequence.addButtonHold(BUTTON_R2);
		sequence.addAxisHold(AXIS_R2, JOYSTICK_MAX);
		sequence.addTimeDelay(500);
		//		sequence.addButtonRelease(BUTTON_L2);
		sequence.addButtonRelease(BUTTON_R2);
		sequence.addAxisHold(AXIS_R2, JOYSTICK_MIN);
		
		sequence.addAxisHold(AXIS_RY, 0);
		sequence.addAxisHold(AXIS_RX, 0);
		
		sequence.send(dualshock);
		
		busy = false;
	}
	
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};

class PdubIt : public Chaos::Modifier {
public:
	static void regist() { Chaos::Modifier::factory["Pdub It"] = [](){return new PdubIt();}; };
	const char* description() { return "RIP bricks and bottles"; };
	
	bool busy;
	
	void begin() {
		busy = true;
		
		Chaos::Sequence sequence;
		
		sequence.disablejoysticks();
		sequence.addAxisPress(AXIS_DY, 1);
		sequence.addTimeDelay(2500);
		//		sequence.addButtonHold(BUTTON_L2);
		//		sequence.addTimeDelay(1000);
		sequence.addButtonHold(BUTTON_R2);
		sequence.addAxisHold(AXIS_R2, JOYSTICK_MAX);
		sequence.addTimeDelay(200);
		//		sequence.addButtonRelease(BUTTON_L2);
		sequence.addButtonRelease(BUTTON_R2);
		sequence.addAxisHold(AXIS_R2, JOYSTICK_MIN);
		
		sequence.send(dualshock);
		
		busy = false;
	}
	
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};

class ChuckStun : public Chaos::Modifier {
public:
	static void regist() { Chaos::Modifier::factory["Chuck Smoke Bomb"] = [](){return new ChuckStun();}; };
	const char* description() { return "It was taking up space anyway"; };
	
	bool busy;
	
	void begin() {
		busy = true;
		
		Chaos::Sequence sequence;
		
		sequence.disablejoysticks();
		sequence.addAxisPress(AXIS_DY, -1);
		sequence.addAxisPress(AXIS_DY, -1);
		sequence.addTimeDelay(2000);
		//		sequence.addButtonHold(BUTTON_L2);
		//		sequence.addTimeDelay(1000);
		sequence.addButtonHold(BUTTON_R2);
		sequence.addAxisHold(AXIS_R2, JOYSTICK_MAX);
		sequence.addTimeDelay(200);
		//		sequence.addButtonRelease(BUTTON_L2);
		sequence.addButtonRelease(BUTTON_R2);
		sequence.addAxisHold(AXIS_R2, JOYSTICK_MIN);
		
		sequence.send(dualshock);
		
		busy = false;
	}
	
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};


class HostagesDontEscape : public Chaos::Modifier {
	bool busy;
public:
	static void regist() { Chaos::Modifier::factory["Infinite Hugging"] = [](){return new HostagesDontEscape();}; };
	const char* description() { return "Hugging enemies from behind forever <3 Enables option Hostages Do Not Escape"; };
	
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

class AlliesDontGetGrabbed : public Chaos::Modifier {
	bool busy;
public:
	static void regist() { Chaos::Modifier::factory["No Plus Signs"] = [](){return new AlliesDontGetGrabbed();}; };
	const char* description() { return "Speedrunners need this.  Allies do not get grabbed."; };
	
	void begin() {
		busy = true;
		Menuing::getInstance()->setCombatAccessibility(COMBAT_ALLIES_DONT_GET_GRABBED, 1, dualshock);
		busy = false;
	}
	void finish() {
		busy = true;
		Menuing::getInstance()->setCombatAccessibility(COMBAT_ALLIES_DONT_GET_GRABBED, 0, dualshock);
		busy = false;
	}
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};

class EnemiesDontFlank : public Chaos::Modifier {
	bool busy;
public:
	static void regist() { Chaos::Modifier::factory["Enemies Do Not Flank"] = [](){return new EnemiesDontFlank();}; };
	const char* description() { return "Very effective on infected Kappa"; };
	
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

class ReducedEnemyPerception : public Chaos::Modifier {
	bool busy;
public:
	static void regist() { Chaos::Modifier::factory["Reduced Enemy Perception"] = [](){return new ReducedEnemyPerception();}; };
	const char* description() { return "Finally clickers are truly blind"; };
	
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

class ReducedEnemyAccuracy : public Chaos::Modifier {
	bool busy;
public:
	static void regist() { Chaos::Modifier::factory["Reduced Enemy Accuracy"] = [](){return new ReducedEnemyAccuracy();}; };
	const char* description() { return "Does not work on melee"; };
	
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

class EnhancedDodge : public Chaos::Modifier {
	bool busy;
public:
	static void regist() { Chaos::Modifier::factory["Enhanced Dodge"] = [](){return new EnhancedDodge();}; };
	const char* description() { return "I wish I knew what this does (built-in)"; };
	
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

class InvisibleWhileProne : public Chaos::Modifier {
	bool busy;
public:
	static void regist() { Chaos::Modifier::factory["Invisible While Prone"] = [](){return new InvisibleWhileProne();}; };
	const char* description() { return "Proning is now even more OP"; };
	
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

class SetThirtyFps : public Chaos::Modifier {
	// crescenterra
	bool busy;
public:
	static void regist() { Chaos::Modifier::factory["30 fps"] = [](){return new SetThirtyFps();}; };
	const char* description() { return "Converting a PS5 to play like PS4"; };
	
	void begin() {
		busy = true;
		Menuing::getInstance()->setDisplayMode(DISPLAY_FRAMERATE, 0, dualshock);
		busy = false;
	}
	void finish() {
		busy = true;
		Menuing::getInstance()->setDisplayMode(DISPLAY_FRAMERATE, 1, dualshock);
		busy = false;
	}
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};

class CameraAssist : public Chaos::Modifier {
	bool busy;
public:
	static void regist() { Chaos::Modifier::factory["Camera Assist"] = [](){return new CameraAssist();}; };
	const char* description() { return "Automatically reorients the camera in the direction of movement."; };
	
	void begin() {
		busy = true;
		Menuing::getInstance()->setAlternateControls(ALTERNATE_CAMERA_ASSIST, 1, dualshock);
		busy = false;
	}
	void finish() {
		busy = true;
		Menuing::getInstance()->setAlternateControls(ALTERNATE_CAMERA_ASSIST, 0, dualshock);
		busy = false;
	}
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};

class LockOnAim : public Chaos::Modifier {
	bool busy;
public:
	static void regist() { Chaos::Modifier::factory["Aimbot"] = [](){return new LockOnAim();}; };
	const char* description() { return "Aimbot... but like a detuned aimbot.  Turns on Lock On Aim"; };
	
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

class ThrowableLockOnAim : public Chaos::Modifier {
	bool busy;
public:
	static void regist() { Chaos::Modifier::factory["Aimbot Throwables"] = [](){return new ThrowableLockOnAim();}; };
	const char* description() { return "Aimbot for throwables. Turns on Lock On Throwables"; };
	
	void begin() {
		busy = true;
		Menuing::getInstance()->setAlternateControls(ALTERNATE_ARC_THROW_LOCK_ON, 1, dualshock);
		busy = false;
	}
	void finish() {
		busy = true;
		Menuing::getInstance()->setAlternateControls(ALTERNATE_ARC_THROW_LOCK_ON, 0, dualshock);
		busy = false;
	}
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};

class AutoPickUp : public Chaos::Modifier {
	bool busy;
public:
	static void regist() { Chaos::Modifier::factory["Kleptomania"] = [](){return new AutoPickUp();}; };
	const char* description() { return "Picking up items at inconvenient times. Turns on Auto Pick-Up"; };
	
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

class LargeHud : public Chaos::Modifier {
	bool busy;
public:
	static void regist() { Chaos::Modifier::factory["Large HUD"] = [](){return new LargeHud();}; };
	const char* description() { return "HUD like in a game from the 1990s"; };
	
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

class HighContrastDisplay : public Chaos::Modifier {
	bool busy;
public:
	static void regist() { Chaos::Modifier::factory["High Contrast Display"] = [](){return new HighContrastDisplay();}; };
	const char* description() { return "Render mode, but overrides any other render mode"; };
	
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

class TraversalAudioCues : public Chaos::Modifier {
	bool busy;
public:
	static void regist() { Chaos::Modifier::factory["Traversal Audio Cues"] = [](){return new TraversalAudioCues();}; };
	const char* description() { return "Add some music to climbable objects"; };
	
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

class CombatAudioCues : public Chaos::Modifier {
	bool busy;
public:
	static void regist() { Chaos::Modifier::factory["Combat Audio Cues"] = [](){return new CombatAudioCues();}; };
	const char* description() { return "Add some music to combat prompts"; };
	
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

class NavigationAssistance : public Chaos::Modifier {
	bool busy;
	double pressTime;
public:
	static void regist() { Chaos::Modifier::factory["Navigation Assistance"] = [](){return new NavigationAssistance();}; };
	const char* description() { return "Keeps pointing camera in direction of story progression"; };
	
	void begin() {
		busy = true;
		Menuing::getInstance()->setNavigationAndTraversal(NAVIGATION_ASSISTANCE, 1, dualshock);
		busy = false;
		DeviceEvent event = {0,0,TYPE_BUTTON, BUTTON_L3};
		dualshock->applyEvent(&event);
		pressTime = 0;
	}
	void finish() {
		busy = true;
		Menuing::getInstance()->setNavigationAndTraversal(NAVIGATION_ASSISTANCE, 0, dualshock);
		busy = false;
		DeviceEvent event = {0,0,TYPE_BUTTON, BUTTON_L3};
		dualshock->applyEvent(&event);
	}

	void update() {
		pressTime += timer.dTime();
		if ( pressTime > 0.1  && dualshock->getState(BUTTON_L3, TYPE_BUTTON) ) {
			DeviceEvent event = {0,0,TYPE_BUTTON, BUTTON_L3};
			dualshock->applyEvent(&event);
			pressTime = 0;
		} else if( pressTime > 5.0 && !dualshock->getState(BUTTON_L3, TYPE_BUTTON) ) {
			DeviceEvent event = {0,1,TYPE_BUTTON, BUTTON_L3};
			dualshock->applyEvent(&event);
			pressTime = 0;
		}
	}
	
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};

class TraversalAssistance : public Chaos::Modifier {
	bool busy;
public:
	static void regist() { Chaos::Modifier::factory["Traversal Assistance"] = [](){return new TraversalAssistance();}; };
	const char* description() { return "Climb when you really do not want to"; };
	
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

class EnhancedListenMode : public Chaos::Modifier {
	bool busy;
public:
	static void regist() { Chaos::Modifier::factory["Enhanced Listen Mode"] = [](){return new EnhancedListenMode();}; };
	const char* description() { return "Press some buttons while in listen mode to find loot"; };
	
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

class InfiniteBreath : public Chaos::Modifier {
	bool busy;
public:
	static void regist() { Chaos::Modifier::factory["Infinite Breath"] = [](){return new InfiniteBreath();}; };
	const char* description() { return "Yup, very useful and is the most OP perk..."; };
	
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


//class Mystery : public Chaos::Modifier {
//	// Prototoxin
//	bool dummy;
//public:
//	static void regist() {
//		Chaos::Modifier::factory["Mystery"] = []() {
//			Mogi::Math::Random rng;
//			int selection = rng.uniform(0, Chaos::Modifier::factory.size()-1);
//			printf("Selection = %d out of %d\n", selection, Chaos::Modifier::factory.size());
//			int count = 0;
//			std::map<std::string, std::function<Chaos::Modifier*()>>:: iterator it;
//			for (it = Chaos::Modifier::factory.begin();
//				 it != Chaos::Modifier::factory.end();
//				 it++) {
//				count++;
//				if (count >= selection) {
//					printf("Mystery: %s\n", it->first.c_str());
//					if (it->first.compare("Mystery") == 0) {
//						continue;
//					}
//					return it->second();
//				}
//			}
//			return (--it)->second();
//			//return new InfiniteBreath();
//			//return new Mystery();	// will do nothing
//		};
//
//	};
//	const char* description() { return "Applies any random modifier"; };
//};

class Mystery : public Chaos::Modifier {
	// Prototoxin
	Chaos::Modifier* trueMod = NULL;
public:
	static void regist() { Chaos::Modifier::factory["Mystery"] = [](){return new Mystery();}; };
	const char* description() { return "Applies any random modifier"; };
	
	Chaos::Modifier* getRandomModifier() {
//		Chaos::Modifier::factory["Mystery"] = []() {
			Mogi::Math::Random rng;
			int selection = rng.uniform(0, Chaos::Modifier::factory.size()-1);
			printf("Selection = %d out of %d\n", selection, Chaos::Modifier::factory.size());
			int count = 0;
			std::map<std::string, std::function<Chaos::Modifier*()>>:: iterator it;
			for (it = Chaos::Modifier::factory.begin();
				 it != Chaos::Modifier::factory.end();
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
//		};
	}
	
	void begin() {
		trueMod = getRandomModifier();
		printf("- Setting parent of sub-mod\n");
		trueMod->setParentModifier(this);
		printf("- Setting chaosEngine and Dualshock of sub-mod\n");
		trueMod->setDualshock(dualshock);
		trueMod->setChaosEngine(chaosEngine);
		printf("- begin() on sub-mod\n");
		trueMod->begin();
		printf("Done.\n");
	}
	void update() {
		if (trueMod != NULL) {
			trueMod->_update(chaosEngine->isPaused());	// Not very elegant, timer needs to be updated
			//trueMod->update();
		}
	}
	void finish() {
		if (trueMod != NULL) {
			trueMod->finish();
			delete trueMod;
		}
	}
	bool tweak( DeviceEvent* event ) {
		if (trueMod != NULL) {
			return trueMod->tweak(event);
		}
		return true;
	}
};

class ChaosMod : public Chaos::Modifier {
	// JustSaft
//	Chaos::Modifier* trueMod;
	std::vector<Chaos::Modifier*> trueModifiers;
	std::vector<std::string> modNames;
public:
	static void regist() { Chaos::Modifier::factory["Chaos"] = [](){return new ChaosMod();}; };
	const char* description() { return "Applies 5 random modifiers"; };
	
	void buildModifiers() {
//		Chaos::Modifier::factory["Mystery"] = []() {
		Mogi::Math::Random rng;
		while(trueModifiers.size() < 5) {
			
			int selection = rng.uniform(0, Chaos::Modifier::factory.size()-1);
			printf("Selection = %d out of %d\n", selection, Chaos::Modifier::factory.size());
			int count = 0;
			std::map<std::string, std::function<Chaos::Modifier*()>>:: iterator it;
			for (it = Chaos::Modifier::factory.begin();
				 it != Chaos::Modifier::factory.end();
				 it++) {
				count++;
				if (count >= selection) {
					if ((it->first.compare("Mystery") == 0) ||
						(it->first.compare("Chaos") == 0)) {
						continue;
					}
					bool copycat = false;
					for (std::vector<std::string>::iterator it2 = modNames.begin();
						 it2 != modNames.end();
						 it2++) {
						if (it->first.compare(*it2) == 0) {
							copycat = true;
							break;
						}
					}
					if (copycat) {
						continue;
					}
					printf("Chaos: %s\n", it->first.c_str());
					modNames.push_back(it->first);
//
//					for (std::vector<Chaos::Modifier*>::iterator it = trueModifiers.begin(); it != trueModifiers.end(); it++) {
//						if (it->second) {
//							<#statements#>
//						}
//					}
//
//					return it->second();
					trueModifiers.push_back(it->second());
					break;
				}
			}
			//if (it == Chaos::Modifier::factory.end()) {
			//	trueModifiers.push_back((--it)->second());
			//}
//			return (--it)->second();
		}
			//return new InfiniteBreath();
			//return new Mystery();	// will do nothing
//		};
	}
	
	void begin() {
		buildModifiers();
		for (std::vector<Chaos::Modifier*>::iterator it = trueModifiers.begin(); it != trueModifiers.end(); it++) {
			(*it)->setParentModifier(this);
			(*it)->setDualshock(dualshock);
			(*it)->setChaosEngine(chaosEngine);
			(*it)->begin();
		}
	}
	void update() {
		//trueMod->_update(chaosEngine->isPaused());	// Not very elegant, may not be needed
		for (std::vector<Chaos::Modifier*>::iterator it = trueModifiers.begin(); it != trueModifiers.end(); it++) {
			(*it)->_update(chaosEngine->isPaused());
			//(*it)->update();
		}
	}
	void finish() {
		for (std::vector<Chaos::Modifier*>::iterator it = trueModifiers.begin(); it != trueModifiers.end(); it++) {
			(*it)->finish();
			delete *it;
		}
	}
	bool tweak( DeviceEvent* event ) {
		bool ret = true;
		for (std::vector<Chaos::Modifier*>::iterator it = trueModifiers.begin(); it != trueModifiers.end(); it++) {
			ret &= (*it)->tweak(event);
		}
		return ret;
	}
};


class MuteAudioEffects : public Chaos::Modifier {
	bool busy;
public:
	static void regist() { Chaos::Modifier::factory["Mute Sound Effects"] = [](){return new MuteAudioEffects();}; };
	const char* description() { return "Gunshots hurt our ears :("; };
	
	void begin() {
		busy = true;
		Menuing::getInstance()->setVolumeMode(AUDIO_EFFECTS, -20, dualshock);
		busy = false;
	}
	void finish() {
		busy = true;
		Menuing::getInstance()->setVolumeMode(AUDIO_EFFECTS, 20, dualshock);
		busy = false;
	}
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};

class MuteAudioMusic : public Chaos::Modifier {
	bool busy;
public:
	static void regist() { Chaos::Modifier::factory["Mute Music"] = [](){return new MuteAudioMusic();}; };
	const char* description() { return "The music is too intense, let's turn it off!"; };
	
	void begin() {
		busy = true;
		Menuing::getInstance()->setVolumeMode(AUDIO_MUSIC, -20, dualshock);
		busy = false;
	}
	void finish() {
		busy = true;
		Menuing::getInstance()->setVolumeMode(AUDIO_MUSIC, 20, dualshock);
		busy = false;
	}
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};

class MuteAudioDialogue : public Chaos::Modifier {
	bool busy;
public:
	static void regist() { Chaos::Modifier::factory["Mute Dialogue"] = [](){return new MuteAudioDialogue();}; };
	const char* description() { return "Characters just need to STFU"; };
	
	void begin() {
		busy = true;
		Menuing::getInstance()->setVolumeMode(AUDIO_DIALOGUE, -20, dualshock);
		busy = false;
	}
	void finish() {
		busy = true;
		Menuing::getInstance()->setVolumeMode(AUDIO_DIALOGUE, 20, dualshock);
		busy = false;
	}
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};

class MonoAudio : public Chaos::Modifier {
	bool busy;
public:
	static void regist() { Chaos::Modifier::factory["Mono Audio"] = [](){return new MonoAudio();}; };
	const char* description() { return "No more stereo.  Where did that gunshot come from?"; };
	
	void begin() {
		busy = true;
		Menuing::getInstance()->setVolumeMode(AUDIO_MONO, 1, dualshock);
		busy = false;
	}
	void finish() {
		busy = true;
		Menuing::getInstance()->setVolumeMode(AUDIO_MONO, -1, dualshock);
		busy = false;
	}
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};


typedef struct _TimeAndEvent{
	double time;
	DeviceEvent event;
} TimeAndEvent;

class InputDelay : public Chaos::Modifier {
	// JustSaft
	std::queue<TimeAndEvent> eventQueue;
	double delayTime;
public:
	static void regist() { Chaos::Modifier::factory["Input Delay"] = [](){return new InputDelay();}; };
	const char* description() { return "Introduces a 0.2 second delay to the controller"; };
	
	void begin() {
		delayTime = 0.2;
	}
	
	void update() {
		while ( !eventQueue.empty() ) {
			if( (timer.runningTime() - eventQueue.front().time) >= delayTime ) {
				// Where events are actually sent:
				chaosEngine->fakePipelinedEvent(&eventQueue.front().event, me);
				eventQueue.pop();
			} else {
				break;
			}
		}
	}
	
	bool tweak( DeviceEvent* event ) {
		eventQueue.push( {this->timer.runningTime(), *event} );
		
		return false;	// Block all events
	}
};

class MeleeDelay : public Chaos::Modifier {
	// JustSaft
	std::queue<TimeAndEvent> eventQueue;
	double delayTime;
public:
	static void regist() { Chaos::Modifier::factory["Melee Delay"] = [](){return new MeleeDelay();}; };
	const char* description() { return "Introduces a 0.5 second delay to square presses"; };
	
	void begin() {
		delayTime = 0.5;
	}
	
	void update() {
		while ( !eventQueue.empty() ) {
			if( (timer.runningTime() - eventQueue.front().time) >= delayTime ) {
				// Where events are actually sent:
				chaosEngine->fakePipelinedEvent(&eventQueue.front().event, me);
				eventQueue.pop();
			} else {
				break;
			}
		}
	}
	
	bool tweak( DeviceEvent* event ) {
		if (event->type == TYPE_BUTTON && event->id == BUTTON_SQUARE) {
			eventQueue.push( {this->timer.runningTime(), *event} );
			
			return false;
		}
		
		return true;
	}
};

class JumpDelay : public Chaos::Modifier {
	// JustSaft
	std::queue<TimeAndEvent> eventQueue;
	double delayTime;
public:
	static void regist() { Chaos::Modifier::factory["Jump Delay"] = [](){return new JumpDelay();}; };
	const char* description() { return "Introduces a 2.0 second delay to X presses"; };
	
	void begin() {
		delayTime = 2.0;
	}
	
	void update() {
		while ( !eventQueue.empty() ) {
			if( (timer.runningTime() - eventQueue.front().time) >= delayTime ) {
				// Where events are actually sent:
				chaosEngine->fakePipelinedEvent(&eventQueue.front().event, me);
				eventQueue.pop();
			} else {
				break;
			}
		}
	}
	
	bool tweak( DeviceEvent* event ) {
		if (event->type == TYPE_BUTTON && event->id == BUTTON_X) {
			eventQueue.push( {this->timer.runningTime(), *event} );
			
			return false;
		}
		
		return true;
	}
};

class DodgeDelay : public Chaos::Modifier {
	// JustSaft
	std::queue<TimeAndEvent> eventQueue;
	double delayTime;
public:
	static void regist() { Chaos::Modifier::factory["Dodge Delay"] = [](){return new DodgeDelay();}; };
	const char* description() { return "Introduces a 0.5 second delay to L1 presses"; };
	
	void begin() {
		delayTime = 0.5;
	}
	
	void update() {
		while ( !eventQueue.empty() ) {
			if( (timer.runningTime() - eventQueue.front().time) >= delayTime ) {
				// Where events are actually sent:
				chaosEngine->fakePipelinedEvent(&eventQueue.front().event, me);
				eventQueue.pop();
			} else {
				break;
			}
		}
	}
	
	bool tweak( DeviceEvent* event ) {
		if (event->type == TYPE_BUTTON && event->id == BUTTON_L1) {
			eventQueue.push( {this->timer.runningTime(), *event} );
			
			return false;
		}
		
		return true;
	}
};

class AimDelay : public Chaos::Modifier {
	// JustSaft
	std::queue<TimeAndEvent> eventQueue;
	double delayTime;
public:
	static void regist() { Chaos::Modifier::factory["Aim Delay"] = [](){return new AimDelay();}; };
	const char* description() { return "Introduces a 1 second delay to L2 presses"; };
	
	void begin() {
		delayTime = 1;
	}
	
	void update() {
		while ( !eventQueue.empty() ) {
			if( (timer.runningTime() - eventQueue.front().time) >= delayTime ) {
				// Where events are actually sent:
				chaosEngine->fakePipelinedEvent(&eventQueue.front().event, me);
				eventQueue.pop();
			} else {
				break;
			}
		}
	}
	
	bool tweak( DeviceEvent* event ) {
		if (((event->type == TYPE_BUTTON) && (event->id == BUTTON_L2)) ||
			((event->type == TYPE_AXIS) && (event->id == AXIS_L2))) {
			eventQueue.push( {timer.runningTime(), *event} );
			
			return false;
		}
		
		return true;
	}
};

class ShootDelay : public Chaos::Modifier {
	// JustSaft
	std::queue<TimeAndEvent> eventQueue;
	double delayTime;
public:
	static void regist() { Chaos::Modifier::factory["Shooting Delay"] = [](){return new ShootDelay();}; };
	const char* description() { return "Introduces a 0.5 second delay to R2 presses"; };
	
	void begin() {
		delayTime = 0.5;
	}
	
	void update() {
		while ( !eventQueue.empty() ) {
			if( (timer.runningTime() - eventQueue.front().time) >= delayTime ) {
				// Where events are actually sent:
				chaosEngine->fakePipelinedEvent(&eventQueue.front().event, me);
				eventQueue.pop();
			} else {
				break;
			}
		}
	}
	
	bool tweak( DeviceEvent* event ) {
		if (((event->type == TYPE_BUTTON) && (event->id == BUTTON_R2)) ||
			((event->type == TYPE_AXIS) && (event->id == AXIS_R2))) {
			eventQueue.push( {timer.runningTime(), *event} );
			
			return false;
		}
		
		return true;
	}
};

class DpadDelay : public Chaos::Modifier {
	// JustSaft
	std::queue<TimeAndEvent> eventQueue;
	double delayTime;
public:
	static void regist() { Chaos::Modifier::factory["Dpad Delay"] = [](){return new DpadDelay();}; };
	const char* description() { return "Introduces a 5 second delay to the D-pad"; };
	
	void begin() {
		delayTime = 5;
	}
	
	void update() {
		while ( !eventQueue.empty() ) {
			if( (timer.runningTime() - eventQueue.front().time) >= delayTime ) {
				// Where events are actually sent:
				chaosEngine->fakePipelinedEvent(&eventQueue.front().event, me);
				eventQueue.pop();
			} else {
				break;
			}
		}
	}
	
	bool tweak( DeviceEvent* event ) {
		if ((event->type == TYPE_AXIS && event->id == AXIS_DY) ||
			(event->type == TYPE_AXIS && event->id == AXIS_DX)) {
			eventQueue.push( {this->timer.runningTime(), *event} );
			
			return false;
		}
		
		return true;
	}
};

class JoystickDelay : public Chaos::Modifier {
	// JustSaft
	std::queue<TimeAndEvent> eventQueue;
	double delayTime;
public:
	static void regist() { Chaos::Modifier::factory["Joystick Delay"] = [](){return new JoystickDelay();}; };
	const char* description() { return "Introduces a 0.2 second delay to joysticks"; };
	
	void begin() {
		delayTime = 0.2;
	}
	
	void update() {
		while ( !eventQueue.empty() ) {
			if( (timer.runningTime() - eventQueue.front().time) >= delayTime ) {
				// Where events are actually sent:
				chaosEngine->fakePipelinedEvent(&eventQueue.front().event, me);
				eventQueue.pop();
			} else {
				break;
			}
		}
	}
	
	bool tweak( DeviceEvent* event ) {
		if ((event->type == TYPE_AXIS && event->id == AXIS_RX) ||
			(event->type == TYPE_AXIS && event->id == AXIS_RY) ||
			(event->type == TYPE_AXIS && event->id == AXIS_LY) ||
			(event->type == TYPE_AXIS && event->id == AXIS_LX)) {
			eventQueue.push( {this->timer.runningTime(), *event} );
			
			return false;
		}
		
		return true;
	}
};

class ShapeShuffle : public Chaos::Modifier {
	// Prototoxin
	std::map<int, int> mapping;
public:
	static void regist() { Chaos::Modifier::factory["Shape Shuffle"] = [](){return new ShapeShuffle();}; };
	const char* description() { return "Randomly remaps the shape buttons (square, circle, triangle, X)"; };
	
	void begin() {
		Mogi::Math::Random rng;
		std::list<int> buttons, buttonsCopy;
		buttons.push_back(BUTTON_X);
		buttons.push_back(BUTTON_SQUARE);
		buttons.push_back(BUTTON_CIRCLE);
		buttons.push_back(BUTTON_TRIANGLE);
		buttonsCopy = buttons;
		
		while (buttons.size() > 0) {
			int index = floor(rng.uniform(0, buttons.size()-0.01));
			std::list<int>::iterator it = buttons.begin();
			std::advance(it, index);
			mapping[buttonsCopy.front()] = *it;
			buttonsCopy.pop_front();
			buttons.erase(it);
		}
	}
	
	bool tweak( DeviceEvent* event ) {
		if ( (event->type == TYPE_BUTTON) &&
			mapping.find(event->id) != mapping.end() ) {
			event->id = mapping[event->id];
		}
		
		return true;
	}
};

class MegaShuffle : public Chaos::Modifier {
	// Prototoxin
	std::map<int, int> mapping;
	
	short remappedR2;
	short remappedL2;
public:
	static void regist() { Chaos::Modifier::factory["Random Remap"] = [](){return new MegaShuffle();}; };
	const char* description() { return "Randomly remaps buttons (square, circle, triangle, X, L1, R1, L2, R2, L3, R3)"; };
	
	void begin() {
		Mogi::Math::Random rng;
		std::list<int> buttons, buttonsCopy;
		buttons.push_back(BUTTON_X);
		buttons.push_back(BUTTON_SQUARE);
		buttons.push_back(BUTTON_CIRCLE);
		buttons.push_back(BUTTON_TRIANGLE);
		buttons.push_back(BUTTON_L1);
		buttons.push_back(BUTTON_L2);
		buttons.push_back(BUTTON_L3);
		buttons.push_back(BUTTON_R1);
		buttons.push_back(BUTTON_R2);
		buttons.push_back(BUTTON_R3);
		//buttons.push_back(BUTTON_TOUCHPAD);
		buttonsCopy = buttons;
		
		while (buttons.size() > 0) {
			int index = floor(rng.uniform(0, buttons.size()-0.01));
			std::list<int>::iterator it = buttons.begin();
			std::advance(it, index);
			mapping[buttonsCopy.front()] = *it;
			buttonsCopy.pop_front();
			buttons.erase(it);
		}
	}
	
	bool tweak( DeviceEvent* event ) {
		if ( (event->type == TYPE_BUTTON) &&
			mapping.find(event->id) != mapping.end() ) {
			event->id = mapping[event->id];
			if (event->id == BUTTON_R2) {
				DeviceEvent fakeEvent = {0, (short int)(event->value ? JOYSTICK_MAX : JOYSTICK_MIN), TYPE_AXIS, AXIS_R2};
				chaosEngine->fakePipelinedEvent(&fakeEvent, me);
			} else if (event->id == BUTTON_L2) {
				 DeviceEvent fakeEvent = {0, (short int)(event->value ? JOYSTICK_MAX : JOYSTICK_MIN), TYPE_AXIS, AXIS_L2};
				 chaosEngine->fakePipelinedEvent(&fakeEvent, me);
			 }
		}
		
		if ((event->type == TYPE_AXIS) &&
			(event->id	== AXIS_L2 || event->id	== AXIS_R2) ) {
			return false;
		}
		
		return true;
	}
};

class TriggerShuffle : public Chaos::Modifier {
	// Prototoxin
	std::map<int, int> mapping;
	
	short remappedR2;
	short remappedL2;
public:
	static void regist() { Chaos::Modifier::factory["Trigger Remap"] = [](){return new TriggerShuffle();}; };
	const char* description() { return "Randomly remaps trigger/bumper buttons (L1, R1, L2, R2)"; };
	
	void begin() {
		Mogi::Math::Random rng;
		std::list<int> buttons, buttonsCopy;
//		buttons.push_back(BUTTON_X);
//		buttons.push_back(BUTTON_SQUARE);
//		buttons.push_back(BUTTON_CIRCLE);
//		buttons.push_back(BUTTON_TRIANGLE);
		buttons.push_back(BUTTON_L1);
		buttons.push_back(BUTTON_L2);
//		buttons.push_back(BUTTON_L3);
		buttons.push_back(BUTTON_R1);
		buttons.push_back(BUTTON_R2);
//		buttons.push_back(BUTTON_R3);
		//buttons.push_back(BUTTON_TOUCHPAD);
		buttonsCopy = buttons;
		
		while (buttons.size() > 0) {
			int index = floor(rng.uniform(0, buttons.size()-0.01));
			std::list<int>::iterator it = buttons.begin();
			std::advance(it, index);
			mapping[buttonsCopy.front()] = *it;
			buttonsCopy.pop_front();
			buttons.erase(it);
		}
	}
	
	bool tweak( DeviceEvent* event ) {
		if ( (event->type == TYPE_BUTTON) &&
			mapping.find(event->id) != mapping.end() ) {
			event->id = mapping[event->id];
			if (event->id == BUTTON_R2) {
				DeviceEvent fakeEvent = {0, (short int)(event->value > 0 ? JOYSTICK_MAX : JOYSTICK_MIN), TYPE_AXIS, AXIS_R2};
				chaosEngine->fakePipelinedEvent(&fakeEvent, me);
			} else if (event->id == BUTTON_L2) {
				 DeviceEvent fakeEvent = {0, (short int)(event->value > 0 ? JOYSTICK_MAX : JOYSTICK_MIN), TYPE_AXIS, AXIS_L2};
				 chaosEngine->fakePipelinedEvent(&fakeEvent, me);
			 }
		}
		
		if ((event->type == TYPE_AXIS) &&
			(event->id	== AXIS_L2 || event->id	== AXIS_R2) ) {
			return false;
		}
		
		return true;
	}
};


int main(int argc, char** argv) {
	std::cout << "Welcome to Chaos" << std::endl;
	
	ControllerRaw dualshock;
	dualshock.initialize();
	dualshock.start();
	
	// Custom:
	Inverted::regist();
	Moonwalk::regist();
	SidewaysMoonwalk::regist();	// new
	MeleeOnly::regist();
	NoMelee::regist();
	Pacifist::regist();
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
	BadStamina::regist();	// new
	DisableJoystick::regist();
	DisableJoystickHorizontal::regist();	// new
	DisableDpad::regist();
	DpadRotate::regist();	// new
	Moose::regist();
	SwapSticks::regist();
	SwapStickDpad::regist();
	SwapStickShapes::regist();
	MotionControls::regist();
	MotionControlAiming::regist();
	TouchpadAiming::regist();

	Nascar::regist();
	Zoolander::regist();
	NoBackward::regist();
	NoForward::regist();
	Drunk::regist();
	MegaScopeSway::regist();
	ProneDive::regist();
	Rubbernecking::regist();

	NoGuns::regist();
	NoShortGuns::regist();
	NoLongGuns::regist();
	NoThrows::regist();
	MaxSensitivity::regist();
	MinSensitivity::regist();
	ControllerMirror::regist();
	ControllerFlip::regist();

	DeskPop::regist();
	DoubleTap::regist();	// new
	EmptyGun::regist();
	TossMolly::regist();
	CtgStrat::regist();
	PdubIt::regist();
	ChuckStun::regist();


	// Main menu:
	RestartCheckpoint::regist();

	//  HUD settings:
	NoReticle::regist();
	NoInventoryHud::regist();	// new
	NoDamageIndicators::regist();	// new
	NoAwarenessIndicators::regist();	// new
	NoHitMarkers::regist();	// new
	NoArcs::regist();	// new
	NoHealthHud::regist();	// new
	PickupNotifications::regist();	// new
	
	// Subtitles
	LargeSubtitles::regist();	// new

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
	GridLocked::regist();
	Blacklight::regist();
	DesertFog::regist();
	Blood::regist();
	Inferno::regist();
	Fire::regist();
	Trinity::regist();
	Pusher::regist();
	Demons::regist();
	DrugZone::regist();
	Moonlight::regist();

	// Gameplay Modes:
	Mirror::regist();
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
	CameraAssist::regist();	// new
	LockOnAim::regist();
	AutoPickUp::regist();

	// Framerate (PS5 only)
	SetThirtyFps::regist();

	// Magnification:
	LargeHud::regist();
	HighContrastDisplay::regist();

	// Navigation And Traversal
	NavigationAssistance::regist();	// new
	TraversalAssistance::regist();
	EnhancedListenMode::regist();
	InfiniteBreath::regist();

	// TTS:
	TraversalAudioCues::regist();	// audio
	CombatAudioCues::regist();		//audio
	

	// Combat Accessibilities:
	HostagesDontEscape::regist();	// "new"
	AlliesDontGetGrabbed::regist(); 	// new
	EnemiesDontFlank::regist();
	ReducedEnemyPerception::regist();
	ReducedEnemyAccuracy::regist();
	EnhancedDodge::regist();
	InvisibleWhileProne::regist();

	StrafeOnly::regist();
	NoStrafe::regist();
	AimMovement::regist();	// new
	LeeroyJenkins::regist();
	ForceAim::regist();
	SpeedrunGlitch::regist();

	Mystery::regist();
	ChaosMod::regist();

	MuteAudioMusic::regist();
	MuteAudioEffects::regist();
	MuteAudioDialogue::regist();
	MonoAudio::regist();

	InputDelay::regist();
	MeleeDelay::regist();
	JumpDelay::regist();	// new
	DodgeDelay::regist();	// new
	ShootDelay::regist();
	AimDelay::regist();		// new
	DpadDelay::regist();
	JoystickDelay::regist();
	
	ShapeShuffle::regist();	// new
	MegaShuffle::regist();	// new
	TriggerShuffle::regist();	// new
	
	// Custom: 48
	// Audio: 10
	// Render/Display: 32
	// Gameplay: 23
	
	int i = 1;
	printf("Registered Modfiers: \n");
	for (auto const& it : Chaos::Modifier::factory) {
		//std::cout << "Registered Mod: " << it.first << std::endl;
		printf("%3d: %s\n", i++, it.first.c_str());
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
