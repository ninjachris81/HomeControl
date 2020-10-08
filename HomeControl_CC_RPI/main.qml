import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.3

import hc 1.0

import StyleDef 1.0
import SharedComponents 1.0
import SharedUtils 1.0

Window {
    id: root

    visible: true
    width: Style.screenWidth
    height: Style.screenHeight

    readonly property int buttonHeight: 88

    InfoPopup {
        id: tankInfoPopup

        contentItem: TankInfoPopup {
        }
    }

    InfoPopup {
        id: dataLogPopup

        contentItem: DataLogPopup {
            isActive: dataLogPopup.opened
        }
    }

    InfoPopup {
        id: solarLogPopup

        contentItem: SolarLogPopup {
            isActive: solarLogPopup.opened
        }
    }

    InfoPopup {
        id: hcLogPopup

        contentItem: HCDataLogPopup {
            isActive: hcLogPopup.opened
        }
    }

    InfoPopup {
        id: heatingLogPopup

        contentItem: HeatingLogPopup {
            isActive: heatingLogPopup.opened
        }
    }

    InfoPopup {
        id: pvLogPopup

        contentItem: PvLogPopup {
            isActive: pvLogPopup.opened
        }
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 4

        // 1st row
        HCGroupBox {
            Layout.preferredHeight: root.buttonHeight + 12
            Layout.fillWidth: true

            RowLayout {
                anchors.fill: parent
                spacing: 0

                HCValueLabel {
                    Layout.preferredHeight: root.buttonHeight
                    Layout.fillWidth: true
                    Layout.margins: 6

                    model: DataBridge.tempControllerModel
                    modelIndex: Enums.TEMPS_HC
                    unit: "°"
                    formatAsFloat: true
                    showTrend: true
                    labelSuffix: ""
                }

                HCSwitchIndicator {
                    Layout.preferredHeight: root.buttonHeight
                    Layout.preferredWidth: root.buttonHeight

                    style: "pump"

                    isOn: hcPumpBinding.value

                    HCModelValueBool {
                        id: hcPumpBinding

                        model: DataBridge.relayControllerModel
                        modelIndex: Enums.RELAYS_HC_PUMP
                    }
                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    hcLogPopup.open()
                }
            }
        }

        // 2nd row
        HCGroupBox {
            Layout.preferredHeight: root.buttonHeight + 12
            Layout.fillWidth: true

            RowLayout {
                anchors.fill: parent
                spacing: 0

                HCValueLabel {
                    Layout.preferredHeight: root.buttonHeight
                    Layout.preferredWidth: 600
                    Layout.margins: 6

                    model: DataBridge.tempControllerModel
                    modelIndex: Enums.TEMPS_TANK
                    unit: "°"
                    formatAsFloat: true
                    showTrend: true
                    labelSuffix: ""
                }

                HCSimpleLabel {
                    Layout.preferredHeight: root.buttonHeight
                    Layout.fillWidth: true
                    Layout.margins: 6

                    labelText: qsTr("Boiler Target Temp")
                    labelValue: DataBridge.infos.boilerTargetTemp
                    unit: "°"
                    formatAsFloat: true
                    labelSuffix: ""
                    isValid: true
                }

                HCSwitchIndicator {
                    Layout.preferredHeight: root.buttonHeight
                    Layout.preferredWidth: root.buttonHeight

                    style: "flame"

                    isOn: boilerOnBinding.value

                    HCModelValueBool {
                        id: boilerOnBinding

                        model: DataBridge.relayControllerModel
                        modelIndex: Enums.RELAYS_BOILER
                    }

                    Image {
                        width: 32
                        height: 32

                        anchors.top: parent.top
                        anchors.right: parent.right

                        source: "qrc:/images/icons/" +
                                (DataBridge.infos.sunExpected==Enums.BOOL_TRUE ? "sun.png" :
                                        DataBridge.infos.sunExpected==Enums.BOOL_FALSE ? "moon.png" : "hour_glass.png")
                    }
                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    tankInfoPopup.open()
                }
            }
        }

        // 3rd row
        HCGroupBox {
            Layout.preferredHeight: root.buttonHeight + 12
            Layout.fillWidth: true

            RowLayout {
                anchors.fill: parent

                HCValueLabel {
                    Layout.preferredHeight: root.buttonHeight
                    Layout.preferredWidth: Style.screenWidth / 2
                    Layout.margins: 6

                    model: DataBridge.tempControllerModel
                    modelIndex: Enums.TEMPS_SOLAR_HC
                    unit: "°"
                    formatAsFloat: true
                    showTrend: true
                    labelSuffix: ""
                }

                HCValueLabel {
                    Layout.preferredHeight: root.buttonHeight
                    Layout.fillWidth: true

                    model: DataBridge.brightnessControllerModel
                    modelIndex: Enums.BRIGHTNESSES_SOLAR
                    unit: "%"
                    showTrend: true
                    labelSuffix: ""
                    conversionFunction: function(v) { if (v && v>0) { return ((v / 4096) * 100).toFixed(0); } else { return 0}};       // 12-bit
                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    solarLogPopup.open()
                }
            }
        }

        HCGroupBox {
            Layout.preferredHeight: root.buttonHeight + 12
            Layout.fillWidth: true

            RowLayout {
                anchors.fill: parent

                HCValueLabel {
                    id: heatingTemp

                    Layout.preferredHeight: root.buttonHeight
                    Layout.preferredWidth: Style.screenWidth / 2
                    Layout.margins: 6

                    model: DataBridge.settingsControllerModel
                    modelIndex: Enums.SETTINGS_HEATING_TEMP
                    unit: "°"
                    formatAsFloat: true
                    showTrend: true
                    labelSuffix: ""
                }

                HCValueLabel {
                    Layout.preferredHeight: root.buttonHeight
                    Layout.fillWidth: true

                    model: DataBridge.tempControllerModel
                    modelIndex: Enums.TEMPS_INSIDE
                    unit: "°"
                    formatAsFloat: true
                    showTrend: true
                    labelSuffix: ""
                }

                HCSwitchIndicator {
                    Layout.preferredHeight: root.buttonHeight
                    Layout.preferredWidth: root.buttonHeight

                    style: "pump"

                    isOn: heatingPumpBinding.value

                    HCModelValueBool {
                        id: heatingPumpBinding

                        model: DataBridge.relayControllerModel
                        modelIndex: Enums.RELAYS_HEATING_PUMP
                    }
                }
            }

            MouseArea {
                anchors.fill: parent

                onClicked: {
                    heatingLogPopup.open()
                }
            }
        }



        HCGroupBox {
            Layout.preferredHeight: root.buttonHeight + 12
            Layout.fillWidth: true

            RowLayout {
                anchors.fill: parent

                HCValueLabel {
                    id: pvMamps

                    Layout.preferredHeight: root.buttonHeight
                    Layout.preferredWidth: Style.screenWidth / 2
                    Layout.margins: 6

                    model: DataBridge.pvControllerModel
                    modelIndex: Enums.PVS_MAMPS
                    unit: "W"
                    unitSpacing: " "
                    showTrend: true
                    labelSuffix: ""
                    formatAsFloat: true
                    labelText: "PV"
                    conversionFunction: function(val) { return val * .230 }
                }

                HCValueLabel {
                    id: basementMamps

                    Layout.preferredHeight: root.buttonHeight
                    Layout.fillWidth: true

                    model: DataBridge.currentControllerModel
                    modelIndex: Enums.CURRENTS_MAIN_BASEMENT
                    unit: "W"
                    unitSpacing: " "
                    showTrend: true
                    formatAsFloat: true
                    labelSuffix: ""
                    conversionFunction: function(val) { return val * .230 }
                }
            }

            MouseArea {
                anchors.fill: parent

                onClicked: {
                    pvLogPopup.open()
                }
            }
        }

        HCGroupBox {
            Layout.preferredHeight: root.buttonHeight + 12
            Layout.fillWidth: true

            RowLayout {
                anchors.fill: parent

                HCValueLabel {
                    Layout.preferredHeight: root.buttonHeight
                    Layout.preferredWidth: Style.screenWidth / 2
                    Layout.margins: 6

                    model: DataBridge.tempControllerModel
                    modelIndex: Enums.TEMPS_OUTSIDE
                    unit: "°"
                    formatAsFloat: true
                    showTrend: true
                    labelSuffix: ""
                }

                HCValueLabel {
                    Layout.preferredHeight: root.buttonHeight
                    Layout.fillWidth: true

                    model: DataBridge.humidityControllerModel
                    modelIndex: Enums.HUMIDITIES_OUTSIDE
                    unit: ""
                    formatAsFloat: true
                    showTrend: true
                    labelSuffix: ""
                }
            }
        }


        HCGroupBox {
            Layout.preferredHeight: root.buttonHeight + 12
            Layout.fillWidth: true

            HCButton {
                anchors.fill: parent

                text: qsTr("Statistics")

                onClicked: {
                    dataLogPopup.open()
                }
            }
        }

        Item {
            Layout.fillHeight: true
        }

        // footer
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

            Item {
                Layout.fillWidth: true
            }

            HCSimpleLabel {
                Layout.preferredWidth: 400
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
