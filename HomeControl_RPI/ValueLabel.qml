import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3
import QtQuick.VirtualKeyboard 2.1

Item {

    property var model
    property int modelIndex
    property string labelText: model.data(model.index(modelIndex, 0), 257)
    property var labelValue: ""
    property bool isValid: false
    property string unit: ""
    property bool allowInput: false
    property var inputHandler
    property alias inputMethodHints: value.inputMethodHints
    property alias inputValidator: value.validator

    Connections {
        target: model

        onDataChanged: {
            labelValue = model ? model.data(model.index(modelIndex, 0), 258) : ""
            isValid = model ? model.data(model.index(modelIndex, 0), 259) : false
        }
    }

    RowLayout {
        Label {
            Layout.preferredWidth: 160
            id: label
            text: labelText + ":"

            font.pointSize: 12
        }

        TextInput {
            id: value

            visible: !checkbox.visible

            text: labelValue
            color: isValid ? "black" : "red"

            readOnly: !allowInput

            font.pointSize: 12

            onAccepted: {
                if (typeof(inputHandler)=="function") inputHandler(text);
            }
        }

        Label {
            text: unit
            font.pointSize: 12
        }

        CheckBox {
            id: checkbox
            visible: typeof(labelValue)=="boolean"
            enabled: allowInput
            checked: labelValue ? labelValue : false

            onToggled: {
                if (typeof(inputHandler)=="function") inputHandler(labelValue);
            }
        }
    }
}
