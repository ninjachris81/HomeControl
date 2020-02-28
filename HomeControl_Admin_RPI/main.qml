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
            showTrend: true
            trend: 1
            backgroundElement.visible: true
            labelSuffix: ""
        }

        HCValueLabel {
            Layout.preferredHeight: 50
            Layout.fillWidth: true
            Layout.margins: 6

            model: DataBridge.tempControllerModel
            modelIndex: Enums.TEMPS_SOLAR_HC
            unit: "°"
            formatAsFloat: true
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
            }

            HCValueLabel {
                Layout.fillWidth: true
                Layout.preferredHeight: 20

                model: DataBridge.settingsControllerModel
                modelIndex: Enums.SETTINGS_PREHEAT_WATER_TEMP
                unit: "°"
                fontPointSize: 9
            }

            HCValueLabel {
                Layout.fillWidth: true
                Layout.preferredHeight: 20

                model: DataBridge.settingsControllerModel
                modelIndex: Enums.SETTINGS_PREHEAT_FROM
                unit: "h"
                fontPointSize: 9
            }

            HCValueLabel {
                Layout.fillWidth: true
                Layout.preferredHeight: 20

                model: DataBridge.settingsControllerModel
                modelIndex: Enums.SETTINGS_PREHEAT_TO
                unit: "h"
                fontPointSize: 9
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
            }

            HCValueLabel {
                Layout.preferredHeight: 20
                Layout.fillWidth: true

                model: DataBridge.settingsControllerModel
                modelIndex: Enums.SETTINGS_PREHEAT_STANDBY_TO
                unit: "h"
                fontPointSize: 9
            }
        }

        HCDividerH {
            Layout.topMargin: 6
            Layout.fillWidth: true
            Layout.preferredHeight: 2
        }

        HCSpinBox {
            Layout.preferredHeight: 30
            Layout.fillWidth: true

            from: -20
            to: 20

            model: DataBridge.settingsControllerModel
            modelIndex: Enums.SETTINGS_TANK_OFFSET

            //value: DataBridge.settingsControllerModel.data(DataBridge.settingsControllerModel.index(Enums.SETTINGS_TANK_OFFSET, 0), 258)
            textFromValue: function(value, locale) {
                return Number(value).toLocaleString(locale, "f", 1) + " °";
            }

            onValueModified: {
                DataBridge.settings.updateSetting(Enums.SETTINGS_TANK_OFFSET, value)
            }
        }

        Item {
            Layout.fillHeight: true
        }

        RowLayout {
            spacing: 0
            Layout.margins: 10
            Layout.preferredHeight: 30
            Layout.fillWidth: true

            HCLabel {
                id: currentDateTime

                Layout.preferredWidth: 580
                Layout.preferredHeight: 30

                color: DataBridge.infos.timeIsOffset ? "red" : Style.fontColor
                text: ""
                fontPointSize: 12

                Timer {
                    id: timer
                    interval: 1000
                    repeat: true
                    running: true
                    triggeredOnStart: true

                    onTriggered: {
                        currentDateTime.text =  Qt.formatDateTime(new Date(), Qt.SystemLocaleDate)
                    }
                }
            }

            HCSimpleLabel {
                Layout.fillWidth: true
                Layout.preferredHeight: 30

                labelValue: DataBridge.infos.systemTemp
                unit: "°"
                formatAsFloat: true
                fontPointSize: 12
                isValid: true
                showTrend: false
                labelText: qsTr("System Temperature")
            }
        }
    }
}
