/*----------------------------------------------------------------------------
* This file is part of Twitch Controls Chaos (TCC).
* Copyright 2021 blegas78
*
* TCC is free software: you can redistribute it and/or modify it under the
* terms of the GNU General Public License as published by the Free Software
* Foundation, either version 3 of the License, or (at your option) any later
* version.
*
* TCC is distributed in the hope that it will be useful, but WITHOUT ANY
* WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
* FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
* details.
*
* You should have received a copy of the GNU General Public License along
* with TCC.  If not, see <https://www.gnu.org/licenses/>.
*---------------------------------------------------------------------------*/
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

namespace Chaos {



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

};

#endif
