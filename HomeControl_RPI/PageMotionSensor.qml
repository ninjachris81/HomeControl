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

    title: qsTr("Motion Sensor")

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 4

        HCValueLabel {
            id: pirSwitch
            Layout.preferredHeight: 30
            Layout.fillWidth: true

            model: DataBridge.switchControllerModel
            modelIndex: Enums.SWITCHES_PIR
        }

        HCValueLabel {
            Layout.preferredHeight: 30
            Layout.fillWidth: true

            model: DataBridge.brightnessControllerModel
            modelIndex: Enums.BRIGHTNESSES_OUTSIDE
        }

        HCValueLabel {
            Layout.preferredHeight: 30
            Layout.fillWidth: true

            model: DataBridge.relayControllerModel
            modelIndex: Enums.RELAYS_LIGHT_OUTSIDE
        }
    }
}
