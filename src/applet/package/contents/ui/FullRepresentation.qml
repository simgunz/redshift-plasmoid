
import QtQuick 2.2
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.3
import org.kde.plasma.extras 2.0 as PlasmaExtras

FocusScope {
    anchors {
	leftMargin: 10
	rightMargin: 10
    }


    ColumnLayout {
	id: mainLayout
	spacing: 2
	
	anchors.fill: parent
	
	Text {
	    id: header
	    text: "Redshift configuration"
	    anchors { top: parent.top; topMargin: 3; left: parent.left; right: parent.right }
	    horizontalAlignment: Text.AlignHCenter
	}
    
	GridLayout {
	    id: formGrid
	    columns: 2
	    Text {
		text: "Latitude"
		Layout.fillWidth: true
	    }
	    SpinBox {
		id:"latitudeEdit"
		decimals: 1
		suffix: "°"
		Layout.preferredWidth: mainLayout.width/2
	    }
	    Text {
		text: "Longitude"
		Layout.fillWidth: true
	    }
	    SpinBox {
		id:"longitudeEdit"
		decimals: 1
		suffix: "°"
		Layout.preferredWidth: mainLayout.width/2
	    }
	    Rectangle {
		Layout.columnSpan: 2
		Layout.preferredHeight: longitudeEdit.height*0.5
	    }
	    Text {
		text: "Day color temperature"
		Layout.preferredWidth: mainLayout.width/2
	    }
	    SpinBox {
		id:"dayTemperatureEdit"
		decimals: 0
		suffix: " K" 
		Layout.preferredWidth: mainLayout.width/2
	    }
	    Text {
		text: "Night color temperature"
		Layout.preferredWidth: mainLayout.width/2
	    }
	    SpinBox {
		id:"nightTemperatureEdit"
		decimals: 0
		suffix: " K"
		Layout.preferredWidth: mainLayout.width/2
	    }
	    Rectangle {
		Layout.columnSpan: 2
		Layout.preferredHeight: longitudeEdit.height*0.5
	    }
	    Text { 
		text: "Brightness"
		Layout.preferredWidth: mainLayout.width/2
	    }
	    SpinBox { 
		id:"brightnessEdit"
		decimals: 2
		Layout.preferredWidth: mainLayout.width/2
	    }
	    Text { 
		text: "Gamma (RGB)"
		Layout.preferredWidth: mainLayout.width/2
	    }
	    RowLayout {
		id: "gammaLayout"
		Layout.preferredWidth: mainLayout.width/2
		
		SpinBox { 
		    id:"gammaREdit"
		    decimals: 2
		    Layout.fillWidth: true
		}
		SpinBox { 
		    id:"gammaGEdit"
		    decimals: 2
		    Layout.fillWidth: true
		}
		SpinBox {
		    id:"gammaBEdit"
		    decimals: 2
		    Layout.fillWidth: true
		}
	    }
	    Rectangle {
		Layout.columnSpan: 2
		Layout.preferredHeight: longitudeEdit.height*0.5
	    }
	    Text {
		text: "Smooth transition"
		Layout.preferredWidth: mainLayout.width/2
	    }
	    Switch {
		id: "smoothTransitionSwitch"
		Layout.preferredWidth: mainLayout.width/2
	    }
	    Text {
		text: "Autostart"
		Layout.preferredWidth: mainLayout.width/2
	    }
	    Switch {
		id: "autostartSwitch"
		Layout.preferredWidth: mainLayout.width/2
	    }
	    Text {
		text: "Adjust method"
		Layout.preferredWidth: mainLayout.width/2
	    }
	    ComboBox {
		id: "methodBox"
		model: [ "Auto", "Randr", "Vidmode" ] 
		Layout.preferredWidth: mainLayout.width/2
	    }
	}
	
	RowLayout {
	    id: resetSaveLayout
	    spacing: 5
	    Layout.alignment: Qt.AlignRight
	    
	    Button {
		id: "resetButton"
		text: "Reset"
	    }
	    Button {
		id: "saveButton"
		text: "Save"
	    }
	}
    }
}