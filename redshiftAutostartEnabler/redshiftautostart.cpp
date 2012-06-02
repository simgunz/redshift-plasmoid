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

/** This application is launched during the autostart phase after the login and
 * it sends a signal to the redshift controller to enable it. This prevent the
 * controller to launch redshift too early during the login driving to an error state.
 * If the controller is launched after the login (e.g. when the plasmoid is placed on
 * the desktop for the first time), the controller probes this application that will
 * respond with the enable signal.
 */

#include <QtCore/QCoreApplication>
#include <QDBusMessage>
#include <QDBusConnection>

#include <redshiftenabler.h>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    RedshiftEnabler *enabler = new RedshiftEnabler();
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.connect("", "/", "org.kde.redshift", "readyCheck",
                 enabler, SLOT(sendSignal()));
    enabler->sendSignal();
    return app.exec();
}
