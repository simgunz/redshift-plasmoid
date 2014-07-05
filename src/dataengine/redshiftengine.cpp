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
 * \file redshiftengine.cpp
 *
 * Contains the implementation of the RedshiftEngine class.
 */

#include "redshiftengine.h"
#include "redshiftservice.h"

RedshiftEngine::RedshiftEngine(QObject* parent, const QVariantList& args)
    : Plasma::DataEngine(parent, args)
{
    Q_UNUSED(args)
}

bool RedshiftEngine::sourceRequestEvent(const QString &name)
{
    if (name == "Controller") {
        Plasma::DataContainer *container = containerForSource("Controller");
        if (!container) {
            addSource(new RedshiftContainer(this));
        }
        return true;
    }
    return false;
}

Plasma::Service *RedshiftEngine::serviceForSource(const QString &source)
{
    RedshiftContainer* container = qobject_cast<RedshiftContainer*>(containerForSource(source));
    if (container) {
        return container->service();
    } else {
        return DataEngine::serviceForSource(source);
    }
}

K_EXPORT_PLASMA_DATAENGINE(timekpr, RedshiftEngine)

#include "redshiftengine.moc"
