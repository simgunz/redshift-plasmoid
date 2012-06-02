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

#ifndef REDSHIFTJOB_H
#define REDSHIFTJOB_H

#include <Plasma/ServiceJob>
#include "redshiftcontroller.h"

class RedshiftJob : public Plasma::ServiceJob
{
    Q_OBJECT

public:
    RedshiftJob(RedshiftController *controller, const QString &operation, QMap<QString, QVariant> &parameters, QObject *parent = 0);
    ~RedshiftJob();

protected:
    void start();
private:
    RedshiftController *m_redshiftController;
};

#endif // REDSHIFTJOB_H
