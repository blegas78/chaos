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

class Inverted : public Modifier {public:
	static void regist() { Modifier::factory["Inverted"] = [](){return new Inverted();}; };
	const char* description() { return "Inverts the Y-axis on the right joystick (aim inversion)"; };
	
	bool tweak( DeviceEvent* event ) {
		if ((event->id == AXIS_RY ) && event->type == TYPE_AXIS) {
			event->value = -((int)event->value+1) ;
		}
		return true;
	}
};
class Moonwalk : public Modifier {/*Suggestion from protoxin:*/public:
	static void regist() { Modifier::factory["Moonwalk"] = [](){return new Moonwalk();}; };
	const char* description() { return "Be like Michael Jackson!  Trying to walk forward will actually make you go backward"; };
	
	bool tweak( DeviceEvent* event ) {
		if ((event->id == AXIS_LY ) && event->type == TYPE_AXIS) {
			event->value = -((int)event->value+1) ;
		}
		return true;
	}
};
class OnlyMelee : public Modifier {public:
	static void regist() { Modifier::factory["Melee Only"] = [](){return new OnlyMelee();}; };
	const char* description() { return "No weapons, no inventory items"; };
	
	bool tweak( DeviceEvent* event ) {
		if (event->id == BUTTON_L2 && event->type == TYPE_BUTTON){ // Disable reload and drawing the bow if aiming
			if (event->id == BOTTON_R2 && event->type == TYPE_BUTTON){
				event->value = 0;
			} else if(event->id == BOTTON_R1 && event->type == TYPE_BUTTON){
				event->value = 0;
			}
		}
		if (event->id == BUTTON_L1 && event->type == TYPE_BUTTON){ // no weapon wheel
			event->value = 0;
		}
		if (event->id == AXIS_DY, -1 && event->type == TYPE_AXIS){ // disable use of inventory item (so you can't use bombs)
			event->value = 0;
		}
		return true;
	}
};
class LiterallyOnlyMelee : public Modifier {public:
	static void regist() { Modifier::factory["Literally Melee Only"] = [](){return new LiterallyOnlyMelee();}; };
	const char* description() { return "Nothing but melee (literally)"; };
	
	bool tweak( DeviceEvent* event ) {
		if (event->id == BUTTON_L2 && event->type == TYPE_BUTTON){
			event->value = 0;
		}
		if (event->id == BUTTON_L1 && event->type == TYPE_BUTTON){ 
			event->value = 0;
		}
		if (event->id == BUTTON_X && event->type == TYPE_BUTTON){ 
			event->value = 0;
		}
		if (event->id == BUTTON_CIRCLE && event->type == TYPE_BUTTON){
			event->value = 0;
		}
		if (event->id == BUTTON_SQUARE && event->type == TYPE_BUTTON){
			event->value = 0;
		}
		if (event->id == BUTTON_TRIANGLE && event->type == TYPE_BUTTON){
			event->value = 0;
		}
		if (event->id == AXIS_DY && event->type == TYPE_AXIS){
			event->value = 0;
		}
		if (event->id == AXIS_DX && event->type == TYPE_AXIS){
			event->value = 0;
		}
		if (event->id == AXIS_LY && event->type == TYPE_AXIS){
			event->value = 0;
		}
		if (event->id == AXIS_LX && event->type == TYPE_AXIS){
			event->value = 0;
		}
		if (event->id == AXIS_RY && event->type == TYPE_AXIS){
			event->value = 0;
		}
		if (event->id == AXIS_RX && event->type == TYPE_AXIS){
			event->value = 0;
		}
		return true;
	}
};
class Pacifist : public Modifier {
public:
	static void regist() { Modifier::factory["Pacifist%"] = [](){return new Pacifist();}; };
	const char* description() { return "No shooting, throwing, nor melee. Good luck NPC friends! :)"; };
	bool tweak( DeviceEvent* event ) {
		if (event->type == TYPE_BUTTON && (event->id == BUTTON_R1/*disable melee*/ || event->id == BUTTON_R2/*Disable heavy melee*/) ) {
			event->value = 0;
		} else if(event->type == TYPE_AXIS && event->id == AXIS_R2) {
			event->value = JOYSTICK_MIN;
		}
		return true;
	}
};
class NoJumpingWhileAiming : public Modifier {
public:
	static void regist() { Modifier::factory["No Juming while aiming"] = [](){return new NoJumpingWhileAiming();}; };
	const char* description() { return "No slowmotion! Aiming is still allowed"; };
	bool tweak( DeviceEvent* event ) {
		if (event->id == BUTTON_X && event->type == TYPE_BUTTON) {
			if (dualshock->getState(BUTTON_L2, TYPE_BUTTON)) {	// only disable X when aiming
				event->value = 0;
			}
		}
		return true;
	}
};
class NoDoubleOrTripleShots : public Modifier {
public:
	static void regist() { Modifier::factory["No Juming while aiming"] = [](){return new NoDoubleOrTripleShots();}; };
	const char* description() { return "No double or triple shots"; };
	bool tweak( DeviceEvent* event ) {
		if (event->id == BUTTON_R1 && event->type == TYPE_BUTTON) {
			if (dualshock->getState(BUTTON_L2, TYPE_BUTTON)) {	// only disable R1 when aiming
				event->value = 0;
			}
		}
		return true;
	}
};
class DisableTriangle : public Modifier {
public:
	static void regist() { Modifier::factory["Disable Triangle"] = [](){return new DisableTriangle();}; };
	const char* description() { return "Disables triangle"; };
	bool tweak( DeviceEvent* event ) {
		if (event->id == BUTTON_TRIANGLE && event->type == TYPE_BUTTON) {
			event->value = 0;
		}
		return true;
	}
};
class DisableX : public Modifier {
public:
	static void regist() { Modifier::factory["No Jumping"] = [](){return new DisableX();}; };
	const char* description() { return "Disables X"; };
	bool tweak( DeviceEvent* event ) {
		if (event->id == BUTTON_X && event->type == TYPE_BUTTON) {
			event->value = 0;
		}
		return true;
	}
};
class DisableCircle : public Modifier {
public:
	static void regist() { Modifier::factory["Disable Circle"] = [](){return new DisableCircle();}; };
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
class DisableSquare : public Modifier {
public:
	static void regist() { Modifier::factory["Disable Square"] = [](){return new DisableSquare();}; };
	const char* description() { return "Square is disabled"; };
	void begin() {
		DeviceEvent event = {0,0,TYPE_BUTTON, BUTTON_SQUARE};
		dualshock->applyEvent(&event);
	}
	bool tweak( DeviceEvent* event ) {
		if (event->id == BUTTON_SQUARE && event->type == TYPE_BUTTON) {
			//return false;
			event->value = 0;
		}
		return true;
	}
};
class Disco : public Modifier {
	// Rename/tuning from HeHathYought
public:
	static void regist() { Modifier::factory["Anthony Caliber"] = [](){return new Disco();}; };
	const char* description() { return "Keeps pressing R3"; };
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
class RachyFlail : public Modifier {
public:
	static void regist() { Modifier::factory["Rachy Flail"] = [](){return new RachyFlail();}; };
	const char* description() { return "The only way to melee is to follow the master techniques of @RachyMonster"; };
	double pressTime;
	void begin() {
		DeviceEvent event = {0,1,TYPE_BUTTON, BUTTON_R1};
		dualshock->applyEvent(&event);
		pressTime = 0;
	}
	void update() {
		pressTime += timer.dTime();
		if ( pressTime > 0.1  && dualshock->getState(BUTTON_R1, TYPE_BUTTON) ) {
			DeviceEvent event = {0,0,TYPE_BUTTON, BUTTON_R1};
			dualshock->applyEvent(&event);
			pressTime = 0;
		} else if( pressTime > 1.1 && !dualshock->getState(BUTTON_R1, TYPE_BUTTON) ) {
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
class KeepJumping : public Modifier {
public:
	static void regist() { Modifier::factory["Keep Jumping"] = [](){return new KeepJumping();}; };
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
class KeepPressingSquare : public Modifier {
public:
	static void regist() { Modifier::factory["Keep Dodging"] = [](){return new KeepPressingSquare();}; };
 	const char* description() { return "Repeated presses of Circle"; };
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
 		} else if( pressTime > 2.0 && !dualshock->getState(BUTTON_SQUARE, TYPE_BUTTON) ) {
 			DeviceEvent event = {0,1,TYPE_BUTTON, BUTTON_SQUARE};
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
class KeepPressingCircle : public Modifier {
public:
	static void regist() { Modifier::factory["Keep Dodging"] = [](){return new KeepPressingCircle();}; };
 	const char* description() { return "Repeated presses of Circle"; };
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
 		} else if( pressTime > 2.0 && !dualshock->getState(BUTTON_CIRCLE, TYPE_BUTTON) ) {
 			DeviceEvent event = {0,1,TYPE_BUTTON, BUTTON_CIRCLE};
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
class PeriodicFocus : public Modifier {
public:
	static void regist() { Modifier::factory["Peridoic Focus scans"] = [](){return new PeriodicFocus();}; };
	const char* description() { return "Repeated presses of R3"; };
	double pressTime;
	void begin() {
		DeviceEvent event = {0,1,TYPE_BUTTON, BUTTON_R3};
		dualshock->applyEvent(&event);
		pressTime = 0;
	}
	void update() {
		pressTime += timer.dTime();
		if ( pressTime > 0.1  && dualshock->getState(BUTTON_R3, TYPE_BUTTON) ) {
			DeviceEvent event = {0,0,TYPE_BUTTON, BUTTON_R3};
			dualshock->applyEvent(&event);
			pressTime = 0;
		} else if( pressTime > 2.0 && !dualshock->getState(BUTTON_R3, TYPE_BUTTON) ) {
			DeviceEvent event = {0,1,TYPE_BUTTON, BUTTON_R3};
			dualshock->applyEvent(&event);
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
class PeriodicMelee : public Modifier {
public:
	static void regist() { Modifier::factory["Peridoic melee attacks"] = [](){return new PeriodicMelee();}; };
	const char* description() { return "Repeated presses of R1"; };
	double pressTime;
	void begin() {
		DeviceEvent event = {0,1,TYPE_BUTTON, BUTTON_R1};
		dualshock->applyEvent(&event);
		pressTime = 0;
	}
	void update() {
		pressTime += timer.dTime();
		if ( pressTime > 0.1  && dualshock->getState(BUTTON_R1, TYPE_BUTTON) ) {
			DeviceEvent event = {0,0,TYPE_BUTTON, BUTTON_R3};
			dualshock->applyEvent(&event);
			pressTime = 0;
		} else if( pressTime > 2.0 && !dualshock->getState(BUTTON_R1, TYPE_BUTTON) ) {
			DeviceEvent event = {0,1,TYPE_BUTTON, BUTTON_R3};
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
class PeriodicHeavyMelee : public Modifier {
public:
	static void regist() { Modifier::factory["Peridoic melee attacks"] = [](){return new PeriodicHeavyMelee();}; };
	const char* description() { return "Periodic presses of R2"; };
	double pressTime;
	void begin() {
		DeviceEvent event = {0,1,TYPE_BUTTON, BUTTON_R2};
		dualshock->applyEvent(&event);
		pressTime = 0;
	}
	void update() {
		pressTime += timer.dTime();
		if ( pressTime > 0.1  && dualshock->getState(BUTTON_R2, TYPE_BUTTON) ) {
			DeviceEvent event = {0,0,TYPE_BUTTON, BUTTON_R2};
			dualshock->applyEvent(&event);
			pressTime = 0;
		} else if( pressTime > 2.0 && !dualshock->getState(BUTTON_R1, TYPE_BUTTON) ) {
			DeviceEvent event = {0,1,TYPE_BUTTON, BUTTON_R2};
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
class PeriodicWeaponWheel : public Modifier {
public:
	static void regist() { Modifier::factory["Periodic Weapon Wheel"] = [](){return new PeriodicWeaponWheel();}; };
	const char* description() { return "Repeated holds of L1"; };
	double pressTime;
	void begin() {
		DeviceEvent event = {0,1,TYPE_BUTTON, BUTTON_L1};
		dualshock->applyEvent(&event);
		pressTime = 0;
	}
	void update() {
		pressTime += timer.dTime();
		if ( pressTime > 3.0  && dualshock->getState(BUTTON_L1, TYPE_BUTTON) ) {
			DeviceEvent event = {0,0,TYPE_BUTTON, BUTTON_L1};
			dualshock->applyEvent(&event);
			pressTime = 0;
		} else if( pressTime > 8.0 && !dualshock->getState(BUTTON_L1, TYPE_BUTTON) ) {
			DeviceEvent event = {0,1,TYPE_BUTTON, BUTTON_L1};
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
class DisableL3 : public Modifier {
public:
	static void regist() { Modifier::factory["Disable L3"] = [](){return new DisableL3();}; };
	const char* description() { return "L3 is disabled"; };
	void begin() {
		DeviceEvent event = {0,0,TYPE_BUTTON, BUTTON_L3};
		dualshock->applyEvent(&event);
	}
	bool tweak( DeviceEvent* event ) {
		if (event->id == BUTTON_L3 && event->type == TYPE_BUTTON) {
			//return false;
			event->value = 0;
		}
		return true;
	}
};
class DisableL2 : public Modifier {
public:
	static void regist() { Modifier::factory["No Aiming"] = [](){return new DisableL2();}; };
	const char* description() { return "Disables L2"; };
	bool tweak( DeviceEvent* event ) {
		if (event->id == BUTTON_L2 && event->type == TYPE_BUTTON) {
			event->value = 0;
		}
		return true;
	}
};
class DisableL1 : public Modifier {
public:
	static void regist() { Modifier::factory["Disable L1"] = [](){return new DisableL1();}; };
	const char* description() { return "Disables weapon wheel"; };
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
class DisableR3 : public Modifier {
public:
	static void regist() { Modifier::factory["Disable R3"] = [](){return new DisableR3();}; };
	const char* description() { return "R3 is disabled - no focus for you anymore"; };
	void begin() {
		DeviceEvent event = {0,0,TYPE_BUTTON, BUTTON_R3};
		dualshock->applyEvent(&event);
	}
	bool tweak( DeviceEvent* event ) {
		if (event->id == BUTTON_R3 && event->type == TYPE_BUTTON) {
			//return false;
			event->value = 0;
		}
		return true;
	}
};
class LeeroyJenkins : public Modifier {
public:
	static void regist() { Modifier::factory["Leeroy Jenkins"] = [](){return new LeeroyJenkins();}; };
	const char* description() { return "Alright let's do this! LEEEEROOOOOY NNNJEEEENNKIINNS!!!"; };
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
class ForceAim : public Modifier {
	//Prototoxin187
public:
	static void regist() { Modifier::factory["Force Aiming"] = [](){return new ForceAim();}; };
	const char* description() { return "I won't let go of L2... I swear Kappa"; };
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
class DisableRightJoystick : public Modifier {
public:
	static void regist() { Modifier::factory["Disable Right Joystick"] = [](){return new DisableRightJoystick();}; };
	const char* description() { return "Good luck with aiming and looking around!"; };
	bool tweak( DeviceEvent* event ) {
		if ( (event->id == AXIS_RX || event->id == AXIS_RY) && event->type == TYPE_AXIS) {
			event->value = 0;
		}
		return true;
	}
};
class DisableLeftJoystick : public Modifier {
public:
	static void regist() { Modifier::factory["Disable Left Joystick"] = [](){return new DisableLeftJoystick();}; };
	const char* description() { return "Good luck with walking!"; };
	bool tweak( DeviceEvent* event ) {
		if ( (event->id == AXIS_LX || event->id == AXIS_LY) && event->type == TYPE_AXIS) {
			event->value = 0;
		}
		return true;
	}
};
class OnlyStrafeMovement : public Modifier {
	//Prototoxin187
public:
	static void regist() { Modifier::factory["Only Strafe Movement"] = [](){return new OnlyStrafeMovement();}; };
	const char* description() { return "X-axis on the left joystick is disabled.  Only forward/back motion is allowed."; };
	bool tweak( DeviceEvent* event ) {
		if ( event->id == AXIS_LY && event->type == TYPE_AXIS) {
			event->value = 0;
		}
		return true;
	}
};
class NoStrafeMovement : public Modifier {
public:
	static void regist() { Modifier::factory["No Strafe Movement"] = [](){return new NoStrafeMovement();}; };
	const char* description() { return "Y-axis on the left joystick is disabled.  Only left/right motion is allowed."; };
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
class SwapStickDpad : public Modifier {
	//JustForSaft, joshuatimes7
public:
	static void regist() { Modifier::factory["Swap D-Pad/Left Joystick"] = [](){return new SwapStickDpad();}; };
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
class MotionControls : public Modifier {
public:
	static void regist() { Modifier::factory["Motion Control Movement"] = [](){return new MotionControls();}; };
	const char* description() { return "No more left joystick, tilt the controller to move instead"; };
	bool tweak( DeviceEvent* event ) {
		if( event->type == TYPE_AXIS ) {
			DeviceEvent newEvent;
			switch (event->id) {
				case AXIS_ACCX:
					newEvent = *event;
					newEvent.id = AXIS_LX;
					newEvent.value = joystickLimit(-event->value/20);
					chaosEngine->fakePipelinedEvent(&newEvent, this);
					break;
				case AXIS_ACCZ:
					newEvent = *event;
					newEvent.id = AXIS_LY;
					newEvent.value = joystickLimit(-event->value/20);
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
	const char* description() { return "No more right joystick, tilt the controller to aim/look instead"; };
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
				chaosEngine->fakePipelinedEvent(&newEvent, this);
				newEvent.id = AXIS_RY;
				newEvent.type = TYPE_AXIS;
				newEvent.value = 0;
				chaosEngine->fakePipelinedEvent(&newEvent, this);
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
						chaosEngine->fakePipelinedEvent(&newEvent, this);
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
						chaosEngine->fakePipelinedEvent(&newEvent, this);
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
	const char* description() { return "There is no going right in Nascar"; };
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
	const char* description() { return "Random joystick motion.  Also don't push things too far, you may stumble"; };
	
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
class TLOUSpeedrunGlitch : public Modifier {
	// HeHathYought
public:
	static void regist() { Modifier::factory["TLOU1 Run Glitch"] = [](){return new TLOUSpeedrunGlitch();}; };
	const char* description() { return "Is this an Any% speedrun of TLOU1?"; };
	
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
		if( event->type == TYPE_AXIS && (event->id == AXIS_DY || event->id == AXIS_DX)) {
			return false;
		}
		return true;
	}
};
class MegaSway : public Modifier {
public:
	static void regist() { Modifier::factory["Mega Scope Sway"] = [](){ return new MegaSway();}; };
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
class DisableDpad : public Modifier {
public:
	static void regist() { Modifier::factory["Disable Dpad"] = [](){return new DisableDpad();}; };
	const char* description() { return "No healing allowed OpieOP"; };
	bool tweak( DeviceEvent* event ) {
		if (event->type == TYPE_AXIS && (event->id == AXIS_DY || event->id == AXIS_DX)) {
			return false;
		}
		return true;
	}
};
class MaxSensitivity : public Modifier {
	// gabemusic
public:
	static void regist() { Modifier::factory["Max Sensitivity"] = [](){return new MaxSensitivity();}; };
	const char* description() { return "Goodbye precision aiming"; };
	
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
	// PrincessDiodes, cloverfieldmel, DJ_Squall_808
public:
	static void regist() { Modifier::factory["Controller Mirror"] = [](){return new ControllerMirror();}; };
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
class ControllerFlip : public Modifier {
	// Hipsterobot
public:
	static void regist() { Modifier::factory["Controller Flip"] = [](){return new ControllerFlip();}; };
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
					chaosEngine->fakePipelinedEvent(&newEvent, this);
					break;
				case BUTTON_L1:
					event->id = BUTTON_L2;
					newEvent.id	= AXIS_L2;
					newEvent.type = TYPE_AXIS;
					newEvent.value = event->value ? JOYSTICK_MAX : JOYSTICK_MIN;
					chaosEngine->fakePipelinedEvent(&newEvent, this);
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
class RestartFromSave : public Modifier {
public:
	static void regist() { Modifier::factory["Restart from save"] = [](){return new RestartFromSave();}; };
	const char* description() { return "Best served near the end of a long encounter"; };
	
	bool busy;
	
	void begin() {
		busy = true;
		
		Sequence sequence;
		sequence.disablejoysticks();
		sequence.addButtonPress( BUTTON_OPTIONS);
		sequence.addAxisPress( AXIS_DY, -1);
		sequence.addButtonPress( BUTTON_X);
		sequence.addTimeDelay(2000);
		sequence.addButtonPress( BUTTON_X);
		sequence.addTimeDelay(2000);
		sequence.send(dualshock);
		
		busy = false;
	}
	
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};
class QuitToMainMenu : public Modifier {
public:
	static void regist() { Modifier::factory["Quit to main menu"] = [](){return new QuitToMainMenu();}; };
	const char* description() { return "Best served near the end of a long encounter"; };
	
	bool busy;
	
	void begin() {
		busy = true;
		
		Sequence sequence;
		sequence.disablejoysticks();
		sequence.addButtonPress( BUTTON_OPTIONS);
		sequence.addAxisPress( AXIS_DY, 1);
		sequence.addButtonPress( BUTTON_X);
		sequence.addTimeDelay(2000);
		sequence.addButtonPress( BUTTON_X);
		sequence.addTimeDelay(2000);
		sequence.send(dualshock);
		
		busy = false;
	}
	
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};
class HydrationBreak : public Modifier {
public:
	static void regist() { Modifier::factory["Hydration break"] = [](){return new HydrationBreak();}; };
	const char* description() { return "Take a 30 second Hydration break"; };
	
	bool busy;
	
	void begin() {
		busy = true;
		
		Sequence sequence;
		sequence.disablejoysticks();
		sequence.addButtonPress( BUTTON_OPTIONS);
		sequence.addTimeDelay(20000);
		sequence.addButtonPress( BUTTON_OPTIONS);
		sequence.send(dualshock);
		
		busy = false;
	}
	
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};
class Heal : public Modifier {public:
	static void regist(){Modifier::factory["Heal. NOW!"] = [](){return new HealNOW();};};
	const char* description() {return "Uses the medicine pouch to heal";};
	bool busy;
	void begin() {
		busy = true;
		Sequence sequence;
		sequence.disablejoysticks();
		sequence.addAxisPress( AXIS_DY, 1);
		sequence.addTimeDelay(2000);
		sequence.send(dualshock);
		busy = false;
	}
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
}
class UseThisInventoryItem : public Modifier {public:
	static void regist(){Modifier::factory["Use this inventory item"] = [](){return new UseThisInventoryItem();};};
	const char* description() {return "Uses the current inventory item";};
	bool busy;
	void begin() {
		busy = true;
		Sequence sequence;
		sequence.disablejoysticks();
		sequence.addAxisPress( AXIS_DY, -1);
		sequence.addTimeDelay(2000);
		sequence.send(dualshock);
		busy = false;
	}
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
}
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
	const char* description() { return "Applys any randomly chosen modifier"; };
};
class DisableDpadUp : public Modifier {public:
	static void regist() { Modifier::factory["No healing"] = [](){return new DisableDpadUp();}; };
	const char* description() { return "No healing allowed OpieOP"; };
	bool tweak( DeviceEvent* event ) {
		if (event->type == TYPE_AXIS && event->id == AXIS_DY, 1) {
			event->value = 0;
		}
		return true;
	}
}
class DisableDpadDown : public Modifier {public:
	static void regist() { Modifier::factory["No inventory items"] = [](){return new DisableDpadDown();}; };
	const char* description() { return "No use of inventory items allowed OpieOP"; };
	bool tweak( DeviceEvent* event ) {
		if (event->type == TYPE_AXIS && event->id == AXIS_DY, -1) {
			event->value = 0;
		}
		return true;
	}
}
class NoMelee : public Modifier {public:
	static void regist() { Modifier::factory["Disable Melee"] = [](){return new NoMelee();}; };
	const char* description() { return "Disables Melee/Spear attacks"; };
	// const char* modIsForGame() { return "HZD"; }; // maybe use something like this to be able to determine what game this mod belongs to?
	bool tweak( DeviceEvent* event ) {
		if (event->id == BUTTON_R2 && event->type == TYPE_BUTTON) {
			if (!dualshock->getState(BUTTON_L2, TYPE_BUTTON)) {	// only disable R2 when not aiming
				event->value = 0;
			}
		}
		if (event->id == BUTTON_R1 && event->type == TYPE_BUTTON) {
			if (!dualshock->getState(BUTTON_L2, TYPE_BUTTON)) {	// only disable R1 when not aiming
				event->value = 0;
			}
		}
		return true;
	}
}



int main(int argc, char** argv) {
	std::cout << "Welcome to Chaos" << std::endl;
	
	ControllerRaw dualshock;
	dualshock.initialize();
	dualshock.start();
	
		/* ========================== */																			/* ========================== */
		/* ==========Trigger========= */																			/* ==========Trigger========= */
		DisableL2::regist();								/* ========================== */						PeriodicHeavyMelee::regist();
		ForceAim::regist();									/* =========Touchpad========= */						PeriodicMelee::regist(); OnlyMelee::regist();
		DisableL1::regist();								    TouchpadAiming::regist();							NoMelee::regist(); LiterallyOnlyMelee::regist();
		PeriodicWeaponWheel::regist();						    MotionControls::regist();							NoDoubleOrTripleShots::regist();
		/* ========================== */					    MotionControlAiming::regist();						/* ========================== */
		/* ========================== */					/* ========================== */						/* ========================== */
															/* ========================== */																				

		Heal::regist();																								  /* ==================== */
		DisableDpad::regist();																						  /* ==================== */
		DisableDpadUp::regist();																					  DisableTriangle::regist();
/* ============ */		/* ============ */															  	DisableSquare::regist();		DisableCircle::regist();
/* Dpad Left=== */		/* Dpad Right== */																KeepPressingSquare::regist();	KeepPressingCircle::regist();
/* ============ */		/* ============ */																/* ======================= */   /* ======================= */
		SwapStickDpad::regist();																	  				  NoJumpingWhileAiming::regist();
		DisableDpadDown::regist();																					  DisableX::regist();
		UseThisInventoryItem::regist();														  						  KeepJumping::regist();


							DisableLeftJoystick::regist();		SwapSticks::regist();			  DisableRightJoystick::regist();
							DisableL3::regist();				ControllerMirror::regist();		  DisableR3::regist();
							Moonwalk::regist();					ControllerFlip::regist();		  PeriodicFocus::regist();
							NoStrafeMovement::regist();			MaxSensitivity::regist();		  MegaSway::regist();
							OnlyStrafeMovement::regist();

	// Other mods
	Disco::regist();
	Nascar::regist();
	Drunk::regist();
	Mystery::regist();
	LeeroyJenkins::regist();
	RachyFlail::regist();
	
	// Menuing
	HydrationBreak::regist();
	RestartFromSave::regist();
	QuitToMainMenu::regist();
	TLOUSpeedrunGlitch::regist();

	// TODO:  HUD settings:
	// NoReticle::regist();

	
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
	
	std::string reply = Modifier::getModList(timePerModifier);
	while(1) {
		chaosEngine.setInterfaceReply( reply );
		usleep(10000000);
	}
	
	return 0;
}
