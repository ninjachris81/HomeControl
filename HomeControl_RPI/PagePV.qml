import QtQuick 2.9
import QtQuick.Controls 2.4 as C2
import QtQuick.Controls 1.4 as C1
import QtQuick.Layouts 1.3

import hc 1.0

import "DateFormat.js" as DateFormat

import StyleDef 1.0
import SharedComponents 1.0

HCPage {
    id: root

    title: qsTr("PV")

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 4

        HCValueLabel {
            id: mamps
            Layout.preferredHeight: 30
            Layout.fillWidth: true

            model: DataBridge.pvControllerModel
            modelIndex: Enums.PVS_MAMPS
            unit: "mAmps"
        }

        HCSimpleLabel {
            Layout.preferredHeight: 30
            Layout.fillWidth: true

            isValid: mamps.isValid
            labelText: "Current Power"
            labelValue: mamps.labelValue * 0.230
            unit: "Watt"
            formatAsFloat: true
        }
    }
}
