
import QtQuick 2.2
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1
import org.kde.plasma.core 2.0 as PlasmaCore

Item {
    id: advancedConfig

    property alias cfg_geoclueLocationEnabled: geoclueLocationEnabled.checked
    property alias cfg_latitude: latitude.value
    property alias cfg_longitude: longitude.value
    property alias cfg_dayTemperature: dayTemperature.value
    property alias cfg_nightTemperature: nightTemperature.value

    property string versionString: 'N/A'
    property string modeString: ''

    PlasmaCore.DataSource {
        id: geolocationDS
        engine: 'geolocation'

        property string locationSource: 'location'

        connectedSources: []

        onNewData: {
            print('geolocation: ' + data.latitude)
            latitude.value = data.latitude
            longitude.value = data.longitude
        }
    }

    GridLayout {
        columns: 4

        Label {
            text: i18n("Location")
            Layout.columnSpan: parent.columns
            font.bold: true
        }
        Label {
            text: i18n("Automatic (geoclue):")
            Layout.alignment: Qt.AlignRight
        }
        CheckBox {
            id: geoclueLocationEnabled
            text: i18n('Enabled')
            Layout.columnSpan: 3
        }
        Label {
            text: i18n('Latitude:')
            Layout.alignment: Qt.AlignRight
            enabled: !geoclueLocationEnabled.checked
        }
        SpinBox {
            id: latitude
            decimals: 2
            stepSize: 1
            minimumValue: -90
            maximumValue: 90

            Layout.preferredWidth: 150
            enabled: !geoclueLocationEnabled.checked
        }

        Button {
            text: i18n('Locate')
            tooltip: i18n('This will use Mozilla Location Service exposed natively in KDE')
            onClicked: {
                geolocationDS.connectedSources.length = 0
                geolocationDS.connectedSources.push(geolocationDS.locationSource)
            }
            Layout.rowSpan: 2
            Layout.columnSpan: 2
            enabled: !geoclueLocationEnabled.checked
        }

        Label {
            text: i18n('Longitude:')
            Layout.alignment: Qt.AlignRight
            enabled: !geoclueLocationEnabled.checked
        }
        SpinBox {
            id: longitude
            decimals: 2
            stepSize: 1
            minimumValue: -180
            maximumValue: 180

            Layout.preferredWidth: 150
            enabled: !geoclueLocationEnabled.checked
        }

        Item {
            width: 2
            height: 10
            Layout.columnSpan: parent.columns
        }

        Label {
            text: i18n('Color Temperature')
            Layout.columnSpan: parent.columns
            font.bold: true
        }

        Label {
            text: i18n('Day:')
            Layout.alignment: Qt.AlignRight
        }
        SpinBox {
            id: dayTemperature
            decimals: 0
            stepSize: 250
            minimumValue: 1000
            maximumValue: 25000
            Layout.columnSpan: 3
        }

        Label {
            text: i18n('Night:')
            Layout.alignment: Qt.AlignRight
        }
        SpinBox {
            id: nightTemperature
            decimals: 0
            stepSize: 250
            minimumValue: 1000
            maximumValue: 25000
            Layout.columnSpan: 3
        }
    }
}
