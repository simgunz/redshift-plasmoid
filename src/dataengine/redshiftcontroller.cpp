/***************************************************************************
 *   Copyright (C) 2012 by Simone Gaiarin <simgunz@gmail.com>              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, see <http://www.gnu.org/licenses/>.  *
 **************************************************************************/

#include "redshiftcontroller.h"
#include "redshiftsettings.h"

#include <signal.h>

#include <QThread>
#include <QProcess>
#include <QDBusConnection>
#include <QDBusMessage>
#include <QDebug>

#include <Plasma/DataEngineManager>

RedshiftController::RedshiftController()
    : m_state(Stopped),
      m_autoState(NotSetted),
      m_runMode(Auto),
      m_readyForStart(false),
      m_manualMode(false),
      m_manualTemp(5000)
{
    m_process = new KProcess();
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.connect("", "/", "org.kde.redshift", "readyForStart", this, SLOT(setReadyForStart()));
    m_activitiesEngine = Plasma::DataEngineManager::self()->engine("org.kde.activities");
    m_activitiesEngine->connectSource("Status", this);
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

bool RedshiftController::state()
{
    return static_cast<bool>(m_state);
}

int RedshiftController::currentTemperature()
{
    if(m_manualMode) {
        return m_manualTemp;
    } else {
        return 0;
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
    m_manualTemp = 5000;
}

void RedshiftController::setTemp(bool increase)
{
    if (m_readyForStart && (m_runMode != AlwaysOff)) {
        m_manualMode = true;
        if (increase) {
            m_manualTemp += 100;
        } else {
            m_manualTemp -= 100;
        }
        //Bound the possible temperature
        //TODO:Set them as constants
        m_manualTemp = std::min(std::max(m_manualTemp,1000),9900);
        readConfig();
        m_state = Stopped;
        if (m_process->state()) {
            m_process->kill();
        }
        m_process->waitForFinished();
        applyChanges(true);
        //m_process->start();
        m_state = Stopped;
    }
}

void RedshiftController::applyChanges(bool toggle)
{
    // If m_readyForStart is false the application sends a probe signal to check
    // if it can be enabled. This prevent to run redshift too early in the login phase.
    if (m_readyForStart) {
        if (m_runMode == AlwaysOn) {
            start();
        } else if (m_runMode == AlwaysOff) {
            stop();
        // If toggle is true the next section perform a toggle of the state whereas
        // if toggle is false it realign the real state with the auto state
        } else if (toggle || (m_autoState != m_state)) {
            if (m_state == Running) {
                stop();
            } else {
                start();
            }
            m_autoState = m_state;
        }
        //TODO: Explain what the states are
        if (m_manualMode) {
            emit stateChanged(2, currentTemperature());
        } else {
            if (m_state == Running) {
                emit stateChanged(1, currentTemperature());
            } else {
                emit stateChanged(0, currentTemperature());
            }
        }

    } else {
        QDBusMessage message = QDBusMessage::createSignal("/", "org.kde.redshift", "readyCheck");
        QDBusConnection::sessionBus().send(message);
    }
}

void RedshiftController::setReadyForStart()
{
    if (!m_readyForStart) {
        m_readyForStart = true;
        applyChanges();
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
    // If the process is running it needs to be stopped and launched again with the new parameters
    if (m_process->state()) {
        m_process->terminate();
    }
    m_process->waitForFinished();
    applyChanges();
}

void RedshiftController::dataUpdated(const QString &sourceName, const Plasma::DataEngine::Data &data)
{
    if(sourceName == "Status") {
        m_currentActivity = data["Current"].toString();
        readConfig();
        applyChanges();
    }
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
    if (m_autoState == NotSetted) {
        if (m_autolaunch) {
            m_autoState = Running;
        } else {
            m_autoState = Stopped;
        }
    }
}
