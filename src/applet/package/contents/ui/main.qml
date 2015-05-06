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

import QtQuick 1.1
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.components 0.1 as PlasmaComponents
import org.kde.qtextracomponents 0.1

Item {
    id: redshiftapplet
    property int minimumWidth: 250
    property int minimumHeight: 10

    PlasmaCore.Theme {
        id: theme
    }

    PlasmaCore.DataSource {
        id: redshiftSource
        engine: "redshift"
        onSourceAdded: {
            connectSources: ["Controller"]
        }
    }

    PlasmaComponents.ContextMenu {
        id: newActivityMenu
        visualParent: newActivityButton
        PlasmaComponents.MenuItem {
            id: templatesItem
            text: i18n("Templates")
        }
        PlasmaComponents.MenuItem {
            icon: QIcon("user-desktop")
            text: i18n("Empty Desktop")
        }
        PlasmaComponents.MenuItem {
            icon: QIcon("edit-copy")
            text: i18n("Clone current activity")
        }
    }

    PlasmaComponents.Label {
        id: header
        text: "Activity manager QML"
        anchors { top: parent.top; topMargin: 3; left: parent.left; right: parent.right }
        horizontalAlignment: Text.AlignHCenter
    }


    Component.onCompleted: {
        plasmoid.setPopupIconByName("redshift")
    }
}
