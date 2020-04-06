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

            format: "hh:ss"
            tickCount: 24
        }

        ValueAxis {
            id: valueAxis
            gridVisible: true
            tickCount: 14
            min: 0
            max: 65
            titleText: ""
        }

        LineSeries {
            id: solarTempSeries

            name: qsTr("Solar Temp")

            axisX: dateAxis
            axisY: valueAxis
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
            DataBridge.dataLogger.solarTempModel.updateSeries(solarTempSeries, dateAxis)
        }
    }
}
