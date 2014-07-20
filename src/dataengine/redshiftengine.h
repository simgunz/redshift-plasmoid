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
 * \file redshiftengine.h
 *
 * Contains the interface of the RedshiftEngine class.
 */

#ifndef REDSHIFTENGINE_H
#define REDSHIFTENGINE_H

#include <Plasma/DataEngine>

/*!
 * The RedshiftEngine class is a Plasma::DataEngine that manages the redshift process, provides
 * info on its status and allows to interact with it.
 *
 * This Plasma::DataEngine can have a single source called "Controller". The data of this source is wrapped
 * in a Plasma::DataContainer which also holds a single system wide redshift process. In this way multiple plasmoids
 * can exist simultaneously (in the desktop, in the panel, in different activities, etc.) and they do not interfere with
 * each others since all of them interact with a single redshift process.
 *
 * This source holds the following data:
 * - Status, the redshift status, which can be either Stopped, Running, RunningManual
 * - Temperature, the current color temperature when redshift is in manual mode
 *
 * The DataEngine provides a way to interact with the redshift process by using a Plasma::Service.
 * In particular the following actions are possible:
 * - toggle, toggles redshift on/off
 * - restart, restarts the redshift process (needed to apply configuration changes)
 * - increase, increases the screen color temperature when redshift is in manual mode
 * - decrease, decreases the screen color temperature when redshift is in manual mode
 */
class RedshiftEngine : public Plasma::DataEngine
{
    Q_OBJECT

public:

    /*!
     * Default constructor.
     */
    RedshiftEngine(QObject* parent, const QVariantList& args);

    /*!
     * Returns a service targeted to a source when a plasmoid requests it.
     *
     * \param source The source to target the Service at.
     *
     * \returns A Plasma::Service that has the source as a destination.
     */
    Plasma::Service *serviceForSource(const QString &source);

protected:

    /*!
     * Returns a source when a plasmoid connects to the dataengine.
     *
     * If the requested source is "Controller" and it doesn't already exists, it is created by instantiating
     * a new RedshiftContainer otherwise the method just confirms that the requested source exists.
     * If the requested source is not "Controller" the method returns false.
     *
     * \param source The name of the source that has been requested.
     *
     * \returns True if a Plasma::DataContainer was set up, false otherwise.
     */
    bool sourceRequestEvent(const QString& source);
};

#endif //REDSHIFTENGINE_H
