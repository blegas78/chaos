
#include "interface.h"

//#include <iostream>
//#include <unistd.h>
//#include <sstream>
//#include <json/json.h>

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
