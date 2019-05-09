import QtQuick 2.9
import QtQuick.Controls 2.4
import QtQuick.Controls 1.4

Page {
    id: root

    width: 1024
    height: 600

    title: qsTr("Logs")

    header: Label {
        text: root.title
        font.pixelSize: Qt.application.font.pixelSize * 2
        padding: 10
    }

    TableView {
        id: tableView
        anchors.fill: parent

        TableViewColumn {
            role: "col_date"
            title: "Date"
            width: 200
        }
        TableViewColumn {
            role: "col_type"
            title: "Type"
            width: 150
        }
        TableViewColumn {
            role: "col_msg"
            title: "Message"
            width: 400
        }

        model: DataBridge.logListModel
    }
}
