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
#include "controller-state.h"

#include <cstddef>

using namespace Chaos;

ControllerState::~ControllerState() {
	
}

void* ControllerState::getHackedState() {
	return hackedState;
}

ControllerState* ControllerState::factory(int vendor, int product) {
	if (vendor == 0x054c && product == 0x09cc) {
		return new ControllerStateDualshock;
	} else if (vendor == 0x054c && product == 0x0ce6) {
		return new ControllerStateDualsense;
	} else if (vendor==0x2f24  && product==0x00f8) {	// Magic-S Pro
		return new ControllerStateDualshock;
	}
	
	return NULL;
}
