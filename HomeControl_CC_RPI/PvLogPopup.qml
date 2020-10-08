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
            min: 0
            max: 1500
            titleText: ""
        }

        LineSeries {
            id: mampsSeries

            name: qsTr("PV")

            axisX: dateAxis
            axisY: valueAxis
            pointsVisible: true
        }

        LineSeries {
            id: currentBasementSeries

            name: qsTr("Basement")

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
            DataBridge.dataLogger.pvModel.refresh();
            DataBridge.dataLogger.currentBasementModel.refresh();

            DataBridge.dataLogger.pvModel.updateXYSeries(mampsSeries, dateAxis)
            DataBridge.dataLogger.currentBasementModel.updateXYSeries(currentBasementSeries, null)
        }
    }

    Component.onCompleted: {
        DataBridge.dataLogger.pvModel.daysLimit = 3
        DataBridge.dataLogger.currentBasementModel.daysLimit = 3
    }
}
