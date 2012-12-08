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

#include "redshiftcontainer.h"
#include "redshiftservice.h"

#include <QDebug>

RedshiftContainer::RedshiftContainer(QObject* parent)
    : DataContainer(parent)
{
    setObjectName("Controller");
    m_controller = new RedshiftController();
    QObject::connect(m_controller, SIGNAL(stateChanged(int)), this, SLOT(updateStatus(int)));
    updateStatus(m_controller->state());
}

RedshiftContainer::~RedshiftContainer()
{
    delete m_controller;
}

void RedshiftContainer::updateStatus(int state)
{
    switch (state) {
        case 0: setData("Status", "Stopped"); break;
        case 1: setData("Status", "Running"); break;
        case 2: setData("Status", "RunningManual"); break;
    }
    checkForUpdate();
}

Plasma::Service* RedshiftContainer::service(QObject* parent)
{
    return new RedshiftService(parent, m_controller);
}
