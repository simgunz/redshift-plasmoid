# -*- coding: utf-8 -*-
# plasma_flux/contents/code/main.py

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

#import kde and qt specific stuff
from PyQt4.QtCore import *
from PyQt4.QtGui import *
from PyKDE4.kdeui import *
from PyKDE4.plasma import Plasma
from PyKDE4 import plasmascript
from plasmaxfluxcfg import FluxConfig
from ConfigParser import ConfigParser

#import commands for executing shell commands
import commands
import os
from subprocess import Popen

#Plasmoid gained by inheritance
class FluxApplet(plasmascript.Applet):

	'''f.lux executable'''
	FLUX = "xflux"
	'''Redshift executable'''
	REDSHIFT = "redshift"

	#constructor
	def __init__(self, parent, args=None):
		plasmascript.Applet.__init__(self, parent)
		self.parent = parent

	#done once when initiating
	def init(self):
		self.button = Plasma.IconWidget(self.parent)
		self.iconOffline = KIcon("user-offline")
		self.iconOnline = KIcon("user-online")
		self.iconUnknown = KIcon("user-busy")
		self.pid = None
		
		self.setHasConfigurationInterface(True)
		#set size of Plasmoid
		self.resize(50, 50)
		self.setAspectRatioMode(Plasma.KeepAspectRatio)
		self.setBackgroundHints(Plasma.Applet.DefaultBackground)
		
		self.theme = Plasma.Svg(self)
		self.theme.setImagePath('widgets/background')
		self.layout = QGraphicsGridLayout(self.applet)
		self.layout.setContentsMargins(0,0,0,0)
		self.setMinimumSize(10,10)
		
		#set timer interval in ms (1000=1s)
		self.startTimer(1000)
		self.button.setIcon(self.iconUnknown)
		QObject.connect(self.button, SIGNAL('clicked()'), self.toggle)
		
		self.cfgfile = ".plasma-flux.cfg"
		strFile = os.path.join(os.path.expanduser('~'), self.cfgfile)
		if os.path.exists(strFile):
			cfgParser = ConfigParser()
			cfgFile = open(strFile)
			cfgParser.readfp(cfgFile)
			try:
				self.lon = float(cfgParser.get('settings', 'lon'))
				self.lat = float(cfgParser.get('settings', 'lat'))
				self.nighttmp = int(cfgParser.get('settings', 'nighttmp'))
				self.daytmp = int(cfgParser.get('settings', 'daytmp'))
				self.smooth = bool(cfgParser.get('settings', 'smooth'))
				self.program = str(cfgParser.get('settings', 'program'))
				self.mode = str(cfgParser.get('settings', 'mode'))
				self.gamma = float(cfgParser.get('settings', 'gamma'))
				cfgFile.close()
			except:
				self.defaultOptions()
		else:
			self.defaultOptions()
			
	def defaultOptions(self):
		self.lon = float(0)
		self.lat = float(0)
		self.nighttmp = int(3400)
		self.daytmp = int(6500)
		self.smooth = True
		self.program = "Redshift"
		self.mode = "randr"
		self.gamma = float(0)

	#done when timer is resetted
	def timerEvent(self, event):
		#call draw method
		self.update()

	#parse the status
	def checkStatus(self):
		if self.pid:
			if self.pid.isdigit():
				return "Running"
			else:
				return "Unknown"
		else:
			return "Stopped"

	#get the pid
	def updateStatus(self):
		self.pid = commands.getoutput("pidof xflux")
		if not self.pid:
			self.pid = commands.getoutput("pidof redshift")

	def toggle(self):
		status = self.checkStatus()
		if status == "Stopped":
			if self.program == "f.lux":
				self.startXflux()
			elif self.program == "Redshift":
				self.startRedshift()
		elif status == "Running":
			self.stopProgram()
		else:
			print("Unknown status")
	
	def startXflux(self):
		print("Starting f.lux with latitude %f, longitude %f, temperature %d" % (self.lat, self.lon, self.nighttmp))
		self.pid = Popen("xflux -l %f -g %f -k %d" % (self.lat, self.lon, self.nighttmp), shell=True).pid

	def startRedshift(self):
		print("Starting Redshift with latitude %f, longitude %f, day temperature %d, night temperature %d, gamma ramp %f, smooth transition = %s" % (self.lat, self.lon, self.daytmp, self.nighttmp, self.gamma, ("yes" if self.smooth else "no")))
		self.pid = Popen("redshift -l %f:%f -t %d:%d -g %f -m %s %s" %(self.lat, self.lon, self.daytmp, self.nighttmp, self.gamma, self.mode, ("-r" if not self.smooth else "")), shell=True).pid

	def stopProgram(self):
		print("Stopping")
		Popen("kill %s" % self.pid, shell=True)

	#draw method
	def paintInterface(self, painter, option, rect):
		self.updateStatus()
		if self.checkStatus() == "Running":
			self.button.setIcon(self.iconOnline)
		elif self.checkStatus() == "Stopped":
			self.button.setIcon(self.iconOffline)
		else:
			self.button.setIcon(self.iconUnknown)
		self.layout.addItem(self.button, 0, 0)

	#create config interface
	def createConfigurationInterface(self, parent):
		values = {'lon':self.lon, 'lat':self.lat, 'daytmp':self.daytmp, 'nighttmp':self.nighttmp, 'program':self.program, 'smooth':self.smooth, 'mode':self.mode, 'gamma':self.gamma}
		self.conf = FluxConfig(self,values)
		page = parent.addPage(self.conf,"")
		self.connect(parent, SIGNAL("okClicked()"), self.configAccepted)
		#self.connect(parent, SIGNAL("cancelClicked()"), self.configDenied)
		
	def configAccepted(self):
		self.lon = self.conf.getLongitude()
		self.lat = self.conf.getLatitude()
		self.nighttmp = self.conf.getNightTemperature()
		self.daytmp = self.conf.getDayTemperature()
		self.smooth = self.conf.getSmooth()
		self.program = self.conf.getProgram()
		self.mode = self.conf.getMode()
		self.gamma = self.conf.getGamma()
		cfgParser = ConfigParser()
		cfgParser.read(self.cfgfile)
		if not cfgParser.has_section('settings'):
			cfgParser.add_section('settings')
		cfgParser.set('settings', 'lat', self.lat)
		cfgParser.set('settings','lon', self.lon)
		cfgParser.set('settings','nighttmp', self.nighttmp)
		cfgParser.set('settings','daytmp', self.daytmp)
		cfgParser.set('settings','smooth', self.smooth)
		cfgParser.set('settings','program', self.program)
		cfgParser.set('settings','mode', self.mode)
		cfgParser.set('settings', 'gamma', self.gamma)
		strFile = os.path.join(os.path.expanduser('~'),self.cfgfile)
		cfgFile = open(strFile,"w")
		cfgParser.write(cfgFile)
		cfgFile.close()
	
	#def configDenied(self):
		

	def showConfigurationInterface(self):
		dialog = KPageDialog()
		dialog.setFaceType(KPageDialog.Plain)
		dialog.setButtons(KDialog.ButtonCode(KDialog.Ok | KDialog.Cancel))
		self.createConfigurationInterface(dialog)
		dialog.exec_()

def CreateApplet(parent):
	return FluxApplet(parent)
	
