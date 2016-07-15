/***************************************************************************
 *   Copyright (C) 2015 by Bernhard Friedreich <friesoft@gmail.com>        *
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
 ***************************************************************************/

#ifndef REDSHIFT_H
#define REDSHIFT_H

#include <QObject>
#include <QVariantMap>

class Redshift : public QObject
{
    Q_OBJECT

public:
    Redshift(QObject *parent = nullptr);

    Q_INVOKABLE void writeConfig(QVariantMap data);
    Q_INVOKABLE QVariantMap readConfig();

    int dayTemp() const;
    void setDayTemp(int temp);

signals:
    void configHasChanged();

private:
    int m_dayTemp;
    int m_nightTemp;
    bool m_autolaunch;
};

#endif // REDSHIFT_H
