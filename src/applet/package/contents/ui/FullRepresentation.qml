
import QtQuick 2.3
import QtQuick.Layouts 1.1

import org.kde.plasma.core 2.0 as PlasmaCore

Flow {
    width: 130
    height:130
	
    Rectangle {
	anchors.centerIn: parent.centerIn;
	width: parent.width
	height:parent.height
	color: "transparent"
	
	Image {
	    id: redshiftSvg
	    width: parent.width
	    height: parent.height
	    fillMode: Image.PreserveAspectCrop
	    source: "redshift.svg"
	    verticalAlignment: Image.AlignVCenter
	    horizontalAlignment: Image.AlignHCenter
	}
    }
}