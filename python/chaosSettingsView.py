
from flexx import flx

class SettingsView(flx.PyWidget):
	def init(self, model):
		super().init()
		self.model = model
		
		styleLabel = "text-align:right"
		styleField = "background-color:#BBBBBB;text-align:center"
		
		with flx.VSplit(flex=1):
			flx.Label(style="text-align:center", text="Settings" )
			with flx.HBox():
				with flx.VBox(flex=1):
					flx.Widget(flex=1)
				with flx.VBox():
					flx.Label(style=styleLabel, text="Time Per Modifier:" )
					flx.Label(style=styleLabel, text="Browser Update Rate:" )
#					flx.Label(style=styleLabel, text="Twitch Bot Oauth:" )
#					flx.Label(style=styleLabel, text="Your Channel Name:" )
				with flx.VBox(flex=1):
					self.timePerModifier = flx.LineEdit(style=styleField, text=str(self.model.relay.timePerModifier))
					self.uiRate = flx.LineEdit(style=styleField, text=str(self.model.relay.ui_rate))
#					self.bot_oauth = flx.LineEdit(style=styleField, placeholder_text=self.model.relay.bot_oauth)
#					self.channel_name = flx.LineEdit(style=styleField, placeholder_text=self.model.relay.channel_name[1:])
				with flx.VBox(flex=1):
					flx.Widget(flex=1)
			with flx.HBox():
				flx.Widget(flex=1)
				self.saveButton = flx.Button(flex=0,text="Save")
				flx.Widget(flex=1)
			with flx.HBox():
				flx.Widget(flex=1)
				self.successLabel = flx.Label(style="text-align:center", text="" )
				flx.Widget(flex=1)
				
	@flx.reaction('saveButton.pointer_click')
	def _button_clicked(self, *events):
		ev = events[-1]
		self.model.relay.set_timePerModifier(float(self.timePerModifier.text))
		self.model.relay.set_ui_rate(float(self.uiRate.text))
#		self.model.relay.saveConfig()
		self.model.relay.set_shouldSave(True)
		self.successLabel.set_text('Saved!')
