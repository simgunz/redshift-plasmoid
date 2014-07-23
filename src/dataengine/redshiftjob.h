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
 * \file redshiftjob.h
 *
 * Contains the interface of the RedshiftJob class.
 */

#ifndef REDSHIFTJOB_H
#define REDSHIFTJOB_H

#include "redshiftcontroller.h"

#include <Plasma/ServiceJob>

/*!
 * The RedshiftJob class performs the operation requested by the service in an asynchronous way.
 */
class RedshiftJob : public Plasma::ServiceJob
{
    Q_OBJECT

public:

    /*!
     * Constructor.
     *
     * \param controller The redshift controller that allows to interact with the redshift process.
     * \param operation The operation to be performed.
     * \param parameters The parameters set by the user for the operation.
     * \param parent The QObject this job is parented to.
     */
    RedshiftJob(RedshiftController *controller, const QString &operation, QMap<QString,
                QVariant> &parameters, QObject *parent = 0);

protected:

    /*!
    * Execute the operation by using the public methods of RedshiftController.
    */
    void start();

private:

    //! The redshift controller that allows to interact with the redshift process.
    RedshiftController *m_redshiftController;
};

#endif // REDSHIFTJOB_H
