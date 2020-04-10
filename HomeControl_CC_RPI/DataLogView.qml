import QtQuick 2.0
import QtCharts 2.3
import QtQuick.Layouts 1.3

ColumnLayout {
    id: root

    property bool isActive: false

    ChartView {
        id: chartView

        Layout.fillHeight: true
        Layout.fillWidth: true

        legend.font.pointSize: 14

        DateTimeAxis {
            id: dateAxis

            format: "hh:mm"
            tickCount: 12
        }

        ValueAxis {
            id: valueAxis
            gridVisible: true
            tickCount: 14
            min: 5
            max: 70
            titleText: ""
        }

        LineSeries {
            id: hcTempSeries

            name: qsTr("HC Temp")

            axisX: dateAxis
            axisY: valueAxis
            pointsVisible: true
        }

        LineSeries {
            id: insideTempSeries

            name: qsTr("Inside Temp")

            axisX: dateAxis
            axisY: valueAxis
            pointsVisible: true
        }

        LineSeries {
            id: tankTempSeries

            name: qsTr("Tank Temp")

            axisX: dateAxis
            axisY: valueAxis
            pointsVisible: true
        }

        ChartViewGestures {
        }
    }

    Timer {
        repeat: true
        running: root.isActive
        interval: 30000
        triggeredOnStart: true

        onTriggered:  {
            DataBridge.dataLogger.hcTempModel.refresh();
            DataBridge.dataLogger.tankTempModel.refresh();
            DataBridge.dataLogger.insideTempModel.refresh();

            DataBridge.dataLogger.hcTempModel.updateSeries(hcTempSeries, null)
            DataBridge.dataLogger.insideTempModel.updateSeries(insideTempSeries, null)
            DataBridge.dataLogger.tankTempModel.updateSeries(tankTempSeries, dateAxis)
        }
    }
}
