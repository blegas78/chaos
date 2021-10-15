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
import copy
from datetime import datetime
import math

import numpy as np
import zmq

#import utility
#import irctwitch as irc

import chatbot
import chaosRelay

#import chaosVoteTimerView
#import chaosActiveView
#import chaosPausedView
#import chaosVoteView
import chaosConfigurationView
import chaosSettingsView
#import chaosConnectionView

import chaoscommunicator
#import sys
#sys.path.append("flexx_gamepad/")
#
#from server import GamepadServer

from flexx import flx, ui
logging.basicConfig(level="INFO")

import pprint


class PlaysModel():
	def __init__(self, chatbot):
		self.chatbot = chatbot
		
		self.context = zmq.Context()
		self.firstTime = True

		#  Socket to talk to server
		logging.info("Connecting to chaos: c++ based server…")
		self.chaosCommunicator = chaoscommunicator.ChaosCommunicator()
		self.chaosCommunicator.attach(self)
		self.chaosCommunicator.start()
		
		self.now = datetime.now()
		currentTime = self.now.strftime("%Y-%m-%d_%H:%M:%S")
		self.votingLog = open("/home/pi/chaosLogs/votes-" + currentTime + ".log","a", buffering=1)
		#self.modifierDataFile = open("/home/pi/chaosLogs/chaosModifierData.json","a", buffering=1)
		
		self.openDatabase("/home/pi/chaosLogs/playsModifierData.json")
#		self.openDatabase("/home/pi/chaosLogs/chaosModifierData.json")
		
		self.pause = True
#		relay.set_paused(True)

		self.tmiChatText = ""

		self.runningVotes = [] #queue.Queue()
		self.voteHistogram = []
		self.needToBuildHistogram = True
		
		self.currentActions = []

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
				
		
	def applyNewMod(self, mod):
#		print("Winning mod: " + mod)
		toSend = {}
		toSend["winner"] = mod
		toSend["timePerModifier"] = relay.timePerModifier
		message = self.chaosCommunicator.sendMessage(json.dumps(toSend))
	
	def initializeData(self):
		logging.info("Initializing modifierData")
		self.modifierData = {}
		for mod in self.modsFromControllerProgram:
			modName = mod["name"]
			self.modifierData[modName] = {}
			self.modifierData[modName]["desc"] = mod["desc"]
			lifespan = mod["lifespan"]
			if lifespan < 0:
				lifespan = 3.0
			self.modifierData[modName]["lifespan"] = lifespan
		self.resetSoftMax()
	
	def resetSoftMax(self):
		logging.info("Resetting SoftMax!")
		for mod in self.modifierData:
			self.modifierData[mod]["count"] = 0
#			self.modifierData[mod]["lifespan"] = 3.0
			
	def verifyDatabaseIntegrity(self):
		if len(self.allMods) != len(self.modifierData):
			self.initializeData()
			return
		for mod in self.allMods:
			if not mod in self.modifierData:
				self.initializeData()
				return
#		logging.info("verifySoftmaxIntegrity() Passed")
		
		
#	def getSoftmaxDivisor(self, data):
#		# determine the sum for the softmax divisor:
#		softMaxDivisor = 0
#		for key in data:
#			softMaxDivisor += data[key]["contribution"]
#		return softMaxDivisor
#
#	def updateSoftmaxProbabilities(self, data):
#		for mod in data:
#			data[mod]["contribution"] = math.exp(self.modifierData[mod]["count"] * math.log(float(relay.softmaxFactor)/100.0))
#		softMaxDivisor = self.getSoftmaxDivisor(data)
#		for mod in data:
#			data[mod]["p"] = data[mod]["contribution"]/softMaxDivisor
#
#	def updateSoftMax(self, newMod):
#		if newMod in self.modifierData:
#			self.modifierData[newMod]["count"] += 1
#
#			# update all probabilities:
#			self.updateSoftmaxProbabilities(self.modifierData)
#			self.saveDatabase()
		
#	def getNewVotingPool(self):
#		# Ignore currently active mods:
#		inactiveMods = set(np.setdiff1d(self.allMods, self.activeMods))
#
#		self.currentMods = []
#		for k in range(self.totalVoteOptions):
#			# build a list of contributor for this selection:
#			votableTracker = {}
#			for mod in inactiveMods:
##				try:
#				votableTracker[mod] = copy.deepcopy(self.modifierData[mod])
##				except Exception as e:
##					logging.info(e)
#
#			# Calculate the softmax probablities (must be done each time):
##			softMaxDivisor = self.getSoftmaxDivisor(votableTracker)
##			for mod in votableTracker:
##				votableTracker[mod]["p"] = votableTracker[mod]["contribution"]/softMaxDivisor
##			self.updateSoftmaxProbabilities(votableTracker)
#			#print("Votables:")
#			# make a decision:
#			theChoice = np.random.uniform(0,1)
#			selectionTracker = 0
#			#print("Choice: " + str(theChoice))
#			for mod in votableTracker:
#				selectionTracker += votableTracker[mod]["p"]
#				#print("Checking " + str(selectionTracker) + ">" + str(theChoice))
#				if selectionTracker > theChoice:
#					#print("Using mod: " + mod)
#					self.currentMods.append(mod)
#					inactiveMods.remove(mod)	#remove this to prevent a repeat
#					break
##		logging.info("New Voting Round:")
##		for mod in self.currentMods:
##			if "p" in self.modifierData[mod]:
##				logging.info(" - %0.2f%% %s" % (self.modifierData[mod]["p"]*100.0, mod))
##			else:
##				logging.info(" - %0.2f%% %s" % (0, mod))
#		# Reset votes since there is a new voting pool
#		self.votes = [0.0] * self.totalVoteOptions
        
        
        
#	def selectWinningModifier(self):
#
#		if self.proportionalVoting:
#			totalVotes = sum(self.votes)
#			if totalVotes < 1:
#				for i in range(len(self.votes)):
#					self.votes[i] += 1
#				totalVotes = sum(self.votes)
#
#			theChoice = np.random.uniform(0,totalVotes)
##			print("theChoice = " + str(theChoice))
#			index = 0
#			accumulator = 0
#			for i in range(len(self.votes)):
#				index = i
#				accumulator += self.votes[i]
#				if accumulator >= theChoice:
#					break
#
#			logging.info("Winning Mod: \"%s\" at %0.2f%% chance" % (self.currentMods[ index ], 100.0 * float(self.votes[index])/totalVotes) )
#			return self.currentMods[ index ]
#		else:
#			return self.currentMods[ self.votes.index(max(self.votes)) ]
			
	def updateChatCredentials(self):
		self.chatbot.setChatRate(relay.chat_rate)
		self.chatbot.setChannelName(relay.channel_name)
		self.chatbot.setBotCredentials(relay.bot_name, relay.bot_oauth)
		self.chatbot.setIrcInfo(relay.ircHost, relay.ircPort)
	
	def flashPause(self):
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
				
	def flashDisconnected(self):
		if self.disconnectedFlashingTimer > 0.5 and relay.connectedBrightBackground == True:
			try:
				relay.set_connectedBrightBackground(False)
			except Exception as e:
				logging.info(e)
		elif self.disconnectedFlashingTimer > 1.0 and relay.connectedBrightBackground == False:
			try:
				relay.set_connectedBrightBackground(True)
				self.disconnectedFlashingTimer = 0.0
			except Exception as e:
				logging.info(e)
	
	
	def _loop(self):
		beginTime = time.time() #0.0
		self.now = beginTime
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
		self.allMods = list(self.modifierData.keys())
		self.verifyDatabaseIntegrity()
		
		#self.allMods = [ "No Run/Dodge", "Disable Crouch/Prone", "Drunk Control"]
		self.currentMods = random.sample(self.allMods, k=self.totalVoteOptions)

		self.pausedFlashingTimer = 0.0
		self.pausedFlashingToggle = True

		self.disconnectedFlashingTimer = 0.0
		self.disconnectedFlashingToggle = True
		while True:
#			time.sleep(1.0/self.rate)
			time.sleep(1.0/relay.ui_rate)
			priorTime = self.now
			self.now = time.time()
			dTime = self.now - priorTime
			self.pausedFlashingTimer += dTime
			self.disconnectedFlashingTimer += dTime
			
			self.updateChatCredentials()
			
			if not relay.paused == self.pause:
				try:
					relay.set_paused(self.pause)
				except Exception as e:
					logging.info(e)
					
			if self.pause:	# hack implementation of pausing
				beginTime += dTime
				dTime = 0
				self.flashPause()
					

			if not relay.connected == self.chatbot.isConnected():
				try:
					relay.set_connected(self.chatbot.isConnected())
				except Exception as e:
					logging.info(e)
					
			if not self.chatbot.isConnected():	# hack implementation of pausing
#				beginTime += dTime
#				dTime = 0
				self.flashDisconnected()
							
			self.voteTime = self.now - beginTime
				
			
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
					
			# If the time is up for the voting round, send the winnder:
			if self.voteTime >= self.timePerVote:
				beginTime = self.now	# reset timer for next round
				
				# Send winning choice:
				#newMod = self.selectWinningModifier()
				
				# Send to chaos controller::
				#self.applyNewMod( newMod )
				if self.gotNewMods:
					self.gotNewMods = False
					self.modsFromControllerProgram = self.newAllMods
					self.allMods = [x["name"] for x in self.modsFromControllerProgram]
					
					self.verifyDatabaseIntegrity()
					self.validData = True

				if not self.validData:
					logging.info("Waiting for controller sync...")
					continue
				
#				try:
#					self.updateSoftMax(newMod)
#				except Exception as e:
#					logging.info(e)
#					continue
#
#				logString = ""
#				for j in range(self.totalVoteOptions):
#					logString += self.currentMods[j] + "," + str(int(self.votes[j])) + ","
#				logString += newMod + "\n"
#				self.votingLog.write(logString)
				
#				# Update view:
#				if not (newMod.isdigit() and 0 < int(newMod) and int(newMod) < 7):
#					finishedModIndex = self.activeModTimes.index(min(self.activeModTimes))
#					self.activeMods[finishedModIndex] = newMod
#					self.activeModTimes[finishedModIndex] = 1.0
				
#				self.getNewVotingPool()
					
				self.votedUsers = []
			
			self.timeToSend = self.voteTime/self.timePerVote
			
			# Get new voting data:
			self.pruneOldVotes()
			self.checkMessages()
			
			# build data:
			self.buildHistogram()
			
			# action handling:
			self.removeOldActions()
			newMod = self.selectNewAction()
			
			if newMod:
				self.applyNewMod( newMod )
			
			
			self.updateUI()
	
			
		
		
	def pruneOldVotes(self):
#		currentTime = time.time()
		oldLength = len(self.runningVotes)
		self.runningVotes = [x for x in self.runningVotes if (self.now - x[1]) < self.modifierData[x[0]]["lifespan"]]
		if not len(self.runningVotes) == oldLength:
#			logging.info("runningVotes was pruned:" + str(self.runningVotes))
			self.needToBuildHistogram = True
	
	def buildHistogram(self):
		if self.needToBuildHistogram:
			self.needToBuildHistogram = False
			self.voteHistogram = dict.fromkeys(self.modifierData, 0)
			for vote in self.runningVotes:
				self.voteHistogram[vote[0]] += 1
			logging.info("Histogram:" + str(self.voteHistogram))
			
	def removeOldActions(self):
		for action in self.currentActions:
			if not "start-time" in self.modifierData[action]:
				self.modifierData[action]["start-time"] = self.now + self.modifierData[action]["lifespan"]
			if self.now - self.modifierData[action]["start-time"] > self.modifierData[action]["lifespan"]:
				self.currentActions.remove(action)
				logging.info("Action '" + action + "' is finished")
				return
				
	def selectNewAction(self):
		if len(self.currentActions) >= 4:
			return None
#		inactiveActions = [x for x in self.voteHistogram.items() if not x.keys()[0] in self.currentActions]
		inactiveActions = {key: value for key, value in self.voteHistogram.items() if not key in self.currentActions}
#		logging.info("inactiveActions:" + str(inactiveActions))
#		winningAction, totalVotes = max(inactiveActions, key=lambda item: item[1])
		winningAction = max(inactiveActions, key=inactiveActions.get)
		if inactiveActions[winningAction] > 0:
			logging.info("winningAction:" + winningAction)
			self.currentActions.append(winningAction)
			self.modifierData[winningAction]["start-time"] = self.now
			return winningAction
		return None
	
	
	def updateUI(self):
		try:
			relay.set_mods( self.currentMods )
		except Exception as e:
			logging.info(e)
		try:
			relay.set_activeMods( self.activeMods )
		except Exception as e:
			logging.info(e)
		try:
			relay.set_votes( self.votes )
		except Exception as e:
			logging.info(e)
		try:
			relay.set_voteTime( self.timeToSend )
		except Exception as e:
			logging.info(e)
		try:
			relay.set_modTimes( self.activeModTimes )
		except Exception as e:
			logging.info(e)
		try:
			relay.set_tmiResponse(self.tmiChatText)
		except Exception as e:
			logging.info(e)
	
	def checkVote(self, message):
		if message in self.modifierData:
#			logging.info("A valid action was provided!")
#			logging.info("Current time: " + str(time.time()))
			self.runningVotes.append( (message, time.time()) )
#			logging.info("New runningVotes:" + str(self.runningVotes))
			self.needToBuildHistogram = True
			return True
		return False
		
	
	def checkMessages(self):
		#if q.qsize() > 0:
		needToUpdateVotes = False
#		while q.qsize() > 0:
		while self.chatbot.messageQueue.qsize() > 0:
			# q.empty(), q.get(), q.qsize()
#			notice = q.get();
			notice = self.chatbot.messageQueue.get();
			
			if not "message" in notice:
				continue
				
			logging.info(notice["user"] + ": " + notice["message"])
			
			if notice["user"] == "tmi":
				self.tmiChatText += "tmi: " + notice["message"] + "\r\n"
				#logging.info( self.tmiChatText )
				continue
					
#			message = notice["message"]
#			if message.isdigit():
#				messageAsInt = int(message) - 1
#				if messageAsInt >= 0 and messageAsInt < self.totalVoteOptions and not notice["user"] in self.votedUsers:
#					self.votedUsers.append(notice["user"])
#					self.votes[messageAsInt] += 1
#					needToUpdateVotes = True
#					continue
			
				
			message = notice["message"]
			command = message.split(" ",1)
			
			if len(command) == 1 and self.checkVote(command[0].lower()):
				continue
			
			firstWord = command[0]
			if firstWord == "!mods":
				self.chatbot.sendReply( "!mods: There are currently " + str(len(self.allMods)) + " modifiers!  See them all with descriptions here: https://github.com/blegas78/chaos/tree/main/docs/TLOU2 @" + notice["user"] );
				continue
				
			if firstWord == "!actions":
				response = "Actions: "
				for key in self.modifierData.keys():
					response += key + ", "
				self.chatbot.sendReply( response );
				continue
							
			if firstWord == "!mod":
				if len(command) == 1:
					message = "Usage: !mod <mod name>"
					self.chatbot.sendReply( message );
					continue
#				argument = command[1]
				argument = (''.join(c for c in command[1] if c.isalnum())).lower()
				message = "!mod: Unrecognized mod :("
				
				for key in self.modifierData.keys():
#					if key.lower() == argument.lower():
					keyReduced = (''.join(c for c in key if c.isalnum())).lower()
					if keyReduced == argument:
						mod = self.modifierData[key]
						if mod["desc"] == "":
							message = "!mod " + key + ": No Description :("
						else:
							message = "!mod " + key + ": " + mod["desc"]
						break
				message += " @" + notice["user"]
				self.chatbot.sendReply( message );
				

#  This is how Flexx API states that data passing should work
relay = chaosRelay.ChaosRelay()
relay.openConfig("/home/pi/playsConfig.json")

class BotSetup(flx.PyWidget):
	def init(self, relay):
		self.relay = relay
		self.configurationView = chaosConfigurationView.ConfigurationView(self)
			
	@relay.reaction('updateTmiResponse')
	def _updateTmiResponse(self, *events):
		for ev in events:
			self.configurationView.updateTmiResponse(ev.value)
			
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
#			self.interface = StreamerInterface(self.relay, title='Interface')
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
	flx.App(Interface, relay).serve()
	
	flx.create_server(host='0.0.0.0', port=relay.uiPort, loop=asyncio.new_event_loop())
	flx.start()

if __name__ == "__main__":
	# for chat
	chatbot = chatbot.Chatbot()
	chatbot.setIrcInfo(relay.ircHost, relay.ircPort)
	chatbot.start()
	
	#startFlexx()
	flexxThread = threading.Thread(target=startFlexx)
	flexxThread.start()

	# Voting model:
	playsModel = PlaysModel(chatbot)
#	chaosModel.start()
	if (not playsModel.process()):
			playsModel.print_help()
			
	logging.info("Stopping threads...")
	
#	flx.stop()
#	flexxThread.join()
	
	chatThread.kill = True
	chatThread.join()

