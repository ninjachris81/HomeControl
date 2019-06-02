import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3
import QtQuick.VirtualKeyboard 2.1

import "../style"

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

    RowLayout {
        Label {
            id: label

            Layout.preferredWidth: labelWidth
            text: labelText + ":"

            font.pointSize: Style.fontPointSize-2
        }

        TextInput {
            id: value

            visible: !checkbox.visible

            text: formatAsFloat ? Number(labelValue).toLocaleString(Qt.locale(), "f", 1) : labelValue
            color: isValid ? "black" : "red"

            readOnly: !allowInput

            font.pointSize: Style.fontPointSize-2

            onAccepted: {
                if (typeof(inputHandler)=="function") inputHandler(text);
            }
        }

        HCLabel {
            text: unit
        }

        CheckBox {
            id: checkbox
            visible: typeof(labelValue)=="boolean"
            enabled: allowInput
            checked: labelValue ? labelValue : false

            font.pointSize: Style.fontPointSize-2

            onToggled: {
                if (typeof(inputHandler)=="function") inputHandler(labelValue);
            }
        }
    }

}
