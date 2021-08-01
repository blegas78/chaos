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

class ChaosVoteView(flx.PyWidget):
	def init(self, model):
		super().init()
		self.model = model
		
		self.label = []
		self.progress = []

		textFormat = "color: " + self.model.relay.text_color
		if self.model.relay.text_bold:
			";".join([textFormat,"font-weight: bold"])
		if self.model.relay.text_italic:
			";".join([textFormat,"font-style: italic"])
		if self.model.relay.text_outline:
			";".join([textFormat,"text-shadow: -1px 0 black, 0 1px black, 1px 0 black, 0 -1px black"])
		
		styleModText   = textFormat + ";text-align:left; vertical-align: middle; line-height: 1.5; min-width:250px;"
		styleTitleText = textFormat + ";text-align:center; vertical-align: bottom; line-height: 1.5; min-width:250px;"
		styleVoteProgress = "background:" + self.model.relay.vote_count_color + "; foreground-color:#808080; border-color:#000000; border-radius:5px;" + textFormat

		totalVotes = sum(self.model.relay.votes)
		with flx.VBox(flex=0):
			with flx.HFix(flex=1):
				self.voteLabel = flx.Label(flex=0,style=styleTitleText, text="Total Votes: " + str(int(totalVotes)) )
				self.blankLabel = flx.Label(flex=0,style=styleTitleText, text=" ")
				
			with flx.HFix(flex=1):
				with flx.VFix(flex=1):
					for i in range(len(self.model.relay.votes)):
						if totalVotes > 0:
							self.progress.append( flx.ProgressBar(flex=2, value=self.model.relay.votes[i]/totalVotes, text='{percent}', style=styleVoteProgress) )
						else:
							self.progress.append( flx.ProgressBar(flex=2, value=1.0/len(self.model.relay.votes), text='{percent}', style=styleVoteProgress) )
							
				with flx.VFix(flex=1):
					for i in range(len(self.model.relay.votes)):
						self.label.append( flx.Label(flex=1,style=styleModText, text=str(i+1) + " " + self.model.relay.mods[i]) )
	@flx.action
	def updateMods(self, mods):
		self.label[0].set_text( "1 " + mods[0])
		self.label[1].set_text( "2 " + mods[1])
		self.label[2].set_text( "3 " + mods[2])
	
	@flx.action
	def updateNumbers(self, votes):
		totalVotes = sum(votes) # votes[0] + votes[1] + votes[2]
		
		self.voteLabel.set_text("Total Votes: " + str(int(totalVotes)))
		if totalVotes > 0:
			self.progress[0].set_value(votes[0]/totalVotes)
			self.progress[1].set_value(votes[1]/totalVotes)
			self.progress[2].set_value(votes[2]/totalVotes)
		else:
			self.progress[0].set_value(1.0/len(self.progress))
			self.progress[1].set_value(1.0/len(self.progress))
			self.progress[2].set_value(1.0/len(self.progress))
