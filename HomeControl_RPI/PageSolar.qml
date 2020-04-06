import QtQuick 2.9
import QtQuick.Controls 2.4 as C2
import QtQuick.Controls 1.4 as C1
import QtQuick.Layouts 1.3

import hc 1.0

import "DateFormat.js" as DateFormat

import StyleDef 1.0
import SharedComponents 1.0

HCPage {
    id: root

    title: qsTr("Solar")

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 4

        HCValueLabel {
            Layout.preferredHeight: 30
            Layout.fillWidth: true

            model: DataBridge.brightnessControllerModel
            modelIndex: Enums.BRIGHTNESSES_SOLAR
            unit: ""
        }

        HCValueLabel {
            Layout.preferredHeight: 30
            Layout.fillWidth: true

            showTrend: true

            model: DataBridge.tempControllerModel
            modelIndex: Enums.TEMPS_SOLAR_HC
            unit: "°"
            formatAsFloat: true
        }

        HCDividerH {
            Layout.fillWidth: true
            Layout.preferredHeight: 2
        }

        GridView {
            id: gridView
            Layout.fillWidth: true
            Layout.fillHeight: true

            model: DataBridge.wfcManager.forecast.icons

            delegate: Item {
                width: gridView.width / 8
                height: 50

                Column {
                    Image {
                        source: DataBridge.wfcManager.forecast.icons[index]

                        HCLabel {
                            anchors.left: parent.left
                            anchors.right: parent.right
                            anchors.top: parent.top
                            horizontalAlignment: Text.AlignHCenter

                            text: DataBridge.wfcManager.forecast.temps[index].toFixed(0) + " °"
                            font.pointSize: 8
                        }

                        HCLabel {
                            anchors.left: parent.left
                            anchors.right: parent.right
                            anchors.bottom: parent.bottom
                            horizontalAlignment: Text.AlignHCenter

                            text: DataBridge.wfcManager.forecast.clouds[index].toFixed(0) + " %"
                            font.pointSize: 8
                        }
                    }

                    HCLabel {
                        anchors.left: parent.left
                        anchors.right: parent.right
                        horizontalAlignment: Text.AlignHCenter

                        text: DateFormat.strftime('%H:%M', new Date(DataBridge.wfcManager.forecast.dates[index] * 1000))
                        font.pointSize: 8
                    }

                    HCLabel {
                        anchors.left: parent.left
                        anchors.right: parent.right
                        horizontalAlignment: Text.AlignHCenter

                        text: DateFormat.strftime('%a', new Date(DataBridge.wfcManager.forecast.dates[index] * 1000))
                        font.pointSize: 8
                    }
                }
            }
        }

        Connections {
            target: DataBridge.wfcManager

            onForecastChanged: {
                console.log("FORECAST CHANGED " + DataBridge.wfcManager.forecast.icons.length)
            }
        }

        Component.onCompleted: {
            DataBridge.wfcManager.requestForecast();
        }

    }
}
