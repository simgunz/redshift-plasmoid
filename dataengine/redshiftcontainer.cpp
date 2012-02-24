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

#include "redshiftcontainer.h"
#include "redshiftservice.h"

#include <KDebug>

RedshiftContainer::RedshiftContainer(QObject* parent)
    : DataContainer(parent)
{
    setObjectName("Controller");    
    m_controller = new RedshiftController();
    QObject::connect(m_controller,SIGNAL(statusChanged(int)),this,SLOT(updateStatus(int)));
    
}

RedshiftContainer::~RedshiftContainer()
{
    m_controller->stop(); 
}

void RedshiftContainer::updateStatus(int status)
{
    if(status)
    {
        setData("Status","Running"); 
    }
    else
    {
        setData("Status","Stopped"); 
    }
    checkForUpdate();
}

Plasma::Service* RedshiftContainer::service(QObject* parent)
{
    return new RedshiftService(parent, m_controller);    
}
