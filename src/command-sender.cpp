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

//#include <iostream>
//#include <unistd.h>
//#include <sstream>
//#include <json/json.h>

using namespace Chaos;

CommandSender::CommandSender() {
//	printf("Constructing CommandSender()\n");
	const std::string endpoint = "tcp://localhost:5556";
	// initialize the 0MQ context
	//zmqpp::context context;

	// generate a pull socket
	zmqpp::socket_type type = zmqpp::socket_type::request;
	socket = new zmqpp::socket(context, type);

	// bind to the socket
//	socket->bind(endpoint);
	socket->connect(endpoint);
//	printf("Done building CommandSender()\n");
}


CommandSender::~CommandSender() {
}

//void CommandSender::doAction() {
//
//	pause();
//}



bool CommandSender::sendMessage(std::string message) {
//	printf("sendMessage from CommandSender...\n");
	socket->send( message.c_str() );
	
	zmqpp::message msg;
	// decompose the message
	socket->receive(msg);	// Blocking
	msg >> reply;
//	printf("sendMessage from CommandSender DONE!\n");
//	resume();
	return true;
}
