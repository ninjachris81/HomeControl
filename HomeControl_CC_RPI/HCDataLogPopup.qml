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

        legend.alignment: Qt.AlignBottom

        BarSeries {
            id: hcBarSeries

            BarCategoryAxis {
                id: hcCatAxis
            }

            ValueAxis {
                id: hcValAxis
                titleText: qsTr("Minutes")
                titleFont.pointSize: 14
                min: 0
            }

            axisX: hcCatAxis
            axisY: hcValAxis

            BarSet {
                id: hcBarSet
                label: qsTr("HC Duration")
            }
        }
    }

    Timer {
        repeat: true
        running: root.isActive
        interval: 30000
        triggeredOnStart: true

        onTriggered:  {
            DataBridge.dataLogger.hcPumpModel.refresh();

            DataBridge.dataLogger.hcPumpModel.updateBarSeries(hcValAxis, hcCatAxis, hcBarSet)
            hcValAxis.applyNiceNumbers()
        }

        Component.onCompleted: {
            DataBridge.dataLogger.hcPumpModel.daysLimit = 7
        }
    }
}
