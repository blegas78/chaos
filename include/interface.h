#ifndef INTERFACE_H
#define INTERFACE_H

#include <mogi/thread.h>
#include <zmqpp/zmqpp.hpp>


class CommandListenerObserver {
public:
	virtual void newCommand( const std::string& command ) = 0;
};


class CommandListener : public Mogi::Thread {
private:
	zmqpp::socket *socket;
	zmqpp::context context;
	
	CommandListenerObserver* observer = NULL;
	
	void doAction();
	
public:
	CommandListener();
	~CommandListener();
	
	std::string reply;
	
	void addObserver( CommandListenerObserver* observer );
	void setReply(const std::string& reply);
};


#endif
