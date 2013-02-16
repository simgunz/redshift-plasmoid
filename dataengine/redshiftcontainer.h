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

#ifndef REDSHIFTCONTAINER_H
#define REDSHIFTCONTAINER_H

#include <Plasma/DataContainer>
#include "redshiftcontroller.h"

class RedshiftContainer : public Plasma::DataContainer
{
    Q_OBJECT

public:
    explicit RedshiftContainer(QObject* parent = 0);
    ~RedshiftContainer();
    Plasma::Service* service(QObject* parent = 0);
public slots:
    void updateStatus(int state, int temp);

private:
    RedshiftController *m_controller;
};

#endif // REDSHIFTCONTAINER_H
