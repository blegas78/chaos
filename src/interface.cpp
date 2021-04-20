
#include "interface.h"

#include <iostream>
#include <unistd.h>
#include <string>

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
