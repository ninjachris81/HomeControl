import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.3

//import "SharedLib/qml"

import hc 1.0

import StyleDef 1.0
import SharedComponents 1.0

Window {
    id: root

    visible: true
    width: Style.screenWidth
    height: Style.screenHeight

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        HCValueLabel {
            Layout.preferredHeight: 50
            Layout.fillWidth: true
            Layout.margins: 6

            model: DataBridge.tempControllerModel
            modelIndex: Enums.TEMPS_HC
            unit: "°"
            formatAsFloat: true
            showTrend: true
            labelWidth: 400
            backgroundElement.visible: true
            labelSuffix: ""
        }

        HCValueLabel {
            Layout.preferredHeight: 50
            Layout.fillWidth: true
            Layout.margins: 6

            model: DataBridge.tempControllerModel
            modelIndex: Enums.TEMPS_WATER
            unit: "°"
            formatAsFloat: true
            labelWidth: 400
            showTrend: true
            backgroundElement.visible: true
            labelSuffix: ""
        }

        HCValueLabel {
            Layout.preferredHeight: 50
            Layout.fillWidth: true
            Layout.margins: 6

            model: DataBridge.tempControllerModel
            modelIndex: Enums.TEMPS_TANK
            unit: "°"
            formatAsFloat: true
            labelWidth: 400
            showTrend: true
            trend: 1
            backgroundElement.visible: true
            labelSuffix: ""
        }

        // Preheat infos

        RowLayout {
            spacing: 10
            Layout.margins: 6
            Layout.preferredHeight: 20
            Layout.fillWidth: true

            HCLabel {
                Layout.preferredWidth: 60

                text: qsTr("Preheat")
                fontPointSize: 9
                font.bold: true
            }

            HCValueLabel {
                Layout.fillWidth: true
                Layout.preferredHeight: 20

                model: DataBridge.settingsControllerModel
                modelIndex: Enums.SETTINGS_PREHEAT_HC_TEMP
                unit: "°"
                fontPointSize: 9
                labelWidth: 120
            }

            HCValueLabel {
                Layout.fillWidth: true
                Layout.preferredHeight: 20

                model: DataBridge.settingsControllerModel
                modelIndex: Enums.SETTINGS_PREHEAT_WATER_TEMP
                unit: "°"
                fontPointSize: 9
                labelWidth: 120
            }

            HCValueLabel {
                Layout.fillWidth: true
                Layout.preferredHeight: 20

                model: DataBridge.settingsControllerModel
                modelIndex: Enums.SETTINGS_PREHEAT_FROM
                unit: "h"
                fontPointSize: 9
                labelWidth: 120
            }

            HCValueLabel {
                Layout.fillWidth: true
                Layout.preferredHeight: 20

                model: DataBridge.settingsControllerModel
                modelIndex: Enums.SETTINGS_PREHEAT_TO
                unit: "h"
                fontPointSize: 9
                labelWidth: 120
            }
        }

        RowLayout {
            spacing: 10
            Layout.margins: 6

            HCLabel {
                Layout.preferredWidth: 60

                text: qsTr("Standby")
                fontPointSize: 9
                font.bold: true
            }

            HCValueLabel {
                Layout.preferredHeight: 20
                Layout.fillWidth: true

                model: DataBridge.settingsControllerModel
                modelIndex: Enums.SETTINGS_PREHEAT_HC_STANDBY_TEMP
                unit: "°"
                fontPointSize: 9
                labelWidth: 100
            }


            Item{
                Layout.preferredHeight: 20
                Layout.fillWidth: true
            }

            HCValueLabel {
                Layout.preferredHeight: 20
                Layout.fillWidth: true

                model: DataBridge.settingsControllerModel
                modelIndex: Enums.SETTINGS_PREHEAT_STANDBY_FROM
                unit: "h"
                fontPointSize: 9
                labelWidth: 100
            }

            HCValueLabel {
                Layout.preferredHeight: 20
                Layout.fillWidth: true

                model: DataBridge.settingsControllerModel
                modelIndex: Enums.SETTINGS_PREHEAT_STANDBY_TO
                unit: "h"
                fontPointSize: 9
                labelWidth: 100
            }
        }

        HCDividerH {
            Layout.topMargin: 6
            Layout.fillWidth: true
            Layout.preferredHeight: 2
        }

        Item {
            Layout.fillHeight: true
        }
    }
}
