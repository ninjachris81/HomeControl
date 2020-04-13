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
            id: heatingBarSeries

            BarCategoryAxis {
                id: heatingCatAxis
            }

            ValueAxis {
                id: heatingValAxis
                titleText: qsTr("Minutes")
                titleFont.pointSize: 14
                min: 0
            }

            axisX: heatingCatAxis
            axisY: heatingValAxis

            BarSet {
                id: hcBarSet
                label: qsTr("Heating Duration")
            }
        }
    }

    Timer {
        repeat: true
        running: root.isActive
        interval: 30000
        triggeredOnStart: true

        onTriggered:  {
            DataBridge.dataLogger.heatingPumpModel.refresh();

            DataBridge.dataLogger.heatingPumpModel.updateBarSeries(heatingValAxis, heatingCatAxis, hcBarSet)
            heatingValAxis.applyNiceNumbers()
        }

        Component.onCompleted: {
            DataBridge.dataLogger.heatingPumpModel.daysLimit = 7
        }
    }
}
