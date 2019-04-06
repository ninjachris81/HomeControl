import QtQuick 2.9
import QtQuick.Controls 2.2

Page {
    id: root

    width: 1024
    height: 600

    title: qsTr("Errors")

    header: Label {
        text: root.title
        font.pixelSize: Qt.application.font.pixelSize * 2
        padding: 10
    }

    ListView {
        id: listView
        anchors.fill: parent
        delegate: Item {
            x: 5
            width: 80
            height: 100
            Row {
                id: row1
                spacing: 10
                Text {
                    text: label
                    font.bold: true
                    anchors.verticalCenter: parent.verticalCenter
                }

                Column {
                    Repeater {
                        model: value
                        Text {
                            text: modelData
                        }
                    }
                }
            }
        }
        model: DataBridge.errorController
    }
}
