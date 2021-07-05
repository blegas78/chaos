
from flexx import flx

class ConfigurationView(flx.PyWidget):
	def init(self, model):
		super().init()
		self.model = model
		
		styleLabel = "text-align:right"
		styleField = "background-color:#BBBBBB;text-align:center"
		
		with flx.VSplit(flex=1):
			flx.Label(style="text-align:center", text="Bot Information" )
			flx.Label(style="text-align:center", wrap=True, html='<a href="https://twitchapps.com/tmi/" target="_blank">Click here to get your bot\'s OAuth Token</a>.  You must be logged in as your bot.' )
			with flx.HBox():
				with flx.VBox(flex=1):
					flx.Widget(flex=1)
				with flx.VBox():
					flx.Label(style=styleLabel, text="Twitch Bot Username:" )
					flx.Label(style=styleLabel, text="Twitch Bot Oauth:" )
					flx.Label(style=styleLabel, text="Your Channel Name:" )
				with flx.VBox(flex=1):
					self.bot_name = flx.LineEdit(style=styleField, placeholder_text=self.model.relay.bot_name)
					self.bot_oauth = flx.LineEdit(style=styleField, placeholder_text=self.model.relay.bot_oauth)
					self.channel_name = flx.LineEdit(style=styleField, placeholder_text=self.model.relay.channel_name[1:])
				with flx.VBox(flex=1):
					flx.Widget(flex=1)
			with flx.HBox():
				flx.Widget(flex=1)
				self.submitButton = flx.Button(flex=0,text="Submit")
				flx.Widget(flex=1)
			with flx.HBox():
				flx.Widget(flex=1)
				self.successLabel = flx.Label(style="text-align:center", text="" )
				flx.Widget(flex=1)
			flx.Label(flex=1,style="text-align:center", wrap=True, html="You may also use your own Twitch account if you do not have a secondary bot account.")
			flx.Label(flex=1,style="text-align:center", wrap=True, html="Things like WizeBot/StreamElements will not work unless you own those accounts." )
			#flx.Widget(flex=1)
			
	@flx.reaction('submitButton.pointer_click')
	def _button_clicked(self, *events):
		ev = events[-1]
		newData = False
		if self.bot_oauth.text != "":
			newData = True
			self.model.relay.set_bot_oauth(self.bot_oauth.text)
		if self.bot_name.text != "":
			newData = True
			self.model.relay.set_bot_name(self.bot_name.text)
		if self.channel_name.text != "":
			newData = True
			self.model.relay.set_channel_name('#' + self.channel_name.text)
		if newData:
			self.successLabel.set_text('Saved!')
			#saveConfig()
			self.model.relay.set_shouldSave(True)
		else:
			self.successLabel.set_text('No Change')
