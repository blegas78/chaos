#!/usr/bin/env python3

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
		self.heartbeatPingPong = 0.0
		self.heartbeatTimeout = 5
		self.reconfigured = True
		
		self.verbose = False
		
		self.chat_rate = 0.67
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
	
	def _chatLoop(self):
		
		thread = threading.currentThread()

		connected = False
		heartbeatPingPong = 0.0
#		heartbeatTimeout = 5
		while not getattr(thread, "kill", False):
#			self.bot_oauth = relay.bot_oauth
#			self.bot_name = relay.bot_name
#			self.channel_name = relay.channel_name.lower()
			
			while not connected and not getattr(thread, "kill", False):
				try:
					self.s = socket.socket()
					self.s.settimeout(self.heartbeatTimeout)
#					self.s.connect((config.HOST, config.PORT))
					self.s.connect((self.host, self.port))
					
					logging.info("Attempting to connect to " + self.channel_name + " as " + self.bot_name + " - " + self.bot_oauth)

					self.s.send("PASS {}\r\n".format( self.bot_oauth ).encode("utf-8"))
					self.s.send("NICK {}\r\n".format( self.bot_name ).encode("utf-8"))
					self.s.send("JOIN {}\r\n".format( self.channel_name ).encode("utf-8"))

					self.s.send("CAP REQ :twitch.tv/tags\r\n".encode("utf-8"))
					self.s.send("CAP REQ :twitch.tv/commands\r\n".encode("utf-8"))
					self.s.send("CAP REQ :twitch.tv/membership\r\n".encode("utf-8"))
					
					time.sleep(1)

					connected = True #Socket successfully connected
				except Exception as e:
					logging.info("Error connecting to Twitch, will attempt again...")
					logging.info(str(e))
					time.sleep(5)
					connected = False #Socket failed to connect

			# yeah starting threads here seems fine :(
			#self.chatResponseThread = threading.Thread(target=self.chatResponseLoop)
			#self.chatResponseThread.start()
		
			#lastTime = 0;
			while connected and not getattr(thread, "kill", False):
				#time.sleep(1 / self.chat_rate)
				#if time.time() - lastTime > 15:
				#	lastTime = time.time()
				#	logging.info("Inserting message into reward queue")
				#	rewardQueue.put("Time: " + str(time.time()))
				if self.reconfigured:
					logging.info("New chat information, re-logging in to IRC")
					#saveConfig()
					self.reconfigured = False
					connected = False
					continue
				
				heartbeatPingPong += self.heartbeatTimeout
				if heartbeatPingPong > 5.25*60:	# 5 minutes, 15 seconds
					connected = False
					continue
				
				if self.s._closed:
					loggin.info("Socket is closed :(")
					
				response = ""
				#try:
#					count = self.s.send(b'a')
				#	utility.chat(self.s, "test", self.channel_name);
#					logging.info("Sent " + str(count) + " bytes")
				#except Exception as e:
				#	logging.info(str(e))
					
				try:
					response = self.s.recv(1024)
				except Exception as e:
					err = e.args[0]
					if err == 'timed out':
						#logging.info('recv timed out, retry later')
						#logging.info("response = " + str(response))
						#time.sleep(heartbeatTimeout)
						continue
					else:
						logging.info(str(e))
						connected = False
						continue
						
				response = response.decode("utf-8")
				if self.verbose:
					logging.info(response)
					
				if response == "PING :tmi.twitch.tv\r\n":
					self.s.send("PONG :tmi.twitch.tv\r\n".encode("utf-8"))
					logging.info("Pong")
					heartbeatPingPong = 0.0
					continue
					
				if len(response) <= 0:
					#logging.info("len(response) <= 0")
					continue
						
				heartbeatPingPong = 0.0
					
				try:
					responses = response.split("\r\n")
					notice = irc.responseToDictionary(response)

				except Exception as e:
					logging.info(str(e))
					continue

				#giftMessage = irc.getRewardMessage(notice)

				if "message" in notice.keys():
					notice["message"] = notice["message"].split("\r\n",1)[0]
					logging.info("Chat " + notice["user"] + ":" + notice["message"])
					#q.put( (str(emoteId),img) )
					#chatRelay.create_message(notice["user"], notice["message"])
			
					#chatRelay.create_message(notice["user"], notice["message"])
					
					#q.put( notice )
					self.messageQueue.put( notice )
				
				
				
	def chatResponseLoop(self):
		thread = threading.currentThread()
		cooldownInSeconds = 2
		timeSinceLastResponse = cooldownInSeconds

		while not getattr(thread, "kill", False):
			time.sleep(1 / self.chat_rate)
			timeSinceLastResponse += (1.0 / self.chat_rate)
			if timeSinceLastResponse < cooldownInSeconds:
				if self.qResponse.qsize() > 0:
					while self.qResponse.qsize() > 0:
						self.qResponse.get()
					utility.chat(self.s, "!mods Cooldown at " + str(int(cooldownInSeconds - timeSinceLastResponse)) + " seconds", self.channel_name);
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
							logging.info("response = " + str(result))
							#time.sleep(heartbeatTimeout)
						else:
							logging.info(str(e))
							connected = False
					time.sleep(1 / self.chat_rate)
				
					timeSinceLastResponse = 0


if __name__ == "__main__":

	logging.basicConfig(level="INFO")
	
	chatbot = Chatbot()
	chatbot.verbose = True
	chatbot.start()
	
	chatbot.setChatRate(0.2)
	chatbot.setBotCredentials("See_Bot", "oauth:")

	count = 0
	while True:
		time.sleep(10)
		
		while chatbot.messageQueue.qsize() > 0:
			logging.info("Read: " + str(chatbot.messageQueue.get()))
#		chatbot.sendReply("Counter: " + str(count))
		chatbot.sendReply(str(count))
		count += 1
		if count > 4:
			count = 0
		
