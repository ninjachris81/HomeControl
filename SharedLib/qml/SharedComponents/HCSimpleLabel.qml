import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3
import QtQuick.VirtualKeyboard 2.1

import StyleDef 1.0

Item {
    id: root

    property string labelText: ""
    property var labelValue: ""
    property bool isValid: false
    property string unit: ""
    property string labelSuffix: ":"
    property bool allowInput: false
    property var inputHandler
    property alias inputMethodHints: value.inputMethodHints
    property alias inputValidator: value.validator
    property bool formatAsFloat: false
    property bool showTrend: false
    property int trend: 0
    property bool showLabel: true
    property alias backgroundElement: backgroundItem
    property int fontPointSize: 0

    // TODO: change to loader
    HCGroupBox {
        id: backgroundItem

        anchors.fill: parent
        visible: false
    }

    RowLayout {
        id: mainLayout

        spacing: 2
        anchors.fill: parent

        HCLabel {
            id: label

            Layout.fillWidth: true

            visible: showLabel

            text: labelText + labelSuffix

            fontPointSize: root.fontPointSize>0 ? root.fontPointSize : Style.fontPointSize-2
        }

        TextInput {
            id: value

            visible: !checkbox.visible

            text: formatAsFloat ? Number(labelValue).toLocaleString(Qt.locale(), "f", 1) : labelValue
            color: isValid ? "black" : "red"

            readOnly: true

            font.pointSize: fontPointSize>0 ? fontPointSize : Style.fontPointSize-2
            font.family: Style.fontFamily

            MouseArea {
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.left: parent.left

                width: value.width + unitLabel.width + mainLayout.spacing + (-anchors.margins * 2)

                anchors.margins: -4

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
            id: unitLabel

            text: unit
            fontPointSize: root.fontPointSize>0 ? root.fontPointSize : Style.fontPointSize-2
        }

        Image {
            id: trendIcon

            visible: showTrend && trend!=0
            source: trend==1 ? "qrc:/images/icons/arrow_right.png" :
                        trend==2 ? "qrc:/images/icons/arrow_up.png" :
                            trend==3 ? "qrc:/images/icons/arrow_down.png" : ""
            opacity: trend==1 ? 0.1 : 1

            fillMode: Image.PreserveAspectFit

            Layout.preferredWidth: root.height - 4
            Layout.preferredHeight: root.height - 4
        }

        CheckBox {
            id: checkbox
            visible: typeof(labelValue)=="boolean"
            enabled: allowInput
            checked: labelValue ? labelValue : false

            font.pointSize: fontPointSize>0 ? fontPointSize : Style.fontPointSize-2

            onToggled: {
                if (typeof(inputHandler)=="function") {
                    inputHandler(labelValue);
                }
                //labelValue = checked
            }
        }
    }

}
