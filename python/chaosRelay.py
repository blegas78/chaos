
from flexx import flx
import json

def get_attribute(data, attribute, default_value):
	result = data.get(attribute)
	if result:
		return result
	data[attribute] = default_value
	return default_value

class ChaosRelay(flx.Component):

#	def __init__(self):
	chaosConfigFile="/home/pi/chaosConfig.json"
	with open(chaosConfigFile) as json_data_file:
		chaosConfig = json.load(json_data_file)
			
	voteTime = flx.FloatProp(0.5, settable=True)
	modTimes = flx.ListProp([0.0,0.0,0.0], settable=True)
	votes = flx.ListProp([0,0,0], settable=True)
	mods = flx.ListProp(["","",""], settable=True)
	activeMods = flx.ListProp(["","",""], settable=True)
#	allMods = flx.ListProp(["1", "2", "3", "4", "5", "6"], settable=True)
	timePerModifier = flx.FloatProp(get_attribute(chaosConfig, "modifier_time", 180.0), settable=True)
	paused = flx.BoolProp(True, settable=True)
	pausedBrightBackground = flx.BoolProp(True, settable=True)
	
	bot_name = flx.StringProp(get_attribute(chaosConfig, "bot_name", "see_bot"), settable=True)
	bot_oauth = flx.StringProp(get_attribute(chaosConfig, "bot_oauth", "oauth:abcdefghijklmnopqrstuvwxyz1234"), settable=True)
	channel_name = flx.StringProp(get_attribute(chaosConfig, "channel_name", "blegas78"), settable=True)
	chat_rate = flx.FloatProp(get_attribute(chaosConfig, "chat-rate", 0.67), settable=True)
	
	ui_rate = flx.FloatProp(get_attribute(chaosConfig, "ui_rate", 20.0), settable=True)
	
	shouldSave = flx.BoolProp(False, settable=True)
	
#	def __init__(self):
#		self.chaosConfigFile="/home/pi/chaosConfig.json"
#		with open(chaosConfigFile) as json_data_file:
#			self.chaosConfig = json.load(json_data_file)

	def saveConfig(self):
		with open(self.chaosConfigFile, 'w') as outfile:
			json.dump(self.chaosConfig, outfile)
	
	@flx.reaction('voteTime')
	def on_voteTime(self, *events):
		for ev in events:
			self.updateVoteTime(ev.new_value)
			
	@flx.reaction('modTimes')
	def on_modTimes(self, *events):
		for ev in events:
			self.updateModTimes(ev.new_value)
			
	@flx.reaction('votes')
	def on_votes(self, *events):
		for ev in events:
			self.updateVotes(ev.new_value)
			
	@flx.reaction('mods')
	def on_mods(self, *events):
		for ev in events:
			self.updateMods(ev.new_value)
			
	@flx.reaction('activeMods')
	def on_activeMods(self, *events):
		for ev in events:
			self.updateActiveMods(ev.new_value)
			
#	@flx.reaction('allMods')
#	def on_allMods(self, *events):
#		for ev in events:
#			chaosConfig["allMods"] = ev.new_value
##			logging.info("Relay set allMods")
			
	@flx.reaction('timePerModifier')
	def on_timePerModifier(self, *events):
		for ev in events:
			self.chaosConfig["modifier_time"]  = ev.new_value
#			self.updateTimePerModifier(ev.new_value)
			
	@flx.reaction('paused')
	def on_paused(self, *events):
		for ev in events:
			self.updatePaused(ev.new_value)
			
	@flx.reaction('pausedBrightBackground')
	def on_pausedBrightBackground(self, *events):
		for ev in events:
			self.updatePausedBrightBackground(ev.new_value)
			
	@flx.reaction('bot_name')
	def on_bot_name(self, *events):
		for ev in events:
			self.chaosConfig["bot_name"] = ev.new_value
			
	@flx.reaction('bot_oauth')
	def on_bot_oauth(self, *events):
		for ev in events:
			self.chaosConfig["bot_oauth"] = ev.new_value
			
	@flx.reaction('channel_name')
	def on_channel_name(self, *events):
		for ev in events:
			self.chaosConfig["channel_name"] = ev.new_value
			
	@flx.reaction('ui_rate')
	def on_ui_rate(self, *events):
		for ev in events:
#			print("new ui_Rate: " + str(ev.new_value))
			self.chaosConfig["ui_rate"]  = ev.new_value
			
	@flx.reaction('shouldSave')
	def on_shouldSave(self, *events):
		for ev in events:
			if ev.new_value:
				print("Saving config to: " + self.chaosConfigFile)
				self.saveConfig()
				self.set_shouldSave(False)
			
	""" Global object to relay paint events to all participants.
	"""
		
	@flx.emitter
	def updateVoteTime(self, value):
		return dict(value=value)
		
	@flx.emitter
	def updateModTimes(self, value):
		return dict(value=value)
		
	@flx.emitter
	def updateVotes(self, value):
		return dict(value=value)
		
	@flx.emitter
	def updateMods(self, value):
		return dict(value=value)
		
	@flx.emitter
	def updateActiveMods(self, value):
		return dict(value=value)
		
	@flx.emitter
	def updatePaused(self, value):
		return dict(value=value)
		
	@flx.emitter
	def updatePausedBrightBackground(self, value):
		return dict(value=value)
        
# Create global relay object, shared by all connections
#relay = Relay()
#chatRelay = ChatRelay()