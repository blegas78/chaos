#ifndef CHAOS_H
#define CHAOS_H

#include <stdio.h>
#include <mogi/thread.h>
#include <mogi/math/systems.h>
#include <list>
#include <string>
#include <functional>
#include <map>
#include <vector>
#include <queue>

#include "interface.h"	// for CommandListener
#include "controller.h"

//#define TIME_PER_MODIFIER (30)

#define TIME_AFTER_JOYSTICK_DISABLE ((unsigned int)(1000000/3))	// in us
//#define TIME_PER_BUTTON_PRESS (50000)	// in us
//#define TIME_PER_BUTTON_RELEASE (50000)	// in us
#define TIME_PER_BUTTON_PRESS ((unsigned int)(50000*1.25))	// in us
#define TIME_PER_BUTTON_RELEASE ((unsigned int)(50000*1.25))	// in us
#define MENU_SELECT_DELAY ((unsigned int)(50)) // in ms

enum OptionsMainMenu {
	MENU_OPTIONS = 0,
	MENU_PHOTOMODE = 1,
	MENU_EXTRAS = 2,
	MENU_TUTORIALS = 3,
	MENU_SAVE = 4,
	MENU_LOAD = 5,
	MENU_RESTART1 = -3,
	MENU_RESTART2 = -2,
	MENU_QUIT = -1
};

enum OptionsOptions {
	OPTIONS_DIFFICULTY = 0,
	OPTIONS_CONTROLS = 1,
	OPTIONS_HUD = 2,
	OPTIONS_ACCESSIBILITY = -1,
};

enum OptionsHUD {
	HUD_RETICLE = 0,
	HUD_WEAPON_CROSS_DISABLE = 1
};

enum OptionsExtras {
	EXTRA_RENDER = 0,
	EXTRA_GAMEPLAY = 1
};

enum OptionsRender {
	RENDER_NONE = 0,
	RENDER_GRAPHIC = 1,
	RENDER_HEADHACHE = 2,
	RENDER_AFTERLIFE = 3,
	RENDER_EIGHTBIT = 4,
	RENDER_WATERCOLOR = 5,
	RENDER_DUNGEON = 6,
	RENDER_VOID = 7,
	RENDER_POPPOSTER = 8,
	RENDER_NINETEENSIXTY = 9,
	RENDER_COOL = 10,
	RENDER_WARM = 11,
	RENDER_VIBRANT = 12,
	RENDER_NOIR = 13,
	RENDER_BLORANGE = 14,
	RENDER_SEPIA = 15,
	RENDER_VINTAGE = -14,
	RENDER_BEASTS = -13,
	RENDER_TERMINATED = -12,
	RENDER_GRIDLOCKED = -11,
	RENDER_BLACKLIGHT = -10,
	RENDER_DESERT_FOG = -9,
	RENDER_BLOOD = -8,
	RENDER_INFERNO = -7,
	RENDER_FIRE = -6,
	RENDER_TRINITY = -5,
	RENDER_PUSHER = -4,
	RENDER_DEMONS = -3,
	RENDER_DRUGZONE = -2,
	RENDER_MOONLIGHT = -1,
};

enum OptionsGameplay {
    GAMEPLAY_MIRROR = 0,
    GAMEPLAY_MIRROR_ON_DEATH = 1,
    GAMEPLAY_SLOW_MOTION = 2,
    GAMEPLAY_BULLET_SPEED = 3,
    GAMEPLAY_INFINITE_AMMO = 4,
    GAMEPLAY_INFINITE_CRAFT = 5,
    GAMEPLAY_INFINITE_MELEE_DURABILITY = 6,
    GAMEPLAY_INFINITE_LISTEN_RANGE = 7,
//    GAMEPLAY_INFINITE_MELEE = 8,			// ????
    GAMEPLAY_ONE_SHOT = -6,
    GAMEPLAY_TOUCH_OF_DEATH = -5,
    GAMEPLAY_AUDIO_EIGHT_BIT = -4,
    GAMEPLAY_AUDIO_FOUR_BIT = -3,
    GAMEPLAY_AUDIO_HELIUM = -2,
    GAMEPLAY_AUDIO_XENON = -1,
};

enum ReticleOptions {
    RETICLE_OFF = 0,
    RETICLE_SIMPLE = 1,
    RETICLE_DEFAULT = 2
};

enum OptionsAccessibility {	// options/accessibility
	ACCESSIBILITY_ALTERNATE_CONTROLS = 0,
	ACCESSIBILITY_MAGNIFICATION_AND_VISUAL_AIDS = 1,
	ACCESSIBILITY_MOTION_SICKNESS = 2,
	ACCESSIBILITY_NAVIGATION_AND_TRAVERSAL = 3,
	ACCESSIBILITY_TTS_AND_AUDIO_CUES = -2,
	ACCESSIBILITY_COMBAT_ACCESSIBILITY = -1,
};

enum OptionsAternateControls {	// options/accessibility/alternate controls
	ALTERNATE_CUSTOMIZE_CONTROLS = 0,
	ALTERNATE_LOCK_ON_AIM = -6,
	ALTERNATE_AUTO_PICK_UP = -1
};

enum OptionsMagnificationAndAids {	// options/accessibility/Magnification
	MAGNIFICATION_HUD_SCALE = 0,	// for larger HUD
	MAGNIFICATION_HIGH_CONTRAST = -3,
};

enum OptionsNavigation {	// options/accessibility/Navigation
	NAVIGATION_ASSISTANCE = 0,
	NAVIGATION_TRAVERSAL_ASSISTANCE = 1,
	NAVIGATION_LEDGE_GUARD = 2,
	NAVIGATION_ENHANCED_LISTEN_MODE = 3,
	NAVIGATION_SCAN_RANGE = 4,
	NAVIGATION_SCAN_TIME = -3,
	NAVIGATION_INFINITE_BREATH = -2,
	NAVIGATION_SKIP_PUZZLE = -1
};

enum OptionsTTS {	// options/accessibility/TTS
	TTS_TTS = 0,
	TTS_TRAVERSAL_CUES = 1,
	TTS_COMBAT_CUES = 2
};

enum OptionsCombat {	// options/accessibility/Combat
	COMBAT_ENABLE = 0,
	COMBAT_HOSTAGES_DONT_ESCAPE = 1,
	COMBAT_ALLIES_DONT_GET_GRABBED = 2,
	COMBAT_ENEMIES_DONT_FLANK = 3,
	COMBAT_REDUCED_ENEMY_PERCEPTION = 4,
	COMBAT_REDUCED_ENEMY_ACCURACY = 5,
	COMBAT_ENHANCED_DODGE = -4,
	COMBAT_INVISBLE_WHILE_PRONE = -3,	// use d-pad, right twitce to sleect
	COMBAT_WEAPON_SWAY = -2,
	COMBAT_SLOW_MOTION = -1	// d-pad, but cannot use this. have slowmo elsewhere
};





class Sequence {
private:
	std::vector<DeviceEvent> events;
	
public:
	Sequence();
	void disablejoysticks();	// Needed for proper menuing, probably
	void addButtonPress( ButtonID id );
	void addButtonHold( ButtonID id );
	void addButtonRelease( ButtonID id );
	void addTimeDelay( unsigned int timeInMilliseconds );
	void addAxisPress( AxisID id, short value );
	void addAxisHold( AxisID id, short value );
	void send(Controller* dualshock);
	
    void clear();
	// For building a sequence:
	//void changeMenuOption();
};

class Menuing {
private:
    Menuing();
    
    Sequence sequence;
    
    int mainMenu;
    int extras;
    int render;
    int gameplay;
    int options;
    int hud;
	
	int accessibility = ACCESSIBILITY_ALTERNATE_CONTROLS;
	int alternateControls = ALTERNATE_CUSTOMIZE_CONTROLS;
	int magnification = MAGNIFICATION_HUD_SCALE;
	int navigation = NAVIGATION_ASSISTANCE;
	int tts = TTS_TTS;
	int numberAudioCuesEnabled = 0;
	int combat = COMBAT_ENABLE;

#define AUDIO_DEFAULT (0)
#define AUDIO_FOUR_BIT (1)
#define AUDIO_EIGHT_BIT (2)
#define AUDIO_HELIUM (3)
#define AUDIO_XENON (4)
	
	int audioCrappifier;
	int audioPitch;
    
    int renderModesApplied;
	
	void moveToMenuItem( int difference );
    
	void enterMainMenu();
	void exitMainMenu();
    void selectMainMenuItem( int selection );
	void deselectMainMenu();
	
	void sendSequence( Controller* dualshock );
    
	// Options:
		void selectExtraItem( int selection );
		void deselectExtra();
			//void selectRenderMode( int selection );
			//void selectGameplayMode( int selection );
    
		void selectOptionItem( int selection );	// HUD, ACCESSIBILITIES
		void deselectOption();
			void selectHudItem( int selection );
			void deselectHud();
	
			void selectAccessibilityItem( int item );
			void deselectAccessibility();
			//	void setCombatAccessibility( int item, bool onOrOff );
    
public:
    static Menuing* getInstance();
    
    void selectRenderMode( int renderMode, Controller* dualshock );
    void teardownRenderMode( Controller* dualshock );
	void teardownAudio( int whatToTeardown, Controller* dualshock );
    void selectGameplayMode( int gameplayMode, Controller* dualshock );
    void selectHudMode( int hudOption, int option, Controller* dualshock );
	
	
	void setAlternateControls( int item, bool enable, Controller* dualshock );
	void setMagnificationAndAids( int item, bool enable, Controller* dualshock );
	void setNavigationAndTraversal( int item, bool enable, Controller* dualshock );
	void setTtsAndAudioCues( int item, bool enable, Controller* dualshock );
	void setCombatAccessibility( int item, bool enable, Controller* dualshock );
};


class ChaosEngine;

class Modifier {
friend ChaosEngine;
protected:
	Modifier();
	
	Mogi::Math::Time timer;
	Controller* dualshock;
	ChaosEngine* chaosEngine;
	
	void _update();	// ChaosEngine call this, which then calls virtual update();
	
public:
	
	static std::map<std::string, std::function<Modifier*()>> factory;
	static Modifier* build( const std::string& name );
	static std::string getModList( double timePerModifier );
	
	void setDualshock(Controller* dualshock);
	void setChaosEngine(ChaosEngine* chaosEngine);
	
	virtual void begin();	// called when first instantiated
	virtual void update();	// called regularly
	virtual void finish();	// called just
	
	double lifetime();
	
	virtual bool tweak( DeviceEvent* event );
};


class ChaosEngine : public CommandListenerObserver, public ControllerInjector, public Mogi::Thread {
private:
	CommandListener commandListener;
	Controller* dualshock;
	
	Mogi::Math::Time time;
	double timePerModifier;
	DeviceEvent event;
	
	std::list<Modifier*> modifiers;
	std::queue<Modifier*> modifiersThatNeedToStart;
	
	bool pause;
	bool pausePrimer = false;
	
	//pthread_mutex_t chaosMutex;
	
	void newCommand(const std::string& command);	// override from CommandListenerObserver
	bool sniffify(const DeviceEvent* input, DeviceEvent* output); // override from DualShockInjector
	void doAction(); // override from Mogi::Thread
	
public:
	ChaosEngine(Controller* dualshock);
	
	void setInterfaceReply(const std::string& reply);
	void setTimePerModifier(double time);
	void fakePipelinedEvent(DeviceEvent* fakeEvent, Modifier* modifierThatSentTheFakeEvent);
};


#endif
