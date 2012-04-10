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

#include "redshiftsettings.h"
#include "redshiftcontroller.h"

#include <signal.h>

#include <QThread>
#include <QProcess>
#include <QDebug>
#include <QDBusConnection>
#include <QDBusMessage>
#include <Plasma/DataEngineManager>

RedshiftController::RedshiftController() : m_state(Stopped),m_autoState(Stopped),m_forceType(0),m_readyForStart(0),m_restarting(false)
{
    m_process = new KProcess();
    //readConfig();
    if(m_autolaunch) {
        m_autoState = Running;
    }
    connect(m_process,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(restartStart()));
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.connect("", "/", "org.kde.redshift", "readyForStart", this, SLOT(setReadyForStart()));
    m_activitiesEngine = Plasma::DataEngineManager::self()->engine("org.kde.activities");
    m_activitiesEngine->connectSource("Status",this);
}

RedshiftController::~RedshiftController()
{
    m_process->terminate();
}

void RedshiftController::setReadyForStart()
{
    qDebug()<<"READYFORSTARTIN";
    if(!m_readyForStart) {
        qDebug()<<"READYFORSTARTINNNER";
        m_readyForStart = true;
        if(m_forceType == 2 || m_autolaunch) {
            toggle();
        }
    }
}

void RedshiftController::dataUpdated(const QString &sourceName, const Plasma::DataEngine::Data &data)
{
    m_currentActivity = data["Current"].toString();
    readConfig();
    toggle(1);
}

bool RedshiftController::state()
{
    return static_cast<bool>(m_state);
}

void RedshiftController::start()
{
    if(m_state == Stopped) {
        m_state = Running;
        if(!m_process->state()) {
            m_process->start();
        } else {
            kill(m_process->pid(),SIGUSR1);
        }
    }
}

void RedshiftController::stop()
{
    if(m_state == Running) {
        m_state = Stopped;
        kill(m_process->pid(),SIGUSR1);
    }
}

void RedshiftController::toggle(int from)
{
    if(m_readyForStart) {
        qDebug() << "TOGGLEIN";
        qDebug() << m_restarting;
        qDebug() << (m_autoState == m_state);
        qDebug() << (m_forceType);
        if(m_forceType == 1) {
            start();
        } else if(m_forceType == 2) {
            stop();
        } else {
            if(!(m_autoState == m_state && from)) {
                if(m_state == Running) {
                    stop();
                } else {
                    start();
                    qDebug()<<"RSTART";
                }
                m_autoState = m_state;
            }
        }
        if(m_state == Running) {
            emit stateChanged(true);
        } else {
            emit stateChanged(false);
        }
    } else {
        qDebug() << "TOGGLEELSE";
        QDBusMessage message = QDBusMessage::createSignal("/", "org.kde.redshift", "readyCheck");
        QDBusConnection::sessionBus().send(message);
    }
}

void RedshiftController::restart()
{
    readConfig();
    m_state = Stopped;
    m_restarting = true;
    if(m_process->state()) {
        m_process->terminate();
    } else {
        restartStart();
    }
}

void RedshiftController::restartStart()
{
    qDebug()<<"STARTIN";
    if(m_restarting) {
        qDebug()<<"STARTINNER";
        m_process->waitForFinished();
        m_restarting = false;
        toggle(2);
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
    m_smooth = RedshiftSettings::smooth();
    m_autolaunch = RedshiftSettings::autolaunch();
    QString command = QString("redshift -c /dev/null -l %1:%2 -t %3:%4 -g %5:%6:%7")
                            .arg(m_latitude,0,'f',1)
                            .arg(m_longitude,0,'f',1)
                            .arg(m_dayTemp).arg(m_nightTemp)
                            .arg(m_gammaR,0,'f',2)
                            .arg(m_gammaG,0,'f',2)
                            .arg(m_gammaB,0,'f',2);
    if(!m_smooth)
        command.append(" -r");
    m_process->setShellCommand(command);

    m_forceType = 0;
    const QStringList alwaysOnActivities = RedshiftSettings::alwaysOnActivities();
    const QStringList alwaysOffActivities = RedshiftSettings::alwaysOffActivities();
    if(alwaysOnActivities.contains(m_currentActivity)) {
        m_forceType = 1;
    } else if (alwaysOffActivities.contains(m_currentActivity)){
        m_forceType = 2;
    }
}

