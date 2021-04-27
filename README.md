# Twitch Controls Chaos (Controller Edition)

![Chaos](https://github.com/blegas78/chaos/blob/main/docs/images/chaos.jpg?raw=true)


This project was inspired after watching Twitch user DarkViperAU stream "Twich Controls Chaos" using  [ChaosModV](https://github.com/gta-chaos-mod/ChaosModV).  Here are a few differences between that and this version:

- This directly intercepts controller inputs instead of being integrated with game mods
- This can run on *any* game (with supported controllers), including on non-modded console games
- This must run on compatible hardware capable of acting as a USB host and device, like a Raspberry Pi 4
- Due to the hardware constraints, this has been written in C++ and Python for a linux environment


>Note: This code is in desperate need of refactoring.  It is highly functional but also hacky.  I'm focusing on function in the meantime and will work on cleaning up things over time.

Currently this is hard-coded for The Last Of Us Part 2.  Controller-based Chaos works very well for this game due to the massive amount of accessibility options, gameplay modifiers, and render modes.  Eventually I will make this more portable for other games, starting with Uncharted 4/The Lost Legacy.

## What is Chaos?

[Watch a minute long video brief explanation of how users can interact with a controller.](https://www.twitch.tv/blegas78/clip/SmellyDepressedPancakeKappaPride-llz6ldXSKjSJLs9s)
![Twitch Clip](https://github.com/blegas78/chaos/blob/main/docs/images/explain.png?raw=true)

## Supported Controllers
I intend to grow this list over time as needed.

1. Dualshock 4 Generation 2 - Has the lightbar in front - CUH-ZCT2U

>Note: The first generation Dualshock 4 controller can only send control inputs over bluetooth, making it unusable for this project.


## Installation
Currently this setup is only supported on a Raspberry Pi 4 with 32-bit headless Raspian, though other setups may work. Also, currently directories are hardcoded to look for /home/pi/chaos, so make sure to check out this project into your default account home directory.  This may change later depending on project interest.


To install:
1. Set up your Raspberry Pi's network.  I *highly* recommend using ethernet for Chaos, though low-interferenced WiFi should be fine too. [WiFi Instructions here](https://www.raspberrypi.org/documentation/configuration/wireless/wireless-cli.md).

2. Run the following:

		cd ~
		git clone https://github.com/blegas78/chaos.git
		cd chaos
		./install
    
## First Time Setup
Now that everything is installed, power up your Raspberry Pi and follow these steps:

1. Your console will need to be set up to prefer USB communication over bluetooth for your controllers.  

2. On the same network as the Raspberry Pi, navigate your browser to raspberrypi.local (if you renamed your pi's hostname then navigate to <pi hostname>.local instead.).  If that doesn't work you may need to figure out your pi's IP from your router and enter that into your browser instead.

3. Click on the link labeled "BotSetup".  Here you can enter your bot account credentials and your channel name.  You can also use your broadcaster account if you do not have a secondary custom bot account.

4. The other links next to "BotSetup" are overlays that can be added as a *browser* source in streaming applications like OBS.  Open these links to view what they are, or copy and paste into a browser source.  I prefer to have the browser sources in OBS set to refresh when not displayed so that I can refresh them easily.

## Running

1. For each start, You must connect your USB controller to the lower left USB port on your Rasbperry Pi.  This is the USB 2.0 port furthest from the ethernet port and closest to circuit board.  Then use another USB-C to USB-A cable to connect the Raspberry Pi's power to your console.

2. Turn on your console.  On a Playstation, pressing the PS button will turn on the console, which will then power the Raspberry Pi.  During boot, bluetooth connection will let you navigate to your game.  At some point, you'll see a USB connectivity notification, meaning that chaos is active.

3. If OBS was already up, refresh your browser sources.  Overlays should be active.

4. Test your connection to your chat: pull up your chat and try to vote.  If a vote goes through, all is connected and ready to go!

## Usage

Outside of pausing/resuming, there's not much that needs to be done on the streamer's side.  With all systems up and running, your viewers should be able to start voting and cause you all sorts of hell.

#### Pausing
Chaos initializes in a paused state.  This paused state prevents any modifier from running, passing through all controls as normal.  Pausing chaos will let you navigate things like your console's system menu without wreaking true havoc.

  - To resume Chaos, press the *Share* button.  
  - To pause Chaos, press the *Option* button.

>Note: This means that you will actively need to resume chaos whenever you enter your game's pause menu.

#### Game-Specific Usage
Currently TLOU2 is hardcoded as the only supported game.  See TLOU2 specific instrucitons here:

[TLOU2 Usage](docs/TLOU2/README.md)

#### OBS, Streamlabs
I recommend using things like a Chat Box and Color corection filters to make chat more readable and change the colors to match your style:

[Stream Setup](docs/streamSetup.md)


## Design
The lowest level aspect of controller chaos is based on forwarding USB protocols using the Linux raw-gadget kernel module.  For every USB request, Chaos duplicates the request and passes it along.  However, in the case of messages corresponding to controller buttons/joysticks the data is passed to other processes that can meddle with the data.

The core of this Chaos implementation uses something called the ChaosEngine, written in C++ for speed.  Stream overlays, the IRC Twitch bot, and vote tracking are built using Python.

The ChaosEngine listens to the Python voting system using ZMQ.  When a winning modifier comes in, ChaosEngine adds the modifier to list of active modifiers.  After a set amount of time, the ChaosEngine will remove the modifier.

Each modifier can perform a set of actions at each of these transitions.  There can be unique actions performed as the modifier is added, while it's currently active, when it ends, and can also perform asynchronous actions that are controller event-based.  This effectively follows UML-style state machine with entry/do/exit/event actions.

ChaosEngine is designed to allow for flexibility in modifiers using the following principles:

- Sniffing - Can read input being sent along the way
- Interception - Can block pipelined commands
- Modification - Replace message data inlcuding the type, id, and value
- Injection - Can generate and send arbitrary messages without any controller events
- State Sampling - See what is being applied versus what is trying to be sent
- Direct Control - Can send commands directly to the output

Some other included helper utlities for modifiers include sequencers for building macro-like behaviors

#### Example Modifiers

Example modifier for applying inverted mode (Event Modification):

```c++
class Inverted : public Modifier {
public:
	static void regist() { Modifier::factory["Inverted"] = [](){return new Inverted();}; };
	bool tweak( DeviceEvent* event ) {
		if ((event->id == AXIS_RY ) && event->type == TYPE_AXIS) {
			event->value = -event->value;
		}
		return true;
	}
};
```

Example modifier for applying a control sequence once (Sequencing and Interception):
```c++
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
		sequence.addTimeDelay(2500);
		sequence.addButtonHold(BUTTON_R2);
		sequence.addTimeDelay(500);
		sequence.addButtonRelease(BUTTON_R2);
		sequence.send(dualshock);
		busy = false;
	}
	
	bool tweak( DeviceEvent* event ) {
		return !busy;
	}
};
```

Example modifier for adding time-functions to a joystick based on a button press (Interception, Modification, Sampling, Injection):

```c++
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
```
