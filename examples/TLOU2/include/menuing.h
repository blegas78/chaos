#ifndef MENUING_H
#define MENUING_H

#include "sequence.h"

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
	OPTIONS_SUBTITLES = 3,
	OPTIONS_DISPLAY = 4,
	OPTIONS_AUDIO = -3,
	OPTIONS_LANGUAGE = -2,
	OPTIONS_ACCESSIBILITY = -1,
};

enum OptionsHUD {
	HUD_RETICLE = 0,
	HUD_WEAPON_CROSS_DISABLE = 1
};

enum OptionsDisplay {
	DISPLAY_BRIGHTNESS = 0,
	DISPLAY_FRAMERATE = 2
};

enum OptionsAudio {
	AUDIO_EFFECTS = 0,
	AUDIO_DIALOGUE = 1,
	AUDIO_MUSIC = 2,
	AUDIO_CINEMATICS = 3,
	AUDIO_ACCESSIBITY_CUES = 4,
	AUDIO_MONO = -2
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


class Menuing {
private:
    Menuing();
    
    Chaos::Sequence sequence;
    
    int mainMenu;
    int extras;
    int render;
    int gameplay;
    int options;
    int hud;
	int display;
	int audio;
	
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
	
	void sendSequence( Chaos::Controller* dualshock );
    
	// Options:
		void selectExtraItem( int selection );
		void deselectExtra();
			//void selectRenderMode( int selection );
			//void selectGameplayMode( int selection );
    
		void selectOptionItem( int selection );	// HUD, ACCESSIBILITIES, DISPLAY, AUDIO
		void deselectOption();
			void selectHudItem( int selection );
			void deselectHud();
	
			void selectDisplayItem( int selection );
			void deselectDisplay();
	
			void selectAudioItem( int selection );
			void deselectAudio();
	
			void selectAccessibilityItem( int item );
			void deselectAccessibility();
			//	void setCombatAccessibility( int item, bool onOrOff );
    
public:
    static Menuing* getInstance();
    
    void selectRenderMode( int renderMode, Chaos::Controller* dualshock );
    void teardownRenderMode( Chaos::Controller* dualshock );
	void teardownAudio( int whatToTeardown, Chaos::Controller* dualshock );
    void selectGameplayMode( int gameplayMode, Chaos::Controller* dualshock );
    void selectHudMode( int hudOption, int option, Chaos::Controller* dualshock );
	
	void setDisplayMode( int displayOption, bool enable, Chaos::Controller* dualshock );
	
	void setVolumeMode( int volumeOption, int amount, Chaos::Controller* dualshock );
	
	
	void setAlternateControls( int item, bool enable, Chaos::Controller* dualshock );
	void setMagnificationAndAids( int item, bool enable, Chaos::Controller* dualshock );
	void setNavigationAndTraversal( int item, bool enable, Chaos::Controller* dualshock );
	void setTtsAndAudioCues( int item, bool enable, Chaos::Controller* dualshock );
	void setCombatAccessibility( int item, bool enable, Chaos::Controller* dualshock );
};


#endif
