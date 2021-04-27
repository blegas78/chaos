#ifndef CHAOS_UHID_H
#define CHAOS_UHID_H

#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <linux/uhid.h>

#include <mogi/thread.h>
#include "controller-state.h"
#include "deviceTypes.h"
// This follows: https://github.com/torvalds/linux/blob/master/samples/uhid/uhid-example.c

class ChaosUhid : public Mogi::Thread {
private:
	int fd;
	const char *path;
	struct pollfd pfds[2];
	int ret;
	struct termios state;
	
	ControllerState* stateToPoll;
	
	void doAction();
	
public:
	ChaosUhid(ControllerState* state);
	~ChaosUhid();
	
	void sendUpdate(void* data);
};

#endif
