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
 * \file redshiftjob.cpp
 *
 * Contains the implementation of the RedshiftJob class.
 */

#include "redshiftjob.h"

RedshiftJob::RedshiftJob(RedshiftController *controller, const QString &operation,
                         QMap<QString, QVariant> &parameters, QObject *parent)
    : ServiceJob(parent->objectName(), operation, parameters, parent),
      m_redshiftController(controller)
{
}

void RedshiftJob::start()
{
    const QString operation = operationName();

    if (operation == "toggle") {
        m_redshiftController->toggle();
        setResult(true);
        return;
    } else if (operation == "restart") {
        m_redshiftController->restart();
        setResult(true);
        return;
    } else if (operation == "increase") {
        m_redshiftController->setTemperature(true);
        setResult(true);
        return;
    } else if (operation == "decrease") {
        m_redshiftController->setTemperature(false);
        setResult(true);
        return;
    }
    setResult(false);
}
