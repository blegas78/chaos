#!/usr/bin/env python3

import sys # just for argument passing for bare test
import socket
import threading
import logging
import queue
import time

import utility
import irctwitch as irc

#from chaosRelay import chaosRelay as relay

class Chatbot():

	def __init__(self):
		self.connected = False
		self.fullyConnected = False
		self.heartbeatPingPong = 0
		self.pingPongRate = 1.0/8.0
		self.lastPingTime = 0
		self.socketTimeout = 0.1
		self.pingWaitCount = 0
		self.reconfigured = True
		
		self.verbose = False
		
		self.chat_rate = 10
		self.host = "irc.twitch.tv"
		self.port = 6667
		self.bot_oauth = "oauth:"
		self.bot_name = "See_Bot"
		self.channel_name = "#blegas78"
		
		self.messageQueue = queue.Queue()
		self.qResponse = queue.Queue()
			
	def setIrcInfo(self, host, port):
		if self.host != host:
			self.host = host
			self.reconfigured = True
		if self.port != port:
			self.port = port
			self.reconfigured = True

	def setBotCredentials(self, name, oauth):
		if self.bot_oauth != oauth:
			self.bot_oauth = oauth
			self.reconfigured = True
		if self.bot_name != name:
			self.bot_name = name
			self.reconfigured = True
			
	def setChannelName(self, name):
		if self.channel_name != name:
			self.channel_name = name
			self.reconfigured = True
			
	def setChatRate(self, rate):
		self.chat_rate = rate
		
	def sendReply(self, message):
		self.qResponse.put( message )

	def start(self):
		self.chatThread = threading.Thread(target=self._chatLoop)
		self.chatThread.start()
		self.chatResponseThread = threading.Thread(target=self.chatResponseLoop)
		self.chatResponseThread.start()
			
	def stop(self):
		self.chatResponseThread.kill = True
		self.chatResponseThread.join()
		self.chatThread.kill = True
		self.chatThread.join()
		
	def isConnected(self):
		return self.fullyConnected
	
	def _chatLoop(self):
		
		thread = threading.currentThread()

		self.connected = False
#		heartbeatPingPong = 0
#		heartbeatTimeout = 5
		self.reconnectTimeFalloff = 0
		self.reconfigured = False
		
		self.heartbeatPingPong = time.time()
		
		while not getattr(thread, "kill", False):
#			self.bot_oauth = relay.bot_oauth
#			self.bot_name = relay.bot_name
#			self.channel_name = relay.channel_name.lower()
#			self.reconnectTimeFalloff = 0
			self.fullyConnected = False
			while not self.connected and not getattr(thread, "kill", False):
				try:
					logging.info("Chatbot: Waiting " + str(self.reconnectTimeFalloff) + " seconds before reconnection attempt")
					time.sleep(self.reconnectTimeFalloff)
					if self.reconnectTimeFalloff == 0:
						self.reconnectTimeFalloff = 1
					elif self.reconnectTimeFalloff < 5:
						self.reconnectTimeFalloff *= 2
					
					self.s = socket.socket()
					self.s.settimeout(self.socketTimeout)
#					self.s.connect((config.HOST, config.PORT))
					self.s.connect((self.host, self.port))
					
					logging.info("Attempting to connect to " + self.channel_name + " as " + self.bot_name )

					self.s.sendall("PASS {}\r\n".format( self.bot_oauth ).encode("utf-8"))
					self.s.sendall("NICK {}\r\n".format( self.bot_name ).encode("utf-8"))
					self.s.sendall("JOIN {}\r\n".format( self.channel_name ).encode("utf-8"))

					self.s.sendall("CAP REQ :twitch.tv/tags\r\n".encode("utf-8"))
					self.s.sendall("CAP REQ :twitch.tv/commands\r\n".encode("utf-8"))
					self.s.sendall("CAP REQ :twitch.tv/membership\r\n".encode("utf-8"))
					
					

					self.connected = True #Socket successfully connected
				except Exception as e:
					logging.info("Error connecting to Twitch, will attempt again...")
					logging.info(str(e))
					#time.sleep(5)
					self.connected = False #Socket failed to connect

			# yeah starting threads here seems fine :(
			#self.chatResponseThread = threading.Thread(target=self.chatResponseLoop)
			#self.chatResponseThread.start()
		
			#lastTime = 0;
#			self.heartbeatPingPong = 0
			self.heartbeatPingPong = time.time()
			self.waitingForWelcome = True
			
			incoming = b''
			response = ""
			while self.connected and not getattr(thread, "kill", False):
				time.sleep(1 / self.chat_rate)
				#if time.time() - lastTime > 15:
				#	lastTime = time.time()
				#	logging.info("Inserting message into reward queue")
				#	rewardQueue.put("Time: " + str(time.time()))
				if self.reconfigured:
					logging.info("New chat information, re-logging in to IRC")
					#saveConfig()
					self.reconfigured = False
					self.connected = False
					self.reconnectTimeFalloff = 0
					break
				
				
				if (time.time() - self.heartbeatPingPong) >= 30:
					logging.info("Pingpong failure, attempting to reconnect, message timeout = " + str(self.heartbeatPingPong))
					self.connected = False
					continue
				
				if self.s._closed:
					logging.info("Socket is closed :(")
					
#				response = ""
				try:
					if self.pingWaitCount < 3 and (time.time() - self.lastPingTime) > (1.0/self.pingPongRate):
						if self.s.sendall("PING :tmi.twitch.tv\r\n".encode("utf-8")):
							logging.info("Error sending a PING")
						self.lastPingTime = time.time()
						self.pingWaitCount += 1
#						self.heartbeatPingPong += 1
						#utility.chat(self.s, "test", self.channel_name);
						if self.verbose:
							logging.info("Sent PING")
				except Exception as e:
					logging.info("Error sending PING:")
					self.connected = False
					logging.info(str(e))
					continue
					
				try:
#					response = self.s.recv(8192)
					incoming = b''
					incoming = self.s.recv(1024)
#					response = self.s.recv()
				except Exception as e:
					err = e.args[0]
					if err == 'timed out':
						if len(incoming) > 0:
							if self.verbose:
								logging.info('recv timed out, retry later')
								logging.info("incoming = " + str(incoming))
						else:
							continue
						#time.sleep(heartbeatTimeout)
#						continue
					else:
						logging.info('Error when calling s.recv():')
						logging.info(str(e))
						self.connected = False
						continue
						
				response += incoming.decode("utf-8")
				if self.verbose:
#					logging.info("Raw Incoming:" + incoming.decode("utf-8"))
					logging.info("Raw Running Response:" + response)
					
#				if response == "PING :tmi.twitch.tv\r\n":
#					self.s.send("PONG :tmi.twitch.tv\r\n".encode("utf-8"))
#					logging.info("Pong")
#					self.heartbeatPingPong = 0.0
#					continue
					
#				if response == "tmi.twitch.tv PONG tmi.twitch.tv :tmi.twitch.tv\r\n":
##					self.s.send("PONG :tmi.twitch.tv\r\n".encode("utf-8"))
#					logging.info("Pong recieved! Chat still connected!")
#					self.heartbeatPingPong = 0.0
#					continue
					
				if len(response) <= 0:
					#logging.info("len(response) <= 0")
					continue
						
				#heartbeatPingPong = 0.0
					
				try:
					responses = response.split("\r\n")
#					logging.info("# of lines in Responses: " + str(len(responses)))

#					numberOfCompleteLines = len(responses[ len(responses) - 1])
#					if numberOfCompleteLines == 0:
##						logging.info("!!! got ourselves a complete termination")
#						response = ""
#					else:
					# complete lines will be blank:
					response = responses[ len(responses) - 1]
						
#					for response in responses:
					for i in range(len(responses)-1):
#						if self.verbose:
#							logging.info("Length of this response:" + str(len(responses[i])))
						self.handleResponseLine(responses[i])
#							self.response = response
#							break

					
						
				except Exception as e:
					logging.info("Error handling response(s) for:" + response)
					response = responses[ len(responses) - 1]
					logging.info(" - Setting response to:" + response)
					logging.info(str(e))
					continue
						
	def handleResponseLine(self, line):
		#giftMessage = irc.getRewardMessage(notice)
		if len(line) == 0:
#			logging.info("Warning: Length of response line is 0")
			return
		notice = irc.responseToDictionary(line)
		if not notice:
			if self.verbose:
				logging.info("This appears to be an invalid IRC response")
			return True

#		logging.info("Yay a Valid Message!")
		if "message" in notice.keys():
			notice["message"] = notice["message"].split("\r\n",1)[0]
			if self.verbose and notice["command"] == "PRIVMSG":
				logging.info("Chat " + notice["user"] + ":" + notice["message"])
			#q.put( (str(emoteId),img) )
			#chatRelay.create_message(notice["user"], notice["message"])
			
			#chatRelay.create_message(notice["user"], notice["message"])
					
			#q.put( notice )
			
			if notice["command"] == "PONG":
				if self.verbose:
					logging.info("Recieved PONG after pinging server!")
#				self.heartbeatPingPong = 0
				self.heartbeatPingPong = time.time()
				self.pingWaitCount = 0
				return
				
			if notice["command"] == "PING":
				if self.verbose:
					logging.info("Recieved PING, sending PONG...")
				self.s.sendall("PONG :tmi.twitch.tv\r\n".encode("utf-8"))
#				self.heartbeatPingPong = 0
				self.heartbeatPingPong = time.time()
				self.pingWaitCount = 0
				return
					
			# The following is definitely not good to check for a valid conenction:
			if self.waitingForWelcome and notice["user"] == "tmi":
				if notice["message"] == "Welcome, GLHF!":
					if self.verbose:
						logging.info("Connection successful!")
					self.fullyConnected = True
					self.waitingForWelcome = False
					self.reconnectTimeFalloff = 0
				else:
					logging.info("Need to attempt a reconnect")
					self.connected = False
								
			self.messageQueue.put( notice )
	
#	def pingThread(self):
#		self.heartbeatPingPong = 0
#		while not getattr(thread, "kill", False):
#			time.sleep(2)
#
#			if self.heartbeatPingPong >= 2:
#				self.connected = False
#				logging.info("Pingpong failure, attempting to reconnect, message timeout = " + str(self.heartbeatTimeout))
#				continue
			
				
	def chatResponseLoop(self):
		thread = threading.currentThread()
		cooldownInSeconds = 2
		timeSinceLastResponse = cooldownInSeconds

		while not getattr(thread, "kill", False):
			time.sleep(1 / self.chat_rate)
			if not self.connected:
				continue
			timeSinceLastResponse += (1.0 / self.chat_rate)
			if timeSinceLastResponse < cooldownInSeconds:
				if self.qResponse.qsize() > 0:
					while self.qResponse.qsize() > 0:
						self.qResponse.get()
#					utility.chat(self.s, "!mods Cooldown at " + str(int(cooldownInSeconds - timeSinceLastResponse)) + " seconds", self.channel_name);
				continue
				
			else:
				while self.qResponse.qsize() > 0:
					# q.empty(), q.get(), q.qsize()
					notice = self.qResponse.get()
				
					print("Sending response message: " + notice)
					try:
						result = utility.chat(self.s, notice, self.channel_name)
						if result:
							logging.info("chatResponseLoop() send result:" + str(result))
					except Exception as e:
						err = e.args[0]
						if err == 'timed out':
							logging.info('utility.chat() timed out, retry later')
#							logging.info("response = " + str(result))
							#time.sleep(heartbeatTimeout)
						else:
							logging.info(str(e))
							self.connected = False
					time.sleep(1 / self.chat_rate)
				
					timeSinceLastResponse = 0


if __name__ == "__main__":
	logging.basicConfig(level="INFO")
	
	chatbot = Chatbot()
	chatbot.verbose = True
	
	chatbot.setChatRate(10)
	if len(sys.argv) == 3:
		logging.info("Setting custom credentials nick: " + sys.argv[1] + " - auth: " + sys.argv[2])
		chatbot.setBotCredentials(sys.argv[1], sys.argv[2])
	
	chatbot.start()
	count = 0
	timeCount = 0
	while True:
		time.sleep(1)
		timeCount += 1
		if timeCount >= 2:
			timeCount = 0
			while chatbot.messageQueue.qsize() > 0:
				logging.info("Chat client example read: " + str(chatbot.messageQueue.get()))
				
		if not chatbot.isConnected():
			logging.info("Chatbot disconnected!")
##		chatbot.sendReply("Counter: " + str(count))
		#chatbot.sendReply(str(count))
		count += 1
		if count > 4:
			count = 0
		
