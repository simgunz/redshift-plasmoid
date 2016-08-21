import QtQuick 2.0

import org.kde.plasma.configuration 2.0

ConfigModel {

    ConfigCategory {
         name: i18n("Basic")
         icon: "redshift"
         source: "config/ConfigRedshiftBasic.qml"
    }
    ConfigCategory {
         name: i18n("Advanced")
         icon: "redshift"
         source: "config/ConfigRedshiftAdvanced.qml"
    }
}
