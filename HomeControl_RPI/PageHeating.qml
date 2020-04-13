import QtQuick 2.9
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3

import hc 1.0

import StyleDef 1.0
import SharedComponents 1.0

HCPage {
    id: root

    title: qsTr("Heating")

    HCGroupBox {
        id: groupBox
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right

        ColumnLayout {
            anchors.fill: parent

            spacing: 18

            RowLayout {
                HCOptionBox {
                    id: heatingModeOptionBox

                    Layout.fillWidth: true

                    model: DataBridge.settingsControllerModel
                    modelIndex: Enums.SETTINGS_HEATING_MODE
                    options: [Enums.SETTING_MODE_AUTOMATIC, Enums.SETTING_MODE_MANUAL]
                    labels: [qsTr("Automatic"), qsTr("Manual")]
                    inputHandler: function(newValue) {
                        DataBridge.settings.updateSetting(Enums.SETTINGS_HEATING_MODE, newValue)
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
                        DataBridge.settings.updateSetting(modelIndex, newValue)
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
                    id: heatingTemp

                    Layout.preferredHeight: 30
                    Layout.fillWidth: true

                    model: DataBridge.settingsControllerModel
                    modelIndex: Enums.SETTINGS_HEATING_TEMP
                    unit: "°"

                    allowInput: true
                    inputMethodHints: Qt.ImhDigitsOnly
                    inputValidator: IntValidator { bottom: 10; top: 50 }
                    inputHandler: function(thisValue) {
                        DataBridge.settings.updateSetting(modelIndex, thisValue);
                    }
                }

                HCValueLabel {
                    Layout.preferredHeight: 30
                    Layout.fillWidth: true

                    model: DataBridge.tempControllerModel
                    modelIndex: Enums.TEMPS_INSIDE
                    unit: "°"
                    showTrend: true
                    allowInput: false
                    formatAsFloat: true
                }
            }

            HCDividerH {
                Layout.fillWidth: true
                Layout.preferredHeight: 2
            }

            RowLayout {
                HCValueLabel {
                    id: useToggleLabel

                    Layout.preferredHeight: 30
                    Layout.fillWidth: true

                    model: DataBridge.settingsControllerModel
                    modelIndex: Enums.SETTINGS_HEATING_USE_TOGGLE
                    allowInput: true
                    inputHandler: function(thisValue) {
                        DataBridge.settings.updateSetting(modelIndex, thisValue);
                    }
                }

                HCValueLabel {
                    id: useToggleDurationLabel

                    Layout.preferredHeight: 30
                    Layout.fillWidth: true

                    enabled: useToggleLabel.labelValue

                    model: DataBridge.settingsControllerModel
                    modelIndex: Enums.SETTINGS_HEATING_TOGGLE_ON_DURATION
                    unit: "ms"
                    allowInput: true
                    inputMethodHints: Qt.ImhDigitsOnly
                    inputValidator: IntValidator { bottom: 5000; top: 999999 }
                    inputHandler: function(thisValue) {
                        DataBridge.settings.updateSetting(modelIndex, thisValue);
                    }
                }
            }

            HCDividerH {
                Layout.fillWidth: true
                Layout.preferredHeight: 2
            }

            RowLayout {
                HCValueLabel {
                    Layout.preferredHeight: 30
                    Layout.fillWidth: true

                    model: DataBridge.settingsControllerModel
                    modelIndex: Enums.SETTINGS_HEATING_HOUR_FROM
                    unit: "h"
                    allowInput: true
                    inputMethodHints: Qt.ImhDigitsOnly
                    inputValidator: IntValidator { bottom: 0; top: 23 }
                    inputHandler: function(thisValue) {
                        DataBridge.settings.updateSetting(modelIndex, thisValue);
                    }
                }

                HCValueLabel {
                    Layout.preferredHeight: 30
                    Layout.fillWidth: true

                    model: DataBridge.settingsControllerModel
                    modelIndex: Enums.SETTINGS_HEATING_HOUR_TO
                    unit: "h"
                    allowInput: true
                    inputMethodHints: Qt.ImhDigitsOnly
                    inputValidator: IntValidator { bottom: 0; top: 23 }
                    inputHandler: function(thisValue) {
                        DataBridge.settings.updateSetting(modelIndex, thisValue);
                    }
                }
            }

            HCDividerH {
                Layout.fillWidth: true
                Layout.preferredHeight: 2
            }

            HCLabel {
                text: qsTr("Month from")
            }

            HCOptionBoxMonth {
                Layout.preferredHeight: 30
                Layout.fillWidth: true

                model: DataBridge.settingsControllerModel
                modelIndex: Enums.SETTINGS_HEATING_MONTH_FROM
                allowInput: true
                inputHandler: function(thisValue, index) {
                    DataBridge.settings.updateSetting(modelIndex, thisValue);
                }
            }

            HCLabel {
                text: qsTr("Month to")
            }

            HCOptionBoxMonth {
                Layout.preferredHeight: 30
                Layout.fillWidth: true

                model: DataBridge.settingsControllerModel
                modelIndex: Enums.SETTINGS_HEATING_MONTH_TO
                allowInput: true
                inputHandler: function(thisValue, index) {
                    DataBridge.settings.updateSetting(modelIndex, thisValue);
                }
            }
        }
    }
}
