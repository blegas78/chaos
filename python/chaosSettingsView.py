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
					flx.Label(style=styleLabel, text="Time Per Modifier (s):" )
					flx.Label(style=styleLabel, text="Browser Update Rate (Hz):" )
					flx.Label(style=styleLabel, text="Repeat Mod Probability (%):" )
					flx.Label(style=styleLabel, text="Reset Repeat Mod Memory:" )
#					flx.Label(style=styleLabel, text="Twitch Bot Oauth:" )
#					flx.Label(style=styleLabel, text="Your Channel Name:" )
				with flx.VBox(flex=1):
					self.timePerModifier = flx.LineEdit(style=styleField, text=str(self.model.relay.timePerModifier))
					self.uiRate = flx.LineEdit(style=styleField, text=str(self.model.relay.ui_rate))
					self.softmaxFactor = flx.Slider(min=1, max=100, step=1, value=self.model.relay.softmaxFactor)
					self.resetButton = flx.Button(flex=0,text="Reset")
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
	def _save_button_clicked(self, *events):
		ev = events[-1]
		self.model.relay.set_timePerModifier(float(self.timePerModifier.text))
		self.model.relay.set_ui_rate(float(self.uiRate.text))
		self.model.relay.set_softmaxFactor(self.softmaxFactor.value)
#		self.model.relay.saveConfig()
		self.model.relay.set_shouldSave(True)
		self.successLabel.set_text('Saved!')

	@flx.reaction('resetButton.pointer_click')
	def _reset_button_clicked(self, *events):
		ev = events[-1]
		self.model.relay.set_resetSoftmax(True)
		self.successLabel.set_text('Reset!')
