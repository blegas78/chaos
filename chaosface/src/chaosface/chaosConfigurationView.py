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

class ConfigurationView(flx.PyWidget):
	def init(self, model):
		super().init()
		self.model = model
		
		styleLabel = "text-align:right"
		styleField = "background-color:#BBBBBB;text-align:center"
		
		with flx.VSplit(flex=1):
			flx.Label(style="font-weight: bold; text-align:center", text="Bot Information" )
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
			flx.Label(flex=1,style="font-weight: bold; text-align:center", wrap=True, html="Twitch Chat Server Responses:" )
			
			with flx.VBox(minsize=450):
#					flx.Widget(flex=1)
				self.tmiResponse = flx.MultiLineEdit(flex=2, style="text-align:left; background-color:#CCCCCC;", text=self.model.relay.tmiResponse)
#					self.tmiResponse = flx.Label(flex=2, style="text-align:left; background-color:#CCCCCC", wrap=True, text=self.model.relay.tmiResponse)
#					flx.Widget(flex=1)
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


	@flx.action
	def updateTmiResponse(self, text):
		self.tmiResponse.set_text(text)
