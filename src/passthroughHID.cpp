#include <cstdio>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
	   #include <sys/stat.h>
	   #include <fcntl.h>
#include <cstring>
#include <cmath>

//#include "chaos.h"

typedef struct
{
  uint8_t  reportId = 0x01;                                 // Report ID = 0x01 (1)
													 // Collection: CA:GamePad
  uint8_t  GD_GamePadX;                              // Usage 0x00010030: X, Value = 0 to 255
  uint8_t  GD_GamePadY;                              // Usage 0x00010031: Y, Value = 0 to 255
  uint8_t  GD_GamePadZ;                              // Usage 0x00010032: Z, Value = 0 to 255
  uint8_t  GD_GamePadRz;                             // Usage 0x00010035: Rz, Value = 0 to 255
  uint8_t  GD_GamePadHatSwitch : 4;                  // Usage 0x00010039: Hat switch, Value = 0 to 7, Physical = Value x 45 in degrees
  uint8_t  BTN_GamePadButton1 : 1;                   // Usage 0x00090001: Button 1 Primary/trigger, Value = 0 to 1, Physical = Value x 315
  uint8_t  BTN_GamePadButton2 : 1;                   // Usage 0x00090002: Button 2 Secondary, Value = 0 to 1, Physical = Value x 315
  uint8_t  BTN_GamePadButton3 : 1;                   // Usage 0x00090003: Button 3 Tertiary, Value = 0 to 1, Physical = Value x 315
  uint8_t  BTN_GamePadButton4 : 1;                   // Usage 0x00090004: Button 4, Value = 0 to 1, Physical = Value x 315
  uint8_t  BTN_GamePadButton5 : 1;                   // Usage 0x00090005: Button 5, Value = 0 to 1, Physical = Value x 315
  uint8_t  BTN_GamePadButton6 : 1;                   // Usage 0x00090006: Button 6, Value = 0 to 1, Physical = Value x 315
  uint8_t  BTN_GamePadButton7 : 1;                   // Usage 0x00090007: Button 7, Value = 0 to 1, Physical = Value x 315
  uint8_t  BTN_GamePadButton8 : 1;                   // Usage 0x00090008: Button 8, Value = 0 to 1, Physical = Value x 315
  uint8_t  BTN_GamePadButton9 : 1;                   // Usage 0x00090009: Button 9, Value = 0 to 1, Physical = Value x 315
  uint8_t  BTN_GamePadButton10 : 1;                  // Usage 0x0009000A: Button 10, Value = 0 to 1, Physical = Value x 315
  uint8_t  BTN_GamePadButton11 : 1;                  // Usage 0x0009000B: Button 11, Value = 0 to 1, Physical = Value x 315
  uint8_t  BTN_GamePadButton12 : 1;                  // Usage 0x0009000C: Button 12, Value = 0 to 1, Physical = Value x 315
  uint8_t  BTN_GamePadButton13 : 1;                  // Usage 0x0009000D: Button 13, Value = 0 to 1, Physical = Value x 315
  uint8_t  BTN_GamePadButton14 : 1;                  // Usage 0x0009000E: Button 14, Value = 0 to 1, Physical = Value x 315
  uint8_t  VEN_GamePad0020 : 6;                      // Usage 0xFF000020: , Value = 0 to 127, Physical = Value x 315 / 127
  uint8_t  GD_GamePadRx;                             // Usage 0x00010033: Rx, Value = 0 to 255, Physical = Value x 21 / 17
  uint8_t  GD_GamePadRy;                             // Usage 0x00010034: Ry, Value = 0 to 255, Physical = Value x 21 / 17
  uint8_t  VEN_GamePad0021[54];                      // Usage 0xFF000021: , Value = 0 to 255, Physical = Value x 21 / 17
} Ds4InputReport;

typedef struct SpoofState {
	//unsigned short buttons = 0;
	uint8_t  BTN_GamePadButton1 : 1;                   // Usage 0x00090001: Button 1 Primary/trigger, Value = 0 to 1, Physical = Value x 315
	uint8_t  BTN_GamePadButton2 : 1;                   // Usage 0x00090002: Button 2 Secondary, Value = 0 to 1, Physical = Value x 315
	uint8_t  BTN_GamePadButton3 : 1;                   // Usage 0x00090003: Button 3 Tertiary, Value = 0 to 1, Physical = Value x 315
	uint8_t  BTN_GamePadButton4 : 1;                   // Usage 0x00090004: Button 4, Value = 0 to 1, Physical = Value x 315
	uint8_t  BTN_GamePadButton5 : 1;                   // Usage 0x00090005: Button 5, Value = 0 to 1, Physical = Value x 315
	uint8_t  BTN_GamePadButton6 : 1;                   // Usage 0x00090006: Button 6, Value = 0 to 1, Physical = Value x 315
	uint8_t  BTN_GamePadButton7 : 1;                   // Usage 0x00090007: Button 7, Value = 0 to 1, Physical = Value x 315
	uint8_t  BTN_GamePadButton8 : 1;                   // Usage 0x00090008: Button 8, Value = 0 to 1, Physical = Value x 315
	uint8_t  BTN_GamePadButton9 : 1;                   // Usage 0x00090009: Button 9, Value = 0 to 1, Physical = Value x 315
	uint8_t  BTN_GamePadButton10 : 1;                  // Usage 0x0009000A: Button 10, Value = 0 to 1, Physical = Value x 315
	uint8_t  BTN_GamePadButton11 : 1;                  // Usage 0x0009000B: Button 11, Value = 0 to 1, Physical = Value x 315
	uint8_t  BTN_GamePadButton12 : 1;                  // Usage 0x0009000C: Button 12, Value = 0 to 1, Physical = Value x 315
	uint8_t  BTN_GamePadButton13 : 1;                  // Usage 0x0009000D: Button 13, Value = 0 to 1, Physical = Value x 315
	uint8_t  BTN_GamePadButton14 : 1;                  // Usage 0x0009000E: Button 14, Value = 0 to 1, Physical = Value x 315
	uint8_t  BTN_GamePadButton15 : 1;	// dummy
	uint8_t  BTN_GamePadButton16 : 1;	// dummy
//	unsigned char dpad = 0;
	uint8_t  GD_GamePadHatSwitch;
	uint8_t  GD_GamePadX;
	uint8_t  GD_GamePadY;
	uint8_t  GD_GamePadZ;
	uint8_t  GD_GamePadRz;
	unsigned char setMeToZero = 0;
} inputReport01_t;

int main(int argc, char** argv) {
	std::cout << "Welcome to Chaos" << std::endl;
	
//	FILE* hidgf = fopen("/dev/hidg0", "a+");
//	FILE* hidrawf = fopen("/dev/hidraw0", "a+");
//	setbuf(hidgf, 0);
//	setbuf(hidrawf,0);
	
	int hidgfd = open( "/dev/hidg0", O_RDWR | O_NONBLOCK);
	int hidrawfd = open( "/dev/hidraw0", O_RDWR | O_NONBLOCK);

	ssize_t hidgn = 0;
	ssize_t hidrawn = 0;
	
	Ds4InputReport ds4Input;
	inputReport01_t fakeJoy;
	
	char buffer[64];
	while(1) {
//		hidgn = fread(buffer, sizeof(char), 64, hidgf);
		hidgn = read( hidgfd, buffer, 64);
		
		
		//std::cerr << "'";
		//fwrite(buffer, sizeof(char), hidgn, hidrawf);
		write(hidrawfd, buffer, hidgn);
		//fflush(hidrawf);
		//std::cerr << ".";
		
//		hidrawn = fread(buffer, sizeof(char), 64, hidrawf);
		hidrawn = read( hidrawfd, buffer, 64);
		
		if (hidrawn == 64 && buffer[0] == 0x01) {
			memcpy(&ds4Input, buffer, 64);
		
			fakeJoy.BTN_GamePadButton1 = ds4Input.BTN_GamePadButton1;
			fakeJoy.BTN_GamePadButton2 = ds4Input.BTN_GamePadButton2;
			fakeJoy.BTN_GamePadButton3 = ds4Input.BTN_GamePadButton3;
			fakeJoy.BTN_GamePadButton4 = ds4Input.BTN_GamePadButton4;
			fakeJoy.BTN_GamePadButton5 = ds4Input.BTN_GamePadButton5;
			fakeJoy.BTN_GamePadButton6 = ds4Input.BTN_GamePadButton6;
			fakeJoy.BTN_GamePadButton7 = ds4Input.BTN_GamePadButton7;
			fakeJoy.BTN_GamePadButton8 = ds4Input.BTN_GamePadButton8;
			fakeJoy.BTN_GamePadButton9 = ds4Input.BTN_GamePadButton9;
			fakeJoy.BTN_GamePadButton10 = ds4Input.BTN_GamePadButton10;
			fakeJoy.BTN_GamePadButton11 = ds4Input.BTN_GamePadButton11;
			fakeJoy.BTN_GamePadButton12 = ds4Input.BTN_GamePadButton12;
			fakeJoy.BTN_GamePadButton13 = ds4Input.BTN_GamePadButton13;
			fakeJoy.BTN_GamePadButton14 = ds4Input.BTN_GamePadButton14;
			
			fakeJoy.GD_GamePadHatSwitch = ds4Input.GD_GamePadHatSwitch;
			
			fakeJoy.GD_GamePadX = ds4Input.GD_GamePadX;
			fakeJoy.GD_GamePadY = ds4Input.GD_GamePadY;
			fakeJoy.GD_GamePadZ = ds4Input.GD_GamePadZ;
			fakeJoy.GD_GamePadRz = ds4Input.GD_GamePadRz;
			
//		fwrite(buffer, sizeof(char), hidrawn, hidgf);
			
			
//			write(hidgfd, buffer, hidrawn);
			write(hidgfd, &fakeJoy, sizeof(fakeJoy));

			
		}
//		fflush(hidgf);
		
	}

	return 0;
}
