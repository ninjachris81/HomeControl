import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3
import QtQuick.VirtualKeyboard 2.1

import StyleDef 1.0

Item {

    property string labelText: ""
    property var labelValue: ""
    property bool isValid: false
    property string unit: ""
    property bool allowInput: false
    property var inputHandler
    property alias inputMethodHints: value.inputMethodHints
    property alias inputValidator: value.validator
    property int labelWidth: 160
    property bool formatAsFloat: false
    property bool showTrend: false
    property int trend: 0
    property bool showLabel: true

    RowLayout {
        //spacing: 0

        Label {
            id: label

            visible: showLabel

            Layout.preferredWidth: labelWidth
            text: labelText + ":"

            font.pointSize: Style.fontPointSize-2
        }

        TextInput {
            id: value

            visible: !checkbox.visible

            text: formatAsFloat ? Number(labelValue).toLocaleString(Qt.locale(), "f", 1) : labelValue
            color: isValid ? "black" : "red"

            readOnly: true

            font.pointSize: Style.fontPointSize-2

            MouseArea {
                anchors.fill: parent
                anchors.leftMargin: -6
                anchors.rightMargin: -26
                anchors.topMargin: -4
                anchors.bottomMargin: -4

                enabled: allowInput

                Rectangle {
                    anchors.fill: parent

                    visible: parent.enabled

                    border.color: "black"
                    opacity: 0.1

                    radius: 2
                }

                onClicked: {
                    HCKeyboardManager.invoke(value.text, function(newText) {
                        value.text = newText

                        if (typeof(inputHandler)=="function") {
                            inputHandler(value.text);
                        } else {
                            console.warn("No input handler")
                        }
                    },
                    value.inputMethodHints, value.validator, value.font)
                }
            }
        }

        HCLabel {
            text: unit
        }

        Image {
            id: trendIcon

            visible: showTrend && trend!=0
            source: trend==1 ? "qrc:/images/icons/arrow_right.png" :
                        trend==2 ? "qrc:/images/icons/arrow_up.png" :
                            trend==3 ? "qrc:/images/icons/arrow_down.png" : ""
            opacity: trend==1 ? 0.1 : 1

            fillMode: Image.PreserveAspectFit

            Layout.preferredWidth: 18
        }

        CheckBox {
            id: checkbox
            visible: typeof(labelValue)=="boolean"
            enabled: allowInput
            checked: labelValue ? labelValue : false

            font.pointSize: Style.fontPointSize-2

            onToggled: {
                if (typeof(inputHandler)=="function") {
                    inputHandler(labelValue);
                }
                //labelValue = checked
            }
        }
    }

}
