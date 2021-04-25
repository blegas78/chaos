#ifndef INTERFACE_H
#define INTERFACE_H

#include <queue>
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

class CommandSender { //}: public Mogi::Thread {
private:
	zmqpp::socket *socket;
	zmqpp::context context;
	
	std::string reply;
	
	//void doAction();
	
public:
	CommandSender();
	~CommandSender();
	
	bool sendMessage(std::string message);
};


class ChaosInterface : public Mogi::Thread {
private:
	CommandListener listener;
	CommandSender sender;
	
	std::queue<std::string> outgoingQueue;
	
	void doAction();
	
public:
	ChaosInterface();
	
	bool sendMessage(std::string message);
	void addObserver( CommandListenerObserver* observer );
};

#endif
