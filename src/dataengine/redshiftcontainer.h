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
 * \file redshiftcontainer.h
 *
 * Contains the interface of the RedshiftContainer class.
 */

#ifndef REDSHIFTCONTAINER_H
#define REDSHIFTCONTAINER_H

#include "redshiftcontroller.h"

#include <Plasma/DataContainer>

/*!
 * The RedshiftContainer class is a Plasma::DataContainer that wraps a system wide redshift process
 * and provides information on its state and on the screen color temperature in manual mode.
 *
 * This DataContainer provides the following data:
 * - Status, the redshift status, which can be either Stopped, Running, RunningManual
 * - Temperature, the current color temperature when redshift is in manual mode
 *
 * It also holds a RedshiftController that manages the system wide redshift process.
 */
class RedshiftContainer : public Plasma::DataContainer
{
    Q_OBJECT

public:

    /*!
    * Default constructor.
    *
    * \param parent The QObject this applet is parented to.
    */
    explicit RedshiftContainer(QObject* parent = 0);

    /*!
    * Deconstructor.
    */
    ~RedshiftContainer();

    /*!
    * Returns a RedshiftService object.
    *
    * \param parent The QObject this applet is parented to.
    * \returns A Plasma::Service to interact with the source, i.e., with the redshift process.
    */
    Plasma::Service* service(QObject* parent = 0);

public slots:

    /*!
    * Update the data according to the status communicated by RedshiftController.
    *
    * This slot must be connected to the RedshiftController stateChanged signal in order to constantly
    * reflect the redshift process state.
    *
    * \param state The RedshiftController state.
    * \param temperature The screen color temperature in Kelvin, when redshift is in manual mode.
    */
    void updateStatus(RedshiftController::RedshiftState state, int temperature);

private:

    //! The controller object of the redshift process.
    RedshiftController *m_controller;
};

#endif // REDSHIFTCONTAINER_H
