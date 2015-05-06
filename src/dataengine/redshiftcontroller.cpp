/************************************************************************
* Copyright (C) 2012 by Simone Gaiarin <simgunz@gmail.com>              *
*                                                                       *
* This program is free software; you can redistribute it and/or modify  *
* it under the terms of the GNU General Public License as published by  *
* the Free Software Foundation; either version 3 of the License, or     *
* (at your option) any later version.                                   *
*                                                                       *
* This program is distributed in the hope that it will be useful,       *
* but WITHOUT ANY WARRANTY; without even the implied warranty of        *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
* GNU General Public License for more details.                          *
*                                                                       *
* You should have received a copy of the GNU General Public License     *
* along with this program; if not, see <http://www.gnu.org/licenses/>.  *
************************************************************************/

/*!
 * \file redshiftcontroller.cpp
 *
 * Contains the implementation of the RedshiftController class.
 */

#include "redshiftcontroller.h"
#include "redshiftsettings.h"

#include <signal.h>

#include <QDBusConnection>
#include <QDBusMessage>

#include <Plasma/DataEngineConsumer>

RedshiftController::RedshiftController()
    : m_readyForStart(false),
      m_state(Stopped),
      m_autoState(Undefined),
      m_runMode(Auto),
      m_manualMode(false),
      m_manualTemp(RedshiftController::DefaultManualTemperature)
{
    m_process = new KProcess();
    //Connects to dbus to receive the enabler signal readyForStart
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.connect("", "/", "org.kde.redshift", "readyForStart", this, SLOT(setReadyForStart()));
    //Connects to the plasma activities dataEngine to monitor if the current activity is changed
    Plasma::DataEngineConsumer dataEngineConsumer = Plasma::DataEngineConsumer();
    m_activitiesEngine = dataEngineConsumer.dataEngine("org.kde.activities");
    m_activitiesEngine->connectSource("Status", this);
    //Calls dataUpdated manually to initialize the controller. The controller reads the configuration file,
    //gets the current activity, and run the redshift process.
    dataUpdated("Status", m_activitiesEngine->query("Status"));
}

RedshiftController::~RedshiftController()
{
    if (m_manualMode) {
        KProcess::execute("redshift",QStringList("-x"));
    } else {
        m_process->terminate();
    }
}

RedshiftController::RedshiftState RedshiftController::state()
{
    if(m_manualMode) {
        return RunningManual;
    }
    return m_state;
}

int RedshiftController::currentTemperature()
{
    if(m_manualMode) {
        return m_manualTemp;
    } else {
        return 0;
    }
}

void RedshiftController::setTemperature(bool increase)
{
    if (m_readyForStart && (m_runMode != AlwaysOff)) {
        m_manualMode = true;
        if (increase) {
            m_manualTemp += RedshiftController::TemperatureStep;
        } else {
            m_manualTemp -= RedshiftController::TemperatureStep;
        }
        //Bounds the possible temperatures
        m_manualTemp = qMin(qMax(m_manualTemp,RedshiftController::MinTemperature),RedshiftController::MaxTemperature);
        readConfig();
        m_state = Stopped;
        //Instantly kills the process without waiting the color transition
        if (m_process->state()) {
            m_process->kill();
        }
        m_process->waitForFinished();
        //Since the state is Stopped, calling  applyChanges with the toggle flag set will run the redshift
        //process thus setting the fixed color temperature.
        applyChanges(true);
        //The start method has previously set the state to Running, but when redshift is ran with the -x flag
        //it exits immediately, so the state should be set to Stopped manually.
        m_state = Stopped;
    }
}

void RedshiftController::toggle()
{
    if (m_manualMode) {
        m_manualMode = false;
        readConfig();
        KProcess::execute("redshift",QStringList("-x"));
    }
    applyChanges(true);
}

void RedshiftController::restart()
{
    readConfig();
    m_state = Stopped;
    //If the process is running it needs to be stopped and launched again with the new parameters
    if (m_process->state()) {
        m_process->terminate();
    }
    //Waits the end of the color out transition before launching the process again
    m_process->waitForFinished();
    applyChanges();
}

void RedshiftController::setReadyForStart()
{
    if (!m_readyForStart) {
        m_readyForStart = true;
        applyChanges();
    }
}

void RedshiftController::dataUpdated(const QString &sourceName, const Plasma::DataEngine::Data &data)
{
    if(sourceName == "Status") {
        m_currentActivity = data["Current"].toString();
        readConfig();
        applyChanges();
    }
}

void RedshiftController::applyChanges(bool toggle)
{
    //If m_readyForStart is false the application sends a probe signal to check
    //if it can be enabled. This prevents to run redshift too early in the login phase.
    if (m_readyForStart) {
        if (m_runMode == AlwaysOn) {
            start();
        } else if (m_runMode == AlwaysOff) {
            stop();
        //If toggle is true the next section performs a toggle of the state whereas
        //if toggle is false it realigns the real state with the auto state
        } else if (toggle || (m_autoState != m_state)) {
            if (m_state == Running) {
                stop();
            } else {
                start();
            }
            m_autoState = m_state;
        }
        if (m_manualMode) {
            emit stateChanged(RunningManual, currentTemperature());
        } else {
            if (m_state == Running) {
                emit stateChanged(Running, currentTemperature());
            } else {
                emit stateChanged(Stopped, currentTemperature());
            }
        }

    } else {
        QDBusMessage message = QDBusMessage::createSignal("/", "org.kde.redshift", "readyCheck");
        QDBusConnection::sessionBus().send(message);
    }
}

void RedshiftController::start()
{
    if (m_state == Stopped) {
        m_state = Running;
        if (!m_process->state()) {
            m_process->start();
        } else {
            kill(m_process->pid(), SIGUSR1);
        }
    }
}

void RedshiftController::stop()
{
    if (m_state == Running) {
        m_state = Stopped;
        if (m_process->state()) {
            kill(m_process->pid(), SIGUSR1);
        }
    }
    m_manualTemp = RedshiftController::DefaultManualTemperature;
}

void RedshiftController::readConfig()
{
    RedshiftSettings::self()->readConfig();
    m_latitude = RedshiftSettings::latitude();
    m_longitude = RedshiftSettings::longitude();
    m_dayTemp = RedshiftSettings::dayTemp();
    m_nightTemp = RedshiftSettings::nightTemp();
    m_gammaR = RedshiftSettings::gammaR();
    m_gammaG = RedshiftSettings::gammaG();
    m_gammaB = RedshiftSettings::gammaB();
    m_brightness = RedshiftSettings::brightness();
    m_smooth = RedshiftSettings::smooth();
    m_autolaunch = RedshiftSettings::autolaunch();
    m_method = RedshiftSettings::method();
    QString command = QString("redshift -c /dev/null -l %1:%2 -t %3:%4 -g %5:%6:%7 -b %8")
                      .arg(m_latitude, 0, 'f', 1)
                      .arg(m_longitude, 0, 'f', 1)
                      .arg(m_dayTemp).arg(m_nightTemp)
                      .arg(m_gammaR, 0, 'f', 2)
                      .arg(m_gammaG, 0, 'f', 2)
                      .arg(m_gammaB, 0, 'f', 2)
                      .arg(m_brightness, 0, 'f', 2);
    if (!m_smooth) {
        command.append(" -r");
    }

    if (m_method == 1) {
        command.append(" -m randr");
    } else if (m_method == 2) {
        command.append(" -m vidmode");
    }

    if (m_manualMode) {
        command.append(" -O ");
        command.append(QString("%1").arg(m_manualTemp));
    }

    m_process->setShellCommand(command);

    m_runMode = Auto;
    const QStringList alwaysOnActivities = RedshiftSettings::alwaysOnActivities();
    const QStringList alwaysOffActivities = RedshiftSettings::alwaysOffActivities();
    if (alwaysOnActivities.contains(m_currentActivity)) {
        m_runMode = AlwaysOn;
    } else if (alwaysOffActivities.contains(m_currentActivity)) {
        m_runMode = AlwaysOff;
    }
    if (m_autoState == Undefined) {
        if (m_autolaunch) {
            m_autoState = Running;
        } else {
            m_autoState = Stopped;
        }
    }
}
