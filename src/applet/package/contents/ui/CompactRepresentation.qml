
import QtQuick 2.2
import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents

MouseArea {
    id: panelIconWidget

    anchors.fill: parent

    onClicked: plasmoid.expanded = !plasmoid.expanded

    PlasmaCore.IconItem {
        id: icon

        anchors.fill: parent
        source: connectionIconProvider.connectionIcon
        colorGroup: PlasmaCore.ColorScope.colorGroup

        PlasmaComponents.BusyIndicator {
            id: connectingIndicator

            anchors.fill: parent
            running: connectionIconProvider.connecting
            visible: running
        }
    }
}
