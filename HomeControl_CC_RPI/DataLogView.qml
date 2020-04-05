import QtQuick 2.0
import QtCharts 2.3
import QtQuick.Layouts 1.3

import QtQuick.Controls 1.4 as C1

ColumnLayout {
    id: root

    //anchors.fill: parent

    property bool isActive: false

    ChartView {
        id: chartView

        Layout.fillHeight: true
        Layout.fillWidth: true

        DateTimeAxis {
            id: dateAxis

            //format: "hh:ss"
            //tickCount:100
        }

        /*
        ValueAxis {
            id: dateAxis
        }*/

        ValueAxis {
            id: valueAxis
            gridVisible: true
            tickCount: 14
            min: 0
            max: 65
            titleText: ""
        }

        LineSeries {
            id: hcTempSeries

            name: qsTr("HC Temp")

            axisX: dateAxis
            axisY: valueAxis
        }

        LineSeries {
            id: tankTempSeries

            name: qsTr("Tank Temp")

            axisX: dateAxis
            axisY: valueAxis
        }
    }

    Timer {
        repeat: true
        running: root.visible
        interval: 2000

        onTriggered:  {
            console.log(DataBridge.dataLogger.hcTempModel.rowCount())
            console.log(DataBridge.dataLogger.tankTempModel.rowCount())


            DataBridge.dataLogger.hcTempModel.refresh();
            DataBridge.dataLogger.tankTempModel.refresh();

            DataBridge.dataLogger.hcTempModel.updateSeries(hcTempSeries, null)
            DataBridge.dataLogger.tankTempModel.updateSeries(tankTempSeries, dateAxis)
        }
    }
}
