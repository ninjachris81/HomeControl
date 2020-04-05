import QtQuick 2.0

Rectangle {
    id: root

    anchors.margins: -4

    border.color: "black"
    border.width: 1

    gradient: Gradient {
        GradientStop { position: 0.0; color: "#ced9eb" }
        GradientStop { position: 1.0; color: "white" }
    }

    radius: 3
}
