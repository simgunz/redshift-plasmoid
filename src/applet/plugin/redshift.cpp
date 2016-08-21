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
    double doubleValue;
    QString stringValue;
    bool somethingChanged = false;

    boolValue = data["autostart"].toBool();
    if (boolValue != RedshiftSettings::autostart()) {
        RedshiftSettings::setAutostart(boolValue);
        somethingChanged = true;
    }
    boolValue = data["smoothTransition"].toBool();
    if (boolValue != RedshiftSettings::smooth()) {
        RedshiftSettings::setSmooth(boolValue);
        somethingChanged = true;
    }
    boolValue = data["geoclueLocationEnabled"].toBool();
    if (boolValue != RedshiftSettings::geoclueLocationEnabled()) {
        RedshiftSettings::setGeoclueLocationEnabled(boolValue);
        somethingChanged = true;
    }
    doubleValue = data["latitude"].toDouble();
    if (doubleValue != RedshiftSettings::latitude()) {
        RedshiftSettings::setLatitude(doubleValue);
        somethingChanged = true;
    }
    doubleValue = data["longitude"].toDouble();
    if (doubleValue != RedshiftSettings::longitude()) {
        RedshiftSettings::setLongitude(doubleValue);
        somethingChanged = true;
    }
    value = data["dayTemperature"].toInt();
    if (value != RedshiftSettings::dayTemperature()) {
        RedshiftSettings::setDayTemperature(value);
        somethingChanged = true;
    }
    value = data["nightTemperature"].toInt();
    if (value != RedshiftSettings::nightTemperature()) {
        RedshiftSettings::setNightTemperature(value);
        somethingChanged = true;
    }
    doubleValue = data["dayBrightness"].toDouble();
    if (doubleValue != RedshiftSettings::dayBrightness()) {
        RedshiftSettings::setDayBrightness(doubleValue);
        somethingChanged = true;
    }
    doubleValue = data["nightBrightness"].toDouble();
    if (doubleValue != RedshiftSettings::nightBrightness()) {
        RedshiftSettings::setNightBrightness(doubleValue);
        somethingChanged = true;
    }
    doubleValue = data["gammaR"].toDouble();
    if (doubleValue != RedshiftSettings::gammaR()) {
        RedshiftSettings::setGammaR(doubleValue);
        somethingChanged = true;
    }
    doubleValue = data["gammaG"].toDouble();
    if (doubleValue != RedshiftSettings::gammaG()) {
        RedshiftSettings::setGammaG(doubleValue);
        somethingChanged = true;
    }
    doubleValue = data["gammaB"].toDouble();
    if (doubleValue != RedshiftSettings::gammaB()) {
        RedshiftSettings::setGammaB(doubleValue);
        somethingChanged = true;
    }
    value = data["manualTemperatureStep"].toInt();
    if (value != RedshiftSettings::manualTemperatureStep()) {
        RedshiftSettings::setManualTemperatureStep(value);
        somethingChanged = true;
    }
    stringValue = data["renderModeString"].toString();
    if (stringValue != RedshiftSettings::renderModeString()) {
        RedshiftSettings::setRenderModeString(stringValue);
        somethingChanged = true;
    }

    stringValue = data["renderMode"].toString();
    if (stringValue != RedshiftSettings::renderMode()) {
        RedshiftSettings::setRenderMode(stringValue);
        somethingChanged = true;
    }
    stringValue = data["renderModeScreen"].toString();
    if (stringValue != RedshiftSettings::renderModeScreen()) {
        RedshiftSettings::setRenderModeScreen(stringValue);
        somethingChanged = true;
    }
    stringValue = data["renderModeCard"].toString();
    if (stringValue != RedshiftSettings::renderModeCard()) {
        RedshiftSettings::setRenderModeCard(stringValue);
        somethingChanged = true;
    }
    stringValue = data["renderModeCrtc"].toString();
    if (stringValue != RedshiftSettings::renderModeCrtc()) {
        RedshiftSettings::setRenderModeCrtc(stringValue);
        somethingChanged = true;
    }
    boolValue = data["preserveScreenColour"].toBool();
    if (boolValue != RedshiftSettings::preserveScreenColour()) {
        RedshiftSettings::setPreserveScreenColour(boolValue);
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
    data["autostart"] = RedshiftSettings::autostart();
    data["smoothTransition"] = RedshiftSettings::smooth();
    data["geoclueLocationEnabled"] = RedshiftSettings::geoclueLocationEnabled();
    data["dayTemperature"] = RedshiftSettings::dayTemperature();
    data["nightTemperature"] = RedshiftSettings::nightTemperature();
    data["latitude"] = RedshiftSettings::latitude();
    data["longitude"] = RedshiftSettings::longitude();
    data["dayBrightness"] = RedshiftSettings::dayBrightness();
    data["nightBrightness"] = RedshiftSettings::nightBrightness();
    data["gammaR"] = RedshiftSettings::gammaR();
    data["gammaG"] = RedshiftSettings::gammaG();
    data["gammaB"] = RedshiftSettings::gammaB();
    data["manualTemperatureStep"] = RedshiftSettings::manualTemperatureStep();
    data["renderModeString"] = RedshiftSettings::renderModeString();

    data["renderMode"] = RedshiftSettings::renderMode();
    data["renderModeScreen"] = RedshiftSettings::renderModeScreen();
    data["renderModeCard"] = RedshiftSettings::renderModeCard();
    data["renderModeCrtc"] = RedshiftSettings::renderModeCrtc();
    data["preserveScreenColour"] = RedshiftSettings::preserveScreenColour();

    return data;
}
