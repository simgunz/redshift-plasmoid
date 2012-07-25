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

#ifndef REDSHIFTSERVICE_H
#define REDSHIFTSERVICE_H

#include <Plasma/Service>
#include <Plasma/ServiceJob>
#include "redshiftcontroller.h"

using namespace Plasma;

class RedshiftService : public Plasma::Service
{
    Q_OBJECT

public:
    RedshiftService(QObject *parent, RedshiftController *controller);
    ServiceJob *createJob(const QString &operation,
                          QMap<QString, QVariant> &parameters);

private:
    RedshiftController *m_redshiftController;
};

#endif // REDSHIFTSERVICE_H
