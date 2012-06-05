/**************************************************************************
*   Copyright (C) 2012 by Simone Gaiarin <simgunz@gmail.com>              *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
**************************************************************************/

#include "redshiftcontroller.h"
#include "redshiftsettings.h"

#include <signal.h>

#include <QThread>
#include <QProcess>
#include <QDBusConnection>
#include <QDBusMessage>

#include <Plasma/DataEngineManager>

RedshiftController::RedshiftController()
    : m_state(Stopped),
      m_manualState(NotSetted),
      m_runMode(Manual),
      m_readyForStart(false)
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
    m_process->terminate();
}

bool RedshiftController::state()
{
    return static_cast<bool>(m_state);
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
        // if toggle is false it realign the real state with the manual state
        } else if (toggle || (m_manualState != m_state)) {
            if (m_state == Running) {
                stop();
            } else {
                start();
            }
            m_manualState = m_state;
        }
        if (m_state == Running) {
            emit stateChanged(true);
        } else {
            emit stateChanged(false);
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
    m_currentActivity = data["Current"].toString();
    readConfig();
    applyChanges();
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
    m_smooth = RedshiftSettings::smooth();
    m_autolaunch = RedshiftSettings::autolaunch();
    QString command = QString("redshift -c /dev/null -l %1:%2 -t %3:%4 -g %5:%6:%7")
                      .arg(m_latitude, 0, 'f', 1)
                      .arg(m_longitude, 0, 'f', 1)
                      .arg(m_dayTemp).arg(m_nightTemp)
                      .arg(m_gammaR, 0, 'f', 2)
                      .arg(m_gammaG, 0, 'f', 2)
                      .arg(m_gammaB, 0, 'f', 2);
    if (!m_smooth) {
        command.append(" -r");
    }

    m_process->setShellCommand(command);

    m_runMode = Manual;
    const QStringList alwaysOnActivities = RedshiftSettings::alwaysOnActivities();
    const QStringList alwaysOffActivities = RedshiftSettings::alwaysOffActivities();
    if (alwaysOnActivities.contains(m_currentActivity)) {
        m_runMode = AlwaysOn;
    } else if (alwaysOffActivities.contains(m_currentActivity)) {
        m_runMode = AlwaysOff;
    }
    if (m_manualState == NotSetted) {
        if (m_autolaunch) {
            m_manualState = Running;
        } else {
            m_manualState = Stopped;
        }
    }
}
