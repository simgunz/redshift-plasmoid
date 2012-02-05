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

#import commands for executing shell commands
import commands
import os
from subprocess import Popen, PIPE
import subprocess
import signal
from time import sleep
'''Theme'''
THEME = 'widgets/background'
'''Icons'''
ICON_PLASMOID = 'redshift'
ICON_STOPPED = 'redshift-status-off'
ICON_RUNNING = 'redshift-status-on'
'''Default night temp'''
DEFAULT_NIGHT = 3700
'''Default day temp'''
DEFAULT_DAY = 5500
'''Get latitude and longitude from KDE and set them as default'''
DEFAULT_LATITUDE = KSystemTimeZones.local().latitude()
DEFAULT_LONGITUDE = KSystemTimeZones.local().longitude()

class RedshiftApplet(plasmascript.Applet):

    def __init__(self, parent, args=None):
        plasmascript.Applet.__init__(self, parent)
        self.parent = parent

    def init(self):
        
        self.iconStopped = KIcon(ICON_STOPPED)
        self.iconRunning = KIcon(ICON_RUNNING)
        self.subp = None

        self.setHasConfigurationInterface(True)
        
        #set size of Plasmoid
        self.resize(50, 50)
        self.setAspectRatioMode(Plasma.KeepAspectRatio)
        self.setBackgroundHints(Plasma.Applet.DefaultBackground)
        self.button = Plasma.IconWidget(self.parent)
        self.button.setIcon(self.iconStopped)
        self.theme = Plasma.Svg(self)
        self.theme.setImagePath(THEME)
        self.layout = QGraphicsGridLayout(self.applet)
        self.layout.setContentsMargins(0,0,0,0)
        self.setMinimumSize(10,10)

        self.connect(self.button, SIGNAL('clicked()'), self.toggle)
        self.appletDestroyed.connect(self.destroy)
        self.connect(self.configScheme(), SIGNAL("configChanged()"), self.configChanged)
        
        # Kill any instance of redshift launched by others
        pid = commands.getoutput('pidof redshift')
        if pid:
            commands.getoutput('pkill -9 redshift')
            commands.getoutput('redshift -x')
        
        self.configChanged()
        self.status = 'Stopped'
        if self.autolaunch:
            print('Auto-starting')
            self.toggle()
            
    def configChanged(self):
        cfgGeneral = self.config().group("General")
        self.latitude = cfgGeneral.readEntry('latitude',DEFAULT_LATITUDE).toFloat()[0]
        self.longitude = cfgGeneral.readEntry('longitude',DEFAULT_LONGITUDE).toFloat()[0]
        self.nighttemp = cfgGeneral.readEntry('nightTemp', DEFAULT_NIGHT).toInt()[0]
        self.daytemp = cfgGeneral.readEntry('dayTemp', DEFAULT_DAY).toInt()[0]
        self.smooth = cfgGeneral.readEntry('smooth', True).toBool()
        self.autolaunch = cfgGeneral.readEntry('autolaunch', False).toBool()
        gammaR = cfgGeneral.readEntry('gammaR', 1.00).toFloat()[0]
        gammaG = cfgGeneral.readEntry('gammaG', 1.00).toFloat()[0]
        gammaB = cfgGeneral.readEntry('gammaB', 1.00).toFloat()[0]        
        self.gamma = str("%.2f:%.2f:%.2f" % (gammaR, gammaG, gammaB))
        self.restartRedshift()


    def toggle(self):
        if self.status == 'Running':
            print('Pausing Redshift')
            commands.getoutput('pkill -USR1 redshift')
            self.status = 'Paused'
        elif self.status == 'Paused':    
            print('Resuming Redshift')
            commands.getoutput('pkill -USR1 redshift')
            self.status = 'Running'
        else:
            self.startRedshift()
            self.status = 'Running'
        self.update()
            
    def startRedshift(self):
        print('Starting Redshift with latitude %.1f, longitude %.1f, day temperature %d, night temperature %d, gamma ramp %s, smooth transition = %s' % (self.latitude, self.longitude, self.daytemp, self.nighttemp, self.gamma, ('yes' if self.smooth else 'no')))
        self.subp = Popen('%s -l %.1f:%.1f -t %d:%d -g %s %s' %('redshift', self.latitude, self.longitude, self.daytemp, self.nighttemp, self.gamma, ('-r' if not self.smooth else '')), shell=True, stdout=PIPE, stderr=PIPE)

    def stopRedshift(self):
        print 'Stopping Redshift'
        if self.subp:
            self.subp.terminate()
            self.subp.wait()
    
    def restartRedshift(self):
        # Called when the configuration is changed, the process is killed and eventually restarted
        if not self.status == 'Stopped':
            self.stopRedshift()
            if self.status == 'Paused':
                self.status = 'Stopped'
            elif self.status == 'Running':
                self.startRedshift()

    def paintInterface(self, painter, option, rect):
        if self.status == 'Running':
            self.button.setIcon(self.iconRunning)
        elif self.status == 'Stopped' or self.status == 'Paused':
            self.button.setIcon(self.iconStopped)
        self.layout.addItem(self.button, 0, 0)

    def destroy(self):
        print 'des'
        if not self.status == 'Stopped':
            self.stopRedshift()
            self.status = "Paused"

def CreateApplet(parent):
    return RedshiftApplet(parent)

