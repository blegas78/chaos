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
#include "interface.h"

#include <iostream>
#include <unistd.h>
#include <string>
#include <json/json.h>

using namespace Chaos;

CommandListener::CommandListener() {
	const std::string endpoint = "tcp://*:5555";

	// initialize the 0MQ context
	//zmqpp::context context;

	// generate a pull socket
	zmqpp::socket_type type = zmqpp::socket_type::reply;
	socket = new zmqpp::socket(context, type);

	// bind to the socket
	socket->bind(endpoint);
}

CommandListener::~CommandListener() {
	if(socket != NULL) {
		delete socket;
	}
}

void CommandListener::addObserver( CommandListenerObserver* observer ) {
	this->observer = observer;
}

void CommandListener::doAction() {
	//std::cout << "in doAction()" << std::endl;
	// receive the message
	zmqpp::message message;
	// decompose the message
	socket->receive(message);	// Blocking
	std::string text;
	message >> text;

	//Do some 'work'
	//std::this_thread::sleep_for(std::chrono::seconds(1));
	//std::cout << "Received: " << text << std::endl;
	socket->send( reply.c_str() );
	
	if (observer != NULL) {
		observer->newCommand(text);
	}
}

void CommandListener::setReply(const std::string& reply) {
	this->reply = reply;
}
