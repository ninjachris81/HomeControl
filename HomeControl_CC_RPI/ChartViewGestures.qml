import QtQuick 2.0
import QtCharts 2.3

MouseArea{
    id: root

    anchors.fill: root.parent
    onDoubleClicked: {
        if (root.parent.isZoomed()) {
            root.parent.zoomReset();
        } else {
            var height = 200
            var width = 200
            var rect = Qt.rect(mouse.x - (width/2), mouse.y - (height/2), width, height)
            root.parent.zoomIn(rect);
        }
    }
}

/*
PinchArea {
    anchors.fill: parent

    onPinchFinished: {
        var rect = pinch.point1
        chartView.zoomReset()
        chartView.zoom(Qt.rect(50, 50, 100, 100))
    }
}*/
