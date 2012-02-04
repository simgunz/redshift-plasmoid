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
DEFAULT_NIGHT = 3700
'''Default day temp'''
DEFAULT_DAY = 5500
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
        self.connect(self.configScheme(), SIGNAL("configChanged ()"), self.configChanged)
        self.updateStatus()
        self.configChanged()

    def configChanged(self):
        cfgGeneral = self.config().group("General")
        self.lat = cfgGeneral.readEntry('lat',DEFAULT_LATITUDE).toFloat()               
        self.lon = cfgGeneral.readEntry('lon',DEFAULT_LONGITUDE).toFloat()
        self.nighttmp = cfgGeneral.readEntry('nighttmp', DEFAULT_NIGHT).toInt()
        self.daytmp = cfgGeneral.readEntry('daytmp', DEFAULT_DAY).toInt()
        self.smooth = cfgGeneral.readEntry('smooth', True).toBool()
        self.program = cfgGeneral.readEntry('program', DEFAULT_PROGRAM).toString()
        self.mode = cfgGeneral.readEntry('mode', DEFAULT_MODE).toString()
        self.gammaR = cfgGeneral.readEntry('gammaR', 1.00).toFloat()
        self.gammaG = cfgGeneral.readEntry('gammaG', 1.00).toFloat()
        self.gammaB = cfgGeneral.readEntry('gammaB', 1.00).toFloat()
        self.auto = cfgGeneral.readEntry('auto', False).toBool()
        
        if self.auto and self.checkStatus() == 'Stopped':
            print('Auto-starting %s' % self.program)
            self.toggle()
            
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
        self.pid = commands.getoutput('pidof %s' % FLUX)
        if not self.pid:
            self.pid = commands.getoutput('pidof %s' % REDSHIFT)
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
        import signal
        #Popen('kill %s' % self.pid, shell=True)
        '''if commands.getoutput('pidof %s' % FLUX):
            #xflux
            if self.subp:
                self.subp.send_signal(signal.SIGTERM)
            else:
                os.kill(int(self.pid), signal.SIGTERM)
        elif commands.getoutput('pidof %s ' % REDSHIFT):
            #redshift
            if self.subp:
                self.subp.send_signal(signal.SIGUSR1)
            else:
                os.kill(int(self.pid), signal.SIGUSR1)'''
        if self.subp:
            self.subp.send_signal(signal.SIGTERM)
        elif self.pid:
            os.kill(int(self.pid), signal.SIGTERM)

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

    def destroy(self):
        self.killTimer(self.timer)
        self.stopProgram()

def CreateApplet(parent):
    return FluxApplet(parent)

