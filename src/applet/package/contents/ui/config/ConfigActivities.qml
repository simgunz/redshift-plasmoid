
import QtQuick 2.2
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1
import org.kde.plasma.core 2.0 as PlasmaCore

Item {
    id: activitiesConfig

    property var cfg_alwaysOnActivities
    property var cfg_alwaysOffActivities

    PlasmaCore.DataSource {
        id: activitySource
        engine: "org.kde.activities"
        onSourceAdded: {
            if (source != "Status") {
                connectSource(source)
            }
        }
        Component.onCompleted: {
            connectedSources = sources.filter(function(val) {
                return val != "Status";
            })
        }
    }

    Component {
        id: activityItem

        ActivityConfigItem {
            id: wrapper
            uid: DataEngineSource
            icon: activitySource.data[uid]["Icon"]
            name: activitySource.data[uid]["Name"]
        }
    }

    ListView {
        id: activityDialog
        anchors.fill: parent
        model: PlasmaCore.DataModel {
            dataSource: activitySource
        }
        delegate: activityItem
    }
}
