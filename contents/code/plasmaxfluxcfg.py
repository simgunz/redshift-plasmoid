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
from PyQt4 import QtCore

class FluxConfig(QWidget, Ui_cfgDialog):

	def __init__(self, parent, values = None):
		QWidget.__init__(self)
		self.parent = parent
		self.setupUi(self)
		if values:
			self.lonBox.setValue(float(values['lon']))
			self.latBox.setValue(float(values['lat']))
			self.nightTmpBox.setValue(int(values['nighttmp']))
			self.dayTmpBox.setValue(int(values['daytmp']))
			self.smoothCheckBox.setChecked(bool(values['smooth']))
			index = self.programComboBox.findText(str(values['program']))
			self.programComboBox.setCurrentIndex(index if not (index < 0) else 0)
			index = self.modeComboBox.findText(str(values['mode']))
			self.modeComboBox.setCurrentIndex(index if not (index < 0) else 0)
			rgb = values['gamma'].split(':')
			if len(rgb) < 3:
				rgb = (float(rgb[0]), float(rgb[0]), float(rgb[0]))
			self.gammaBoxR.setValue(float(rgb[0]))
			self.gammaBoxG.setValue(float(rgb[1]))
			self.gammaBoxB.setValue(float(rgb[2]))
			self.autolaunchCheckBox.setChecked(bool(values['auto']))
		self.toggleProgram()
		QtCore.QObject.connect(self.programComboBox, QtCore.SIGNAL("currentIndexChanged(int)"), self.toggleProgram)


	def getLongitude(self):
		return self.lonBox.value()

	def getLatitude(self):
		return self.latBox.value()

	def getNightTemperature(self):
		return self.nightTmpBox.value()

	def getDayTemperature(self):
		return self.dayTmpBox.value()

	def getSmooth(self):
		return self.smoothCheckBox.isChecked()

	def getProgram(self):
		return str(self.programComboBox.currentText())

	def getMode(self):
		return str(self.modeComboBox.currentText())
		
	def getGamma(self):
		return str("%.2f:%.2f:%.2f" % (self.gammaBoxR.value(), self.gammaBoxG.value(), self.gammaBoxB.value()))
		
	def getAutoLaunch(self):
		return self.autolaunchCheckBox.isChecked()

	def toggleProgram(self):
		enabled = bool(self.getProgram() == 'Redshift')
		self.smoothCheckBox.setEnabled(enabled)
		self.dayTmpLabel.setEnabled(enabled)
		self.dayTmpBox.setEnabled(enabled)
		self.modeLabel.setEnabled(enabled)
		self.modeComboBox.setEnabled(enabled)
		self.gammaLabel.setEnabled(enabled)
		self.gammaBoxR.setEnabled(enabled)
		self.gammaBoxG.setEnabled(enabled)
		self.gammaBoxB.setEnabled(enabled)
