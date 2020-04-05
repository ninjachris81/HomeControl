import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtCharts 2.3

import StyleDef 1.0
import SharedComponents 1.0
import SharedUtils 1.0

import hc 1.0

ColumnLayout {

    ChartView {
        id: chartView
        Layout.fillHeight: true
        Layout.fillWidth: true

        /*
        LineSeries {
            id: lineSeries
            name: qsTr("Tank Schedule")
        }*/

        ValueAxis {
            id: axisY
            gridVisible: true
            tickCount: 14
            min: 0
            max: 65
            titleText: ""
        }

        ValueAxis  {
            id: axisX
            min: 0
            max: 24
            gridVisible: true
            tickCount: 24
            labelFormat: "%d"
            titleText: qsTr("Hour of day")
        }
    }

    Connections {
        target: DataBridge.infos

        onTankTempScheduleReady: {
            console.log("RELOAD MODEL " + DataBridge.infos.tankTempSchedule)
            console.log("RELOAD MODEL " + DataBridge.infos.tankTempSchedule.length)

            chartView.removeAllSeries();
            var line = chartView.createSeries(ChartView.SeriesTypeLine, qsTr("Tank Target Temperature"), axisX, axisY);
            line.width = 4

            for (var i=0;i<DataBridge.infos.tankTempSchedule.length;i++) {
                console.log("APPEND " + DataBridge.infos.tankTempSchedule[i])
                line.append(i, DataBridge.infos.tankTempSchedule[i])
            }

            line.append(i, DataBridge.infos.tankTempSchedule[23])
            //axisY.applyNiceNumbers()
        }

    }

    /*
    HCSpinBox {
        Layout.preferredHeight: 60
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

        up.indicator.width: 100
        down.indicator.width: 100
    }
    */

    /*
    HCSwitchIndicator {
        Layout.preferredHeight: 60
        Layout.preferredWidth: 60

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
    }*/

    /*
    RowLayout {
        Layout.fillWidth: true
        Layout.preferredHeight: 200

        Repeater {
            id: itemRep

            model: 24

            Rectangle {
                color: "red"

                border.width: 1
                border.color: "black"

                radius: 4

                Layout.preferredHeight: DataBridge.infos.tankTempSchedule[index] * 3
                Layout.alignment: Qt.AlignBottom
                Layout.fillWidth: true

                Text {
                    anchors.bottom: parent.top
                    anchors.bottomMargin: 10
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.pointSize: 20
                    text: DataBridge.infos.tankTempSchedule[index] + "°"
                }

                Text {
                    anchors.top: parent.bottom
                    anchors.topMargin: 10
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.pointSize: 20
                    text: index
                }
            }
        }
    }


    Connections {
        target: DataBridge.infos

        onTankTempScheduleChanged: {
            console.log("RELOAD MODEL " + DataBridge.infos.tankTempSchedule)
            itemRep.model = 0
            itemRep.model = 24
        }

    }*/

    HCSimpleLabel {
        Layout.preferredHeight: 60
        Layout.fillWidth: true
        Layout.margins: 6

        labelText: qsTr("Average Temp")
        labelValue: DataBridge.infos.avgTempForecast
        unit: "°"
        labelSuffix: ""
        isValid: true
    }

    HCSimpleLabel {
        Layout.preferredHeight: 60
        Layout.fillWidth: true
        Layout.margins: 6

        labelText: qsTr("Average Clouds")
        labelValue: DataBridge.infos.avgCloudsForecast
        unit: "%"
        labelSuffix: ""
        isValid: true
    }

}
