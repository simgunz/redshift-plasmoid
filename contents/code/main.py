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
from PyKDE4.kdecore import KSystemTimeZones
from PyKDE4 import plasmascript
from plasmaxfluxcfg import FluxConfig
from ConfigParser import ConfigParser

#import commands for executing shell commands
import commands
import os
from subprocess import Popen, PIPE

'''Theme'''
THEME = 'widgets/background'
'''Icons'''
ICON_PLASMOID = 'help-hint'
ICON_STOPPED = 'user-offline'
ICON_RUNNING = 'user-online'
ICON_UNKNOWN = 'user-busy'
'''f.lux executable'''
FLUX = 'xflux'
'''Redshift executable'''
REDSHIFT = 'redshift'
'''Refresh (check if running) rate, in ms'''
REFRESH = 1000
'''Default night temp'''
DEFAULT_NIGHT = 4000
'''Default day temp'''
DEFAULT_DAY = 6500
'''Default Redshift mode'''
DEFAULT_MODE = 'randr'
'''Default program'''
DEFAULT_PROGRAM = 'Redshift'
'''Get latitude and longitude from KDE and set them as default'''
DEFAULT_LATITUDE = KSystemTimeZones.local().latitude()
DEFAULT_LONGITUDE = KSystemTimeZones.local().longitude()

#Plasmoid gained by inheritance
class FluxApplet(plasmascript.Applet):

	#constructor
	def __init__(self, parent, args=None):
		plasmascript.Applet.__init__(self, parent)
		self.parent = parent

	#done once when initiating
	def init(self):
		self.button = Plasma.IconWidget(self.parent)
		self.iconStopped = KIcon(ICON_STOPPED)
		self.iconRunning = KIcon(ICON_RUNNING)
		self.iconUnknown = KIcon(ICON_UNKNOWN)
		self.pid = None
		self.subp = None
		self.waiting = False

		self.setHasConfigurationInterface(True)
		#set size of Plasmoid
		self.resize(50, 50)
		self.setAspectRatioMode(Plasma.KeepAspectRatio)
		self.setBackgroundHints(Plasma.Applet.DefaultBackground)

		self.theme = Plasma.Svg(self)
		self.theme.setImagePath(THEME)
		self.layout = QGraphicsGridLayout(self.applet)
		self.layout.setContentsMargins(3,3,3,3)
		self.setMinimumSize(10,10)

		#set timer interval
		self.timer = self.startTimer(REFRESH)
		self.button.setIcon(self.iconUnknown)
		QObject.connect(self.button, SIGNAL('clicked()'), self.toggle)
		self.appletDestroyed.connect(self.destroy)

		self.cfgfile = '.plasma-flux.cfg'
		strFile = os.path.join(os.path.expanduser('~'), self.cfgfile)
		if os.path.exists(strFile):
			cfgParser = ConfigParser()
			cfgFile = open(strFile)
			cfgParser.readfp(cfgFile)
			try:
				self.lon = cfgParser.getfloat('settings', 'lon')
				self.lat = cfgParser.getfloat('settings', 'lat')
				self.nighttmp = cfgParser.getint('settings', 'nighttmp')
				self.daytmp = cfgParser.getint('settings', 'daytmp')
				self.smooth = cfgParser.getboolean('settings', 'smooth')
				self.program = cfgParser.get('settings', 'program')
				self.mode = cfgParser.get('settings', 'mode')
				self.gamma = cfgParser.get('settings', 'gamma')
				self.auto = cfgParser.getboolean('settings', 'auto')
				cfgFile.close()
			except:
				self.defaultOptions()
		else:
			self.defaultOptions()
		self.updateStatus()
		if self.auto and self.checkStatus() == 'Stopped':
			print('Auto-starting %s' % self.program)
			self.toggle()

	def defaultOptions(self):
		self.lon = float(DEFAULT_LONGITUDE)
		self.lat = float(DEFAULT_LATITUDE)
		self.nighttmp = int(DEFAULT_NIGHT)
		self.daytmp = int(DEFAULT_DAY)
		self.smooth = True
		self.program = str(DEFAULT_PROGRAM)
		self.mode = str(DEFAULT_MODE)
		self.gamma = '1.00:1.00:1.00'
		self.auto = False

	#done when timer is resetted
	def timerEvent(self, event):
		#call draw method
		self.update()

	#parse the status
	def checkStatus(self):
		self.updateStatus()
		if self.pid:
			if self.pid.isdigit():
				return 'Running'
			else:
				return 'Unknown'
		else:
			return 'Stopped'

	#get the pid
	def updateStatus(self):
		self.pid = commands.getoutput('pidof xflux')
		if not self.pid:
			self.pid = commands.getoutput('pidof redshift')
		if not self.waiting:
			self.waiting = True
			if self.subp:
				if not self.pid:
					retcode = os.waitpid(int(self.subp.pid), os.WNOHANG)
					if retcode[1]:
						stderr = self.subp.stderr.read()
						stdout = self.subp.stdout.read()
						QMessageBox.critical(self.parent, 'An error has occurred', '%s exited abnormally, probably due to wrong configuration.\nPlease report this to the developer:\n===================================================\n\nExit code:\n(%d, %s)\n\nstderr:\n%s\n\nstdout:\n%s\n===================================================' % (self.program, retcode[1], os.WEXITSTATUS(self.subp.pid), stderr, stdout))
					#self.subp.stderr.close()
					#self.subp.stdout.close()
					self.subp = None
			self.waiting = False

	def toggle(self):
		status = self.checkStatus()
		if status == 'Stopped':
			if self.program == 'f.lux':
				self.startXflux()
			elif self.program == 'Redshift':
				self.startRedshift()
		elif status == 'Running':
			self.stopProgram()
		else:
			print('Unknown status')
			#May be more than one instance running?
			self.killProgram()
			self.toggle()
	
	def startXflux(self):
		print('Starting f.lux with latitude %.1f, longitude %.1f, temperature %d' % (self.lat, self.lon, self.nighttmp))
		self.subp = Popen('%s -l %.1f -g %.1f -k %d -nofork' % (FLUX, self.lat, self.lon, self.nighttmp), shell=True, stdout=PIPE, stderr=PIPE)

	def startRedshift(self):
		print('Starting Redshift with latitude %.1f, longitude %.1f, day temperature %d, night temperature %d, gamma ramp %s, smooth transition = %s' % (self.lat, self.lon, self.daytmp, self.nighttmp, self.gamma, ('yes' if self.smooth else 'no')))
		self.subp = Popen('%s -l %.1f:%.1f -t %d:%d -g %s -m %s %s' %(REDSHIFT, self.lat, self.lon, self.daytmp, self.nighttmp, self.gamma, self.mode, ('-r' if not self.smooth else '')), shell=True, stdout=PIPE, stderr=PIPE)

	def stopProgram(self):
		print('Stopping')
		Popen('kill %s' % self.pid, shell=True)

	def killProgram(self):
		print('Killing processes: %s' % self.pid)
		Popen('kill -9 %s' % self.pid, shell=True)

	#draw method
	def paintInterface(self, painter, option, rect):
		if self.checkStatus() == 'Running':
			self.button.setIcon(self.iconRunning)
		elif self.checkStatus() == 'Stopped':
			self.button.setIcon(self.iconStopped)
		else:
			self.button.setIcon(self.iconUnknown)
		self.layout.addItem(self.button, 0, 0)

	#create config interface
	def createConfigurationInterface(self, parent):
		values = {'lon':self.lon, 'lat':self.lat, 'daytmp':self.daytmp, 'nighttmp':self.nighttmp, 'program':self.program, 'smooth':self.smooth, 'mode':self.mode, 'gamma':self.gamma, 'auto':self.auto}
		self.conf = FluxConfig(self,values)
		self.conf.setWindowTitle('Widget configuration')
		page = parent.addPage(self.conf,'Configuration')
		page.setIcon(KIcon(ICON_PLASMOID))
		self.connect(parent, SIGNAL('okClicked()'), self.configAccepted)
		#self.connect(parent, SIGNAL('cancelClicked()'), self.configDenied)

	def configAccepted(self):
		self.lon = self.conf.getLongitude()
		self.lat = self.conf.getLatitude()
		self.nighttmp = self.conf.getNightTemperature()
		self.daytmp = self.conf.getDayTemperature()
		self.smooth = self.conf.getSmooth()
		self.program = self.conf.getProgram()
		self.mode = self.conf.getMode()
		self.gamma = self.conf.getGamma()
		self.auto = self.conf.getAutoLaunch()
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
		cfgParser.set('settings', 'auto', self.auto)
		strFile = os.path.join(os.path.expanduser('~'),self.cfgfile)
		cfgFile = open(strFile,'w')
		cfgParser.write(cfgFile)
		cfgFile.close()

	def destroy(self):
		self.killTimer(self.timer)

def CreateApplet(parent):
	return FluxApplet(parent)

