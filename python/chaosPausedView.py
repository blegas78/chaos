
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
