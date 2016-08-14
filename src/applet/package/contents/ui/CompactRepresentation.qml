
import QtQuick 2.2
import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.plasmoid 2.0

import org.kde.plasma.private.redshift 1.0

MouseArea {
    id: panelIconWidget

    anchors.fill: parent

    property bool isConfiguring: false

    PlasmaCore.IconItem {
        id: icon
        anchors.fill: parent
        source: "redshift-status-off"
    }

    Redshift{
        id: redshift
        onConfigHasChanged: runOperation("restart")
    }

    PlasmaCore.DataSource {
        id: dataSource
        engine: "redshift"
        connectedSources: ["Controller"]

        onNewData: {
            if (sourceName == "Controller") {
                if (data.Status == "Running") {
                    icon.source = "redshift-status-on"
                    plasmoid.toolTipMainText = "Click to toggle off."
                    plasmoid.toolTipSubText = "Scroll the mouse wheel to set the color temperature manually."
                    //Plasmoid.image = "redshift-status-on";
                    //m_appletStatus = Plasma::PassiveStatus;
                } else {
                    icon.source = "redshift-status-off"
                    plasmoid.toolTipMainText = "Click to toggle on."
                    plasmoid.toolTipSubText = "Scroll the mouse wheel to set the color temperature manually."
                    //Plasmoid.image = "redshift-status-off"
                    //m_appletStatus = Plasma::PassiveStatus;
                }
                if (data.Status == "RunningManual") {
                    icon.source = "redshift-status-manual"
                    plasmoid.toolTipMainText = "Click to switch to auto mode."
                    plasmoid.toolTipSubText = "Scroll the mouse wheel to change the color temperature."
                    //m_appletStatus = Plasma::ActiveStatus;
                }
                syncPlasmoidConfig()
                //Start the timer to change the status, if the timer is already active this will restart it
                //m_setStatusTimer->start();
            }
            var temperature = data.Temperature;
            //Show the OSD only if the temperature is non-zero, i.e., only when redshift is inn "Manual" mode
            if(temperature) {
                showOSD(temperature);
            }
        }
    }

    onClicked: runOperation("toggle")

    Osd {
        id: redshiftOSD
        icon: "redshift-status-on"
    }

    // When we use the mouse wheel over the plasmoid we contact the dataEngine to increase/decrease the color temperature manually
    onWheel: {
        if(wheel.angleDelta.y > 0) {
            runOperation("increase")
        } else {
            runOperation("decrease")
        }
    }

    function runOperation(operationName)
    {
        var operation = dataSource.serviceForSource("Controller").operationDescription(operationName);
        dataSource.serviceForSource("Controller").startOperationCall(operation);
    }

    function showOSD(temperature)
    {
        redshiftOSD.osdValue = temperature + " K"
        redshiftOSD.animateOpacity = false
        redshiftOSD.opacity = 1
        redshiftOSD.visible = true
        redshiftOSD.animateOpacity = true
        redshiftOSD.opacity = 0
    }

    function saveConfiguration()
    {
        console.log('Save config')
        var data = {};
        data.dayTemperature = plasmoid.configuration.dayTemperature
        data.nightTemperature = plasmoid.configuration.nightTemperature
        data.autostart = plasmoid.configuration.autostart
        redshift.writeConfig(data)
    }

    function syncPlasmoidConfig(){
        console.log('Sync config')
        var data = redshift.readConfig()
        console.log(data.dayTemperature)
        plasmoid.configuration.dayTemperature = data.dayTemperature
        plasmoid.configuration.nightTemperature = data.nightTemperature
        plasmoid.configuration.autostart = data.autostart
    }

    Connections {
        target: plasmoid
        onUserConfiguringChanged: {
            if (panelIconWidget.isConfiguring) {
                saveConfiguration()
            }
            panelIconWidget.isConfiguring = !panelIconWidget.isConfiguring
        }
    }
}
