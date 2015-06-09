
import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1

import org.kde.plasma.extras 2.0 as PlasmaExtras

FocusScope {

    id: scrollView

    anchors {
	bottom: parent.bottom
	left: parent.left
	right: parent.right
    }


    ColumnLayout {
	id: mainLayout
	spacing: 2
	
	Text {
	    id: header
	    text: "Redshift configuration"
	    anchors { top: parent.top; topMargin: 3; left: parent.left; right: parent.right }
	    horizontalAlignment: Text.AlignHCenter
	}
    
	GridLayout {
	    id: formGrid

	    columns: 2

	    Text { text: "Latitude"; }
	    SpinBox { id:"latitude"; decimals: 1; suffix: "°" }
	    Text { text: "Longitude"; }
	    SpinBox { id:"longitude"; decimals: 1; suffix: "°" }
	    Text { text: "Day color temperature"; }
	    SpinBox { id:"dayTemperature"; decimals: 0; suffix: " K" }
	    Text { text: "Night color temperature"; }
	    SpinBox { id:"nightTemperature";  decimals: 0; suffix: " K" }
	    Text { text: "Brightness"; }
	    SpinBox { id:"brightness";  decimals: 2; }
	    Text { text: "Gamma (RGB)"; }
	    RowLayout {
	      id: "gammaLayout"
	      SpinBox { id:"gammaR"; decimals: 2; }
	      SpinBox { id:"gammaG"; decimals: 2; }
	      SpinBox { id:"gammaB"; decimals: 2; }
	    }
	    Text { text: "Smooth transition"; }
	    Switch { id: "smoothTransition"; }
	    Text { text: "Autostart"; }
	    Switch { id: "autostart"; }
	    Text { text: "Adjust method"; }
	    ComboBox { id: "methodBox";  model: [ "Auto", "Randr", "Vidmode" ] }
	}
    }

}
