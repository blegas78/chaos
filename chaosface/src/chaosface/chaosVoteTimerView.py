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

from flexx import flx

#class VoteTimer(flx.PyWidget):
#	def init(self, relay):
#		self.voteTimerView = ChaosVoteTimerView(self)
#		self.relay = relay
#
#	@self.relay.reaction('updateVoteTime')
#	def _updateVoteTime(self, *events):
#		for ev in events:
#			self.voteTimerView.updateTime(ev.value)
		
class ChaosVoteTimerView(flx.PyWidget):
	def init(self, model):
		super().init()
		self.model = model
		
		styleTime = " background-color:#808080; foreground-color:#808080; color:#000000; border-color:#000000; border-radius:5px"
		self.progressTime = flx.ProgressBar(flex=1, value=self.model.relay.voteTime, text='', style=styleTime)
		
	@flx.action
	def updateTime(self, voteTime):
		self.progressTime.set_value(voteTime)

