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

#include "redshift.h"
#include "redshiftsettings.h"

#include <QObject>

Redshift::Redshift(QObject *parent) : QObject(parent)
{

}

void Redshift::writeConfig(QVariantMap data)
{
//     QStringList alwaysOnActivities;
//     QStringList alwaysOffActivities;

//     QTreeWidget *activitiesList = m_activitiesUi->activities;
//     for (int i = 0; i < activitiesList->topLevelItemCount(); ++i) {
//         QTreeWidgetItem *item = activitiesList->topLevelItem(i);
//         QComboBox *itemCombo = static_cast<QComboBox *>(activitiesList->itemWidget(item, 1));
//         const QString act = item->data(0, Qt::UserRole).toString();
//         if (itemCombo->currentIndex() == 1) {
//             alwaysOnActivities << act;
//         } else if (itemCombo->currentIndex() == 2) {
//             alwaysOffActivities << act;
//         }
//     }
//     RedshiftSettings::setAlwaysOnActivities(alwaysOnActivities);
//     RedshiftSettings::setAlwaysOffActivities(alwaysOffActivities);

    int value;
    bool boolValue;
    bool somethingChanged = false;

    value = data["dayTemperature"].toInt();
    if (value != RedshiftSettings::dayTemp()) {
        RedshiftSettings::setDayTemp(value);
        somethingChanged = true;
    }
    value = data["nightTemperature"].toInt();
    if (value != RedshiftSettings::nightTemp()) {
        RedshiftSettings::setNightTemp(value);
        somethingChanged = true;
    }
    boolValue = data["autostart"].toBool();
    if (boolValue != RedshiftSettings::autostart()) {
        RedshiftSettings::setAutostart(boolValue);
        somethingChanged = true;
    }
    if (somethingChanged) {
        RedshiftSettings::self()->save();
        emit configHasChanged();
    }
}

QVariantMap Redshift::readConfig()
{
    RedshiftSettings::self()->load();

    QVariantMap data;
    data["dayTemperature"] = RedshiftSettings::dayTemp();
    data["nightTemperature"] = RedshiftSettings::nightTemp();
    data["autostart"] = RedshiftSettings::autostart();

    return data;
}
