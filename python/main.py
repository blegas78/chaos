#!/usr/bin/env python3

#import argparse
import sys
#import fcntl, os
import errno
import time

import threading
import asyncio
#import queue
import logging
import random
import json
from datetime import datetime
import math

import numpy as np
import zmq

#import utility
#import irctwitch as irc

import chatbot
import chaosRelay

import chaosVoteTimerView
import chaosActiveView
import chaosPausedView
import chaosVoteView
import chaosConfigurationView
import chaosSettingsView

import chaoscommunicator
#import sys
#sys.path.append("flexx_gamepad/")
#
#from server import GamepadServer

from flexx import flx, ui
logging.basicConfig(level="INFO")

import pprint


class ChaosModel():
	def __init__(self, chatbot):
		self.chatbot = chatbot
		#self.parser = argparse.ArgumentParser()
		#super(ImageScroller, self).__init__(*args, **kwargs)
		#self.parser.add_argument("-s", "--size", help="Scale of twitch emotes, 1,2, or 3", default="1")
		#self.parser.add_argument("-g", "--reward", help="Whether to display messages or emotes on gift (1 or 0)", default=False)
		
		self.context = zmq.Context()
		self.firstTime = True

		#  Socket to talk to server
		print("Connecting to chaos: c++ based server…")
		self.chaosCommunicator = chaoscommunicator.ChaosCommunicator()
		self.chaosCommunicator.attach(self)
		self.chaosCommunicator.start()
		
		
#		self.socket = self.context.socket(zmq.REQ)
#		self.socket.connect("tcp://localhost:5555")
		
		now = datetime.now()
		currentTime = now.strftime("%Y-%m-%d_%H:%M:%S")
		self.votingLog = open("/home/pi/chaosLogs/votes-" + currentTime + ".log","a", buffering=1)
		#self.modifierDataFile = open("/home/pi/chaosLogs/chaosModifierData.json","a", buffering=1)
		
		self.openDatabase("/home/pi/chaosLogs/chaosModifierData.json")
		
		self.pause = True
#		relay.set_paused(True)
#		self.chaosListener = ChaosListener()
#		self.chaosListener.start()

	def openDatabase(self, modifierDataFile):
		self.modifierDataFile = modifierDataFile
		try:
			with open(modifierDataFile) as json_data_file:
				self.modifierData = json.load(json_data_file)
			logging.info("Successfully loaded modifier data from " + modifierDataFile)
			self.validData = True
		except Exception as e:
			self.modifierData = { "1":{"desc":""}, "2":{"desc":""}, "3":{"desc":""}, "4":{"desc":""}, "5":{"desc":""}, "6":{"desc":""}}
			self.validData = False
			logging.info("Generating modifier data, " + modifierDataFile + " load error")
		

	def saveDatabase(self):
		with open(self.modifierDataFile, 'w') as outfile:
			json.dump(self.modifierData, outfile)

	def start(self):
		self.thread = threading.Thread(target=self.process)
		self.thread.start()

	def process(self):
		#self.args = self.parser.parse_args()
		
		try:
			# Start loop
			print("Press CTRL-C to stop sample")
			self._loop()
		except KeyboardInterrupt:
			print("Exiting\n")
			sys.exit(0)

		return True
		
	def updateCommand(self, message ) -> None:
		#logging.info("Recieved message from C++: " + str(message))
		y = json.loads(message)

		if "mods" in y:
#			logging.info("Got new mods!")
			#oldModLength = len(self.allMods)	# HACK
			#if not oldModLength == len(y["mods"]):
			self.newAllMods = y["mods"]
			self.gotNewMods = True
				#self.resetSoftMax()
		
#		if "voteTime" in y:
#			logging.info("Got new voteTime: " + str(y["voteTime"]))
#			self.timePerVote = y["voteTime"]
			
		if "pause" in y:
			logging.info("Got a pause command of: " + str(y["pause"]))
			self.pause = y["pause"]
#			try:
#				relay.set_paused(y["pause"])
#			except Exception as e:
#				logging.info(e)
				
		
	def applyNewMod(self, mod):
#		print("Winning mod: " + mod)
		#self.socket.send(mod)
		toSend = {}
		toSend["winner"] = mod
		toSend["timePerModifier"] = relay.timePerModifier
#		message = self.chaosCommunicator.sendMessage(mod)
		message = self.chaosCommunicator.sendMessage(json.dumps(toSend))
#		self.socket.send_string(mod)
#		    #  Get the reply.
#		message = self.socket.recv()
		#print(f"Received reply [ {message} ]")
		#self.allMods = message.decode("utf-8").split(',')
	
	def initializeData(self):
		logging.info("Initializing modifierData")
		self.modifierData = {}
		for mod in self.modsFromControllerProgram:
			modName = mod["name"]
			self.modifierData[modName] = {}
			self.modifierData[modName]["desc"] = mod["desc"]
		self.resetSoftMax()
	
	def resetSoftMax(self):
		logging.info("Resetting SoftMax!!!")
#		self.modifierData = {}
		for mod in self.modifierData:
#			modName = mod["name"]
#			self.modifierData[modName] = {}
			self.modifierData[mod]["count"] = 0
#			mod["contribution"] = math.exp(0)
#			self.modifierData[modName]["desc"] = mod["desc"]
			
	def verifySoftmaxIntegrity(self):
		if len(self.allMods) != len(self.modifierData):
			self.initializeData()
			return
		for mod in self.allMods:
			if not mod in self.modifierData:
				self.initializeData()
				return
#		logging.info("verifySoftmaxIntegrity() Passed")
		
		
	def getSoftmaxDivisor(self, data):
		# determine the sum for the softmax divisor:
		softMaxDivisor = 0
		for key in data:
			softMaxDivisor += data[key]["contribution"]
		return softMaxDivisor
		
	def updateSoftmaxProbabilities(self, data):
		for mod in data:
			data[mod]["contribution"] = math.exp(self.modifierData[mod]["count"] * math.log(float(relay.softmaxFactor)/100.0))
		softMaxDivisor = self.getSoftmaxDivisor(data)
		for mod in data:
			data[mod]["p"] = data[mod]["contribution"]/softMaxDivisor
			
	def updateSoftMax(self, newMod):
		if newMod in self.modifierData:
			self.modifierData[newMod]["count"] += 1
#			self.modifierData[newMod]["contribution"] = math.exp(self.modifierData[newMod]["count"] * -2.0)
#			self.modifierData[newMod]["contribution"] = math.exp(self.modifierData[newMod]["count"] * math.log(float(relay.softmaxFactor)/100.0))
				
			# update all probabilities:
#			softMaxDivisor = self.getSoftmaxDivisor(self.modifierData)
#			for key in self.modifierData:
#				self.modifierData[key]["p"] = self.modifierData[key]["contribution"]/softMaxDivisor
			self.updateSoftmaxProbabilities(self.modifierData)
			#print("Update Probs:")
			#for key in self.modifierData:
			#	print( key + ": " + str(100*self.modifierData[key]["p"]))
			self.saveDatabase()
		
	def getNewVotingPool(self):
		# Ignore currently active mods:
		inactiveMods = set(np.setdiff1d(self.allMods, self.activeMods))
				
		self.currentMods = []
		for k in range(self.totalVoteOptions):
			# build a list of contributor for this selection:
			votableTracker = {}
			for mod in inactiveMods:
#				try:
				votableTracker[mod] = self.modifierData[mod]
#				except Exception as e:
#					logging.info(e)
							
			# Calculate the softmax probablities (must be done each time):
#			softMaxDivisor = self.getSoftmaxDivisor(votableTracker)
#			for mod in votableTracker:
#				votableTracker[mod]["p"] = votableTracker[mod]["contribution"]/softMaxDivisor
			self.updateSoftmaxProbabilities(votableTracker)
			#print("Votables:")
			# make a decision:
			theChoice = np.random.uniform(0,1)
			selectionTracker = 0
			#print("Choice: " + str(theChoice))
			for mod in votableTracker:
				selectionTracker += votableTracker[mod]["p"]
				#print("Checking " + str(selectionTracker) + ">" + str(theChoice))
				if selectionTracker > theChoice:
					#print("Using mod: " + mod)
					self.currentMods.append(mod)
					inactiveMods.remove(mod)	#remove this to prevent a repeat
					break
		logging.info("New Voting Round:")
		for mod in self.currentMods:
			logging.info(" - %0.2f%% %s" % (self.modifierData[mod]["p"]*100.0, mod))
		# Reset votes since there is a new voting pool
		self.votes = [0.0] * self.totalVoteOptions
        
        
        
	def selectWinningModifier(self):
		if self.proportionalVoting:
			totalVotes = sum(self.votes)
			if totalVotes < 1:
				for i in range(len(self.votes)):
					self.votes[i] += 1
				totalVotes = sum(self.votes)
			
			theChoice = np.random.uniform(0,totalVotes)
#			print("theChoice = " + str(theChoice))
			index = 0
			accumulator = 0
			for i in range(len(self.votes)):
				index = i
				accumulator += self.votes[i]
				if accumulator >= theChoice:
					break
			
			logging.info("Winning Mod: \"%s\" at %0.2f%% chance" % (self.currentMods[ index ], 100.0 * float(self.votes[index])/totalVotes) )
			return self.currentMods[ index ]
		else:
			return self.currentMods[ self.votes.index(max(self.votes)) ]
			
	def updateChatCredentials(self):
		self.chatbot.setChatRate(relay.chat_rate)
		self.chatbot.setChannelName(relay.channel_name)
		self.chatbot.setBotCredentials(relay.bot_name, relay.bot_oauth)
		self.chatbot.setIrcInfo(relay.ircHost, relay.ircPort)

	
	def _loop(self):
		beginTime = time.time() #0.0
		now = beginTime
#		self.rate = 20.0
#		dTime = 1.0/self.rate
		dTime = 1.0/relay.ui_rate
		priorTime = beginTime - dTime
		
		self.timePerVote = 1.0	# This will be set by the C program
				
		self.totalVoteOptions = 3
		self.votes = [0.0] * self.totalVoteOptions
		self.votedUsers = []
		
		self.activeMods = ["", "", ""]
		self.activeModTimes = [0.0, 0.0, 0.0]
		
		self.gotNewMods = False
		
		self.proportionalVoting = True
		
		# allMods will be set by thte C program
		#self.allMods = ["1", "2", "3", "4", "5", "6"]
		#self.allMods = relay.allMods
#		self.modsFromControllerProgram = [{"name":"1","desc":""},{"name":"2","desc":""},{"name":"3","desc":""},{"name":"4","desc":""},{"name":"5","desc":""},{"name":"6","desc":""}]
#		self.allMods = [x["name"] for x in self.modsFromControllerProgram]
		self.allMods = list(self.modifierData.keys())
#		pprint.pprint([x["name"] for x in self.modsFromControllerProgram])
#		pprint.pprint(self.allMods)
#		self.resetSoftMax()
		self.verifySoftmaxIntegrity()
		
		#self.allMods = [ "No Run/Dodge", "Disable Crouch/Prone", "Drunk Control"]
		self.currentMods = random.sample(self.allMods, k=self.totalVoteOptions)

		
		#relay.newMods(self.currentMods, self.activeMods)
		try:
			relay.set_mods( self.currentMods )
			relay.set_activeMods( self.activeMods )
		except Exception as e:
			logging.info(e)
					
		self.pausedFlashingTimer = 0.0
		self.pausedFlashingToggle = True
		while True:
#			time.sleep(1.0/self.rate)
			time.sleep(1.0/relay.ui_rate)
			priorTime = now
			now = time.time()
			self.voteTime =  now - beginTime
			dTime = now - priorTime
			self.pausedFlashingTimer += dTime
			
			self.updateChatCredentials()
			
			if not relay.paused == self.pause:
				try:
					relay.set_paused(self.pause)
				except Exception as e:
					logging.info(e)
					
			if self.pause:	# hack implementation of pausing
				beginTime += dTime
				dTime = 0
				if self.pausedFlashingTimer > 0.5 and relay.pausedBrightBackground == True:
					try:
						relay.set_pausedBrightBackground(False)
					except Exception as e:
						logging.info(e)
				elif self.pausedFlashingTimer > 1.0 and relay.pausedBrightBackground == False:
					try:
						relay.set_pausedBrightBackground(True)
						self.pausedFlashingTimer = 0.0
					except Exception as e:
						logging.info(e)
					
#				continue
			
			if not self.timePerVote == (relay.timePerModifier/3.0 - 0.5):
				self.timePerVote = relay.timePerModifier/3.0 - 0.5
#				relay.saveConfig()
				newVoteTime={}
				newVoteTime["timePerModifier"] = relay.timePerModifier
				self.chaosCommunicator.sendMessage(json.dumps(newVoteTime))
			
			for i in range(len(self.activeModTimes)):
				self.activeModTimes[i] -= dTime/((self.timePerVote+0.25)*self.totalVoteOptions)
				
			if self.firstTime and self.gotNewMods:
				self.voteTime = self.timePerVote+1
				self.firstTime = False
			
			if relay.resetSoftmax:
				try:
					relay.set_resetSoftmax(False)
					self.resetSoftMax()
				except Exception as e:
					logging.info(e)
					
			
			if self.voteTime >= self.timePerVote:
				beginTime = now
				
				# Send winning choice:
				#newMod = self.currentMods[ self.votes.index(max(self.votes)) ]
				newMod = self.selectWinningModifier()
				self.applyNewMod( newMod )
				if self.gotNewMods:
					self.gotNewMods = False
					self.modsFromControllerProgram = self.newAllMods
					self.allMods = [x["name"] for x in self.modsFromControllerProgram]
#					while True:
#						try:
#							relay.set_allMods(self.allMods)
#							break
#						except Exception as e:
#							logging.info(e)
#					self.resetSoftMax()
					
					self.verifySoftmaxIntegrity()
					self.validData = True
#					self.allMods = list(self.modifierData.keys())
#				self.allMods = relay.allMods

				if not self.validData:
					logging.info("Waiting for controller sync...")
					continue
				
#				# update softmax
#				if newMod in self.modifierData:
#					self.modifierData[newMod]["count"] += 1
##					self.modifierData[newMod]["contribution"] = math.exp(self.modifierData[newMod]["count"] * -2.0)
#					self.modifierData[newMod]["contribution"] = math.exp(self.modifierData[newMod]["count"] * math.log(float(relay.softmaxFactor)/100.0))
#					softMaxDivisor = 0
#					for key in self.modifierData:
#						softMaxDivisor += self.modifierData[key]["contribution"]
#					for key in self.modifierData:
#						self.modifierData[key]["p"] = self.modifierData[key]["contribution"]/softMaxDivisor
#					#print("Update Probs:")
#					#for key in self.modifierData:
#					#	print( key + ": " + str(100*self.modifierData[key]["p"]))
				try:
					self.updateSoftMax(newMod)
				except Exception as e:
					logging.info(e)
					continue
				
				logString = ""
				for j in range(self.totalVoteOptions):
					logString += self.currentMods[j] + "," + str(int(self.votes[j])) + ","
				logString += newMod + "\n"
				self.votingLog.write(logString)
				
				# Update view:
				if not (newMod.isdigit() and 0 < int(newMod) and int(newMod) < 7):
					finishedModIndex = self.activeModTimes.index(min(self.activeModTimes))
					self.activeMods[finishedModIndex] = newMod
					self.activeModTimes[finishedModIndex] = 1.0
				
#				# Select new mods
#				inactiveMods = set(np.setdiff1d(self.allMods, self.activeMods))
#
#				self.currentMods = []
#				for k in range(self.totalVoteOptions):
#					# build a list of contributor for this selection:
#					votableTracker = {}
#					for mod in inactiveMods:
##						try:
#						votableTracker[mod] = self.modifierData[mod]
##						except Exception as e:
##							logging.info(e)
#
#					# Calculate the softmax probablities:
#					softMaxDivisor = 0
#					for mod in votableTracker:
#						softMaxDivisor += votableTracker[mod]["contribution"]
#					for mod in votableTracker:
#						votableTracker[mod]["p"] = votableTracker[mod]["contribution"]/softMaxDivisor
#					#print("Votables:")
#					# make a decision:
#					theChoice = np.random.uniform(0,1)
#					selectionTracker = 0
#					#print("Choice: " + str(theChoice))
#					for mod in votableTracker:
#						selectionTracker += votableTracker[mod]["p"]
#						#print("Checking " + str(selectionTracker) + ">" + str(theChoice))
#						if selectionTracker > theChoice:
#							#print("Using mod: " + mod)
#							self.currentMods.append(mod)
#							inactiveMods.remove(mod)	#remove this to prevent a repeat
#							break
				self.getNewVotingPool()
					
				#self.currentMods = random.sample(inactiveMods, k=self.totalVoteOptions)
				#relay.newMods(self.currentMods, self.activeMods)
				try:
					relay.set_mods( self.currentMods )
					relay.set_activeMods( self.activeMods )
				except Exception as e:
					logging.info(e)
				
#				# Reset votes
#				self.votes = [0.0] * self.totalVoteOptions
				#relay.newVotes(self.votes)
				try:
					relay.set_votes( self.votes )
				except Exception as e:
					logging.info(e)
					
				self.votedUsers = []
			
			
			#relay.newTime( self.voteTime/self.timePerVote, self.activeModTimes )
			timeToSend = self.voteTime/self.timePerVote
			
			try:
				relay.set_voteTime( timeToSend )
				relay.set_modTimes( self.activeModTimes )
			except Exception as e:
				logging.info(e)
				
			#if q.qsize() > 0:
			needToUpdateVotes = False
#			while q.qsize() > 0:
			while chatbot.messageQueue.qsize() > 0:
				# q.empty(), q.get(), q.qsize()
#				notice = q.get();
				notice = chatbot.messageQueue.get();
					
				message = notice["message"]
				if message.isdigit():
					messageAsInt = int(message) - 1
					if messageAsInt >= 0 and messageAsInt < self.totalVoteOptions and not notice["user"] in self.votedUsers:
						self.votedUsers.append(notice["user"])
						self.votes[messageAsInt] += 1
						needToUpdateVotes = True
						continue
								
				command = message.split(" ",1)
				firstWord = command[0]
				if firstWord == "!mods":
#					response = str(str(self.allMods) + " @" + notice["user"]).replace('\'', '').replace('[','').replace(']','')
#					splitMessage = [response[i:i+484] for i in range(0, len(response), 484)]
#					print("number of elements in splitMessage: " + str(len(splitMessage)))
#					for message in splitMessage:
#						#response =	"!mods: " + message
#
#						qResponse.put( "!mods: " + message );
					self.chatbot.sendReply( "!mods: There are currently " + str(len(self.allMods)) + " modifiers!  See them all with descriptions here: https://github.com/blegas78/chaos/tree/main/docs/TLOU2 @" + notice["user"] );
					continue
							
				if firstWord == "!mod":
					if len(command) == 1:
						message = "Usage: !mod <mod name>"
						self.chatbot.sendReply( message );
						continue
					argument = command[1]
					message = "!mod: Unrecognized mod :("
#					for x in self.modsFromControllerProgram:
#						if x["name"].lower() == argument.lower():
					for key in self.modifierData.keys():
						if key.lower() == argument.lower():
							mod = self.modifierData[key]
							if mod["desc"] == "":
								message = "!mod " + key + ": No Description :("
							else:
								message = "!mod " + key + ": " + mod["desc"]
							break
					message += " @" + notice["user"]
					self.chatbot.sendReply( message );
				
			if needToUpdateVotes:
				#relay.newVotes(self.votes)
				try:
					relay.set_votes( self.votes )
								
				except Exception as e:
					logging.info(e)


#  This is how Flexx API states that data passing should work
relay = chaosRelay.ChaosRelay()

class ActiveMods(flx.PyWidget):
	def init(self, relay):
		self.relay = relay
		self.chaosActiveView = chaosActiveView.ChaosActiveView(self)
		
	@relay.reaction('updateModTimes')
	def _updateModTimes(self, *events):
		for ev in events:
			self.chaosActiveView.updateTime(ev.value)
			
	@relay.reaction('updateActiveMods')
	def _updateActiveMods(self, *events):
		for ev in events:
			self.chaosActiveView.updateMods(ev.value)

class StreamerInterfaceLayout(ui.HVLayout):
	def init(self):
		self.set_flex(1)
		self.set_orientation('v')
		self.apply_style('background:#000000;')

class StreamerInterface(flx.PyWidget):

#	CSS = """
#    .flx-Widget {
#        background: #0C0C0C;
#    }
#    """
	def init(self, relay):
		self.relay = relay
		with StreamerInterfaceLayout() as self.s:
			self.voteTimerView = chaosVoteTimerView.ChaosVoteTimerView(self)
			self.chaosActiveView = chaosActiveView.ChaosActiveView(self)
			self.chaosPausedView = chaosPausedView.ChaosPausedView(self)
			
	@relay.reaction('updateVoteTime')
	def _updateVoteTime(self, *events):
		for ev in events:
			self.voteTimerView.updateTime(ev.value)
			
	@relay.reaction('updateModTimes')
	def _updateModTimes(self, *events):
		for ev in events:
			self.chaosActiveView.updateTime(ev.value)
			
	@relay.reaction('updateActiveMods')
	def _updateActiveMods(self, *events):
		for ev in events:
			self.chaosActiveView.updateMods(ev.value)
			
	@relay.reaction('updatePaused')
	def _updatePaused(self, *events):
		for ev in events:
			self.chaosPausedView.updatePaused(ev.value)
			
	@relay.reaction('updatePausedBrightBackground')
	def _updatePausedBrightBackground(self, *events):
		for ev in events:
			self.chaosPausedView.updatePausedBrightBackground(ev.value)
			

class VoteTimer(flx.PyWidget):
	def init(self, relay):
		self.relay = relay
		self.voteTimerView = chaosVoteTimerView.ChaosVoteTimerView(self)
			
	@relay.reaction('updateVoteTime')
	def _updateVoteTime(self, *events):
		for ev in events:
			self.voteTimerView.updateTime(ev.value)
		
class Votes(flx.PyWidget):
	def init(self, relay):
		self.relay = relay
		self.chaosVoteView = chaosVoteView.ChaosVoteView(self)
		
	@relay.reaction('updateMods')
	def _updateMods(self, *events):
		for ev in events:
			self.chaosVoteView.updateMods(ev.value)
			
	@relay.reaction('updateVotes')
	def _updateVotes(self, *events):
		for ev in events:
			self.chaosVoteView.updateNumbers(ev.value)
	
class BotSetup(flx.PyWidget):
	def init(self, relay):
		self.relay = relay
		self.ConfigurationView = chaosConfigurationView.ConfigurationView(self)
			
class Settings(flx.PyWidget):
	def init(self, relay):
		self.relay = relay
		self.settingsView = chaosSettingsView.SettingsView(self)
		

class Interface(flx.PyWidget):

#	CSS = """
#    .flx-Widget {
#        background: #0C0C0C;
#    }
#    """
	def init(self, relay):
		self.relay = relay
		with ui.TabLayout(style="background: #aaa; color: #000; text-align: center; foreground-color:#808080") as self.t:
		#with StreamerInterfaceLayout() as self.s:
			self.interface = StreamerInterface(self.relay, title='Interface')
			self.settings = Settings(self.relay, title='Settings')
			self.botSetup = BotSetup(self.relay, title='Bot Setup')
#		self.t.apply_style("background: #222; color: #fff; text-align: center;")


	def dispose(self):
		super().dispose()
		# ... do clean up or notify other parts of the app
		self.interface.dispose()
		self.settings.dispose()
		self.botSetup.dispose()

		
def startFlexx():
	flx.App(ActiveMods, relay).serve()
#	flx.App(StreamerInterface, relay).serve()
	flx.App(VoteTimer, relay).serve()
	flx.App(Votes, relay).serve()
#	flx.App(BotSetup, relay).serve()
#	flx.App(Settings, relay).serve()
	flx.App(Interface, relay).serve()
#	flx.App(GamepadServer).serve()
	
	flx.create_server(host='0.0.0.0', port=relay.uiPort, loop=asyncio.new_event_loop())
	flx.start()

if __name__ == "__main__":
	# for chat
	chatbot = chatbot.Chatbot()
	chatbot.setIrcInfo(relay.ircHost, relay.ircPort)
	
	chatbot.start()
	
#	chaosListener = ChaosListener()
#	chaosListener.start()
	
	#startFlexx()
	flexxThread = threading.Thread(target=startFlexx)
	flexxThread.start()

	# Voting model:
	chaosModel = ChaosModel(chatbot)
#	chaosModel.start()
	if (not chaosModel.process()):
			chaosModel.print_help()

#	while True:
#		chatbot.setChatRate(relay.chat_rate)
#		chatbot.setChannelName(relay.channel_name)
#		chatbot.setBotCredentials(relay.bot_name, relay.bot_oauth)
#		chatbot.setIrcInfo(relay.chaosConfig["host"], relay.chaosConfig["port"])
#		time.sleep(0.5)
			
	logging.info("Stopping threads...")
	
	flx.stop()
	flexxThread.join()
	
	chatThread.kill = True
	chatThread.join()

