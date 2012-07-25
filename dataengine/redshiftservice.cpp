/***************************************************************************
 *   Copyright (C) 2012 by Simone Gaiarin <simgunz@gmail.com>              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, see <http://www.gnu.org/licenses/>.  *
 **************************************************************************/

#include "redshiftservice.h"
#include "redshiftjob.h"

RedshiftService::RedshiftService(QObject *parent, RedshiftController *controller)
    : m_redshiftController(controller)
{
    setName("redshift");
}

ServiceJob *RedshiftService::createJob(const QString &operation, QMap<QString, QVariant> &parameters)
{
    return new RedshiftJob(m_redshiftController, operation, parameters, this);
}

#include "redshiftservice.moc"
