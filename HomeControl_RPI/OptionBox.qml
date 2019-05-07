import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.4

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
                property int myValue: options[index]

                enabled: allowInput

                height: 20
                text: labels[index]
                checked: currentValue===index

                font.pointSize: 12

                onToggled: {
                    if (typeof(inputHandler)=="function") inputHandler(myValue);
                }
            }
        }
    }
}
