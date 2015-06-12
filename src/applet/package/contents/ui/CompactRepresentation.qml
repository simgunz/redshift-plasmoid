
import QtQuick 2.2
import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents

MouseArea {
    id: panelIconWidget

    anchors.fill: parent
    
    onClicked: toogle()//plasmoid.expanded = !plasmoid.expanded
    //onClicked: functionChange
    
    PlasmaCore.IconItem {
        id: icon

        anchors.fill: parent
        source: "redshift-status-off"
    }
    
    /*PlasmaCore.ToolTip {
	id: tooltip
	target: panelIconWidget
	mainText: "Click to toggle on."
	subText: "Scroll the mouse wheel to set the color temperature manually."
	image: "redshift-status-off"
    }*/
    
    PlasmaCore.DataSource {
	id: dataSource
	engine: "redshift"
	connectedSources: ["Controller"]
	interval: 500
    
	onNewData:{
	    if (sourceName == "Controller") {
		if (data.Status[0] == "Running") {
		    icon.source = "redshift-status-on"
	/*	    tooltip.mainText = "Click to toggle off."
		    tooltip.subTest = "Scroll the mouse wheel to set the color temperature manually."
		    tooltip.image = "redshift-status-on";*/
		    //m_appletStatus = Plasma::PassiveStatus;
		} else {
		    icon.source = "redshift-status-off"
	/*	    tooltip.mainText = "Click to toggle on."
		    tooltip.subText = "Scroll the mouse wheel to set the color temperature manually."
		    tooltip.image = "redshift-status-off"*/
		    //m_appletStatus = Plasma::PassiveStatus;
		}
		if (data.Status == "RunningManual") {
		    icon.setImage("redshift-status-manual")
	/*	    tooltip.mainText = "Click to switch to auto mode."
		    tooltip.subText = "Scroll the mouse wheel to change the color temperature."*/
		    //m_appletStatus = Plasma::ActiveStatus;
		}
		//Start the timer to change the status, if the timer is already active this will restart it
		//m_setStatusTimer->start();
		//Plasma::ToolTipManager::self()->setContent(this, tooltip);
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
	if(icon.source == "redshift-status-off") {
	    icon.source = "redshift-status-on";
	} else {
	    icon.source = "redshift-status-off";
	}
    }
}