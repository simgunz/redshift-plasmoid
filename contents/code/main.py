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

#Plasmoid gained by inheritance
class FluxApplet(plasmascript.Applet):

	DEBUG = False

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
			self.lon = float(cfgParser.get('settings', 'lon'))
			self.lat = float(cfgParser.get('settings', 'lat'))
			self.tmp = int(cfgParser.get('settings', 'tmp'))
			cfgFile.close()
		else:
			self.lon = float(0)
			self.lat = float(0)
			self.tmp = int(3400)

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
		
	def toggle(self):
		status = self.checkStatus()
		if status is "Stopped":
			self.startXflux()
		elif status is "Running":
			self.stopXflux()
		else:
			print("Unknown status")
	
	def startXflux(self):
		print("Starting xflux with latitude %f, longitude %f, temperature %d" % (self.lat, self.lon, self.tmp))
		result = commands.getoutput("xflux -l %f -g %f -k %d" % (self.lat, self.lon, self.tmp))
		if self.DEBUG:
			print(result)
		
	def stopXflux(self):
		print("Stopping xflux")
		result = commands.getoutput("kill %s" % self.pid)
		if self.DEBUG:
			print(result)

	#draw method
	def paintInterface(self, painter, option, rect):
		self.updateStatus()
		if self.checkStatus() is "Running":
			self.button.setIcon(self.iconOnline)
		elif self.checkStatus() is "Stopped":
			self.button.setIcon(self.iconOffline)
		else:
			self.button.setIcon(self.iconUnknown)
		self.layout.addItem(self.button, 0, 0)

	#create config interface
	def createConfigurationInterface(self, parent):
		values = {'lon':self.lon,'lat':self.lat,'tmp':self.tmp}
		self.conf = FluxConfig(self,values)
		page = parent.addPage(self.conf,"")
		self.connect(parent, SIGNAL("okClicked()"), self.configAccepted)
		#self.connect(parent, SIGNAL("cancelClicked()"), self.configDenied)
		
	def configAccepted(self):
		self.lon = self.conf.getLongitude()
		self.lat = self.conf.getLatitude()
		self.tmp = self.conf.getTemperature()
		cfgParser = ConfigParser()
		cfgParser.read(self.cfgfile)
		if not cfgParser.has_section('settings'):
			cfgParser.add_section('settings')
		cfgParser.set('settings', 'lat', self.lat)
		cfgParser.set('settings','lon', self.lon)
		cfgParser.set('settings','tmp', self.tmp)
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
	
