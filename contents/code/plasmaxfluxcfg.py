# -*- coding: utf-8 -*-
# plasma_flux/contents/code/plasmafluxcfg.py

'''
Provides a switch to start/stop F.lux daemon, and to change location/temperature
Copyright (C) 2011 Diego Cristina

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
'''

from PyQt4.QtGui import QWidget
from cfgdialog import Ui_cfgDialog

class FluxConfig(QWidget, Ui_cfgDialog):

	def __init__(self, parent, values = None):
		QWidget.__init__(self)
		self.parent = parent
		self.setupUi(self)
		if values:
			self.lonBox.setValue(float(values['lon']))
			self.latBox.setValue(float(values['lat']))
			self.tmpBox.setValue(int(values['tmp']))

	def getLongitude(self):
		return self.lonBox.value()
	
	def getLatitude(self):
		return self.latBox.value()
	
	def getTemperature(self):
		return self.tmpBox.value()

