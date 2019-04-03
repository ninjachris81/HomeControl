import QtQuick 2.9
import QtQuick.Controls 2.2

Page {
    id: root

    width: 1024
    height: 600

    title: qsTr("Heating")

    header: Label {
        id: label
        width: 600
        height: 47
        text: root.title
        font.pixelSize: Qt.application.font.pixelSize * 2
        padding: 10
    }
}
