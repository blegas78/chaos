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
#include "modifier.h"

#include <cmath>
#include <iostream>
#include <unistd.h>
#include <sstream>
#include <json/json.h>

using namespace Chaos;

Modifier::Modifier() {
  this->timer.initialize();
  me = this;
  pauseTimeAccumulator = 0;
}

void Modifier::setDualshock(Controller* dualshock) {
  this->dualshock = dualshock;
}

void Modifier::setChaosEngine(Engine* chaosEngine) {
  this->chaosEngine = chaosEngine;
}

void Modifier::_update(bool isPaused) {
  timer.update();
  if (isPaused) {
    pauseTimeAccumulator += timer.dTime();
  }
  this->update();
}
 
void Modifier::begin() {	// virtual override
}

void Modifier::update() {	// virtual override
}

void Modifier::finish() {	// virtual override
}

const char* Modifier::description() {	// virtual override
  return "";
}

std::map<std::string, std::function<Modifier*()>> Modifier::factory;

Modifier* Modifier::build( const std::string& name ) {
  Modifier* mod = NULL;
	
  if (factory.count(name) > 0) {
    mod = factory[name]();
  }
	
  return mod;
}

Json::Value modifierToJsonObject( std::string name, Modifier* mod ) {
  Json::Value result;
  result["name"] = name;
  result["desc"] = mod->description();
  return result;
}

std::string Modifier::getModList(double timePerModifier) {
  Json::Value root;
  Json::Value & data = root["mods"];
  int i = 0;
  for (std::map<std::string,std::function<Modifier*()>>::iterator it = factory.begin();
       it != factory.end();
       it++) {
    Modifier* tempMod = it->second();
    data[i++] = modifierToJsonObject( it->first, tempMod);
    delete tempMod;
  }
  Json::StreamWriterBuilder builder;
//	const std::string json_file = Json::writeString(builder, root);
//	std::cout << json_file << std::endl;	
  return Json::writeString(builder, root);
}

double Modifier::lifetime() {
  return timer.runningTime() - pauseTimeAccumulator;
}

bool Modifier::tweak( DeviceEvent* event ) {
  return true;	// by default it is valid and we do nothing
}

void Modifier::setParentModifier(Modifier* parent) {
  this->me = parent;
}

// Test if a button is currently pressed
bool Modifier::is_pressed(unsigned char button) {
  return dualshock->getState(button, TYPE_BUTTON);
}

// Returns false if the event applies to the specified axis. When passed back to
// the chaos engine, this return value is a signal to drop the event so that no other
// modifiers that have yet to be executed will receive this event.
bool Modifier::drop_axis_event(DeviceEvent* event, AxisID axis) {
  return !(event->id == axis && event->type == TYPE_AXIS);
}

bool Modifier::drop_pos_axis_event(DeviceEvent* event, AxisID axis) {
  return !(event->id == axis && event->type == TYPE_AXIS && event->value > 0);
}

bool Modifier::drop_neg_axis_event(DeviceEvent* event, AxisID axis) {
  return !(event->id == axis && event->type == TYPE_AXIS && event->value < 0);
}

bool Modifier::drop_axis_events(DeviceEvent* event, AxisID axis_one, AxisID axis_two) {
  return !(event->type == TYPE_AXIS && (event->id == axis_one || event->id == axis_two));
}

// Force the button's state to off
void Modifier::button_off(DeviceEvent* event, ButtonID button) {
  if (event->id == button && event->type == TYPE_BUTTON) {
    event->value = 0;
  }
}

// Force the button's state to on
void Modifier::button_on(DeviceEvent* event, ButtonID button) {
  if (event->id == button && event->type == TYPE_BUTTON) {
    event->value = 1;
  }
}

void Modifier::axis_off(DeviceEvent* event, AxisID axis) {
  if (event->id == axis && event-type == TYPE_AXIS) {
    event-value = 0;
  }
}

// Force an axis to its minimum value
void Modifier::min_axis(DeviceEvent* event, AxisID axis) {
  if (event->id == axis && event->type == TYPE_AXIS) {
    event->value = JOYSTICK_MIN;
  }
}

// Force an axis to its maximum value
void Modifier::max_axis(DeviceEvent* event, AxisID axis) {
  if (event->id == axis && event->type == TYPE_AXIS) {
    event->value = JOYSTICK_MAX;
  }
}

// Invert the value of an axis
// Why do we add 1 before inverting? -- Poly
void Modifier::invert_axis(DeviceEvent* event, AxisID axis) {
  if ((event->id == axis ) && event->type == TYPE_AXIS) {
    event->value = -((int)event->value+1);
  }
}

// Only allow positive axis values
void Modifier::only_positive(DeviceEvent* event, AxisID axis) {
  if (event->id == axis && event->type == TYPE_AXIS && event->value < 0) {
    event->id = 0
  }
}

// Only allow negative axis values
void Modifier::only_negative(DeviceEvent* event, AxisID axis) {
  if (event->id == axis && event->type == TYPE_AXIS && event->value > 0) {
    event->id = 0
  }
}

// Return the absolute value of the axis
void Modifier::absolute_axis(DeviceEvent* event, AxisID axis) {
  if (event->id == axis && event->type == TYPE_AXIS) {
    event->value = std::abs((int) event->value);
  }
}

// Return the negative of the absolute value of the axis
void Modifier::neg_absolute_axis(DeviceEvent* event, AxisID axis) {
  if (event->id == axis && event->type == TYPE_AXIS) {
    event->value = -(std::abs((int) event->value));
  }
}

