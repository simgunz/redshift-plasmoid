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

/*! \file redshiftautostart.cpp
 * Auxiliary program needed to autostart the redshift process at the right time during kde login phase.
 *
 * This application is launched during the desktop environment applications autostart phase,
 * after the login, and it sends a signal to the redshift data engine to enable it to start
 * the redshift process. This prevents the data engine to launch redshift too early during the
 * login phase, driving to an error state, i.e, the screen color isn't adjusted.
 * If the data engine is launched after the login (e.g. when a plasmoid is placed
 * on the desktop for the first time), the data engine probes this application that
 * will respond with the enable signal.
 */

#include <QtCore/QCoreApplication>
#include <QDBusConnection>

#include <redshiftenabler.h>

/*!
 * Main function.
 *
 * Creates a RedshiftEnabler object and connects it to the readyCheck dbus signal, so that if a new redshift plasmoid
 * is created (this sends the readyCheck signal) this is enabled by this program. Moreover this program sends the
 * readyForStart signal once launched to enable already existent redshift plasmoids during the login phase.
 */
int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    RedshiftEnabler *enabler = new RedshiftEnabler();
    //Responds to the readyCheck signal with a readyForStart signal
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.connect("", "/", "org.kde.redshift", "readyCheck",
                 enabler, SLOT(sendSignal()));
    //Sends the signal once started to enable already possibly running instances of redshift data engine
    enabler->sendSignal();
    return app.exec();
}
