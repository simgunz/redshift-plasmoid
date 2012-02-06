# -*- coding: utf-8 -*-
# plasma_flux/contents/code/main.py

'''
Provides a configuration interface and a switch to start/stop Redshift daemon. 
Copyright (C) 2011 Diego Cristina
Copyright (C) 2012 Simone Gaiarin

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
from subprocess import Popen

# Theme
THEME = 'widgets/background'
# Icons
ICON_PLASMOID = 'redshift'
ICON_STOPPED = 'redshift-status-off'
ICON_RUNNING = 'redshift-status-on'
# Default color temperature
DEFAULT_NIGHT = 3700
DEFAULT_DAY = 5500
# Status constants
STOP,RUN,PAUSE = range(3)

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
        # Kill any instance of redshift launched by others
        pid = commands.getoutput('pidof redshift')
        if pid:
            commands.getoutput('pkill -9 redshift')
            commands.getoutput('redshift -x')
        self.configChanged()
        # Set the default latitude and longitude values in the configuration file 
        # so that the config dialog can read them
        if not (self.latitude or self.longitude):
            default_latitude = KSystemTimeZones.local().latitude()
            default_longitude = KSystemTimeZones.local().longitude()
            cfgGeneral = self.config().group("General")
            cfgGeneral.writeEntry('latitude',default_latitude)
            cfgGeneral.writeEntry('longitude',default_longitude)
        self.status = STOP
        if self.autolaunch:
            print('Autostarting Redshift')
            self.toggle()
            
    def configChanged(self):
        cfgGeneral = self.config().group("General")
        self.latitude = cfgGeneral.readEntry('latitude',0).toFloat()[0]
        self.longitude = cfgGeneral.readEntry('longitude',0).toFloat()[0]
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
        if self.status == RUN:
            print('Pausing Redshift')
            commands.getoutput('pkill -USR1 redshift')
            self.status = PAUSE
        elif self.status == PAUSE:    
            print('Resuming Redshift')
            commands.getoutput('pkill -USR1 redshift')
            self.status = RUN
        else:
            self.startRedshift()
            self.status = RUN
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
        if not self.status == STOP:
            self.stopRedshift()
            if self.status == PAUSE:
                self.status = STOP
            elif self.status == RUN:
                self.startRedshift()

    def paintInterface(self, painter, option, rect):
        if self.status == RUN:
            self.button.setIcon(self.iconRunning)
        else:
            self.button.setIcon(self.iconStopped)
        self.layout.addItem(self.button, 0, 0)

    def destroy(self):
        if not self.status == STOP:
            print 'Stopping Redshift'
            if self.subp:
                self.subp.terminate()

def CreateApplet(parent):
    return RedshiftApplet(parent)

