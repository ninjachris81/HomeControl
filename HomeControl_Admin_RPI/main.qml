import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.3

//import "SharedLib/qml"

import hc 1.0

import StyleDef 1.0
import SharedComponents 1.0

Window {
    id: root

    visible: true
    width: Style.screenWidth
    height: Style.screenHeight

    ColumnLayout {
        anchors.fill: parent

        RowLayout {

            HCValueLabel {
                Layout.preferredHeight: 30
                Layout.fillWidth: true

                model: DataBridge.tempControllerModel
                modelIndex: Enums.TEMPS_HC
                unit: "°"
                formatAsFloat: true
                showTrend: true
            }

            HCValueLabel {
                Layout.preferredHeight: 30
                Layout.fillWidth: true

                model: DataBridge.tempControllerModel
                modelIndex: Enums.TEMPS_WATER
                unit: "°"
                formatAsFloat: true
                showTrend: true
            }

            HCValueLabel {
                Layout.preferredHeight: 30
                Layout.fillWidth: true

                model: DataBridge.tempControllerModel
                modelIndex: Enums.TEMPS_TANK
                unit: "°"
                formatAsFloat: true
                showTrend: true
            }

        }

    }

}
