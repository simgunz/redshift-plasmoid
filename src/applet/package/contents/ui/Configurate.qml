
import QtQuick 2.2
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.3
import org.kde.plasma.extras 2.0 as PlasmaExtras

Item {
    id: configurationPage
    width: childrenRect.width
    height: childrenRect.height
    
    property alias cfg_latitude: latitudeEdit.value
    property alias cfg_longitude: latitudeEdit.value
    property alias cfg_dayTemperature: dayTemperatureEdit.value
    property alias cfg_nightTemperature: nightTemperatureEdit.value
    property alias cfg_brightness: brightnessEdit.value
    property alias cfg_gammaR: gammaREdit.value
    property alias cfg_gammaG: gammaGEdit.value
    property alias cfg_gammaB: gammaBEdit.value
    property alias cfg_smoothTransition: smoothTransitionSwitch.checked
    property alias cfg_autostart: autostartSwitch.checked
    property alias cfg_method: methodBox.currentText
    
   GridLayout {
	id: mainLayout
	
	anchors.fill: parent
	anchors.leftMargin: 10
	anchors.rightMargin: 10
    
	columns: 2
	Text {
	    text: "Latitude"
	    Layout.fillWidth: true
	}
	SpinBox {
	    id:"latitudeEdit"
	    decimals: 1
	    suffix: "°"
	    minimumValue: -90
	    maximumValue: 90
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
	    minimumValue: -180
	    maximumValue: 180
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
	    minimumValue: 0
	    maximumValue: 10000
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
	    minimumValue: 0
	    maximumValue: 10000
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
	    minimumValue: 0.1
	    maximumValue: 1
	    stepSize: 0.05
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
		minimumValue: 0.1
		maximumValue: 10
		stepSize: 0.1
		Layout.fillWidth: true
	    }
	    SpinBox { 
		id:"gammaGEdit"
		decimals: 2
		minimumValue: 0.1
		maximumValue: 10
		stepSize: 0.1
		Layout.fillWidth: true
	    }
	    SpinBox {
		id:"gammaBEdit"
		decimals: 2
		minimumValue: 0.1
		maximumValue: 10
		stepSize: 0.1
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
}