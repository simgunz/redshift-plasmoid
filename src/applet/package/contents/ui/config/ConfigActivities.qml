
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

            onRedshiftActivityModeChanged: {
                var modified = 0
                if(redshiftActivityMode === 1) {
                        if(cfg_alwaysOnActivities.indexOf(uid) === -1) {
                        modified = 1
                        cfg_alwaysOnActivities.push(uid)
                        var indexToPop = cfg_alwaysOffActivities.indexOf(uid);
                        if(indexToPop > -1) {
                            cfg_alwaysOffActivities.splice(indexToPop, 1)
                        }
                    }
                } else if(redshiftActivityMode === 2) {
                    if(cfg_alwaysOffActivities.indexOf(uid) === -1) {
                        modified = 1
                        cfg_alwaysOffActivities.push(uid)
                        var indexToPop = cfg_alwaysOnActivities.indexOf(uid);
                        if(indexToPop > -1) {
                            cfg_alwaysOnActivities.splice(indexToPop, 1)
                        }
                    }
                } else {
                    var indexToPop = cfg_alwaysOnActivities.indexOf(uid);
                    if(indexToPop > -1) {
                        modified = 1
                        cfg_alwaysOnActivities.splice(indexToPop, 1)
                    } else {
                        indexToPop = cfg_alwaysOffActivities.indexOf(uid);
                        if(indexToPop > -1) {
                            modified = 1
                            cfg_alwaysOffActivities.splice(indexToPop, 1)
                        }
                    }
                }
                if(modified) {
                    // Deep copy the array over itself to enable the Apply button
                    // Apparently push and pop do not affect the status
                    cfg_alwaysOnActivities = cfg_alwaysOnActivities.slice()
                    cfg_alwaysOffActivities = cfg_alwaysOffActivities.slice()
                }
            }
            Component.onCompleted: {
                if( cfg_alwaysOnActivities!==undefined && cfg_alwaysOnActivities.indexOf(wrapper.uid) > -1) {
                    redshiftActivityMode = 1
                } else if (cfg_alwaysOnActivities!==undefined && cfg_alwaysOffActivities.indexOf(wrapper.uid) > -1) {
                    redshiftActivityMode = 2
                } else {
                    redshiftActivityMode = 0
                }
            }
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
