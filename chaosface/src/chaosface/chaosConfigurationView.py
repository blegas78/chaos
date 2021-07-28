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
from matplotlib.colors import is_color_like

def valueChanged(newtext, origtext):
	return (newtext and newtext.strip() and newtext.strip() != origtext)

def validateColor(newcolor, origcolor):
	if not is_color_like(newcolor):
		newcolor = "white"
	return (valueChanged(newcolor, origcolor), newcolor)

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
				with flx.VBox(flex=1):
					flx.Widget(flex=1)
				with flx.VBox():
					self.textColor = flx.LineEdit(title="Text Color:", placeholder_text=self.model.relay.textColor)
					self.textBold = flx.CheckBox(text="Bold Text")
					self.textBold.set_checked(self.model.relay.textBold)
					self.textItalic = flx.CheckBox(text="Italic Text")
					self.textItalic.set_checked(self.model.relay.textItalics)
					self.outlineText = flx.CheckBox(text="Outline Text")
					self.voteTimePBColor = flx.LineEdit(title="Color of progress bar for voting time:", placeholder_text=self.model.relay.voteTimePBColor)
					self.voteCountPBColor = flx.LineEdit(title="Color of progress bar for vote counts:", placeholder_text=self.model.relay.voteCountPBColor)
					self.modTimePBColor = flx.LineEdit(title="Color of progress bar for mod time:", placeholder_text=self.model.relay.modTimePBColor)
					
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
				self.tmiResponse = flx.MultiLineEdit(flex=2, style="text-align:left; background-color:#CCCCCC;", text=self.model.relay.tmiResponse)

			
	@flx.reaction('submitButton.pointer_click')
	def _button_clicked(self, *events):
		ev = events[-1]
		newData = False
		if valueChange(self.bot_oauth.text, self.model.relay.bot_oauth):
			newData = True
			self.model.relay.set_bot_oauth(self.bot_oauth.text)
		if valueChange(self.bot_name.text, self.model.relay.bot_name):
			newData = True
			self.model.relay.set_bot_name(self.bot_name.text)
		if valueChange(self.channel_name.text, self.model.relay.channel_name):
			newData = True
			self.model.relay.set_channel_name('#' + self.channel_name.text)
		if not is_color_like(self.textColor.text):
			self.textColor.set_text("white")
		if valueChange(self.textColor.text, self.model.relay.textColor):
			newData = True
			self.model.relay.textColor = self.textColor.text.strip()
		if not is_color_like(self.voteTimePBColor.text):
			self.voteTimePBColor.set_text("white")
		if valueChange(self.voteTimePBColor.text, self.model.relay.voteTimePBColor):
			newData = True
			self.model.relay.voteTimePBColor = self.voteTimePBColor.text.strip()
		if not is_color_like(self.voteCountPBColor.text):
			self.voteCountPBColor.set_text("white")
		if valueChange(self.voteCountPBColor.text, self.model.relay.voteCountPBColor):
			newData = True
			self.model.relay.voteCountPBColor = self.voteCountPBColor.text.strip()
		if not is_color_like(self.modTimePBColor.text):
			self.modTimePBColor.set_text("white")
		if valueChange(self.modTimePBColor.text, self.model.relay.modTimePBColor):
			newData = True
			self.model.relay.modTimePBColor = self.modTimePBColor.text.strip()
		if self.textBold.checked != self.model.relay.textBold:
			self.model.relay.textBold = self.textBold.checked
			newData = True
		if self.textItalic.checked != self.model.relay.textItalic:
			self.model.relay.textItalic = self.textItalic.checked
			newData = True
		if self.outlineText.checked != self.model.relay.outlineText:
			self.model.relay.outlineText = self.outlineText.checked
			newData = True

		self.model.relay.textFormat = "color: " + self.model.relay.textColor
		if (self.textBold.checked):
			";".join(self.model.relay.textFormat,"font-weight: bold")
		if (self.textItalic.checked):
			";".join(self.model.relay.textFormat,"font-style: italic")
		if (self.outlineText.checked):
			";".join(self.model.relay.textFormat,"text-shadow: -1px 0 black, 0 1px black, 1px 0 black, 0 -1px black")

		if newData:
			self.successLabel.set_text('Saved!')
			#saveConfig()
			self.model.relay.set_shouldSave(True)
		else:
			self.successLabel.set_text('No Change')


	@flx.action
	def updateTmiResponse(self, text):
		self.tmiResponse.set_text(text)
