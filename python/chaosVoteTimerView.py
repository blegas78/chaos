
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

