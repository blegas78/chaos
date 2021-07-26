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

class ChaosPausedView(flx.PyWidget):
	def init(self, model):
		super().init()
		self.model = model

		#self.apply_style("background-color:black")

		self.stylePausedDark = "color:white;text-shadow: -1px 0 black, 0 1px black, 1px 0 black, 0 -1px black;text-align:center;font-weight: bold; vertical-align: middle; font-size:50px; min-width:250px; line-height: 2.0; background-color:black"
		self.stylePausedBright = "color:white;text-shadow: -1px 0 black, 0 1px black, 1px 0 black, 0 -1px black;text-align:center;font-weight: bold; vertical-align: middle; font-size:50px; min-width:250px; line-height: 2.0; background-color:white"
		if self.model.relay.paused:
			text="Paused"
		else:
			text="Running"
		self.pausedText = flx.Label(flex=3, style=self.stylePausedDark, text=text )

	@flx.action
	def updatePaused(self, paused):
		if paused:
			self.pausedText.set_text("Paused")
		else:
			self.pausedText.apply_style(self.stylePausedDark)
			self.pausedText.set_text("Running")


	@flx.action
	def updatePausedBrightBackground(self, pausedBright):
		if pausedBright:
			self.pausedText.apply_style(self.stylePausedBright)
		else:
			self.pausedText.apply_style(self.stylePausedDark)
