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

            format: "ddd hh:mm"
            tickCount: 14
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

        property date minDate
        property date maxDate

        onTriggered:  {
            DataBridge.dataLogger.hcTempModel.refresh();
            DataBridge.dataLogger.tankTempModel.refresh();
            DataBridge.dataLogger.insideTempModel.refresh();

            DataBridge.dataLogger.hcTempModel.updateXYSeries(hcTempSeries, null)
            DataBridge.dataLogger.insideTempModel.updateXYSeries(insideTempSeries, null)
            DataBridge.dataLogger.tankTempModel.updateXYSeries(tankTempSeries, null)

            minDate = new Date()
            maxDate = new Date(0)

            checkModel(DataBridge.dataLogger.hcTempModel)
            checkModel(DataBridge.dataLogger.insideTempModel)
            checkModel(DataBridge.dataLogger.tankTempModel)

            dateAxis.min = minDate
            dateAxis.max = maxDate
        }

        function checkModel(model) {
            if (model.minDate<minDate) minDate = model.minDate
            if (model.maxDate>maxDate) maxDate = model.maxDate
        }
    }

    Component.onCompleted: {
        DataBridge.dataLogger.hcTempModel.daysLimit = 7
        DataBridge.dataLogger.tankTempModel.daysLimit = 7
        DataBridge.dataLogger.insideTempModel.daysLimit = 7
    }
}
