import QtQuick 2.9
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3

import hc 1.0

import "."

HCPage {
    id: root

    title: qsTr("Heating")

    GroupBox {
        id: groupBox
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 2

        title: qsTr("Heating")

        ColumnLayout {
            anchors.fill: parent

            RowLayout {
                HCOptionBox {
                    id: heatingModeOptionBox

                    Layout.fillWidth: true

                    model: DataBridge.settingsControllerModel
                    modelIndex: Enums.SETTINGS_HEATING_MODE
                    options: [Enums.SETTING_MODE_AUTOMATIC, Enums.SETTING_MODE_MANUAL]
                    labels: [qsTr("Automatic"), qsTr("Manual")]
                    inputHandler: function(newValue) {
                        DataBridge.settings.setValue(Enums.SETTINGS_HEATING_MODE, newValue)
                    }
                }

                HCSwitchButton {
                    Layout.preferredHeight: 40
                    Layout.fillWidth: true

                    labelText: qsTr("Manual state")

                    model: DataBridge.settingsControllerModel
                    modelIndex: Enums.SETTINGS_HEATING_MANUAL_STATE

                    visible: heatingModeOptionBox.currentValue===Enums.SETTING_MODE_MANUAL
                    inputHandler: function(newValue){
                        DataBridge.settings.setValue(modelIndex, newValue)
                    }
                }
            }

            HCValueLabel {
                id: heatingPumpLabel

                Layout.preferredHeight: 40
                Layout.fillWidth: true

                model: DataBridge.relayControllerModel
                modelIndex: Enums.RELAYS_HEATING_PUMP
                unit: ""
            }

            RowLayout {
                HCValueLabel {
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

                HCValueLabel {
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
