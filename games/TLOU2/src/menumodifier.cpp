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
#include "menumodifier.h"

using namespace Chaos;

MenuModifier::tweak(DeviceEvent* event) {
  return !busy;
}

