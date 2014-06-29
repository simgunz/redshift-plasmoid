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

#ifndef REDSHIFTENGINE_H
#define REDSHIFTENGINE_H

#include <Plasma/DataEngine>
#include <KSharedConfigPtr>
#include <KSharedConfig>
#include <KConfigGroup>
#include <KProcess>

#include <QStringList>
#include <QDate>
#include <QFile>
#include <QFileSystemWatcher>

#include "redshiftcontroller.h"
#include "redshiftcontainer.h"

class RedshiftEngine : public Plasma::DataEngine
{
    Q_OBJECT

public:
    RedshiftEngine(QObject* parent, const QVariantList& args);
    Plasma::Service *serviceForSource(const QString &source);
protected:
    bool sourceRequestEvent(const QString& name);
};

#endif //REDSHIFTENGINE_H
