import QtQuick 2.9
import QtQuick.Controls 2.4

Page {
    id: root

    width: 1024
    height: 600

    title: qsTr("Water Management")

    header: Label {
        text: root.title
        font.pixelSize: Qt.application.font.pixelSize * 2
        padding: 10
    }
}
