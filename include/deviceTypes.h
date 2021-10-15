#ifndef DEVICETYPES_H
#define DEVICETYPES_H


namespace Chaos {



 // This is for the PS4:
typedef enum ButtonID {
	BUTTON_X = 0,
	BUTTON_CIRCLE = 1,
	BUTTON_TRIANGLE = 2,
	BUTTON_SQUARE = 3,
	BUTTON_L1 = 4,
	BUTTON_R1 = 5,
	BUTTON_L2 = 6,
	BUTTON_R2 = 7,
	BUTTON_SHARE = 8,
	BUTTON_OPTIONS = 9,
	BUTTON_PS = 10,
	BUTTON_L3 = 11,
	BUTTON_R3 = 12,
	//BUTTON_TOUCHPAD = 200	// This is handled separately as a mouse event
	BUTTON_TOUCHPAD = 13,	// This is handled separately as a mouse event
	BUTTON_TOUCHPAD_ACTIVE = 14,
	BUTTON_TOUCHPAD_ACTIVE_2 = 15
} ButtonID;
 
 typedef enum AxisID {
	 AXIS_LX = 0,
	 AXIS_LY = 1,
	 AXIS_L2 = 2,
	 AXIS_RX = 3,
	 AXIS_RY = 4,
	 AXIS_R2 = 5,
	 AXIS_DX = 6,
	 AXIS_DY = 7,
	 AXIS_ACCX = 8,
	 AXIS_ACCY = 9,
	 AXIS_ACCZ = 10,
	 AXIS_GYRX = 11,
	 AXIS_GYRY = 12,
	 AXIS_GYRZ = 13,
	 AXIS_TOUCHPAD_X = 14,
	 AXIS_TOUCHPAD_Y = 15,
	 AXIS_TOUCHPAD_X_2 = 16,
	 AXIS_TOUCHPAD_Y_2 = 17
 } AxisID;
 
/*
// This is for the PS5:
typedef enum ButtonID {
   BUTTON_X = 1,
   BUTTON_CIRCLE = 2,
   BUTTON_TRIANGLE = 3,
   BUTTON_SQUARE = 0,
   BUTTON_L1 = 4,
   BUTTON_R1 = 5,
   BUTTON_L2 = 6,
   BUTTON_R2 = 7,
   BUTTON_SHARE = 8,
   BUTTON_OPTIONS = 9,
   BUTTON_PS = 12,
   BUTTON_L3 = 10,
   BUTTON_R3 = 11,
   BUTTON_TOUCHPAD = 13	// on the PS5 this is handled here
} ButtonID;

typedef enum AxisID {
	AXIS_LX = 0,
	AXIS_LY = 1,
	AXIS_L2 = 3,
	AXIS_RX = 2,
	AXIS_RY = 5,
	AXIS_R2 = 4,
	AXIS_DX = 6,
	AXIS_DY = 7
} AxisID;
*/
/* hidg0:
 X = 0
 C = 1
 S = 2
 T = 3
 L1 = 4
 R1 = 5
 L2 = 6
 R2 = 7
 share = 8
 options = 9
 L3 = 10
 R3 = 11
 DYup = 12
 DYdown = 13
 ? = 6
 ? = 7
*/
typedef enum ButtonType {
	TYPE_BUTTON = 0,
	TYPE_AXIS = 1
} ButtonType;


// Structures

typedef struct DeviceEvent {
	unsigned int time;
	short int value;
	unsigned char type;
	unsigned char id;
	
	bool operator==(const DeviceEvent &other) const {
		return type == other.type && id == other.id;
	}

	bool operator<(const DeviceEvent &other) const {
		return type < other.type || (type == other.type && id < other.id);
	}
} DeviceEvent;

};

#endif
