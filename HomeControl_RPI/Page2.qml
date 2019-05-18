import QtQuick 2.9
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3

import hc 1.0

import "."

HCPage {
    id: root

    title: qsTr("Water Management")

    GroupBox {
        id: groupBox
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 2

        title: qsTr("Preheating")

        ColumnLayout {
            anchors.fill: parent

            RowLayout {
                Button {
                    Layout.preferredHeight: Style.buttonHeight

                    font.pointSize: Style.fontPointSize

                    text: qsTr("Start preheat")

                    onClicked: {
                        console.log("Start preheat " + Enums.CMD_START_PREHEAT)
                        DataBridge.settings.sendCmd(Enums.CMD_START_PREHEAT)
                    }
                }

                HCButton {
                    Layout.preferredHeight: Style.buttonHeight

                    text: qsTr("Stop preheat")

                    onClicked: {
                        console.log("Stop preheat " + Enums.CMD_STOP_PREHEAT)
                        DataBridge.settings.sendCmd(Enums.CMD_STOP_PREHEAT)
                    }
                }
            }

            HCOptionBox {
                id: modeOptionBox

                model: DataBridge.settingsControllerModel
                modelIndex: Enums.SETTINGS_PREHEAT_MODE
                options: [Enums.SETTING_MODE_AUTOMATIC, Enums.SETTING_MODE_MANUAL]
                labels: [qsTr("Automatic"), qsTr("Manual")]
                inputHandler: function(newValue) {
                    DataBridge.settings.setValue(Enums.SETTINGS_PREHEAT_MODE, newValue)
                }
                allowInput: false
            }

            GridLayout {
                columns: 4

                HCValueLabel {
                    Layout.preferredHeight: 30
                    Layout.fillWidth: true

                    model: DataBridge.tempControllerModel
                    modelIndex: Enums.TEMPS_HC
                    unit: "°"
                }

                HCValueLabel {
                    Layout.preferredHeight: 30
                    Layout.fillWidth: true

                    model: DataBridge.tempControllerModel
                    modelIndex: Enums.TEMPS_WATER
                    unit: "°"
                }

                HCValueLabel {
                    Layout.preferredHeight: 30
                    Layout.fillWidth: true

                    model: DataBridge.tempControllerModel
                    modelIndex: Enums.TEMPS_TANK
                    unit: "°"
                }

                Item {
                    Layout.preferredHeight: 30
                    Layout.fillWidth: true
                }

                // next row

                HCValueLabel {
                    Layout.preferredHeight: 30
                    Layout.fillWidth: true

                    model: DataBridge.settingsControllerModel
                    modelIndex: Enums.SETTINGS_PREHEAT_HC_TEMP
                    unit: "°"
                    allowInput: true
                    inputMethodHints: Qt.ImhDigitsOnly
                    inputValidator: IntValidator { bottom: 10; top: 50 }
                    inputHandler: function(thisValue) {
                        DataBridge.settings.setValue(modelIndex, thisValue);
                    }
                }

                HCValueLabel {
                    Layout.preferredHeight: 30
                    Layout.fillWidth: true

                    model: DataBridge.settingsControllerModel
                    modelIndex: Enums.SETTINGS_PREHEAT_WATER_TEMP
                    unit: "°"
                    allowInput: true
                    inputMethodHints: Qt.ImhDigitsOnly
                    inputValidator: IntValidator { bottom: 10; top: 50 }
                    inputHandler: function(thisValue) {
                        DataBridge.settings.setValue(modelIndex, thisValue);
                    }
                }

                HCValueLabel {
                    Layout.preferredHeight: 30
                    Layout.fillWidth: true

                    model: DataBridge.settingsControllerModel
                    modelIndex: Enums.SETTINGS_PREHEAT_FROM
                    unit: "h"
                    allowInput: true
                    inputMethodHints: Qt.ImhDigitsOnly
                    inputValidator: IntValidator { bottom: 0; top: 23 }
                    inputHandler: function(thisValue) {
                        DataBridge.settings.setValue(modelIndex, thisValue);
                    }
                }

                HCValueLabel {
                    Layout.preferredHeight: 30
                    Layout.fillWidth: true

                    model: DataBridge.settingsControllerModel
                    modelIndex: Enums.SETTINGS_PREHEAT_TO
                    unit: "h"
                    allowInput: true
                    inputMethodHints: Qt.ImhDigitsOnly
                    inputValidator: IntValidator { bottom: 0; top: 23 }
                    inputHandler: function(thisValue) {
                        DataBridge.settings.setValue(modelIndex, thisValue);
                    }
                }

                // next row
                HCValueLabel {
                    Layout.preferredHeight: 30
                    Layout.fillWidth: true

                    model: DataBridge.settingsControllerModel
                    modelIndex: Enums.SETTINGS_PREHEAT_HC_STANDBY_TEMP
                    unit: "°"
                    allowInput: true
                    inputMethodHints: Qt.ImhDigitsOnly
                    inputValidator: IntValidator { bottom: 10; top: 50 }
                    inputHandler: function(thisValue) {
                        DataBridge.settings.setValue(modelIndex, thisValue);
                    }
                }


                Item{
                    Layout.preferredHeight: 30
                    Layout.fillWidth: true
                }

                HCValueLabel {
                    Layout.preferredHeight: 30
                    Layout.fillWidth: true

                    model: DataBridge.settingsControllerModel
                    modelIndex: Enums.SETTINGS_PREHEAT_STANDBY_FROM
                    unit: "h"
                    allowInput: true
                    inputMethodHints: Qt.ImhDigitsOnly
                    inputValidator: IntValidator { bottom: 0; top: 23 }
                    inputHandler: function(thisValue) {
                        DataBridge.settings.setValue(modelIndex, thisValue);
                    }
                }

                HCValueLabel {
                    Layout.preferredHeight: 30
                    Layout.fillWidth: true

                    model: DataBridge.settingsControllerModel
                    modelIndex: Enums.SETTINGS_PREHEAT_STANDBY_TO
                    unit: "h"
                    allowInput: true
                    inputMethodHints: Qt.ImhDigitsOnly
                    inputValidator: IntValidator { bottom: 0; top: 23 }
                    inputHandler: function(thisValue) {
                        DataBridge.settings.setValue(modelIndex, thisValue);
                    }
                }



                // next row

                Repeater {
                    id: repeater3
                    model: 2

                    HCValueLabel {
                        id: valueLabel3

                        Layout.preferredHeight: 30
                        Layout.fillWidth: true

                        model: DataBridge.relayControllerModel
                        modelIndex: index
                        unit: ""
                    }
                }
                Item{
                    Layout.preferredHeight: 30
                    Layout.fillWidth: true
                }
                Item{
                    Layout.preferredHeight: 30
                    Layout.fillWidth: true
                }

            }
        }

    }

}
