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
 * \file redshiftservice.h
 *
 * Contains the interface of the RedshiftService class.
 */

#ifndef REDSHIFTSERVICE_H
#define REDSHIFTSERVICE_H

#include "redshiftcontroller.h"

#include <Plasma/Service>
#include <Plasma/ServiceJob>

using namespace Plasma;

/*!
 * The RedshiftService class provides a system to interact with the system wide redshift process.
 *
 * The following actions are possible:
 * - toggle, toggle redshift on/off
 * - restart, restarts the redshift process (needed to apply configuration changes)
 * - increase, increases the screen color temperature when redshift is in manual mode
 * - decrease, decreases the screen color temperature when redshift is in manual mode
 *
 * To run an operation from a plasmoid:
 *
 *       Plasma::Service *service = dataEngine("redshift")->serviceForSource("Controller");
 *       service->startOperationCall(service->operationDescription("toggle"));
 */
class RedshiftService : public Plasma::Service
{
    Q_OBJECT

public:
    /*!
     * Default constructor.
     *
     * \param parent The QObject this applet is parented to.
     * \param controller The RedshiftController on which the operation should be executed.
     */
    RedshiftService(QObject *parent, RedshiftController *controller);

    /*!
     * Called when a job should be created by the Service.
     *
     * This method is invoked automatically when startOperationCall is executed.
     *
     * \param operation Which operation to work on.
     * \param parameters The parameters set by the user for the operation.
     * \returns A ServiceJob that can be started and monitored by the consumer.
     */
    ServiceJob *createJob(const QString &operation,
                          QMap<QString, QVariant> &parameters);

private:

    //! The redshift controller that allows to interact with the redshift process.
    RedshiftController *m_redshiftController;
};

#endif // REDSHIFTSERVICE_H
