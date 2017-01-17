/**************************************************************************
*   Copyright (C) 2016 by Simone Gaiarin <simgunz@gmail.com>              *
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

import QtQuick 2.2
import QtQuick.Controls 1.3
import org.kde.plasma.core 2.0 as PlasmaCore

Item {
    id: activityItem
    property string uid
    property string name
    property string icon

    property alias redshiftActivityMode: modeCombo.currentIndex

    height: container.childrenRect.height
    width: parent.width


    Row {
        id: container
        PlasmaCore.IconItem {
            id: activityIcon
            width: 32
            height: 32
            z: 900
            source: activityItem.icon
        }
        Label {
            id: activityLabel
            height: paintedHeight
            text: name
        }
        ComboBox {
            id: modeCombo
            model: ListModel {
                ListElement {
                    text: 'Auto'
                    val: 'auto'
                }
                ListElement {
                    text: 'Always Enabled'
                    val: 'on'
                }
                ListElement {
                    text: 'Always Disabled'
                    val: 'off'
                }
            }
        }
    }
}
