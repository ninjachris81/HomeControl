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
            id: solarTempSeries

            name: qsTr("Solar Temp")

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
            DataBridge.dataLogger.solarTempModel.refresh();
            DataBridge.dataLogger.solarTempModel.updateXYSeries(solarTempSeries, dateAxis)
        }
    }
}
