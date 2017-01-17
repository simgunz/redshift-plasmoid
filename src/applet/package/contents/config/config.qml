import QtQuick 2.0

import org.kde.plasma.configuration 2.0

ConfigModel {

    ConfigCategory {
         name: i18n("Redshift")
         icon: "redshift"
         source: "config/ConfigRedshiftBasic.qml"
    }
    ConfigCategory {
         name: i18n("Redshift (Advanced)")
         icon: "redshift"
         source: "config/ConfigRedshiftAdvanced.qml"
    }
    ConfigCategory {
         name: i18n("Activities")
         icon: "preferences-activities"
         source: "config/ConfigActivities.qml"
    }
}
