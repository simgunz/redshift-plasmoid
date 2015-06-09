/*
    Copyright 2013-2014 Jan Grulich <jgrulich@redhat.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/

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
