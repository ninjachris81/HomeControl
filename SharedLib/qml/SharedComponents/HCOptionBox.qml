import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.4

import StyleDef 1.0

Item {
    property var model
    property int modelIndex
    property var options
    property var labels
    property int currentValue: model.data(model.index(modelIndex, 0), 258)
    property bool allowInput: true
    property var inputHandler

    height: options.length * 20

    Connections {
        target: model

        onDataChanged: {
            currentValue = model.data(model.index(modelIndex, 0), 258)
        }
    }

    RowLayout {

        Repeater {
            model: options.length

            RadioButton {
                id: radioButton

                property int myValue: options[index]

                enabled: allowInput

                height: 20
                text: labels[index]
                checked: currentValue===options[index]

                font.pointSize: Style.fontPointSize

                onToggled: {
                    if (typeof(inputHandler)=="function") inputHandler(myValue, index);
                }
            }
        }
    }
}
