import QtQuick 2.9
import QtQuick.Controls 2.4 as C2
import QtQuick.Controls 1.4 as C1
import QtQuick.Layouts 1.3

import hc 1.0

import "style"
import "components"

HCPage {
    id: root

    title: qsTr("Security")

    ColumnLayout {
        anchors.fill: parent

        HCValueLabel {
            Layout.preferredHeight: 30
            Layout.fillWidth: true

            model: DataBridge.switchControllerModel
            modelIndex: Enums.SWITCHES_WINTERGARDEN
            unit: ""
        }
    }
}
