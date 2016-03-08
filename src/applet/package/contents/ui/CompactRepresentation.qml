
import QtQuick 2.2
import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.plasmoid 2.0

MouseArea {
    id: panelIconWidget

    anchors.fill: parent

    onClicked: toogle()//plasmoid.expanded = !plasmoid.expanded

    Plasmoid.toolTipMainText: "Redshift - Click to toggle on."
    Plasmoid.toolTipSubText: "Scroll the mouse wheel to set the color temperature manually."

    PlasmaCore.IconItem {
        id: icon
        anchors.fill: parent
        source: "redshift-status-off"
    }


    PlasmaCore.DataSource {
    id: dataSource
    engine: "redshift"
    connectedSources: ["Controller"]

    onNewData:{
        if (sourceName == "Controller") {
        if (data.Status[0] == "Running") {
            icon.source = "redshift-status-on"
            Plasmoid.toolTipMainText = "Click to toggle off."
            Plasmoid.toolTipSubText = "Scroll the mouse wheel to set the color temperature manually."
            //tooltip.image = "redshift-status-on";
            //m_appletStatus = Plasma::PassiveStatus;
        } else {
            icon.source = "redshift-status-off"
            Plasmoid.toolTipMainText = "Click to toggle on."
            Plasmoid.toolTipSubText = "Scroll the mouse wheel to set the color temperature manually."
            //tooltip.image = "redshift-status-off"
            //m_appletStatus = Plasma::PassiveStatus;
        }
        if (data.Status == "RunningManual") {
            icon.source = "redshift-status-manual"
            Plasmoid.toolTipMainText = "Click to switch to auto mode."
            Plasmoid.toolTipSubText = "Scroll the mouse wheel to change the color temperature."
            //m_appletStatus = Plasma::ActiveStatus;
        }
        //Start the timer to change the status, if the timer is already active this will restart it
        //m_setStatusTimer->start();
        }
        /*int temperature = data["Temperature"].toInt();
        //Show the OSD only if the temperature is non-zero, i.e., only when redshift is inn "Manual" mode
        if(temperature) {
        showRedshiftOSD(temperature);
        }*/
    }
    }


    function toogle()
    {
        var operation = dataSource.serviceForSource("Controller").operationDescription("toggle");
        dataSource.serviceForSource("Controller").startOperationCall(operation);
    }
}
