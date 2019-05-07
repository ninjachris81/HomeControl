import QtQuick 2.9
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3

import hc 1.0

Page {
    id: root

    width: 1024
    height: 600

    title: qsTr("Heating")

    header: Label {
        id: label
        width: 600
        height: 47
        text: root.title
        font.pixelSize: Qt.application.font.pixelSize * 2
        padding: 10
    }

    GroupBox {
        id: groupBox
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 2

        title: qsTr("Preheating")

        ColumnLayout {
            anchors.fill: parent

            Button {
                text: qsTr("Start preheat")

                onClicked: {
                    console.log("Start preheat")
                    DataBridge.settings.sendCmd(Enums.MQTT_CMD_START_PREHEAT)
                }
            }

            OptionBox {
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

                Repeater {
                    id: repeater
                    model: 3

                    ValueLabel {
                        id: valueLabel

                        Layout.preferredHeight: 30
                        Layout.fillWidth: true

                        model: DataBridge.tempControllerModel
                        modelIndex: index
                        unit: "°"
                    }
                }
                Item {
                    Layout.preferredHeight: 30
                    Layout.fillWidth: true
                }

                // next row

                ValueLabel {
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

                ValueLabel {
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

                ValueLabel {
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

                ValueLabel {
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


                // next row

                Repeater {
                    id: repeater3
                    model: 2

                    ValueLabel {
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

        /*
        ListView {
            id: listView
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            height: 300

            delegate: Item {
                x: 5
                width: 80
                height: 40
                Row {
                    id: row1
                    spacing: 10
                    Text {
                        text: label
                        font.bold: true
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    Text {
                        text: value
                        font.bold: true
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
            }
            model: DataBridge.tempControllerModel
        }*/
    }

    GroupBox {
        id: groupBox2
        anchors.top: groupBox.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 2

        title: qsTr("Heating")

        ColumnLayout {
            anchors.fill: parent

            OptionBox {
                id: heatingModeOptionBox

                model: DataBridge.settingsControllerModel
                modelIndex: Enums.SETTINGS_HEATING_MODE
                options: [Enums.SETTING_MODE_AUTOMATIC, Enums.SETTING_MODE_MANUAL]
                labels: [qsTr("Automatic"), qsTr("Manual")]
                inputHandler: function(newValue) {
                    DataBridge.settings.setValue(Enums.SETTINGS_HEATING_MODE, newValue)
                }
            }

            SwitchButton {
                Layout.preferredHeight: 40

                model: DataBridge.settingsControllerModel
                modelIndex: Enums.SETTINGS_HEATING_MANUAL_STATE

                allowInput: heatingModeOptionBox.currentValue===Enums.SETTING_MODE_MANUAL
                inputHandler: function(newValue){
                    DataBridge.settings.setValue(modelIndex, newValue)
                }
            }

            RowLayout {
                ValueLabel {
                    Layout.preferredHeight: 30
                    Layout.fillWidth: true

                    model: DataBridge.settingsControllerModel
                    modelIndex: Enums.SETTINGS_HEATING_TEMP
                    unit: "°"
                    allowInput: true
                    inputMethodHints: Qt.ImhDigitsOnly
                    inputValidator: IntValidator { bottom: 10; top: 50 }
                    inputHandler: function(thisValue) {
                        DataBridge.settings.setValue(modelIndex, thisValue);
                    }
                }

                ValueLabel {
                    Layout.preferredHeight: 30
                    Layout.fillWidth: true

                    model: DataBridge.tempControllerModel
                    modelIndex: Enums.TEMPS_INSIDE
                    unit: "°"
                    allowInput: false
                }
            }

        }
    }
}