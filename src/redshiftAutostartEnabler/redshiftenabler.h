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
 * \file redshiftenabler.h
 *
 * Contains the interface of the RedshiftEnabler class.
 */

#ifndef REDSHIFTENABLER_H
#define REDSHIFTENABLER_H

#include <QObject>

/*!
 * The RedshiftEnabler class provides a system to send a custom signal over dbus (readyForStart)
 * to inform the redshift dateEngine that it is allowed to start the redshift process.
 */
class RedshiftEnabler : public QObject
{
    Q_OBJECT

public slots:
    
    /*!
     * Sends a dbus signal called readyForStart.
     */
    void sendSignal();
};

#endif // REDSHIFTENABLER_H
