
from flexx import flx

class ChaosConnectionView(flx.PyWidget):
	def init(self, model):
		super().init()
		self.model = model

		#self.apply_style("background-color:black")

		self.styleDisconnectedDark = "color:white;text-shadow: -1px 0 black, 0 1px black, 1px 0 black, 0 -1px black;text-align:center;font-weight: bold; vertical-align: middle; font-size:25px; min-width:250px; line-height: 1.1; background-color:black"
		self.styleDisconnectedBright = "color:white;text-shadow: -1px 0 black, 0 1px black, 1px 0 black, 0 -1px black;text-align:center;font-weight: bold; vertical-align: middle; font-size:25px; min-width:250px; line-height: 1.1; background-color:red"
		if self.model.relay.connected:
			text="Chat Connected"
		else:
			text="Chat Disconnected"
		self.connectedText = flx.Label(flex=0, style=self.styleDisconnectedDark, text=text )

	@flx.action
	def updateConnected(self, connected):
		if connected:
			self.connectedText.set_text("Chat Connected")
			self.connectedText.apply_style(self.styleDisconnectedDark)
		else:
			self.connectedText.set_text("Chat Disconnected")


	@flx.action
	def updateConnectedBrightBackground(self,  connectedBright):
		if connectedBright:
			self.connectedText.apply_style(self.styleDisconnectedBright)
		else:
			self.connectedText.apply_style(self.styleDisconnectedDark)
