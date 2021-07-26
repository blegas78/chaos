#!/usr/bin/env python3
#-----------------------------------------------------------------------------
# This file is part of Twitch Controls Chaos (TCC).
# Copyright 2021 blegas78
#
# TCC is free software: you can redistribute it and/or modify it under the
# terms of the GNU General Public License as published by the Free Software
# Foundation, either version 3 of the License, or (at your option) any later
# version.
#
# TCC is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
# details.
#
# You should have received a copy of the GNU General Public License along
# with TCC.  If not, see <https://www.gnu.org/licenses/>.
#-----------------------------------------------------------------------------
# follows patterns from here: https://refactoring.guru/design-patterns/observer/python/example

from __future__ import annotations
from abc import ABC, abstractmethod
from random import randrange
from typing import List

import threading
import json
import zmq

import time

class ChaosSubject(ABC):
	"""
	The Subject interface declares a set of methods for managing subscribers.
	"""

	@abstractmethod
	def attach(self, observer: ChaosListenerObserver) -> None:
		"""
		Attach an observer to the subject.
		"""
		pass

	@abstractmethod
	def detach(self, observer: ChaosListenerObserver) -> None:
		"""
		Detach an observer from the subject.
		"""
		pass

	@abstractmethod
	def notify(self, message) -> None:
		"""
		Notify all observers about an event.
		"""
		pass


class ChaosCommunicator(ChaosSubject):
	_state: int = None
	_observers: List[ChaosListenerObserver] = []
	
	def attach(self, observer: Observer) -> None:
		print("Subject: Attached an observer.")
		self._observers.append(observer)

	def detach(self, observer: Observer) -> None:
		self._observers.remove(observer)
		
	"""
	The subscription management methods.
	"""

	def notify(self, message) -> None:
		"""
		Trigger an update in each subscriber.
		"""

		#print("Subject: Notifying observers...")
		for observer in self._observers:
			observer.updateCommand(message)
	
	def start(self):
		#self.q = queue.Queue()
		self.context = zmq.Context()
		self.socketListen = self.context.socket(zmq.REP)
		self.socketListen.bind("tcp://*:5556")
				
		print("Connecting to chaos: c++ based server…")
		self.socketTalk = self.context.socket(zmq.REQ)
		self.socketTalk.connect("tcp://localhost:5555")
				
		self.keepRunning = True
		self.thread = threading.Thread(target=self.listenLoop)
		self.thread.start()
		
	def stop(self):
		self.keepRunning = False
		
	def listenLoop(self):
		while self.keepRunning:
			#  Wait for next request from client
			message = self.socketListen.recv()
			#logging.info("ChaosListener() - Received request: " + message.decode("utf-8"))

			#self.q.put(message)
			self.notify(message.decode("utf-8"))

			#  Send reply back to client
			self.socketListen.send(b"Pong")
	
	def sendMessage(self, message):
		self.socketTalk.send_string(message)
		return self.socketTalk.recv()
		

class ChaosListenerObserver(ABC):
	"""
	An Observer for listening to ChaosListener events form the C++ source.
	"""
	
	@abstractmethod
	def updateCommand(self, message ) -> None:
		"""
		Corresponding message form ZMQ
		"""
		pass

class TestObserver(ChaosListenerObserver):
	#def updateCommand(self, chaosSubject: ChaosSubject, message ) -> None:
	def updateCommand(self, message ) -> None:
		print("Message: " + str(message))


if __name__ == "__main__":
	subject = ChaosCommunicator()
	subject.start()
	
	testObserver = TestObserver()
	subject.attach(testObserver)
	
	while True:
		subject.sendMessage("Hello Chaos!")
		time.sleep(1)
	

