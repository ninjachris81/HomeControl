import QtQuick 2.0
import QtQuick.Controls 2.12

import StyleDef 1.0
import SharedComponents 1.0
import SharedUtils 1.0

Popup {
    id: root

    modal: true
    anchors.centerIn: parent

    width: Style.screenWidth - 100
    height: Style.screenHeight - 80

    topPadding: closeButton.height + (closeButton.anchors.topMargin * 2)

    background: Rectangle {
        border.color: "black"
        border.width: 1

        gradient: Gradient {
            GradientStop { position: 0.0; color: "#ada989" }
            GradientStop { position: 1.0; color: "white" }
        }

        radius: 3

        HCButton {
            id: closeButton
            anchors.right: parent.right
            anchors.rightMargin: 4
            anchors.top: parent.top
            anchors.topMargin: 4

            height: 60
            width: 300

            text: qsTr("Close")

            onClicked: root.close()
        }
    }

}
