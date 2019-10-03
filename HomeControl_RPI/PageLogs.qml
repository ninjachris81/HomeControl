import QtQuick 2.9
import QtQuick.Controls 2.4 as C2
import QtQuick.Controls 1.4 as C1
import QtQuick.Layouts 1.3

import "style"
import "components"

HCPage {
    id: root

    title: qsTr("Logs")

    ColumnLayout {
        anchors.fill: parent

        anchors.margins: 5

        RowLayout {
            Layout.fillWidth: true
            Layout.preferredHeight: Style.buttonHeight

            C2.ComboBox {
                id: logFilter

                Layout.preferredHeight: Style.buttonHeight
                Layout.preferredWidth: 200

                font.pointSize: Style.fontPointSize

                currentIndex: 0
                model: [qsTr("All"), qsTr("Info"), qsTr("Error"), qsTr("Startup")]

                onCurrentIndexChanged: {
                    DataBridge.logListModel.setTypeFilter(currentIndex)
                }
            }

            C2.ComboBox {
                id: sourceFilter

                Layout.preferredHeight: Style.buttonHeight
                Layout.preferredWidth: 200

                font.pointSize: Style.fontPointSize

                currentIndex: 0
                model: [qsTr("All"), "SERV", "TERM", "4RLB", "WTB1", "ITB1", "SHRC", "SSEN"]

                onCurrentTextChanged: {
                    if (currentIndex==0) {
                        DataBridge.logListModel.setSourceFilter("")
                    } else {
                        DataBridge.logListModel.setSourceFilter(sourceFilter.currentText)
                    }
                }
            }

            C2.Button {
                Layout.preferredHeight: Style.buttonHeight
                Layout.preferredWidth: 100

                font.pointSize: Style.fontPointSize

                text: qsTr("Refresh")

                onClicked: {
                    DataBridge.logs.refreshLog()
                }
            }

            C2.Button {
                Layout.preferredHeight: Style.buttonHeight
                Layout.preferredWidth: 100

                font.pointSize: Style.fontPointSize

                text: qsTr("Clear")

                onClicked: {
                    DataBridge.logs.clearLog(logFilter.currentIndex)
                }
            }
        }

        C1.TableView {
            id: tableView

            Layout.fillHeight: true
            Layout.fillWidth: true

            C1.TableViewColumn {
                role: "col_date"
                title: "Date"
                width: 200
            }
            C1.TableViewColumn {
                role: "col_type"
                title: "Type"
                width: 150
            }
            C1.TableViewColumn {
                role: "col_source"
                title: "Source"
                width: 50
            }
            C1.TableViewColumn {
                role: "col_msg"
                title: "Message"
                width: 400
            }

            model: DataBridge.logListModel
        }
    }
}
