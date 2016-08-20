
import QtQuick 2.2
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1
import org.kde.plasma.core 2.0 as PlasmaCore

Item {
    id: advancedRedshiftConfig

    property alias cfg_dayBrightness: dayBrightness.value
    property alias cfg_nightBrightness: nightBrightness.value
    property alias cfg_gammaR: gammaR.value
    property alias cfg_gammaG: gammaG.value
    property alias cfg_gammaB: gammaB.value
    property string cfg_renderMode
    property alias cfg_renderModeScreen: renderModeScreen.text
    property alias cfg_renderModeCard: renderModeCard.text
    property alias cfg_renderModeCrtc: renderModeCrtc.text
    property alias cfg_preserveScreenColour: preserveScreenColour.checked
    property string cfg_renderModeString

    property string versionString: 'N/A'
    property string modeString: ''

    onCfg_renderModeChanged: {
        print('restore: ' + cfg_renderMode)
        var comboIndex = modeCombo.find(cfg_renderMode)
        print('restore index: ' + comboIndex)
        if (comboIndex > -1) {
            modeCombo.currentIndex = comboIndex
        }
    }

    GridLayout {
        columns: 4

        Label {
            text: i18n('Brightness')
            Layout.columnSpan: parent.columns
            font.bold: true
        }
        Label {
            text: i18n('Day:')
            Layout.alignment: Qt.AlignRight
        }
        SpinBox {
            id: dayBrightness
            decimals: 2
            stepSize: 0.1
            minimumValue: 0.1
            maximumValue: 1
            Layout.columnSpan: 3
        }
        Label {
            text: i18n('Night:')
            Layout.alignment: Qt.AlignRight
        }
        SpinBox {
            id: nightBrightness
            decimals: 2
            stepSize: 0.1
            minimumValue: 0.1
            maximumValue: 1
            Layout.columnSpan: 3
        }

        Item {
            width: 2
            height: 10
            Layout.columnSpan: parent.columns
        }
        Label {
            text: i18n('Gamma')
            Layout.columnSpan: parent.columns
            font.bold: true
        }
        Label {
            text: i18n('RGB:')
            Layout.alignment: Qt.AlignRight
        }
        RowLayout {
            Layout.columnSpan: 3

            SpinBox {
                id: 'gammaR'
                decimals: 2
                minimumValue: 0.1
                maximumValue: 10
                stepSize: 0.1
            }
            SpinBox {
                id: 'gammaG'
                decimals: 2
                minimumValue: 0.1
                maximumValue: 10
                stepSize: 0.1
            }
            SpinBox {
                id: 'gammaB'
                decimals: 2
                minimumValue: 0.1
                maximumValue: 10
                stepSize: 0.1
            }
        }

        Item {
            width: 2
            height: 10
            Layout.columnSpan: parent.columns
        }
        Label {
            text: i18n('Render Mode')
            Layout.columnSpan: parent.columns
            font.bold: true
        }
        Label {
            text: i18n('Mode:')
            Layout.alignment: Qt.AlignRight
        }
        // col 1
        ComboBox {
            id: modeCombo
            Layout.columnSpan: 3
            model: ListModel {
                ListElement {
                    text: 'Auto'
                    val: ''
                }
                ListElement {
                    text: 'drm'
                    val: 'drm'
                }
                ListElement {
                    text: 'randr'
                    val: 'randr'
                }
                ListElement {
                    text: 'vidmode'
                    val: 'vidmode'
                }
                ListElement {
                    text: 'Manual'
                    val: 'Manual'
                }
            }
            onCurrentIndexChanged: {
                cfg_renderMode = model.get(currentIndex).val
                print('saved: ' + cfg_renderMode)
                modeChanged()
            }
        }
        Label {
            id: lblCard
            visible: isMode(['drm', 'card'])
            text: i18n('Card:')
            Layout.alignment: Qt.AlignRight
        }
        TextField {
            id: renderModeCard
            Layout.columnSpan: 3
            visible: isMode(['drm', 'card'])
            onTextChanged: modeChanged()
        }
        Label {
            id: lblScreen
            visible: isMode(['randr', 'vidmode'])
            text: i18n('Screen:')
            Layout.alignment: Qt.AlignRight
        }
        TextField {
            id: renderModeScreen
            Layout.columnSpan: 3
            visible: isMode(['randr', 'vidmode'])
            onTextChanged: modeChanged()
        }
        Label {
            id: lblCRTC
            visible: isMode(['drm', 'randr'])
            text: i18n('CRTC:')
            Layout.alignment: Qt.AlignRight
        }
        TextField {
            id: renderModeCrtc
            Layout.columnSpan: 3
            visible: isMode(['drm', 'randr'])
            width: advancedRedshiftConfig / 8
            onTextChanged: modeChanged()
        }
        Label {
            text: i18n('Preserve screen colour:')
            Layout.alignment: Qt.AlignRight
            visible: isMode(['randr', 'vidmode'])
        }
        // col 4
        CheckBox {
            id: preserveScreenColour
            Layout.columnSpan: 3
            text: i18n('Enabled')
            visible: isMode(['randr', 'vidmode'])
            enabled: parseFloat(versionString) >= 1.11
            onCheckedChanged: modeChanged()
        }
        Label {
            id: lblCustomMode
            text: i18n('Custom mode options:')
            Layout.alignment: Qt.AlignRight
            visible: !isMode([''])
        }
        TextField {
            id: modeString
            Layout.columnSpan: 3
            Layout.preferredWidth: parent.width - lblCustomMode.width*1.1
            enabled: isMode(['Manual'])
            visible: !isMode([''])
            onTextChanged: cfg_renderModeString = text
        }
    }

    function modeChanged() {
        switch (cfg_renderMode) {
        case 'drm':
            modeString.text = '-m drm' + (renderModeCard.text.length > 0 ? ':card=' + renderModeCard.text : '') + (renderModeCrtc.text.length > 0 ? ':crtc=' + renderModeCrtc.text : '')
            break
        case 'randr':
            modeString.text = '-m randr' + (renderModeScreen.text.length > 0 ? ':screen=' + renderModeScreen.text : '') + (renderModeCrtc.text.length > 0 ? ':crtc=' + renderModeCrtc.text : '') + (preserveScreenColour.enabled && preserveScreenColour.checked ? ':preserve=1' : '')
            break
        case 'vidmode':
            modeString.text = '-m vidmode' + (renderModeScreen.text.length > 0 ? ':screen=' + renderModeScreen.text : '') + (preserveScreenColour.enabled && preserveScreenColour.checked ? ':preserve=1' : '')
            break
        default:
            modeString.text = ''
        }
        cfg_renderModeString = modeString.text
    }

    function isMode(modes) {
        var currentMode = modeCombo.model.get(modeCombo.currentIndex).val
        return modes.some(function (iterMode) {
            return currentMode === iterMode
        })
    }

    //     Label {
    //         id: versionStringLabel
    //         text: versionString
    //         anchors.right: parent.right
    //     }
    //     Label {
    //         text: i18n('Redshift version') + ': '
    //         font.bold: true
    //         anchors.right: versionStringLabel.left
    //     }

    PlasmaCore.DataSource {
        id: getOptionsDS
        engine: 'executable'

        connectedSources: ['redshift -V']

        onNewData: {
            connectedSources.length = 0
            if (data['exit code'] > 0) {
                print('Error running redshift with command: ' + sourceName + '   ...stderr: ' + data.stderr)
                return
            }
            versionString = data.stdout.split(' ')[1]
        }
    }
}
