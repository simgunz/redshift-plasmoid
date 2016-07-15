
import QtQuick 2.2
import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.plasmoid 2.0

MouseArea {
    id: panelIconWidget

    anchors.fill: parent

    PlasmaCore.IconItem {
        id: icon
        anchors.fill: parent
        source: "redshift-status-off"
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

    // When this timer is running the wheel event is ignored, so it imposes a limit on how fast we can change the color temperature manually.
    // This is required to avoid to send to many operationCall to the dataEngine, since this causes plasma to crash.
    // FIXME: This is just a workaround, probably it's better to invoke redshift torugh the 'executable' dataEngine which doesn't make plasma crash.
    Timer {
        id: inhibitTimer
        interval: 250;
    }

    onClicked: runOperation("toggle")

    // When we use the mouse wheel over the plasmoid we contact the dataEngine to increase/decrease the color temperature manually
    onWheel: {
            if(!inhibitTimer.running) {
                if (wheel.angleDelta.y > 0) {
                    var operation = dataSource.serviceForSource("Controller").operationDescription("increase");
                } else {
                    var operation = dataSource.serviceForSource("Controller").operationDescription("decrease");
                }
                dataSource.serviceForSource("Controller").startOperationCall(operation);
                inhibitTimer.running = true;
            }
        }

    function runOperation(operationName)
    {
        var operation = dataSource.serviceForSource("Controller").operationDescription(operationName);
        dataSource.serviceForSource("Controller").startOperationCall(operation);
    }
}
