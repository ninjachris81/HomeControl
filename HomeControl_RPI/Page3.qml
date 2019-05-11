import QtQuick 2.9
import QtQuick.Controls 2.4
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3

Page {
    id: root

    width: 1024
    height: 600

    title: qsTr("Logs")

    ColumnLayout {
        anchors.fill: parent

        anchors.margins: 5

        RowLayout {
            Layout.fillWidth: true
            Layout.preferredHeight: 30

            ComboBox {
                id: logFilter

                Layout.preferredHeight: 30
                Layout.preferredWidth: 200

                currentIndex: 0
                model: [qsTr("All"), qsTr("Info"), qsTr("Error"), qsTr("Startup")]

                onCurrentIndexChanged: {
                    DataBridge.logListModel.setTypeFilter(currentIndex)
                }
            }

            Button {
                Layout.preferredHeight: 30
                Layout.preferredWidth: 100

                text: qsTr("Refresh")

                onClicked: {
                    DataBridge.logs.refreshLog()
                }
            }

            Button {
                Layout.preferredHeight: 30
                Layout.preferredWidth: 100

                text: qsTr("Clear")

                onClicked: {
                    DataBridge.logs.clearLog(logFilter.currentIndex)
                }
            }
        }

        TableView {
            id: tableView

            Layout.fillHeight: true
            Layout.fillWidth: true

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
}
